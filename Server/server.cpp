#include "Server.h"
#include "QDebug"

Server::Server(QObject *parent)
	: QTcpServer(parent)
{
	//初始化本地连接数据库
	database.setDatabaseName("softchat.db");
	database.open();

	//创建acount用户信息表
	query.prepare("create table acount(id char(10) not null,name char(50) not null,password char(50) not null);");
	query.exec();

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
			emit sender->sendMsgToClientSignal("LoginRepeat"); //重复登陆
			return;
		}
	}

	//验证登陆是否正确
	query.exec(QString("select * from acount where id='%1' and password='%2'").arg(id).arg(password));
	if (query.next())
	{
		emit sender->sendMsgToClientSignal("LoginSuccess"); //登陆成功
		sender->setID(id);
	}
	else
	{
		emit sender->sendMsgToClientSignal("LoginFailed"); //登陆失败
	}
}

void Server::registerRequestHandle(ClientThread * sender, QStringList msgList)
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
	emit sender->sendMsgToClientSignal(QString("RegisterSuccess") + ' ' + id); //注册成功
}

void Server::myInfoRequestHandle(ClientThread * sender, QStringList msgList)
{
	//获取发送方id
	QString id = sender->getID();
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
	emit sender->sendMsgToClientSignal(myInfo);
}

void Server::friendListRequestHandle(ClientThread * sender, QStringList msgList)
{
	QString id = sender->getID(); //获取发送方id
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
	emit sender->sendMsgToClientSignal(friendList);
}

void Server::messageHandle(ClientThread * sender, QStringList msgList)
{
	//获取接收方id
	QString recvID = msgList[2];
	//在连接上的客户端池中查找接收方
	for (ClientThread *c : threadPool)
	{
		//如果在的话
		if (c->getID() == recvID)
		{
			//原封不动发送消息
			emit c->sendMsgToClientSignal(msgList.join(' '));
			return;
		}
	}
	//如果没有上线的话就向发送方发送相关提示信息
	emit sender->sendMsgToClientSignal(QString("Message %1 %2 %3").arg(recvID).arg(msgList[1]).arg(tr(u8"该用户目前没有上线，你的消息可能无法到达！")));
}

void Server::addFriendRequestHandle(ClientThread * sender, QStringList msgList)
{
	//获取被请求添加方id
	QString friendID = msgList[1];

	//验证是否有这个用户
	query.exec(QString("select id from acount where id='%1';").arg(friendID));
	//如果没有这个用户的话
	if (!query.next())
	{
		emit sender->sendMsgToClientSignal(QString("NoThisUser"));
	}
	else
	{
		//否则给双方添加好友
		query.exec(QString("insert into friend%1 values('%2');").arg(sender->getID()).arg(friendID));
		query.exec(QString("insert into friend%1 values('%2');").arg(friendID).arg(sender->getID()));

		//给发送方更新好友列表
		friendListRequestHandle(sender, QString("FriendListRequest").split(' '));

		//如果被添加方也在线的话，则也给被添加方更新好友列表
		for (int i = 0; i < threadPool.count(); i++)
		{
			if (threadPool[i]->getID() == friendID)
			{
				friendListRequestHandle(threadPool[i], QString("FriendListRequest").split(' '));
				break;
			}
		}
	}
}

void Server::delFriendRequestHandle(ClientThread * sender, QStringList msgList)
{
	//获取要被删除的好友id
	QString friendID = msgList[1];

	//给请求方和被请求方互相删除
	query.exec(QString("delete from friend%1 where friendid='%2';").arg(sender->getID()).arg(friendID));
	query.exec(QString("delete from friend%1 where friendid='%2';").arg(friendID).arg(sender->getID()));

	//如果被请求方在线的话，给被请求方更新好友列表
	for (int i = 0; i < threadPool.count(); i++)
	{
		if (threadPool[i]->getID() == friendID)
		{
			friendListRequestHandle(threadPool[i], QString("FriendListRequest").split(' '));
			break;
		}
	}
}