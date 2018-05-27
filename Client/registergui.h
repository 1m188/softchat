#ifndef REGISTERGUI_H
#define REGISTERGUI_H

#include <QDialog>
#include "QApplication"
#include "QDeskTopWidget"
#include "QLabel"
#include "QLineEdit"
#include "QPushButton"
#include "QMessageBox"

class RegisterGui : public QDialog
{
	Q_OBJECT

public:
	RegisterGui(QWidget *parent);
	~RegisterGui();

private:
	QLineEdit *nameLineEdit;
	QLineEdit *passwordLineEdit;

signals:
	void registerSignal(QString acountInfo);

private slots:
	void registerButtonClicked();

public slots:
	void registerSuccessSlot(QString id);
};

#endif // REGISTERGUI_H
