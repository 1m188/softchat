#include "QCoreApplication"

#include "server.h"

int main(int argc, char *argv[])
{

#ifdef RELEASE
    QCoreApplication::addLibraryPath("./plugins"); //做发布版本时要加上
#endif                                         // RELEASE

    QCoreApplication a(argc, argv);
    Server server;
    return a.exec();
}
