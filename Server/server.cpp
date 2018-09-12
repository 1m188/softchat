#include "server.h"
#include "QDebug"

Server::Server(QObject *parent)
	: QTcpServer(parent), threadPool({}), database(QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"))), query(QSqlQuery(database))
{
	//初始化本地连接数据库
	database.setDatabaseName("softchat");
	database.setHostName("127.0.0.1");
	database.setPort(3306);
	database.setUserName("root");
	database.setPassword("123456");
	database.open();

	//开始监听本地ip，8888端口
	listen(QHostAddress::Any, 8888);
	qDebug() << u8"开始监听本地IP，8888端口...";
}

Server::~Server()
{
	//释放数据库连接资源
	query.clear();
	database.close();
}

void Server::incomingConnection(qintptr handle)
{
	ClientThread *clientThread = new ClientThread(nullptr, handle);
	connect(clientThread, &ClientThread::disconnectToClientSignal, this, &Server::disconnectToClientSlot); //断开连接则转到相应处理函数
	connect(clientThread, &ClientThread::getMsgFromClientSignal, this, &Server::getMsgFromClientSlot); //获得消息
	threadPool.push_back(clientThread); //加入线程池
	clientThread->start(); //启动线程

	qDebug() << QString("a new connection with handle %1, the num of the client is %2").arg(handle).arg(threadPool.size());
}

void Server::disconnectToClientSlot()
{
	ClientThread *sender = dynamic_cast<ClientThread *>(this->sender());
	sender->exit(); //退出线程
	threadPool.removeOne(sender); //从线程池中删除
	sender->deleteLater(); //释放资源

	qDebug() << QString("a connection disconnect, the num of the client is %1").arg(threadPool.size());
}

void Server::getMsgFromClientSlot(QString msg)
{
	//获取发送方
	ClientThread *sender = dynamic_cast<ClientThread *>(this->sender());
	//输出连接相关信息
	qDebug() << sender->getID() << "id send message" << msg;
	//获取消息列表
	QStringList msgList = msg.split(' ');

	//判定
	//登陆请求
	if (msgList[0] == "login")
	{
		loginRequestHandle(sender, msgList);
	}
	//注册请求
	else if (msgList[0] == "register")
	{
		registerRequestHandle(sender, msgList);
	}
	//自己的用户信息请求
	else if (msgList[0] == "myInfo")
	{
		myInfoRequestHandle(sender, msgList);
	}
	//好友列表请求
	else if (msgList[0] == "friendList")
	{
		friendListRequestHandle(sender, msgList);
	}
	//发送聊天消息
	else if (msgList[0] == "Message")
	{
		messageHandle(sender, msgList);
	}
}

void Server::loginRequestHandle(ClientThread * sender, QStringList msgList)
{
	//获取登陆id和密码
	QString id = msgList[1];
	QString password = msgList[2];

	//验证是否已经登陆
	for (ClientThread *c : threadPool)
	{
		if (c->getID() == id)
		{
			emit sender->sendMsgToClientSignal("loginRepeat"); //重复登陆
			return;
		}
	}

	//验证登陆是否正确
	query.exec(QString("select * from acount where id='%1' and password='%2'").arg(id).arg(password));
	if (query.next())
	{
		emit sender->sendMsgToClientSignal("loginSuccess"); //登陆成功
		sender->setID(id);
	}
	else
	{
		emit sender->sendMsgToClientSignal("loginFailed"); //登陆失败
	}
}

void Server::registerRequestHandle(ClientThread * sender, QStringList msgList)
{
	//获取注册的昵称和密码
	QString name = msgList[1];
	QString password = msgList[2];
	//获取新的id
	query.exec("select * from acount");
	QString id = QString::number(query.size() + 1);

	query.exec(QString("insert into acount values('%1','%2','%3')").arg(id).arg(name).arg(password)); //插入用户相关信息到用户表中
	query.exec(QString("create table friend%1(friendid char(10) not null);").arg(id)); //建立一个用户的好友表
	emit sender->sendMsgToClientSignal(QString("registerSuccess") + ' ' + id); //注册成功
}

void Server::myInfoRequestHandle(ClientThread * sender, QStringList msgList)
{
	//获取发送方id
	QString id = sender->getID();
	query.exec(QString("select name from acount where id='%1';").arg(id)); //查询自己id对应的id和昵称作为自己的用户信息返回
	//整理发送的返回消息
	QString myInfo = "myInfo";
	myInfo += ' ';
	myInfo += id;
	myInfo += ' ';
	if (query.next())
	{
		myInfo += query.value("name").toString();
	}
	//发送消息
	emit sender->sendMsgToClientSignal(myInfo);
}

void Server::friendListRequestHandle(ClientThread * sender, QStringList msgList)
{
	QString id = sender->getID(); //获取发送方id
	query.exec(QString("select id,name from acount,friend%1 where acount.id=friend%2.friendid;").arg(id).arg(id)); //查询相应id的好友表中的所有的好友的id和昵称
	//整理发送返回的消息
	QString friendList = "friendList";
	while (query.next())
	{
		friendList += ' ';
		friendList += query.value("id").toString();
		friendList += '_';
		friendList += query.value("name").toString();
	}
	//发送
	emit sender->sendMsgToClientSignal(friendList);
}

void Server::messageHandle(ClientThread * sender, QStringList msgList)
{
	//获取接收方id
	QString recvID = msgList[2];
	//在连接上的客户端池中查找接收方
	for (ClientThread *c : threadPool)
	{
		if (c->getID() == recvID)
		{
			emit c->sendMsgToClientSignal(msgList.join(' '));
			return;
		}
	}
	//如果没有上线的话就发送
	emit sender->sendMsgToClientSignal(u8"该用户目前没有上线，你的消息可能无法到达！");
}