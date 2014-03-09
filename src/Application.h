#ifndef MOCAM_APPLICATION_H
#define MOCAM_APPLICATION_H

#include <string>
#include <memory>
#include <utility>

#include "ApplicationImpl.h"

namespace mocam {
  class Application;
  typedef std::shared_ptr<Application> APtr;
  
  class Application {
  public:
    Application();

    static ImplAPtr app() { return impl; }

    /**
     * Get unique ID and name (in that order) of default video device.
     */
    std::pair<std::string, std::string> getDefaultDevice() const;

  private:
    static ImplAPtr impl;
  };
}

#endif // MOCAM_APPLICATION_H
