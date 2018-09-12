#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>

//和客户端连接的socket
class TcpSocket : public QTcpSocket
{
	Q_OBJECT

public:
	TcpSocket(QObject *parent);
	~TcpSocket();

private:
	QByteArray buffer; //消息缓冲区
	const qint32 headLen; //包头大小
	qint32 msgLen; //包身大小

signals:
	void getMsgSignal(QString msg); //获得的完整的新消息的信号

private slots:
	void getMsg(); //当有新的消息过来的时候就调用这个，通过粘包处理的方式读取新的消息

public:
	void writeMsg(QString msg); //发送消息，粘包处理
};

#endif // TCPSOCKET_H
