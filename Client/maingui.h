#ifndef MAINGUI_H
#define MAINGUI_H

#include <QWidget>
#include "QApplication"
#include "QDeskTopWidget"
#include "QListWidget"
#include "QMenuBar"
#include "QGridLayout"
#include "chatframe.h"
#include "extern.h"

class MainGui : public QWidget
{
	Q_OBJECT

public:
	MainGui(QWidget *parent);
	~MainGui();

private:
	UserInfo myInfo;
	QListWidget *friendList;
	QWidget *framePlace;
	QVector<ChatFrame *> chatFramePool;

protected:
	void resizeEvent(QResizeEvent *event);

signals:
	void sendMsgSignal(QString msg, QString recverID);
	void getMsgSignal(QString msg, QString senderID);

private slots:
	void friendListItemDoubleClicked(QListWidgetItem *item);
	void addFriendActionTriggered();
	void delFriendActionTriggered();
	void videoActionTriggered();

public slots:
	void getMyInfoSlot(QString id, QString name);
	void updateFriendList(QStringList friendList);
	void getMsgSlot(QString msg, QString senderID);
};

#endif // MAINGUI_H
