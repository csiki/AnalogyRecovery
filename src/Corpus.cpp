
#include "Corpus.h"

unordered_set<char> Corpus::forbidden_chars; // numbers, non-alphabetical characters
set<string> Corpus::forbidden_words;

void Corpus::generate_voc_and_ctx()
{
	if (source.empty())
		throw std::exception("Corpus has no source to generate vocabulary and contexts from!");

	gen_voc();
	gen_ctx();
}

void Corpus::gen_voc()
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
}

bool Corpus::try_form_well(const string& orig, string& res)
{
	// to lowercase
	string lower = orig;
	std::transform(orig.begin(), orig.end(), lower.begin(), ::tolower);

	// check if contains forbidden characters
	bool forbidden = false;
	auto fchars_cpy = forbidden_chars;
	std::for_each(lower.begin(), lower.end(),
		[&forbidden, &fchars_cpy] (char c) {
			if (fchars_cpy.find(c) != fchars_cpy.end())
			{
				forbidden = true;
				return;
			}
	});
	if (forbidden) return false;

	// check if a forbidden word
	std::for_each(forbidden_words.begin(), forbidden_words.end(),
		[&forbidden, res] (const string& fword) {
			if (std::mismatch(res.begin(), res.end(), fword.begin()).second == fword.end())
			{
				forbidden = true;
				return;
			}
	});

	// find characters outside of [-1, 255]
	auto found = std::find_if_not(lower.begin(), lower.end(),
		[] (int c) { return c >= -1 && c <= 255; });
	if (found != lower.end()) // contains character outside of bound
		return false;
	
	// remove punctuation
	std::remove_copy_if(lower.begin(), lower.end(),
		std::back_inserter(res),
		std::ptr_fun<int, int>(&std::ispunct));

	return true;
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