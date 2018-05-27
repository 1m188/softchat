#include "registergui.h"

RegisterGui::RegisterGui(QWidget *parent)
	: QDialog(parent), nameLineEdit(new QLineEdit(this)), passwordLineEdit(new QLineEdit(this))
{
	setAttribute(Qt::WA_QuitOnClose, false);
	setAttribute(Qt::WA_DeleteOnClose, true);
	setWindowFlags(Qt::WindowCloseButtonHint);

	setWindowTitle(tr(u8"×¢²á"));
	setFixedSize(600, 300);

	QRect rect = frameGeometry();
	rect.moveCenter(QApplication::desktop()->availableGeometry().center());
	move(rect.topLeft());

	//¿Ø¼þ+²¼¾Ö
	QLabel *infoLabel = new QLabel(this);
	infoLabel->setAlignment(Qt::AlignCenter);
	infoLabel->setFont(QFont(u8"Î¢ÈíÑÅºÚ", 12));
	infoLabel->setText(tr(u8"ÇëÊäÈëÒª×¢²áµÄêÇ³ÆºÍÃÜÂë"));
	infoLabel->resize(infoLabel->sizeHint());
	infoLabel->move(width() / 2 - infoLabel->width() / 2, height() / 6);

	QLabel *nameLabel = new QLabel(this);
	nameLabel->setAlignment(Qt::AlignCenter);
	nameLabel->setFont(QFont(u8"Î¢ÈíÑÅºÚ", 10));
	nameLabel->setText(tr(u8"êÇ³Æ"));
	nameLabel->resize(nameLabel->sizeHint());
	nameLabel->move(infoLabel->x() + infoLabel->width() / 2 - 150, infoLabel->y() + infoLabel->height() + 50);

	QLabel *passwordLabel = new QLabel(this);
	passwordLabel->setAlignment(Qt::AlignCenter);
	passwordLabel->setFont(QFont(u8"Î¢ÈíÑÅºÚ", 10));
	passwordLabel->setText(tr(u8"ÃÜÂë"));
	passwordLabel->resize(passwordLabel->sizeHint());
	passwordLabel->move(nameLabel->x(), nameLabel->y() + nameLabel->height() + 50);

	nameLineEdit->resize(nameLineEdit->sizeHint());
	nameLineEdit->move(width() / 2 + (width() / 2 - nameLabel->x() - nameLabel->width()) - 70, nameLabel->y());

	passwordLineEdit->setEchoMode(QLineEdit::Password);
	passwordLineEdit->resize(passwordLineEdit->sizeHint());
	passwordLineEdit->move(nameLineEdit->x(), passwordLabel->y());

	QPushButton *registerButton = new QPushButton(this);
	registerButton->setDefault(true);
	registerButton->setText(tr(u8"×¢²á"));
	registerButton->resize(registerButton->sizeHint());
	registerButton->move(width() - registerButton->width() - 10, height() - registerButton->height() - 10);
	connect(registerButton, &QPushButton::clicked, this, &RegisterGui::registerButtonClicked);
}

RegisterGui::~RegisterGui()
{

}

void RegisterGui::registerButtonClicked()
{
	if (nameLineEdit->text() == "" || passwordLineEdit->text() == "")
	{
		QMessageBox::warning(this, tr(u8"¾¯¸æ"), tr(u8"êÇ³Æ»òÃÜÂë²»¿ÉÎª¿Õ£¡"));
	}
	else
	{
		emit registerSignal(nameLineEdit->text() + ' ' + passwordLineEdit->text());
	}
}

void RegisterGui::registerSuccessSlot(QString id)
{
	QMessageBox::about(this, tr(u8"×¢²á³É¹¦"), tr(u8"ÄãµÄidÊÇ" + id.toUtf8()));
	nameLineEdit->clear();
	passwordLineEdit->clear();
}