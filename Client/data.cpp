#include "data.h"

Data::Data(QObject *parent)
	: QObject(parent), connectToServer(nullptr), myInfo(nullptr)
{
	QThread *thread = new QThread(this);
	moveToThread(thread);
	connect(thread, &QThread::started, this, &Data::init);
	thread->start();
}

Data::~Data()
{
	connectToServer->write(QString("disconnect").toUtf8());
	connectToServer->waitForBytesWritten();

	delete myInfo;
}

void Data::addSignalSlotsForClassSlot(LoginGui *loginGui)
{
	connect(loginGui, &LoginGui::loginSignal, this, &Data::loginSlot);
	connect(loginGui, &LoginGui::registerSignal, this, &Data::registerSlot);
	connect(this, &Data::loginSignal, loginGui, &LoginGui::accept);
	connect(this, &Data::loginFailedSignal, loginGui, &LoginGui::loginFailedSlot);
	connect(this, &Data::loginRepeatSignal, loginGui, &LoginGui::loginRepeatSlot);
	connect(this, &Data::registerSuccessSignal, loginGui, &LoginGui::registerSuccessSignal);
}

void Data::addSignalSlotsForClassSlot(MainGui *mainGui)
{
	connect(mainGui, &MainGui::sendMsgSignal, this, &Data::sendMsgSlot);
	connect(this, &Data::getMyInfoSignal, mainGui, &MainGui::getMyInfoSlot);
	connect(this, &Data::getFriendListSignal, mainGui, &MainGui::updateFriendList);
	connect(this, &Data::getMsgSignal, mainGui, &MainGui::getMsgSlot);

	connectToServer->write(QString("myInfo").toUtf8());
	connectToServer->waitForBytesWritten();
	connectToServer->waitForReadyRead();
	connectToServer->write(QString("friendList").toUtf8());
	connectToServer->waitForBytesWritten();
}

void Data::init()
{
	connect(this, static_cast<void (Data::*) (LoginGui *)>(&Data::addSignalSlotsForClassSignal), this, static_cast<void (Data::*) (LoginGui *)>(&Data::addSignalSlotsForClassSlot));
	connect(this, static_cast<void (Data::*) (MainGui *)>(&Data::addSignalSlotsForClassSignal), this, static_cast<void (Data::*) (MainGui *)>(&Data::addSignalSlotsForClassSlot));

	connectToServer = new QTcpSocket(this);
	connect(connectToServer, &QTcpSocket::readyRead, this, &Data::getMsgFromServer);
	connectToServer->connectToHost("127.0.0.1", 8888);

	myInfo = new UserInfo();
}

void Data::getMsgFromServer()
{
	QString msg = QString::fromUtf8(connectToServer->readAll());

	qDebug() << msg;

	if (msg == "loginSuccess")
	{
		emit loginSignal();
	}
	else if (msg == "loginFailed")
	{
		emit loginFailedSignal();
	}
	else if (msg == "loginRepeat")
	{
		emit loginRepeatSignal();
	}
	else
	{
		QStringList msgList = msg.split(' ');

		if (msgList[0] == "registerSuccess")
		{
			emit registerSuccessSignal(msgList[1]);
		}
		else if (msgList[0] == "myInfo")
		{
			myInfo->id = msgList[1];
			myInfo->name = msgList[2];
			emit getMyInfoSignal(msgList[1], msgList[2]);
		}
		else if (msgList[0] == "friendList")
		{
			msgList.pop_front();
			emit getFriendListSignal(msgList);
		}
		else if (msgList[0] == "Message")
		{
			QString senderID = msgList[1];
			QString recverID = msgList[2];
			msgList.pop_front();
			msgList.pop_front();
			msgList.pop_front();
			QString msg = msgList.join(' ');
			emit getMsgSignal(msg, senderID);
		}
	}
}

void Data::loginSlot(QString acountInfo)
{
	QStringList acountInfoList = acountInfo.split(' ');

	//粘包解决！
	QString msg = QString("login") + ' ' + acountInfoList[0] + ' ' + acountInfoList[1]; //要发送的信息
	QByteArray sendMsg; //发送的包
	QDataStream out(&sendMsg, QIODevice::WriteOnly); //用来操纵包的QDataStream
	out.setByteOrder(QDataStream::BigEndian);
	out << static_cast<qint32>(msg.size()) << msg; //读入一个4字节长度的东西占位，是之后发送的消息的长度，然后写入发送的消息
	connectToServer->write(sendMsg); //发送~
}

void Data::registerSlot(QString acountInfo)
{
	QStringList acountInfoList = acountInfo.split(' ');
	connectToServer->write((QString("register") + ' ' + acountInfoList[0] + ' ' + acountInfoList[1]).toUtf8());
	connectToServer->waitForBytesWritten();
}

void Data::sendMsgSlot(QString msg, QString recverID)
{
	connectToServer->write((QString("Message") + ' ' + myInfo->id + ' ' + recverID + ' ' + msg).toUtf8());
	connectToServer->waitForBytesWritten();
}