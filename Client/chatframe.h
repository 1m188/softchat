#ifndef CHATFRAME_H
#define CHATFRAME_H

#include <QFrame>
#include "QTextEdit"
#include "extern.h"

//聊天面板
class ChatFrame : public QFrame
{
	Q_OBJECT

public:
	ChatFrame(QWidget *parent, UserInfo friendInfo);
	~ChatFrame();

private:
	UserInfo friendInfo; //该聊天面板代表的好友的用户信息
	QTextEdit *sendTextEdit; //发送消息的textedit
	QTextEdit *recvTextEdit; //接收消息的textedit

public:
	const QString getID() const { return friendInfo.id; } //获取该好友id
	const QString getName() const { return friendInfo.name; } //获取该好友昵称

signals:
	void sendMsgSignal(QString msg, QString recverID); //发送聊天消息

	private slots:
	void sendButtonClicked(); //单击发送按钮

	public slots:
	void getMsgSlot(QString msg, QString senderID); //接收好友的聊天消息
};

#endif // CHATFRAME_H
