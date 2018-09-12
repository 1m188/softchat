#include <QtWidgets/QApplication>
#include "server.h"

int main(int argc, char *argv[])
{
#ifdef NDEBUG
	QApplication::addLibraryPath("./plugins"); //做发布版本时要加上
#endif // NDEBUG

	QApplication a(argc, argv);

	Server server(nullptr);

	return a.exec();
}
