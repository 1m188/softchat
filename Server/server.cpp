#include "Server.h"
#include "QDebug"

Server::Server(QObject *parent) :QWebSocketServer("", QWebSocketServer::NonSecureMode)
{
	//初始化本地连接数据库
	database.setDatabaseName("softchat.db");
	database.open();

	//创建acount用户信息表
	query.prepare("create table acount(id char(10) not null,name char(50) not null,password char(50) not null);");
	query.exec();

	//新的连接到来的时候
	connect(this, &Server::newConnection, this, &Server::onNewConnection);

	//开始监听本地ip，8888端口
	listen(QHostAddress::Any, 8888);
	qDebug() << "start listen at localhost with port 8888...";
}

Server::~Server()
{
	//释放数据库连接资源
	query.clear();
	database.close();
}

void Server::onNewConnection()
{
	QWebSocket *connectToClient = nextPendingConnection();
	//资源管理
	connect(this, &Server::destroyed, connectToClient, &QWebSocket::deleteLater);
	//获取消息
	connect(connectToClient, &QWebSocket::textMessageReceived, this, &Server::getMsgFromClientSlot);
	//断开连接处理
	connect(connectToClient, &QWebSocket::disconnected, this, &Server::disconnectToClientSlot);
	//加入连接池
	connectionPool.append(connectToClient);

	qDebug() << QString("a new connection from %1:%2 and connectionPool's size is %3").arg(connectToClient->peerAddress().toString()).arg(connectToClient->peerPort()).arg(connectionPool.size());
}

void Server::disconnectToClientSlot()
{
	QWebSocket *sender = dynamic_cast<QWebSocket *>(this->sender());
	sender->close();
	connectionPool.removeOne(sender); //从连接池中删除
	sender->deleteLater(); //释放资源

	qDebug() << QString("a connection disconnect, the num of the client is %1").arg(connectionPool.size());
}

void Server::getMsgFromClientSlot(QString msg)
{
	//获取发送方
	QWebSocket *sender = dynamic_cast<QWebSocket *>(this->sender());
	//输出连接相关信息
	qDebug() << sender->objectName() << "id send message" << msg;
	//获取消息列表
	QStringList msgList = msg.split(' ');

	//判定
	//登陆请求
	if (msgList[0] == "LoginRequest")
	{
		loginRequestHandle(sender, msgList);
	}
	//注册请求
	else if (msgList[0] == "RegisterRequest")
	{
		registerRequestHandle(sender, msgList);
	}
	//自己的用户信息请求
	else if (msgList[0] == "MyInfoRequest")
	{
		myInfoRequestHandle(sender, msgList);
	}
	//好友列表请求
	else if (msgList[0] == "FriendListRequest")
	{
		friendListRequestHandle(sender, msgList);
	}
	//发送聊天消息
	else if (msgList[0] == "Message")
	{
		messageHandle(sender, msgList);
	}
	//添加好友请求
	else if (msgList[0] == "AddFriendRequest")
	{
		addFriendRequestHandle(sender, msgList);
	}
	//删除好友请求
	else if (msgList[0] == "DelFriendRequest")
	{
		delFriendRequestHandle(sender, msgList);
	}
}

void Server::loginRequestHandle(QWebSocket * sender, QStringList msgList)
{
	//获取登陆id和密码
	QString id = msgList[1];
	QString password = msgList[2];

	//验证是否已经登陆
	for (QWebSocket *c : connectionPool)
	{
		if (c->objectName() == id)
		{
			sender->sendTextMessage("LoginRepeat"); //重复登陆
			return;
		}
	}

	//验证登陆是否正确
	query.exec(QString("select * from acount where id='%1' and password='%2'").arg(id).arg(password));
	if (query.next())
	{
		sender->sendTextMessage("LoginSuccess"); //登陆成功
		sender->setObjectName(id);
	}
	else
	{
		sender->sendTextMessage("LoginFailed"); //登陆失败
	}
}

