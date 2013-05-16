#ifndef PWD_HASH_H
#define PWD_HASH_H

#include <string>

using namespace std;

extern string gensetting();
extern string genhash(const string &input, const string &setting);
extern bool checkhash(const string &input, const string &hash);

#endif
