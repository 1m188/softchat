#ifndef DATA_H
#define DATA_H

#include <QObject>
#include "QThread"
#include "tcpsocket.h"
#include "logingui.h"
#include "maingui.h"
#include "extern.h"

#include "QDebug"

class Data : public QObject
{
	Q_OBJECT

public:
	Data(QObject *parent);
	~Data();

private:
	TcpSocket *connectToServer;
	UserInfo *myInfo;

public:
	void addSignalSlots(LoginGui *loginGui) { emit addSignalSlotsForClassSignal(loginGui); }
	void addSignalSlots(MainGui *mainGui) { emit addSignalSlotsForClassSignal(mainGui); }

signals:
	void addSignalSlotsForClassSignal(LoginGui *loginGui);
	void addSignalSlotsForClassSignal(MainGui *mainGui);

	void loginSignal();
	void loginSuccessSignal();
	void loginFailedSignal();
	void loginRepeatSignal();
	void registerSuccessSignal(QString id);

	void getMyInfoSignal(QString id, QString name);
	void getFriendListSignal(QStringList friendList);
	void getMsgSignal(QString msg, QString senderID);

private slots:
	void addSignalSlotsForClassSlot(LoginGui *loginGui);
	void addSignalSlotsForClassSlot(MainGui *mainGui);

	void init();
	void getMsgFromServer();

	void loginSlot(QString acountInfo);
	void registerSlot(QString acountInfo);

	void sendMsgSlot(QString msg, QString recverID);
};

#endif // DATA_H
