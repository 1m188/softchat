#include "data.h"

Data::Data(QObject *parent)
	: QObject(parent), connectToServer(nullptr), myInfo(nullptr)
{
	QThread *thread = new QThread();
	moveToThread(thread);
	connect(thread, &QThread::started, this, &Data::init);
	connect(this, &Data::destroyed, thread, &QThread::quit);
	thread->start();
}

Data::~Data()
{
	delete myInfo;
}

void Data::init()
{
	connect(this, static_cast<void (Data::*) (LoginGui *)>(&Data::addSignalSlotsForClassSignal), this, static_cast<void (Data::*) (LoginGui *)>(&Data::addSignalSlotsForClassSlot));
	connect(this, static_cast<void (Data::*) (MainGui *)>(&Data::addSignalSlotsForClassSignal), this, static_cast<void (Data::*) (MainGui *)>(&Data::addSignalSlotsForClassSlot));

	connectToServer = new TcpSocket(this);
	connect(connectToServer, &TcpSocket::getMsgSignal, this, &Data::getMsgFromServer);
	connectToServer->connectToHost("127.0.0.1", 8888);

	myInfo = new UserInfo();
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

	connectToServer->writeMsg("myInfo");
	connectToServer->writeMsg("friendList");
}

void Data::getMsgFromServer(QString msg)
{
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
	connectToServer->writeMsg(QString("login") + ' ' + acountInfoList[0] + ' ' + acountInfoList[1]);
}

void Data::registerSlot(QString acountInfo)
{
	QStringList acountInfoList = acountInfo.split(' ');
	connectToServer->writeMsg(QString("register") + ' ' + acountInfoList[0] + ' ' + acountInfoList[1]);
}

void Data::sendMsgSlot(QString msg, QString recverID)
{
	connectToServer->writeMsg(QString("Message") + ' ' + myInfo->id + ' ' + recverID + ' ' + msg);
}