
// TODO update uml

#include <exception>
#include <iostream>
#include <iterator>
#include "SparseVector.h"
#include "Corpus.h"
#include "Word.h"
#include "test.h"

using namespace std;

int main(int argc, char* argv[])
{
	// after adjusting Corpus::min_num_of_occurrence
	create_save_voc_ctx();

	// after adjusting Corpus::detail_multiplier, Context::distance_multiplier
	create_save_vec();

	// after the previous two was done
	load_test_vec();

	return 0;
}