void Server::registerRequestHandle(QWebSocket * sender, QStringList msgList)
{
	//获取注册的昵称和密码
	QString name = msgList[1];
	QString password = msgList[2];
	//获取新的id
	query.exec("select * from acount");
	QString id;
	if (query.last())
	{
		id = QString::number(query.value(0).toInt() + 1);
	}
	else
	{
		id = "1";
	}

	query.exec(QString("insert into acount values('%1','%2','%3')").arg(id).arg(name).arg(password)); //插入用户相关信息到用户表中
	query.exec(QString("create table friend%1(friendid char(10) not null);").arg(id)); //建立一个用户的好友表
	sender->sendTextMessage(QString("RegisterSuccess") + ' ' + id); //注册成功
}

void Server::myInfoRequestHandle(QWebSocket * sender, QStringList msgList)
{
	//获取发送方id
	QString id = sender->objectName();
	query.exec(QString("select name from acount where id='%1';").arg(id)); //查询自己id对应的id和昵称作为自己的用户信息返回
	//整理发送的返回消息
	QString myInfo = "MyInfo";
	myInfo += ' ';
	myInfo += id;
	myInfo += ' ';
	if (query.next())
	{
		myInfo += query.value("name").toString();
	}
	//发送消息
	sender->sendTextMessage(myInfo);
}

void Server::friendListRequestHandle(QWebSocket * sender, QStringList msgList)
{
	QString id = sender->objectName(); //获取发送方id
	query.exec(QString("select id,name from acount,friend%1 where acount.id=friend%2.friendid;").arg(id).arg(id)); //查询相应id的好友表中的所有的好友的id和昵称
	//整理发送返回的消息
	QString friendList = "FriendList";
	while (query.next())
	{
		friendList += ' ';
		friendList += query.value("id").toString();
		friendList += '_';
		friendList += query.value("name").toString();
	}
	//发送
	sender->sendTextMessage(friendList);
}

void Server::messageHandle(QWebSocket * sender, QStringList msgList)
{
	//获取接收方id
	QString recvID = msgList[2];
	//在连接上的客户端池中查找接收方
	for (QWebSocket *c : connectionPool)
	{
		//如果在的话
		if (c->objectName() == recvID)
		{
			//原封不动发送消息
			c->sendTextMessage(msgList.join(' '));
			return;
		}
	}
	//如果没有上线的话就向发送方发送相关提示信息
	sender->sendTextMessage(QString("Message %1 %2 %3").arg(recvID).arg(msgList[1]).arg(tr(u8"该用户目前没有上线，你的消息可能无法到达！")));
}

void Server::addFriendRequestHandle(QWebSocket * sender, QStringList msgList)
{
	//获取被请求添加方id
	QString friendID = msgList[1];

	//验证是否有这个用户
	query.exec(QString("select id from acount where id='%1';").arg(friendID));
	//如果没有这个用户的话
	if (!query.next())
	{
		sender->sendTextMessage("NoThisUser");
	}
	else
	{
		//否则给双方添加好友
		query.exec(QString("insert into friend%1 values('%2');").arg(sender->objectName()).arg(friendID));
		query.exec(QString("insert into friend%1 values('%2');").arg(friendID).arg(sender->objectName()));

		//给发送方更新好友列表
		friendListRequestHandle(sender, QString("FriendListRequest").split(' '));

		//如果被添加方也在线的话，则也给被添加方更新好友列表
		for (int i = 0; i < connectionPool.count(); i++)
		{
			if (connectionPool[i]->objectName() == friendID)
			{
				friendListRequestHandle(connectionPool[i], QString("FriendListRequest").split(' '));
				break;
			}
		}
	}
}

void Server::delFriendRequestHandle(QWebSocket * sender, QStringList msgList)
{
	//获取要被删除的好友id
	QString friendID = msgList[1];

	//给请求方和被请求方互相删除
	query.exec(QString("delete from friend%1 where friendid='%2';").arg(sender->objectName()).arg(friendID));
	query.exec(QString("delete from friend%1 where friendid='%2';").arg(friendID).arg(sender->objectName()));

	//如果被请求方在线的话，给被请求方更新好友列表
	for (int i = 0; i < connectionPool.count(); i++)
	{
		if (connectionPool[i]->objectName() == friendID)
		{
			friendListRequestHandle(connectionPool[i], QString("FriendListRequest").split(' '));
			break;
		}
	}
}