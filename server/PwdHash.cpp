#include "PwdHash.h"
#include "ow-crypt/ow-crypt.h"
#include "Logging.h"
#include <cstring>
#include <cerrno>

#define RANDOMNESS_SIZE 256
#define RANDOMNESS_SOURCE "/dev/urandom"
#define CRYPT_ITERATIONS 8

string gensetting() {
  FILE *source = fopen("/dev/urandom", "r");
  if(!source) {
    WARN("failed to read from randomness source " RANDOMNESS_SOURCE ": " + string(strerror(errno)));
    throw;
  }

  char randomness[RANDOMNESS_SIZE];
  size_t read = fread(&randomness, 1, RANDOMNESS_SIZE, source);
  if(read != RANDOMNESS_SIZE) {
    WARN("failed to read sufficient random data for salt: " + string(strerror(errno)));
    throw;
  }

  char *setting = crypt_gensalt_ra("$2a$", CRYPT_ITERATIONS, randomness, RANDOMNESS_SIZE);
  if(!setting)  {
    WARN("crypt_gensalt_ra failed: " + string(strerror(errno)));
    throw;
  }

  fclose(source);

  string ret(setting);
  free(setting);

  return ret;
}

string genhash(const string &input, const string &setting)
{
  void *nil = NULL;
  int size = 0;
  char *hash = crypt_ra(input.c_str(), setting.c_str(), &nil, &size);
  if(!hash)  {
    WARN("crypt_ra failed: " + string(strerror(errno)));
    throw;
  }
  
  string ret(hash);
  free(hash);

  return ret;
}

bool checkhash(const string &input, const string &hash)
{
  return genhash(input, hash) == hash;
}

