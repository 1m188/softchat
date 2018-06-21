#include "clientthread.h"

ClientThread::ClientThread(QObject *parent, qintptr handle)
	: QThread(parent), connectToClient(nullptr), handle(handle), id(""), headLen(sizeof(qint32)), msgLen(0), buffer(QByteArray())
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
	//Õ³°ü½â¾ö£¡
	buffer.append(connectToClient->readAll());
	int totalLen = buffer.size();
	while (totalLen)
	{
		QDataStream in(&buffer, QIODevice::ReadOnly);
		in.setByteOrder(QDataStream::BigEndian);
		if (totalLen >= headLen)
		{
			in >> msgLen;
			if (totalLen >= msgLen)
			{
				QString msg;
				in >> msg;
				emit getMsgFromClientSignal(msg);
				buffer = buffer.right(totalLen - msgLen);
				totalLen = buffer.size();
			}
		}
	}

	//emit getMsgFromClientSignal(QString::fromUtf8(connectToClient->readAll()));
}