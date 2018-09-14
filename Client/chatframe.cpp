#include "chatframe.h"
#include "QPushButton"
#include "QGridLayout"

ChatFrame::ChatFrame(QWidget *parent, UserInfo friendInfo)
	: QFrame(parent), friendInfo(friendInfo), sendTextEdit(new QTextEdit(this)), recvTextEdit(new QTextEdit(this))
{
	//控件
	recvTextEdit->setReadOnly(true);

	QPushButton *sendButton = new QPushButton(this);
	sendButton->setDefault(true);
	sendButton->setFont(QFont(u8"微软雅黑", 10));
	sendButton->setText(tr(u8"发送"));
	sendButton->resize(sendButton->sizeHint());
	connect(sendButton, &QPushButton::clicked, this, &ChatFrame::sendButtonClicked);

	//布局
	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(recvTextEdit, 0, 0, 10, 10);
	layout->addWidget(sendTextEdit, 10, 0, 5, 10);
	layout->addWidget(sendButton, 15, 9, 1, 1);
}

ChatFrame::~ChatFrame()
{

}

void ChatFrame::sendButtonClicked()
{
	//获取要发送的消息
	QString msg = sendTextEdit->toPlainText();
	//如果不为空
	if (msg != "")
	{
		//更新接收消息文本框
		recvTextEdit->append(u8"我: " + msg);
		//发送
		emit sendMsgSignal(msg, friendInfo.id);
		//清空发送文本框
		sendTextEdit->clear();
	}
}

void ChatFrame::getMsgSlot(QString msg, QString senderID)
{
	//如果这个消息是发给自己的
	if (senderID == friendInfo.id)
	{
		//更新接收消息文本框
		recvTextEdit->append(friendInfo.name + ": " + msg);
	}
}