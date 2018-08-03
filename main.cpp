#include "mathmodeling.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MathModeling MM;
    MM.setMinimumHeight(480);
    MM.setMinimumWidth(950);
    MM.show();

    return a.exec();
}
