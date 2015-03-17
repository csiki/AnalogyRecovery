
#include "Corpus.h"

unordered_set<char> Corpus::forbidden_chars; // numbers, non-alphabetical characters
unordered_set<string> Corpus::forbidden_words;
unordered_set<char> Corpus::endofsentence_chars;
bool Corpus::split_ctx_at_sentence = true;
size_t Corpus::min_num_of_occurrence = 50; // TODO
double Corpus::detail_multiplier = 5.0; // TODO check for other values
const double Corpus::analogy_eps = 1e-4;

WordPtr Corpus::analogy_3_cos_add(WordPtr a, WordPtr a_, WordPtr b)
{
	auto similar_to = b->features - a->features + a_->features;
	return *std::max_element(vocabulary.begin(), vocabulary.end(),
		[&similar_to] (const WordPtr& wpleft, const WordPtr& wpright) {
			return SparseVector<float>::cos_sim(wpleft->features, similar_to)
				< SparseVector<float>::cos_sim(wpright->features, similar_to);
	});
}
string Corpus::analogy_3_cos_add(string a, string a_, string b)
{
	auto dummy_aptr = std::make_shared<Word>(Word(a));
	auto dummy_a_ptr = std::make_shared<Word>(Word(a_));
	auto dummy_bptr = std::make_shared<Word>(Word(b));

	auto aptr = vocabulary.find(dummy_aptr);
	auto a_ptr = vocabulary.find(dummy_a_ptr);
	auto bptr = vocabulary.find(dummy_bptr);

	if (aptr == vocabulary.end() || a_ptr == vocabulary.end() || bptr == vocabulary.end())
		throw std::exception("Given word not found in analogy task!");

	return analogy_3_cos_add(*aptr, *a_ptr, *bptr)->word;
}

WordPtr Corpus::analogy_3_cos_mul(WordPtr a, WordPtr a_, WordPtr b)
{
	return *std::max_element(vocabulary.begin(), vocabulary.end(), // TODO replace with simple loop
		[&a, &a_, &b] (const WordPtr& wpleft, const WordPtr& wpright) {
			return
				(SparseVector<float>::cos_sim(wpleft->features, b->features)
				* SparseVector<float>::cos_sim(wpleft->features, a_->features)
				/ (SparseVector<float>::cos_sim(wpleft->features, a->features) + analogy_eps))
				<
				(SparseVector<float>::cos_sim(wpright->features, b->features)
				* SparseVector<float>::cos_sim(wpright->features, a_->features)
				/ (SparseVector<float>::cos_sim(wpright->features, a->features) + analogy_eps));
	});
}
string Corpus::analogy_3_cos_mul(string a, string a_, string b)
{
	auto dummy_aptr = std::make_shared<Word>(Word(a));
	auto dummy_a_ptr = std::make_shared<Word>(Word(a_));
	auto dummy_bptr = std::make_shared<Word>(Word(b));

	auto aptr = vocabulary.find(dummy_aptr);
	auto a_ptr = vocabulary.find(dummy_a_ptr);
	auto bptr = vocabulary.find(dummy_bptr);

	if (aptr == vocabulary.end() || a_ptr == vocabulary.end() || bptr == vocabulary.end())
		throw std::exception("Given word not found in analogy task!");

	return analogy_3_cos_mul(*aptr, *a_ptr, *bptr)->word;
}

Corpus::Corpus(bool is_source_preprocessed_) : is_source_preprocessed(is_source_preprocessed_)
{
	std::cout << std::fixed;
	std::cout << std::setprecision(2);
}

void Corpus::generate_voc()
{
	if (sources_path.empty())
		throw std::exception("Corpus has no source to generate the vocabulary from!");

	for (auto& s : sources_path)
	{
		std::ifstream fin(s, std::ios::in);
		if (!fin)
		{
			string errmsg = "File ";
			errmsg += s;
			errmsg += " cannot be opened!";
			throw std::exception(errmsg.c_str());
		}
		std::cout << "Source " << s << " is being processed." << std::endl << "Vocabulary size sofar: " << vocabulary.size() << std::endl;

		while (!fin.eof())
		{
			auto curr_word = read_word(fin);
			if (curr_word.get() != nullptr)
			{
				// add word to vocabulary
				auto insw = vocabulary.insert(curr_word);
				if (!insw.second)
					(*insw.first)->inc_freq(); // not inserted, thus already inside
			}
		}
	}
	// remove words with low occurrence
	for (auto vit = vocabulary.begin(); vit != vocabulary.end(); )
	{
		if ((*vit)->get_freq() < min_num_of_occurrence)
			vit = vocabulary.erase(vit);
		else
			++vit;
	}
}

