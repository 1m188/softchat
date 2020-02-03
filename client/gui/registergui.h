#ifndef REGISTERGUI_H
#define REGISTERGUI_H

#include "QDialog"
#include "QLineEdit"

//注册界面
class RegisterGui : public QDialog
{
	Q_OBJECT

public:
	RegisterGui(QWidget *parent);
	~RegisterGui();

private:
	QLineEdit *nameLineEdit; //昵称输入框
	QLineEdit *passwordLineEdit; //密码输入框

signals:
	void registerRequestSignal(QString acountInfo); //注册请求

	private slots:
	void registerButtonClicked(); //注册按钮按下

	public slots:
	void registerSuccessSlot(QString id); //注册成功响应
};

#endif // REGISTERGUI_H
