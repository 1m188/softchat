#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include "tcpsocket.h"
#include "QDataStream"

class ClientThread : public QThread
{
	Q_OBJECT

public:
	ClientThread(QObject *parent, qintptr handle);
	~ClientThread();

private:
	TcpSocket *connectToClient;
	qintptr handle;
	QString id;

public:
	const QString getID() const { return id; }
	void setID(QString _id) { id = _id; }

protected:
	void run();

signals:
	void getMsgFromClientSignal(QString msg);
	void sendMsgToClientSignal(QString msg);
};

#endif // CLIENTTHREAD_H
