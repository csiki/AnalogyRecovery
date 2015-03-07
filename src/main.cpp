
// TODO update uml

#include <exception>
#include <iostream>
#include <iterator>
#include "SparseVector.h"
#include "Corpus.h"
#include "Word.h"

using namespace std;

int main(int argc, char* argv[])
{
	Corpus::init_forbidden();
	Corpus corpus;
	corpus.sources_path.push_back("c:/wiki_corpus/test");
	corpus.generate_voc_and_ctx();
	corpus.calc_feature_vectors();
	cout << "rudi" << endl;

	return 0;
}