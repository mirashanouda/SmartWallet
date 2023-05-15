#include "myserver.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MyServer w;
    w.show();
    return a.exec();
}
