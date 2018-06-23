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

	void writeMsg(QString msg);

private:
	
};

#endif // TCPSOCKET_H
