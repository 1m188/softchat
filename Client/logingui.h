#ifndef LOGINGUI_H
#define LOGINGUI_H

#include <QDialog>
#include "QApplication"
#include "QDeskTopWidget"
#include "QLabel"
#include "QLineEdit"
#include "QPushButton"
#include "QMessageBox"
#include "registergui.h"

class LoginGui : public QDialog
{
	Q_OBJECT

public:
	LoginGui(QWidget *parent);
	~LoginGui();

private:
	QLineEdit *idLineEdit; //id ‰»ÎøÚ
	QLineEdit *passwordLineEdit; //√‹¬Î ‰»ÎøÚ

signals:
	void loginSignal(QString acountInfo);
	void registerSignal(QString acountInfo);
	void registerSuccessSignal(QString id);

private slots:
	void loginButtonClicked();
	void registerButtonClicked();

public slots:
	void loginFailedSlot();
	void loginRepeatSlot();
};

#endif // LOGINGUI_H
