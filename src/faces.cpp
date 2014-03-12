#include <QDebug>
#include <QImage>
#include <QString>
#include <QCoreApplication>

#include <memory>

#include <opencv2/opencv.hpp>

#include "Util.h"
#include "Types.h"
using namespace mocam;

struct Arguments {
  QString filename;
};

void usage(char **argv) {
  qDebug() << "Usage: " << argv[0] << " <input file>";
}

std::unique_ptr<Arguments> parseArgs(int argc, char **argv) {
  if (argc < 2) {
    return nullptr;
  }

  auto args = std::unique_ptr<Arguments>(new Arguments);

  /* No options yet.
  for (int i = 1; i < argc; i++) {
    QString arg = QString::fromUtf8(argv[i]).trimmed().toLower();
  }
  */

  args->filename = QString::fromUtf8(argv[argc - 1]);
  return args;
}

int main(int argc, char **argv) {
  QCoreApplication app(argc, argv);

  auto args = parseArgs(argc, argv);
  if (args == nullptr) {
    usage(argv);
    return -1;
  }

  qDebug() << "Input:" << args->filename;

  MatPtr mat = Util::imageToMat(args->filename);
  if (mat == nullptr) {
    qCritical() << "Invalid image!";
    return -1;
  }
  qDebug() << "Loaded successfully..";
  
  return 0;
}
