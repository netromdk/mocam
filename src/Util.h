#ifndef MOCAM_UTIL_H
#define MOCAM_UTIL_H

#include <string>

namespace mocam {
  class Util {
  public:
    static bool writeFile(const std::string &filename, const char *data,
                          int length);
  };
}

#endif // MOCAM_UTIL_H