void Corpus::generate_ctx()
{
	if (sources_path.empty())
		throw std::exception("Corpus has no source to generate the contexts from!");
	if (vocabulary.empty())
		throw std::exception("Call generate_voc first - vocabulary is empty!");

	for (auto& s : sources_path)
	{
		std::ifstream fin(s, std::ios::in);
		if (!fin)
		{
			string errmsg = "File ";
			errmsg += s;
			errmsg += " cannot be opened!";
			throw std::exception(errmsg.c_str());
		}
		std::cout << "Source " << s << " is being processed:" << std::endl;

		// get file size
		fin.seekg(0, fin.end);
		auto fsize = fin.tellg();
		fin.seekg(0, fin.beg);
		read_print_state = 1;

		ctx_hist.clear();
		string line;
		while (std::getline(fin, line, '\n')) // read line (sentence in processed data)
		{
			std::istringstream ssline(line, std::ios::in);
			while (!ssline.eof())
			{
				auto curr_word = read_word(ssline);
				if (curr_word.get() != nullptr)
				{
					auto word_found = vocabulary.find(curr_word);
					if (word_found != vocabulary.end())
						arrange_ctx(*word_found);
					else
						insert_null_to_ctx_hist(); // insert dummy wordptr
				}
				else insert_null_to_ctx_hist();
			}
			ctx_hist.clear(); // after every line (sentence) clear history
			
			// console feedback
			print_read_info((float)fin.tellg() / fsize);
		}
	}
}

void Corpus::calc_feature_vectors()
{
	if (vocabulary.empty() || contexts.empty())
		throw std::exception("No vocabualary or context to work with!");

	// init vector length for all words
	for (auto& word : vocabulary)
		word->features.set_len(contexts.size());

	// fill vectors
	size_t vindex = 0;
	size_t voc_size = vocabulary.size();
	for (auto& ctx : contexts)
	{
		auto ctx_freq = ctx->get_freq();
		auto dist_mul = std::pow(Context::distance_multiplier, std::abs(ctx->pos));

		std::for_each(ctx->surr_begin(), ctx->surr_end(),
			[vindex, voc_size, ctx_freq, dist_mul] (const std::pair<WordPtr, size_t>& wcfreq) {
				auto PMI = std::logf( (float)(wcfreq.second * voc_size) * dist_mul * detail_multiplier // TODO
					/ (wcfreq.first->get_freq() * ctx_freq) );
				if (PMI > 0)
					wcfreq.first->features[vindex] = PMI;
		});
		++vindex;
	}
}

void Corpus::ser_voc_and_ctx(std::ostream& voc_out, std::ostream& ctx_out) const
{
	if (vocabulary.empty() || contexts.empty())
		throw std::exception("No vocabualary or context to work with!");

	for (auto& w : vocabulary)
		voc_out << w;
	for (auto& c : contexts)
		ctx_out << c;
}

void Corpus::deser_voc_and_ctx(std::istream& voc_in, std::istream& ctx_in)
{
	WordPtr wp;
	while (voc_in >> wp)
		vocabulary.insert(wp);

	CtxPtr cp(nullptr);
	while (ctx_in >> cp)
	{
		cp->update_surr(vocabulary);
		contexts.insert(std::move(cp));
	}
}

void Corpus::ser_voc_sparse_vec(std::ostream& out) const
{
	if (vocabulary.empty())
		throw std::exception("No vocabualary to work with!");

	out << (*(vocabulary.begin()))->features.get_len() << std::endl;
	for (auto& w : vocabulary)
		out << w->word << " " << w->get_freq() << " " << w->features << std::endl;
}

