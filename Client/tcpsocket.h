#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>

//和服务器连接的socket
class TcpSocket : public QTcpSocket
{
	Q_OBJECT

public:
	TcpSocket(QObject *parent);
	~TcpSocket();

private:
	QByteArray buffer; //接收消息缓冲区
	const qint32 headLen; //包头大小
	qint32 msgLen; //包身长

public:
	void writeMsg(QString msg); //发送消息（经过粘包处理）

signals:
	void getMsgSignal(QString msg); //接收到新消息信号

	private slots:
	void getMsg(); //粘包处理
};

#endif // TCPSOCKET_H
