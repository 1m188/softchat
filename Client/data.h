#ifndef DATA_H
#define DATA_H

#include <QObject>
#include "tcpsocket.h"
#include "logingui.h"
#include "maingui.h"
#include "extern.h"

//后台线程运行的数据类（单例）
class Data : public QObject
{
	Q_OBJECT

private:
	//单例句柄
	static Data *instance;

	//构造函数，私有化以防止外部构造
	Data();

	//连接到服务器的socket
	TcpSocket *connectToServer = nullptr;
	//自己的用户信息
	UserInfo *myInfo = nullptr;

	void loginSuccessHandle(QStringList msgList); //登陆成功的响应
	void loginFailedHandle(QStringList msgList); //登陆失败的响应
	void loginRepeatHandle(QStringList msgList); //重复登陆的响应
	void registerSuccessHandle(QStringList msgList); //注册成功的响应
	void getMyInfoHandle(QStringList msgList); //获取自己的用户信息的响应
	void getFriendListHandle(QStringList msgList); //获取好友列表的响应
	void messageHandle(QStringList msgList); //接收好友发送过来的聊天消息的响应
	void noThisUserHandle(QStringList msgList); //添加好友时返回没有这个用户

public:
	//获取单例句柄
	static Data *getInstance();

	//删除相关的其他构造函数
	Data(const Data &) = delete;
	Data &operator=(const Data &) = delete;

	~Data();

signals:
	void loginSignal(); //登陆信号
	void loginFailedSignal(); //登陆失败
	void loginRepeatSignal(); //重复登陆
	void registerSuccessSignal(QString id); //注册成功

	void getMyInfoSignal(QString id, QString name); //获取自己的用户信息
	void getFriendListSignal(QStringList friendList); //获取好友列表
	void getMsgSignal(QString msg, QString senderID); //接收聊天消息

	void noThisUserSignal(); //添加好友时返回没有这个用户

private slots:
	void init(); //data类在另一个线程中的初始化
	void getMsgFromServer(QString msg); //从服务器获取消息

public slots:
	void loginRequestSlot(QString acountInfo); //发送登陆请求
	void registerRequestSlot(QString acountInfo); //发送注册请求

	void sendMsgSlot(QString msg, QString recverID); //发送聊天消息

	void addFriendRequestSlot(QString friendID); //发送添加好友请求
	void delFriendRequestSlot(QString friendID); //发送删除好友请求

	void getMyUserInfoSlot(); //获取自己的用户信息
	void getFriendListSlot(); //获取好友列表
};

#endif // DATA_H
