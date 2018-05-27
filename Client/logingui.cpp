#include "logingui.h"

LoginGui::LoginGui(QWidget *parent)
	: QDialog(parent), idLineEdit(new QLineEdit(this)), passwordLineEdit(new QLineEdit(this))
{
	setAttribute(Qt::WA_QuitOnClose, true);
	setAttribute(Qt::WA_DeleteOnClose, true);
	setWindowFlags(Qt::WindowCloseButtonHint);

	setWindowTitle(tr(u8"µÇÂ½"));
	setFixedSize(600, 300);

	QRect rect = frameGeometry();
	rect.moveCenter(QApplication::desktop()->availableGeometry().center());
	move(rect.topLeft());

	//¿Ø¼þ+²¼¾Ö
	QLabel *infoLabel = new QLabel(this);
	infoLabel->setAlignment(Qt::AlignCenter);
	infoLabel->setFont(QFont(u8"Î¢ÈíÑÅºÚ", 12));
	infoLabel->setText(tr(u8"ÇëÊäÈëÕËºÅºÍÃÜÂëµÇÂ½"));
	infoLabel->resize(infoLabel->sizeHint());
	infoLabel->move(width() / 2 - infoLabel->width() / 2, height() / 6);

	QLabel *idLabel = new QLabel(this);
	idLabel->setAlignment(Qt::AlignCenter);
	idLabel->setFont(QFont(u8"Î¢ÈíÑÅºÚ", 10));
	idLabel->setText(tr(u8"ÕËºÅ"));
	idLabel->resize(idLabel->sizeHint());
	idLabel->move(infoLabel->x() + infoLabel->width() / 2 - 150, infoLabel->y() + infoLabel->height() + 50);

	QLabel *passwordLabel = new QLabel(this);
	passwordLabel->setAlignment(Qt::AlignCenter);
	passwordLabel->setFont(QFont(u8"Î¢ÈíÑÅºÚ", 10));
	passwordLabel->setText(tr(u8"ÃÜÂë"));
	passwordLabel->resize(passwordLabel->sizeHint());
	passwordLabel->move(idLabel->x(), idLabel->y() + idLabel->height() + 50);

	idLineEdit->resize(idLineEdit->sizeHint());
	idLineEdit->move(width() / 2 + (width() / 2 - idLabel->x() - idLabel->width()) - 70, idLabel->y());

	passwordLineEdit->setEchoMode(QLineEdit::Password);
	passwordLineEdit->resize(passwordLineEdit->sizeHint());
	passwordLineEdit->move(idLineEdit->x(), passwordLabel->y());

	QPushButton *loginButton = new QPushButton(this);
	loginButton->setDefault(true);
	loginButton->setText(tr(u8"µÇÂ½"));
	loginButton->resize(loginButton->sizeHint());
	loginButton->move(width() - loginButton->width() - 10, height() - loginButton->height() - 10);
	connect(loginButton, &QPushButton::clicked, this, &LoginGui::loginButtonClicked);

	QPushButton *registerButton = new QPushButton(this);
	registerButton->setText(tr(u8"×¢²á"));
	registerButton->resize(registerButton->sizeHint());
	registerButton->move(width() - loginButton->x() - loginButton->width(), loginButton->y());
	connect(registerButton, &QPushButton::clicked, this, &LoginGui::registerButtonClicked);
}

LoginGui::~LoginGui()
{

}

void LoginGui::loginButtonClicked()
{
	if (idLineEdit->text() == "" || passwordLineEdit->text() == "")
	{
		QMessageBox::warning(this, tr(u8"¾¯¸æ"), tr(u8"ÕËºÅ»òÃÜÂë²»¿ÉÎª¿Õ£¡"));
	}
	else
	{
		emit loginSignal(idLineEdit->text() + ' ' + passwordLineEdit->text());
	}
}

void LoginGui::registerButtonClicked()
{
	RegisterGui *registerGui = new RegisterGui(nullptr);
	connect(registerGui, &RegisterGui::registerSignal, this, &LoginGui::registerSignal);
	connect(this, &LoginGui::registerSuccessSignal, registerGui, &RegisterGui::registerSuccessSlot);
	registerGui->exec();
}

void LoginGui::loginFailedSlot()
{
	QMessageBox::critical(this, tr(u8"µÇÂ½Ê§°Ü"), tr(u8"ÓÃ»§Ãû»òÃÜÂë´íÎó£¡"));
	passwordLineEdit->clear();
}

void LoginGui::loginRepeatSlot()
{
	QMessageBox::critical(this, tr(u8"µÇÂ½Ê§°Ü"), tr(u8"¸ÃÓÃ»§ÒÑ¾­µÇÂ½£¡"));
	passwordLineEdit->clear();
}