void Corpus::deser_voc_sparse_vec(std::istream& in)
{
	size_t veclen;
	in >> veclen;
	in.get();
	
	string line;
	string word;
	size_t freq;
	SparseVector<float> sv(veclen);
	while (std::getline(in, line, '\n'))
	{
		sv.clear();
		std::istringstream ssline(line, std::ios::in);
		ssline >> word >> freq >> sv;
		auto wp = std::make_shared<Word>(Word(word, freq));
		wp->features = sv;
		vocabulary.insert(wp);
	}
}

WordPtr Corpus::read_word(std::istream& stream)
{
	string raw, wellf;
	stream >> raw;
	if (try_form_well(raw, wellf, is_source_preprocessed)) // if preprocessed, use lowcost
	{
		auto wordptr = std::make_shared<Word>(Word(wellf));
		return wordptr;
	}
	return WordPtr(nullptr);
}

void Corpus::arrange_ctx(WordPtr curr_word)
{
	int pos = ctx_hist.size();
	for (auto histit = ctx_hist.begin(); histit != ctx_hist.end(); ++histit)
	{
		if (histit->get() != nullptr)
		{
			// create ctx from history words to assign it to curr_word
			auto ctx_hist_ptr = contexts.insert(unique_ptr<Context>(
				new Context(*histit, -pos))); // pos: -n, -(n-1), ..., -2, -1
			if (!ctx_hist_ptr.second)
				(*ctx_hist_ptr.first)->inc_freq();
			(*ctx_hist_ptr.first)->surround_word(curr_word);

			// create ctx from curr_word to assign it to prev window_size words
			auto ctx_curr_ptr = contexts.insert(unique_ptr<Context>(
				new Context(curr_word, pos))); // pos: n, n-1, ..., 2, 1
			if (!ctx_curr_ptr.second)
				(*ctx_curr_ptr.first)->inc_freq();
			(*ctx_curr_ptr.first)->surround_word(*histit);
		}
		--pos;
	}
	
	// append curr_word & rm words outside of window_size
	ctx_hist.push_back(curr_word);
	if (ctx_hist.size() > Context::window_size)
		ctx_hist.pop_front();
}

bool Corpus::try_form_well(string orig, string& res, bool lowcost)
{
	if (!lowcost)
	{
		// to lowercase
		std::transform(orig.begin(), orig.end(), orig.begin(), ::tolower);

		// check if contains forbidden characters or characters outside [-1,255]
		bool forbidden = false;
		for (auto lit = orig.begin(); !forbidden && lit != orig.end(); ++lit)
				if (*lit < -1 || *lit > 255
					|| forbidden_chars.find(*lit) != forbidden_chars.end())
					forbidden = true;
		if (forbidden) return false;

		// check if a forbidden word
		if (forbidden_words.find(orig) != forbidden_words.end())
			return false;
	}

	// remove punctuation
	std::remove_copy_if(orig.begin(), orig.end(),
		std::back_inserter(res),
		std::ptr_fun<int, int>(&std::ispunct));

	// throw out empty word
	if (res.empty())
		return false;

	return true;
}

void Corpus::print_read_info(float rate)
{
	if (rate * 10 > read_print_state)
	{
		++read_print_state;
		std::cout << rate * 100.0 << "% - ";
		std::cout << "voc size: " << vocabulary.size() << "; ctx size: " << contexts.size() << std::endl;
	}
}

void Corpus::insert_null_to_ctx_hist()
{
	ctx_hist.push_back(WordPtr(nullptr));
	if (ctx_hist.size() > Context::window_size)
		ctx_hist.pop_front();
}

void Corpus::init()
{
	// forbidden characters
	forbidden_chars.insert('<');
	forbidden_chars.insert('>');
	forbidden_chars.insert('|');
	forbidden_chars.insert('/');
	forbidden_chars.insert('\\');
	forbidden_chars.insert('=');
	for (char n = '0'; n <= '9'; ++n)
		forbidden_chars.insert(n);

	// forbidden words
	forbidden_words.insert("endofarticle.");

	// end-of-sentence characters
	endofsentence_chars.insert('.');
	endofsentence_chars.insert('?');
	endofsentence_chars.insert('!');
}