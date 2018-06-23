#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QTcpSocket>
#include "QDataStream"

class TcpSocket : public QTcpSocket
{
	Q_OBJECT

public:
	TcpSocket(QObject *parent);
	~TcpSocket();

private:
	QByteArray buffer;
	const qint32 headLen;
	qint32 msgLen;

signals:
	void getMsgSignal(QString msg);

private slots:
	void getMsg();

public:
	void writeMsg(QString msg);
};

#endif // TCPSOCKET_H
