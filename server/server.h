#ifndef SERVER_H
#define SERVER_H

#include "QWebSocketServer"
#include "QWebSocket"
#include "QSqlQuery"

//服务器
class Server : public QWebSocketServer
{
	Q_OBJECT

public:
	Server(QObject *parent);
	~Server();

private:
	QVector<QWebSocket *> connectionPool; //连接上的客户端的连接池
	QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE"); //连接到的本地MySQL数据库
	QSqlQuery query = QSqlQuery(database); //查询

	void loginRequestHandle(QWebSocket *sender, QStringList msgList); //登陆请求处理函数
	void registerRequestHandle(QWebSocket *sender, QStringList msgList); //注册请求处理函数
	void myInfoRequestHandle(QWebSocket *sender, QStringList msgList); //自身用户信息请求处理函数
	void friendListRequestHandle(QWebSocket *sender, QStringList msgList); //好友列表请求处理函数
	void messageHandle(QWebSocket *sender, QStringList msgList); //发送聊天消息处理函数
	void addFriendRequestHandle(QWebSocket *sender, QStringList msgList); //添加好友请求处理函数
	void delFriendRequestHandle(QWebSocket *sender, QStringList msgList); //删除好友请求处理函数

private slots:
	void onNewConnection(); //新的连接到来
	void disconnectToClientSlot(); //断开连接槽
	void getMsgFromClientSlot(QString msg); //获得消息的处理函数
};

#endif // SERVER_H
