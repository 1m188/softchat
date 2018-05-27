#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include "QTcpSocket"

class ClientThread : public QThread
{
	Q_OBJECT

public:
	ClientThread(QObject *parent, qintptr handle);
	~ClientThread();

private:
	QTcpSocket *connectToClient;
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

private slots:
	void getMsgFromClient();
};

#endif // CLIENTTHREAD_H
