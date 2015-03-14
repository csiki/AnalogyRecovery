
#include "Corpus.h"

unordered_set<char> Corpus::forbidden_chars; // numbers, non-alphabetical characters
unordered_set<string> Corpus::forbidden_words;
unordered_set<char> Corpus::endofsentence_chars;
bool Corpus::split_ctx_at_sentence = true;
double Corpus::analogy_eps = 1e-4;

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
	WordPtr dummy_aptr(new Word(a));
	WordPtr dummy_a_ptr(new Word(a_));
	WordPtr dummy_bptr(new Word(b));

	auto aptr = *vocabulary.find(dummy_aptr);
	auto a_ptr = *vocabulary.find(dummy_a_ptr);
	auto bptr = *vocabulary.find(dummy_bptr);

	return analogy_3_cos_add(aptr, a_ptr, bptr)->word;
}

WordPtr Corpus::analogy_3_cos_mul(WordPtr a, WordPtr a_, WordPtr b)
{
	return *std::max_element(vocabulary.begin(), vocabulary.end(),
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
	WordPtr dummy_aptr(new Word(a));
	WordPtr dummy_a_ptr(new Word(a_));
	WordPtr dummy_bptr(new Word(b));

	auto aptr = *vocabulary.find(dummy_aptr);
	auto a_ptr = *vocabulary.find(dummy_a_ptr);
	auto bptr = *vocabulary.find(dummy_bptr);

	return analogy_3_cos_mul(aptr, a_ptr, bptr)->word;
}

Corpus::Corpus(bool is_source_preprocessed_) : is_source_preprocessed(is_source_preprocessed_)
{
	std::cout << std::fixed;
	std::cout << std::setprecision(2);
}

void Corpus::generate_voc_and_ctx()
{
	if (sources_path.empty())
		throw std::exception("Corpus has no source to generate vocabulary and contexts from!");

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
		while (!fin.eof()) // TODO READ LINES INSTEAD
		{
			auto curr_word = read_word(fin);
			if (curr_word.get() != nullptr)
			{
				// add word to vocabulary
				auto insw = vocabulary.insert(curr_word);
				if (!insw.second)
					(*insw.first)->inc_freq(); // not inserted, thus already inside

				auto curr_ctxs = arrange_ctx(*insw.first);
				if (!curr_ctxs.empty())
				{
					// add context
					for (auto c : curr_ctxs)
					{
						auto insc = contexts.insert(c);
						if (!insc.second)
							(*insc.first)->inc_freq(); // not inserted, thus already inside
						// update word-ctx freq for the middle word in ctx_hist
						// (not exactly the middle, as Corpus::arrange_ctx already removed the first of the 2*wsize+1 elements)
						(*insc.first)->surround_word(ctx_hist[Context::window_size - 1]);
					}
				}
				// split ctx at end of sentence
				if (split_ctx_at_sentence
					&& endofsentence_chars.find(curr_word->word.back()) != endofsentence_chars.end())
					ctx_hist.clear();
			}
			//else
			//	ctx_hist.clear(); // if could not read word, start a new context buffer

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
				auto PMI = std::log( (float)(wcfreq.second * voc_size) * dist_mul
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
	
	CtxPtr cp;
	while (ctx_in >> cp)
	{
		cp->update_surr(vocabulary);
		contexts.insert(cp);
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

vector<CtxPtr> Corpus::arrange_ctx(WordPtr curr_word)
{
	ctx_hist.push_back(curr_word);
	if (ctx_hist.size() == 2 * Context::window_size + 1) // enough words to form a context
	{
		vector<CtxPtr> ctxs;
		ctxs.reserve(Context::window_size * 2);
		signed char pos = - Context::window_size;
		for (auto w : ctx_hist)
		{
			if (pos != 0)
			{
				auto ctxptr = std::make_shared<Context>(Context(w, pos));
				ctxs.push_back(ctxptr);
			}
			++pos;
		}
		ctx_hist.pop_front();
		return ctxs;
	}
	return vector<CtxPtr>();
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