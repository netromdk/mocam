#include <fstream>

#include "Util.h"

namespace mocam {
  bool Util::writeFile(const std::string &filename, const char *data,
                       int length) {
    std::ofstream file;
    file.open(filename, std::ios::out | std::ios::trunc | std::ios::binary);
    if (!file.is_open()) {
      return false;
    }
    file.write(data, length);    
    file.close();
    return true;
  }
}
