#include <QMetaType>

#include "Types.h"

namespace mocam {
  void initTypes() {
    qRegisterMetaType<FramePtr>("FramePtr");
  }
}
