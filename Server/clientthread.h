#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include "TcpSocket.h"

//连接上的客户端所在的线程
class ClientThread : public QThread
{
	Q_OBJECT

public:
	ClientThread(QObject *parent, qintptr handle);
	~ClientThread();

private:
	TcpSocket *connectToClient; //和客户端连接的socket
	qintptr handle; //socket描述句柄
	QString id; //用户id

public:
	const QString getID() const { return id; } //获取这个线程中所连接的client的id
	void setID(QString id) { this->id = id; } //设置这个线程所连接的client的id

protected:
	void run() override; //重写run函数

signals:
	void disconnectToClientSignal(); //和客户端断开连接信号
	void getMsgFromClientSignal(QString msg); //从客户端获取消息信号
	void sendMsgToClientSignal(QString msg); //给客户端发送消息信号
};

#endif // CLIENTTHREAD_H
