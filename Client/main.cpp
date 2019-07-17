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

	Data::getInstance(); //初始化数据类

	//实例化登陆界面并且和数据类建立联系
	LoginGui *loginGui = new LoginGui(nullptr);
	loginGui->show();

	//判定对话框显示结果
	if (loginGui->exec() == QDialog::Accepted)
	{
		//如果位Accept则出现主界面，进入正式的事件循环
		MainGui *mainGui = new MainGui(nullptr);
		mainGui->show();
		a.exec();
	}
	return 0;
}
