#include "parser.h"

Parser::Parser()
{
	filters.push_back(std::bind(&Parser::check_forbidden_line_starters, this, placeholders::_1, placeholders::_2));
	filters.push_back(std::bind(&Parser::to_lower, this, placeholders::_1, placeholders::_2));
	filters.push_back(std::bind(&Parser::rm_non_alpha, this, placeholders::_1, placeholders::_2));
	filters.push_back(std::bind(&Parser::ignore_nearend_section_titles, this, placeholders::_1, placeholders::_2));
}

void Parser::parse()
{
	// figure out ign_sec_title_max_len
	ign_sec_title_max_len = max(ignored_section_titles.begin(), ignored_section_titles.end(),
		[] (unordered_set<string>::const_iterator str1, unordered_set<string>::const_iterator str2) {
			return str1->length() < str2->length();
	})->length();

	for (auto& sio : source_fileio)
	{
		ifstream fin(sio.first, ios::in);
		ofstream fout(sio.second, ios::out | ios::trunc);
		if (!fin || !fout)
			cerr << "Cannot open " << sio.first << " or " << sio.second << " !" << endl;

		cout << sio.first << " is being processed." << endl;
		string line;
		while (getline(fin, line))
		{
			bool success = true;
			for (auto fit = filters.begin(); success && fit != filters.end(); ++fit)
				success = (*fit)(fin, line);

			if (success) extract_sentences(line, fout);
		}
	}
}

bool Parser::check_forbidden_line_starters(ifstream& fin, string& line)
{
	for (auto& fls : forbidden_line_starters)
	{
		auto pos = line.find(fls);
		if (pos == 0)
			return false;
	}
	return true;
}

bool Parser::to_lower(ifstream& fin, string& line)
{
	std::transform(line.begin(), line.end(), line.begin(), ::tolower);
	return true;
}

bool Parser::ignore_nearend_section_titles(ifstream& fin, string& line)
{
	if (line.length() > ign_sec_title_max_len + 3) // too long to be any of it
		return true;

	string tmpline;
	std::remove_copy_if(line.begin(), line.end(),
		std::back_inserter(tmpline),
		std::ptr_fun<int, int>(&std::isspace));

	if (ignored_section_titles.find(tmpline) == ignored_section_titles.end())
		return true;

	while (!check_forbidden_line_starters(fin, line)) // till found ENDOFARTICLE OR <doc...
		getline(fin, line);

	return false;
}

bool Parser::rm_non_alpha(ifstream& fin, string& line)
{
	string word;
	string outline;
	istringstream iss(line);
	ostringstream oss;
	while (iss >> word)
	{
		bool nona_found = false;
		for (auto lit = word.begin(); !nona_found && lit != word.end(); ++lit)
			if (*lit < -1 || *lit > 255
				|| non_alpha.find(*lit) != non_alpha.end())
				nona_found = true;
		if (!nona_found)
			oss << word << " ";
	}
	if (oss.str().empty()) return false;
	line = oss.str();
	line.pop_back(); // rm last inserted space
	
	return true;
}

void Parser::extract_sentences(const string& line, ofstream& fout)
{
	istringstream iss(line);
	string sentence, word;
	size_t word_counter = 0;
	while (iss >> word)
	{
		sentence += word + " ";
		++word_counter;
		if (sentence_ending_chars.find(word.back()) != sentence_ending_chars.end()
			&& sentence.length() > 1) // found sentence ending char
		{
			sentence.pop_back();
			if (word_counter >= 2 * wsize + 1)
				fout << sentence << endl;
			sentence.clear();
			word_counter = 0;
		}
	}
}