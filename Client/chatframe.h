#ifndef CHATFRAME_H
#define CHATFRAME_H

#include <QFrame>
#include "QTextEdit"
#include "QPushButton"
#include "QGridLayout"
#include "extern.h"

class ChatFrame : public QFrame
{
	Q_OBJECT

public:
	ChatFrame(QWidget *parent, UserInfo friendInfo);
	~ChatFrame();

private:
	UserInfo friendInfo;
	QTextEdit *sendTextEdit;
	QTextEdit *recvTextEdit;

public:
	const QString getID() const { return friendInfo.id; }
	const QString getName() const { return friendInfo.name; }

signals:
	void sendMsgSignal(QString msg, QString recverID);

private slots:
	void sendButtonClicked();

public slots:
	void getMsgSlot(QString msg, QString senderID);
};

#endif // CHATFRAME_H
