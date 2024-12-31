#include "irviewer.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  IRViewer w;
  w.show();

  return app.exec();
}
