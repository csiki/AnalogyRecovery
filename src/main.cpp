
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
	corpus.source.push_back("c:/wiki_corpus/englishText_0_10000");
	corpus.generate_voc_and_ctx();
	cout << "rudi" << endl;
	

	return 0;
}