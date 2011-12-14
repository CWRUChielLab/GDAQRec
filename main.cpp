#include <QtGui>
#include "plotter.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Plotter plotter;
    plotter.setWindowTitle(QObject::tr("GDAQ recorder"));
    plotter.show();
    return app.exec();
}
