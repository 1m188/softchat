#ifndef DATA_H
#define DATA_H

#include <QObject>
#include "tcpsocket.h"
#include "logingui.h"
#include "maingui.h"
#include "extern.h"

#ifdef _DEBUG
#include "QDebug"
#endif // _DEBUG

//后台线程运行的数据类
class Data : public QObject
{
	Q_OBJECT

public:
	Data(QObject *parent);
	~Data();

private:
	TcpSocket *connectToServer; //连接到服务器的socket
	UserInfo *myInfo; //自己的用户信息

	void loginSuccessHandle(QStringList msgList); //登陆成功的响应
	void loginFailedHandle(QStringList msgList); //登陆失败的响应
	void loginRepeatHandle(QStringList msgList); //重复登陆的响应
	void registerSuccessHandle(QStringList msgList); //注册成功的响应
	void getMyInfoHandle(QStringList msgList); //获取自己的用户信息的响应
	void getFriendListHandle(QStringList msgList); //获取好友列表的响应
	void messageHandle(QStringList msgList); //接收好友发送过来的聊天消息的响应
	void noThisUserHandle(QStringList msgList); //添加好友时返回没有这个用户

public:
	//对外的用来做登陆界面和主界面的信号及信号槽的连接的接口
	void addSignalSlots(LoginGui *loginGui) { emit addSignalSlotsForClassSignal(loginGui); }
	void addSignalSlots(MainGui *mainGui) { emit addSignalSlotsForClassSignal(mainGui); }

signals:
	//上面说到的接口中发送的信号，使得真正的连接工作在另一个线程中完成
	void addSignalSlotsForClassSignal(LoginGui *loginGui);
	void addSignalSlotsForClassSignal(MainGui *mainGui);

	void loginSignal(); //登陆信号
	void loginFailedSignal(); //登陆失败
	void loginRepeatSignal(); //重复登陆
	void registerSuccessSignal(QString id); //注册成功

	void getMyInfoSignal(QString id, QString name); //获取自己的用户信息
	void getFriendListSignal(QStringList friendList); //获取好友列表
	void getMsgSignal(QString msg, QString senderID); //接收聊天消息

	void noThisUserSignal(); //添加好友时返回没有这个用户

	private slots:
	//用来在另一个线程中响应添加信号及信号槽连接的信号槽
	void addSignalSlotsForClassSlot(LoginGui *loginGui);
	void addSignalSlotsForClassSlot(MainGui *mainGui);

	void init(); //data类在另一个线程中的初始化
	void getMsgFromServer(QString msg); //从服务器获取消息

	void loginRequestSlot(QString acountInfo); //发送登陆请求
	void registerRequestSlot(QString acountInfo); //发送注册请求

	void sendMsgSlot(QString msg, QString recverID); //发送聊天消息

	void addFriendSlot(QString friendID); //发送添加好友请求
};

#endif // DATA_H
