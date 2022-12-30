#include <QApplication>
#include "primewindow.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    primewindow w;
    w.show();

    return a.exec();
}
