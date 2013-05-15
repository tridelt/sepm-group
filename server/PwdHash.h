#ifndef PWD_HASH_H
#define PWD_HASH_H

#include <string>

using namespace std;

extern string gensalt();
extern string makehash(const string &input, const string &salt);
extern bool checkhash(const string &input, const string &hash, const string &salt);

#endif
