#ifndef _CORPUS_H
#define _CORPUS_H

#include <vector>
#include <memory>
#include <tuple>
#include "Word.h"
#include "Context.h"

using std::vector;
using std::tuple;
using std::string;
using std::shared_ptr;

class Corpus
{
public:
    vector<string> source;
    vector<shared_ptr<Word>> vocabulary;
    vector<shared_ptr<Context>> contexts;
    
    void generate_voc_and_ctx();

private:
    void gen_voc();
    void gen_ctx();
};

#endif //_CORPUS_H