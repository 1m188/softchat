#include "chatframe.h"
#include "QPushButton"
#include "QGridLayout"
#include "QEvent"
#include "QKeyEvent"

ChatFrame::ChatFrame(QWidget *parent, UserInfo friendInfo)
	: QFrame(parent), friendInfo(friendInfo), sendTextEdit(new QTextEdit(this)), recvTextEdit(new QTextEdit(this))
{
	//控件
	recvTextEdit->setReadOnly(true); //将接收消息文本框设置为只读
	sendTextEdit->installEventFilter(this); //安装事件过滤器，以实现回车键发送消息，其他键+回车键换行的功能

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

bool ChatFrame::eventFilter(QObject * watched, QEvent * event)
{
	//如果当前焦点聚焦到发送文本框
	if (watched == sendTextEdit)
	{
		//如果事件为按键
		if (event->type() == QEvent::KeyPress)
		{
			QKeyEvent *key = static_cast<QKeyEvent *>(event);
			//如果按下的是回车键且没有任何修饰键
			if (key->modifiers() == Qt::KeyboardModifier::NoModifier && key->key() == Qt::Key_Return)
			{
				//则触发发送消息
				sendButtonClicked();
				return true;
			}
			//否则如果按下的键是回车
			else if (key->key() == Qt::Key_Return)
			{
				key->setModifiers(Qt::KeyboardModifier::NoModifier); //取消修饰键，以让发送文本框执行换行操作
				return false;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	//否则交给父类的相关函数实现
	else
	{
		return QWidget::eventFilter(watched, event);
	}
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