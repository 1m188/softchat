#include <QtWidgets/QApplication>
#include "data.h"
#include "logingui.h"
#include "maingui.h"

int main(int argc, char *argv[])
{
#ifdef NDEBUG
	QApplication::addLibraryPath("./plugins"); //做发布版本时要加上
#endif // NDEBUG

	QApplication a(argc, argv);

	Data *data = new Data(nullptr); //初始化数据类

	//实例化登陆界面并且和数据类建立联系
	LoginGui *loginGui = new LoginGui(nullptr);
	data->addSignalSlots(loginGui);
	loginGui->show();

	//判定对话框显示结果
	if (loginGui->exec() == QDialog::Accepted)
	{
		//如果位Accept则出现主界面，连接数据类并且进入正式的事件循环
		MainGui *mainGui = new MainGui(nullptr);
		data->addSignalSlots(mainGui);
		mainGui->show();
		a.exec();
	}
	delete data;
	return 0;
}
