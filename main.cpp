#include "mathmodelling.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MathModelling w;
    w.show();

    return a.exec();
}
