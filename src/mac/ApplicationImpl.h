#ifndef MOCAM_APPLICATION_IMPL_H
#define MOCAM_APPLICATION_IMPL_H

#include <string>
#include <memory>
#include <utility>

namespace mocam {
  class ApplicationImpl;
  typedef std::shared_ptr<ApplicationImpl> ImplAPtr;
  
  class ApplicationImpl {
  public:
    ApplicationImpl();

    std::pair<std::string, std::string> getDefaultDevice() const;

  private:
    std::pair<std::string, std::string> defDev;
  };
}

#endif // MOCAM_APPLICATION_IMPL_H)
