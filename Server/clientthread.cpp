#include "clientthread.h"

ClientThread::ClientThread(QObject *parent, qintptr handle)
	: QThread(parent), connectToClient(nullptr), handle(handle), id("")
{

}

ClientThread::~ClientThread()
{

}

void ClientThread::run()
{
	connectToClient = new TcpSocket(nullptr);
	connectToClient->setSocketDescriptor(handle);
	connect(this, &QThread::finished, connectToClient, &QTcpSocket::deleteLater);
	connect(connectToClient, &TcpSocket::getMsgSignal, this, &ClientThread::getMsgFromClientSignal);
	connect(this, &ClientThread::sendMsgToClientSignal, connectToClient, [&](QString msg) {connectToClient->writeMsg(msg); });
	exec();
}