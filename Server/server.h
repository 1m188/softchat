#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include "QSqlQuery"
#include "ClientThread.h"

//服务器
class Server : public QTcpServer
{
	Q_OBJECT

public:
	Server(QObject *parent);
	~Server();

private:
	QVector<ClientThread *> threadPool; //连接上的客户端的线程池
	QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE"); //连接到的本地MySQL数据库
	QSqlQuery query = QSqlQuery(database); //查询

	void loginRequestHandle(ClientThread *sender, QStringList msgList); //登陆请求处理函数
	void registerRequestHandle(ClientThread *sender, QStringList msgList); //注册请求处理函数
	void myInfoRequestHandle(ClientThread *sender, QStringList msgList); //自身用户信息请求处理函数
	void friendListRequestHandle(ClientThread *sender, QStringList msgList); //好友列表请求处理函数
	void messageHandle(ClientThread *sender, QStringList msgList); //发送聊天消息处理函数
	void addFriendRequestHandle(ClientThread *sender, QStringList msgList); //添加好友请求处理函数
	void delFriendRequestHandle(ClientThread *sender, QStringList msgList); //删除好友请求处理函数

protected:
	void incomingConnection(qintptr handle) override; //重写连接到来的时候处理

private slots:
	void disconnectToClientSlot(); //断开连接槽
	void getMsgFromClientSlot(QString msg); //获得消息的处理函数
};

#endif // SERVER_H
