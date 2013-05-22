#include <QtGui/QApplication>
#include "mathparse.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MathParse w;
    w.show();

    return a.exec();
}
