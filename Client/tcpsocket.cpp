#include "TcpSocket.h"
#include "QDataStream"

TcpSocket::TcpSocket(QObject *parent)
	: QTcpSocket(parent), buffer(QByteArray()), headLen(sizeof(qint32)), msgLen(0)
{
	connect(this, &TcpSocket::readyRead, this, &TcpSocket::getMsg);
}

TcpSocket::~TcpSocket()
{

}

void TcpSocket::writeMsg(QString msg)
{
	QByteArray sendMsg; //要发送的包
	QDataStream out(&sendMsg, QIODevice::WriteOnly); //用来操纵包读写的QDataStream
	out.setVersion(QDataStream::Qt_5_9);
	out << static_cast<qint32>(msg.size() * 2) << msg; //读入一个4字节的要发送的消息的大小（这里QString会转为unicode，每个原来的字符占两个字节，所以字节大小要在原来的size的基础上x2），然后再读入要发送的消息 //关键之处
	write(sendMsg); //发送
}

void TcpSocket::getMsg()
{
	buffer.append(readAll());
	int totalLen = buffer.size();
	while (totalLen)
	{
		QDataStream in(&buffer, QIODevice::ReadOnly);
		in.setVersion(QDataStream::Qt_5_9);

		if (msgLen == 0)
		{
			if (totalLen >= headLen)
			{
				in >> msgLen;
				if (totalLen >= headLen + msgLen)
				{
					QString msg;
					in >> msg;
					emit getMsgSignal(msg);
					buffer = buffer.right(totalLen - headLen - msgLen - 4);
					msgLen = 0;
					totalLen = buffer.size();
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			if (totalLen >= headLen + msgLen)
			{
				//预先读出头
				int temp = 0;
				in >> temp;
				//读包身消息
				QString msg;
				in >> msg;
				emit getMsgSignal(msg);
				buffer = buffer.right(totalLen - headLen - msgLen - 4);
				msgLen = 0;
				totalLen = buffer.size();
			}
			else
			{
				break;
			}
		}
	}
}