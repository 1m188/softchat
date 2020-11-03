#include "data.h"

#include "QApplication"
#include "QThread"

#ifdef DEBUG
#include "QDebug"
#endif // DEBUG

Data *Data::instance = nullptr; //初始化单例句柄

Data *Data::getInstance()
{
    if (!instance)
        instance = new Data();
    return instance;
}

Data::Data()
{
    //把本类的所有信号槽响应放在另一个线程中
    QThread *thread = new QThread();
    moveToThread(thread);
    connect(thread, &QThread::started, this, &Data::init); //线程开始则启动初始化
    //资源管理
    connect(this, &Data::destroyed, thread, &QThread::quit);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start(); //启动
}

Data::~Data()
{
    //释放相关内存资源
    delete myInfo;
}

void Data::init()
{
    connectToServer = new QWebSocket(nullptr);                                                 //初始化和服务器连接的socket
    connect(this, &Data::destroyed, connectToServer, &QWebSocket::deleteLater);                //资源管理
    connect(connectToServer, &QWebSocket::textMessageReceived, this, &Data::getMsgFromServer); //从服务器获取的消息处理
    connectToServer->open(QUrl("ws://127.0.0.1:8888"));                                        //连接到相关服务器（本地）的8888端口

    myInfo = new UserInfo(); //初始化自己的用户信息
}

void Data::getMsgFromServer(QString msg)
{
    //输出调试信息
#ifdef DEBUG
    qDebug() << msg;
#endif // DEBUG

    //获取消息通过空格分隔的列表
    QStringList msgList = msg.split(' ');
    //判断
    //登陆成功
    if (msgList[0] == "LoginSuccess")
        loginSuccessHandle(msgList);
    //登陆失败
    else if (msgList[0] == "LoginFailed")
        loginFailedHandle(msgList);
    //重复登陆
    else if (msgList[0] == "LoginRepeat")
        loginRepeatHandle(msgList);
    //注册成功
    else if (msgList[0] == "RegisterSuccess")
        registerSuccessHandle(msgList);
    //获取自己的用户信息
    else if (msgList[0] == "MyInfo")
        getMyInfoHandle(msgList);
    //获取好友列表
    else if (msgList[0] == "FriendList")
        getFriendListHandle(msgList);
    //接收好友发送过来的聊天消息
    else if (msgList[0] == "Message")
        messageHandle(msgList);
    //添加好友，没有这个用户的回应
    else if (msgList[0] == "NoThisUser")
        noThisUserHandle(msgList);
}

void Data::loginSuccessHandle(QStringList msgList)
{
    emit loginSignal();
}

void Data::loginFailedHandle(QStringList msgList)
{
    emit loginFailedSignal();
}

void Data::loginRepeatHandle(QStringList msgList)
{
    emit loginRepeatSignal();
}

void Data::registerSuccessHandle(QStringList msgList)
{
    emit registerSuccessSignal(msgList[1]);
}

void Data::getMyInfoHandle(QStringList msgList)
{
    //设置自己用户信息
    myInfo->id = msgList[1];
    myInfo->name = msgList[2];
    //传递给主界面
    emit getMyInfoSignal(msgList[1], msgList[2]);
}

void Data::getFriendListHandle(QStringList msgList)
{
    msgList.pop_front();               //去掉FriendList的消息头
    emit getFriendListSignal(msgList); //把纯粹的好友列表的信息发送过去
}

void Data::messageHandle(QStringList msgList)
{
    QString senderID = msgList[1]; //获取发送方id
    QString recverID = msgList[2]; //获取接收方id
    //去掉Message、发送方id和接收方id
    msgList.pop_front();
    msgList.pop_front();
    msgList.pop_front();
    //获取完整的消息
    QString msg = msgList.join(' ');
    //发送消息和发送方id
    emit getMsgSignal(msg, senderID);
}

void Data::noThisUserHandle(QStringList msgList)
{
    emit noThisUserSignal();
}

void Data::loginRequestSlot(QString acountInfo)
{
    QStringList acountInfoList = acountInfo.split(' ');
    connectToServer->sendTextMessage(QString("LoginRequest %1 %2").arg(acountInfoList[0]).arg(acountInfoList[1]));
}

void Data::registerRequestSlot(QString acountInfo)
{
    QStringList acountInfoList = acountInfo.split(' ');
    connectToServer->sendTextMessage(QString("RegisterRequest %1 %2").arg(acountInfoList[0]).arg(acountInfoList[1]));
}

void Data::sendMsgSlot(QString msg, QString recverID)
{
    connectToServer->sendTextMessage(QString("Message %1 %2 %3").arg(myInfo->id).arg(recverID).arg(msg));
}

void Data::addFriendRequestSlot(QString friendID)
{
    connectToServer->sendTextMessage(QString("AddFriendRequest %1").arg(friendID));
}

void Data::delFriendRequestSlot(QString friendID)
{
    connectToServer->sendTextMessage(QString("DelFriendRequest %1").arg(friendID));
}

void Data::getMyUserInfoSlot()
{
    connectToServer->sendTextMessage("MyInfoRequest"); //发送自己用户信息请求
}

void Data::getFriendListSlot()
{
    connectToServer->sendTextMessage("FriendListRequest"); //发送好友列表请求
}