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
	connectToClient = new QTcpSocket(nullptr);
	connectToClient->setSocketDescriptor(handle);
	connect(this, &QThread::finished, connectToClient, &QTcpSocket::deleteLater);
	connect(connectToClient, &QTcpSocket::readyRead, this, &ClientThread::getMsgFromClient);
	connect(this, &ClientThread::sendMsgToClientSignal, connectToClient, [&](QString msg) {connectToClient->write(msg.toUtf8()); connectToClient->waitForBytesWritten(); });
	exec();
}

void ClientThread::getMsgFromClient()
{
	emit getMsgFromClientSignal(QString::fromUtf8(connectToClient->readAll()));
}