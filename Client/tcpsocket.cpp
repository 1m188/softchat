#include "tcpsocket.h"

TcpSocket::TcpSocket(QObject *parent)
	: QTcpSocket(parent)
{

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