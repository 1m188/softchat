#include "ClientThread.h"

ClientThread::ClientThread(QObject *parent, qintptr handle)
	: QThread(parent), connectToClient(nullptr), handle(handle), id("")
{

}

ClientThread::~ClientThread()
{

}

void ClientThread::run()
{
	//在新线程中创建和客户端连接的socket
	connectToClient = new TcpSocket(nullptr);
	connectToClient->setSocketDescriptor(handle);
	connect(this, &QThread::finished, connectToClient, &QTcpSocket::deleteLater); //线程退出则此socket销毁
	connect(connectToClient, &TcpSocket::disconnected, this, &ClientThread::disconnectToClientSignal); //断开连接
	connect(connectToClient, &TcpSocket::getMsgSignal, this, &ClientThread::getMsgFromClientSignal); //获得了新的消息则转到线程中另外的处理函数
	connect(this, &ClientThread::sendMsgToClientSignal, connectToClient, [&](QString msg) {connectToClient->writeMsg(msg); }); //发送粘包处理过的消息
	exec(); //事件循环
}