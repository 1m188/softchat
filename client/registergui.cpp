#include "registergui.h"
#include "QApplication"
#include "QDeskTopWidget"
#include "QLabel"
#include "QPushButton"
#include "QMessageBox"

RegisterGui::RegisterGui(QWidget *parent)
	: QDialog(parent), nameLineEdit(new QLineEdit(this)), passwordLineEdit(new QLineEdit(this))
{
	//界面基本设置
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);
	setWindowFlags(Qt::WindowCloseButtonHint);

	//设置标题和大小
	setWindowTitle(tr(u8"注册"));
	setFixedSize(600, 300);

	//移动到屏幕中央
	QRect rect = frameGeometry();
	rect.moveCenter(QApplication::desktop()->availableGeometry().center());
	move(rect.topLeft());

	//控件+布局
	QLabel *infoLabel = new QLabel(this);
	infoLabel->setAlignment(Qt::AlignCenter);
	infoLabel->setFont(QFont(u8"微软雅黑", 12));
	infoLabel->setText(tr(u8"请输入要注册的昵称和密码"));
	infoLabel->resize(infoLabel->sizeHint());
	infoLabel->move(width() / 2 - infoLabel->width() / 2, height() / 6);

	QLabel *nameLabel = new QLabel(this);
	nameLabel->setAlignment(Qt::AlignCenter);
	nameLabel->setFont(QFont(u8"微软雅黑", 10));
	nameLabel->setText(tr(u8"昵称"));
	nameLabel->resize(nameLabel->sizeHint());
	nameLabel->move(infoLabel->x() + infoLabel->width() / 2 - 150, infoLabel->y() + infoLabel->height() + 50);

	QLabel *passwordLabel = new QLabel(this);
	passwordLabel->setAlignment(Qt::AlignCenter);
	passwordLabel->setFont(QFont(u8"微软雅黑", 10));
	passwordLabel->setText(tr(u8"密码"));
	passwordLabel->resize(passwordLabel->sizeHint());
	passwordLabel->move(nameLabel->x(), nameLabel->y() + nameLabel->height() + 50);

	nameLineEdit->resize(nameLineEdit->sizeHint());
	nameLineEdit->move(width() / 2 + (width() / 2 - nameLabel->x() - nameLabel->width()) - 70, nameLabel->y());

	passwordLineEdit->setEchoMode(QLineEdit::Password);
	passwordLineEdit->resize(passwordLineEdit->sizeHint());
	passwordLineEdit->move(nameLineEdit->x(), passwordLabel->y());

	QPushButton *registerButton = new QPushButton(this);
	registerButton->setDefault(true);
	registerButton->setText(tr(u8"注册"));
	registerButton->resize(registerButton->sizeHint());
	registerButton->move(width() - registerButton->width() - 10, height() - registerButton->height() - 10);
	connect(registerButton, &QPushButton::clicked, this, &RegisterGui::registerButtonClicked);
}

RegisterGui::~RegisterGui()
{

}

void RegisterGui::registerButtonClicked()
{
	//如果注册昵称或密码为空
	if (nameLineEdit->text() == "" || passwordLineEdit->text() == "")
	{
		QMessageBox::warning(this, tr(u8"警告"), tr(u8"昵称或密码不可为空！"));
	}
	else
	{
		//否则发送注册信息
		emit registerRequestSignal(nameLineEdit->text() + ' ' + passwordLineEdit->text());
	}
}

void RegisterGui::registerSuccessSlot(QString id)
{
	QMessageBox::about(this, tr(u8"注册成功"), tr(u8"你的id是" + id.toUtf8()));
	nameLineEdit->clear();
	passwordLineEdit->clear();
}