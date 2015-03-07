
#include "Corpus.h"

unordered_set<char> Corpus::forbidden_chars; // numbers, non-alphabetical characters
set<string> Corpus::forbidden_words;

Corpus::Corpus()
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
		while (!fin.eof())
		{
			auto curr_word = read_word(fin);
			if (curr_word.get() != nullptr)
			{
				// add word to vocabulary
				auto insw = vocabulary.insert(curr_word);
				if (!insw.second)
					(*insw.first)->inc_freq(); // not inserted, thus already inside

				auto curr_ctx = arrange_ctx(*insw.first);
				if (curr_ctx.get() != nullptr)
				{
					// add context
					auto insc = contexts.insert(curr_ctx);
					if (!insc.second)
						(*insc.first)->inc_freq(); // not inserted, thus already inside

					// update word-ctx freq for the middle word in ctx_hist
					// (not exactly the middle, as Corpus::arrange_ctx already removed the first of the 2*wsize+1 elements)
					//ctx_hist[Context::window_size]->appears_in(*insc.first); // old
					(*insc.first)->surround_word(*insw.first);
				}
			}
			else
				ctx_hist.clear(); // if could not read word, start a new context buffer

			// console feedback
			print_read_info((float)fin.tellg() / fsize);
		}
		std::cout << "finished!" << std::endl;
	}
}

void Corpus::calc_feature_vectors()
{
	if (vocabulary.empty() || contexts.empty())
		throw std::exception("No vocabualary or context to work with!");

	// init vector length for all words
	for (auto word : vocabulary)
		word->features.set_len(contexts.size());

	// fill vectors
	size_t vindex = 0;
	size_t voc_size = vocabulary.size();
	for (auto ctx : contexts)
	{
		size_t ctx_freq = ctx->get_freq();
		std::for_each(ctx->surr_begin(), ctx->surr_end(),
			[vindex, voc_size, ctx_freq] (const std::pair<WordPtr, size_t>& wcfreq) {
				auto PMI = std::log( (double)(wcfreq.second * voc_size)
					/ (wcfreq.first->get_freq() * ctx_freq) );
				if (PMI > 0)
					wcfreq.first->features[vindex] = PMI;
		});
		++vindex;
	}
}

WordPtr Corpus::read_word(std::istream& stream)
{
	string raw, wellf;
	stream >> raw;
	if (try_form_well(raw, wellf))
	{
		auto wordptr = std::make_shared<Word>(Word(wellf));
		return wordptr;
	}
	return WordPtr(nullptr);
}

CtxPtr Corpus::arrange_ctx(WordPtr curr_word)
{
	ctx_hist.push_back(curr_word);
	if (ctx_hist.size() == 2 * Context::window_size + 1) // enough words to form a context
	{
		auto ctxptr = std::make_shared<Context>(Context(ctx_hist));
		ctx_hist.pop_front();
		return ctxptr;
	}
	return CtxPtr(nullptr);
}

bool Corpus::try_form_well(const string& orig, string& res)
{
	// to lowercase
	string lower = orig;
	std::transform(orig.begin(), orig.end(), lower.begin(), ::tolower);

	// check if contains forbidden characters or characters outside [-1,255]
	bool forbidden = false;
	auto fchars_cpy = forbidden_chars;
	std::for_each(lower.begin(), lower.end(),
		[&forbidden, &fchars_cpy] (char c) {
			if (c < -1 || c > 255
				|| fchars_cpy.find(c) != fchars_cpy.end())
			{
				forbidden = true;
				return;
			}
	});
	if (forbidden) return false;

	// check if a forbidden word
	std::for_each(forbidden_words.begin(), forbidden_words.end(),
		[&forbidden, lower] (const string& fword) {
			if (lower.compare(fword) == 0)
			{
				forbidden = true;
				return;
			}
	});
	
	// remove punctuation
	std::remove_copy_if(lower.begin(), lower.end(),
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
		std::cout << rate * 100.0 << "% ";

		/*// most frequent word
		size_t maxnum = 0;
		string maxword;
		for (auto w : vocabulary)
		{
			if (maxnum < w->get_freq())
			{
				maxnum = w->get_freq();
				maxword = w->word;
			}
		}
		std::cout << "word with most freq: " << maxword << " - " << maxnum << " / " << vocabulary.size() << std::endl;*/
	}
}

bool Corpus::is_forbidden(const string& str)
{
	if (str.empty()) return false;

	// check if forbidden
	bool forbidden = false;
	std::for_each(forbidden_words.begin(), forbidden_words.end(),
		[&forbidden, str] (const string& fword) {
			if (std::mismatch(str.begin(), str.end(), fword.begin()).second == fword.end())
			{
				forbidden = true;
				return;
			}
	});
	if (forbidden) return false;

	// check if contains forbidden characters
	auto fchars_cpy = forbidden_chars;
	std::for_each(str.begin(), str.end(),
		[&forbidden, &fchars_cpy] (char c) {
			if (fchars_cpy.find(c) != fchars_cpy.end())
			{
				forbidden = true;
				return;
			}
	});

	return !forbidden;
}

void Corpus::init_forbidden()
{
	// characters
	forbidden_chars.insert('<');
	forbidden_chars.insert('>');
	forbidden_chars.insert('|');
	forbidden_chars.insert('/');
	forbidden_chars.insert('\\');
	forbidden_chars.insert('=');
	for (char n = '0'; n <= '9'; ++n)
		forbidden_chars.insert(n);

	// words
	forbidden_words.insert("endofarticle.");
}

/*void Corpus::gen_voc()
{
	for (auto& s : source)
	{
		std::ifstream fin(s, std::ios::in);
		if (!fin)
		{
			string errmsg = "File ";
			errmsg += s;
			errmsg += " cannot be opened!";
			throw std::exception(errmsg.c_str());
		}
		
		// read word by word
		string raw;
		while (fin >> raw)
		{
			string wellf;
			if (try_form_well(raw, wellf))
			{
				auto wordptr = std::make_shared<Word>(Word(wellf));
				auto ins = vocabulary.insert(wordptr);
				if (!ins.second)
					(*ins.first)->inc_freq(); // not inserted, thus already inside
			}
		}
	}
}

void Corpus::gen_ctx()
{
	// TODO
}*/