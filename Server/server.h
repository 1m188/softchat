#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include "QTcpSocket"
#include "QSqlDatabase"
#include "QSqlQuery"
#include "clientthread.h"

#include "QDebug"

class Server : public QTcpServer
{
	Q_OBJECT

public:
	Server(QObject *parent);
	~Server();

private:
	QVector<ClientThread *> threadPool;
	QSqlDatabase database;
	QSqlQuery query;

protected:
	void incomingConnection(qintptr handle);

private slots:
	void getMsgFromClientSlot(QString msg);
};

#endif // SERVER_H
