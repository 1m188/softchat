#include <QtWidgets/QApplication>
#include "data.h"
#include "logingui.h"
#include "maingui.h"

int main(int argc, char *argv[])
{
	//QApplication::addLibraryPath("./plugins"); //做发布版本时要加上

	QApplication a(argc, argv);

	Data *data = new Data(nullptr);

	LoginGui *loginGui = new LoginGui(nullptr);
	data->addSignalSlots(loginGui);
	loginGui->show();

	if (loginGui->exec() == QDialog::Accepted)
	{
		MainGui *mainGui = new MainGui(nullptr);
		data->addSignalSlots(mainGui);
		mainGui->show();
		a.exec();
	}
	delete data;
	return 0;
}
