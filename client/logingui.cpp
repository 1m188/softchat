#include "logingui.h"
#include "QApplication"
#include "QDeskTopWidget"
#include "QLabel"
#include "QPushButton"
#include "QMessageBox"
#include "registergui.h"
#include "data.h"

LoginGui::LoginGui(QWidget *parent)
	: QDialog(parent), idLineEdit(new QLineEdit(this)), passwordLineEdit(new QLineEdit(this))
{
	//界面基本设置
	setAttribute(Qt::WA_QuitOnClose, true);
	setAttribute(Qt::WA_DeleteOnClose, true);
	setWindowFlags(Qt::WindowCloseButtonHint);

	//设置标题和大小
	setWindowTitle(tr(u8"登陆"));
	setFixedSize(600, 300);

	//把界面移动到屏幕中央
	QRect rect = frameGeometry();
	rect.moveCenter(QApplication::desktop()->availableGeometry().center());
	move(rect.topLeft());

	//控件+布局
	QLabel *infoLabel = new QLabel(this);
	infoLabel->setAlignment(Qt::AlignCenter);
	infoLabel->setFont(QFont(u8"微软雅黑", 12));
	infoLabel->setText(tr(u8"请输入账号和密码登陆"));
	infoLabel->resize(infoLabel->sizeHint());
	infoLabel->move(width() / 2 - infoLabel->width() / 2, height() / 6);

	QLabel *idLabel = new QLabel(this);
	idLabel->setAlignment(Qt::AlignCenter);
	idLabel->setFont(QFont(u8"微软雅黑", 10));
	idLabel->setText(tr(u8"账号"));
	idLabel->resize(idLabel->sizeHint());
	idLabel->move(infoLabel->x() + infoLabel->width() / 2 - 150, infoLabel->y() + infoLabel->height() + 50);

	QLabel *passwordLabel = new QLabel(this);
	passwordLabel->setAlignment(Qt::AlignCenter);
	passwordLabel->setFont(QFont(u8"微软雅黑", 10));
	passwordLabel->setText(tr(u8"密码"));
	passwordLabel->resize(passwordLabel->sizeHint());
	passwordLabel->move(idLabel->x(), idLabel->y() + idLabel->height() + 50);

	idLineEdit->resize(idLineEdit->sizeHint());
	idLineEdit->move(width() / 2 + (width() / 2 - idLabel->x() - idLabel->width()) - 70, idLabel->y());

	passwordLineEdit->setEchoMode(QLineEdit::Password);
	passwordLineEdit->resize(passwordLineEdit->sizeHint());
	passwordLineEdit->move(idLineEdit->x(), passwordLabel->y());

	QPushButton *loginButton = new QPushButton(this);
	loginButton->setDefault(true);
	loginButton->setText(tr(u8"登陆"));
	loginButton->resize(loginButton->sizeHint());
	loginButton->move(width() - loginButton->width() - 10, height() - loginButton->height() - 10);
	connect(loginButton, &QPushButton::clicked, this, &LoginGui::loginButtonClicked);

	QPushButton *registerButton = new QPushButton(this);
	registerButton->setText(tr(u8"注册"));
	registerButton->resize(registerButton->sizeHint());
	registerButton->move(width() - loginButton->x() - loginButton->width(), loginButton->y());
	connect(registerButton, &QPushButton::clicked, this, &LoginGui::registerButtonClicked);

	Data *data = Data::getInstance();
	connect(this, &LoginGui::loginRequestSignal, data, &Data::loginRequestSlot); //登陆请求
	connect(this, &LoginGui::registerRequestSignal, data, &Data::registerRequestSlot); //注册请求
	connect(data, &Data::loginSignal, this, &LoginGui::accept); //使之登陆
	connect(data, &Data::loginFailedSignal, this, &LoginGui::loginFailedSlot); //登陆失败
	connect(data, &Data::loginRepeatSignal, this, &LoginGui::loginRepeatSlot); //重复登陆
	connect(data, &Data::registerSuccessSignal, this, &LoginGui::registerSuccessSignal); //注册成功
}

LoginGui::~LoginGui()
{

}

void LoginGui::loginButtonClicked()
{
	//如果id或密码有一个为空
	if (idLineEdit->text() == "" || passwordLineEdit->text() == "")
	{
		QMessageBox::warning(this, tr(u8"警告"), tr(u8"账号或密码不可为空！"));
	}
	else
	{
		//否则发送登陆信息
		emit loginRequestSignal(idLineEdit->text() + ' ' + passwordLineEdit->text());
	}
}

void LoginGui::registerButtonClicked()
{
	//注册界面
	RegisterGui *registerGui = new RegisterGui(nullptr);
	connect(registerGui, &RegisterGui::registerRequestSignal, this, &LoginGui::registerRequestSignal);
	connect(this, &LoginGui::registerSuccessSignal, registerGui, &RegisterGui::registerSuccessSlot);
	registerGui->exec();
}

void LoginGui::loginFailedSlot()
{
	QMessageBox::critical(this, tr(u8"登陆失败"), tr(u8"用户名或密码错误！"));
	passwordLineEdit->clear();
}

void LoginGui::loginRepeatSlot()
{
	QMessageBox::critical(this, tr(u8"登陆失败"), tr(u8"该用户已经登陆！"));
	passwordLineEdit->clear();
}