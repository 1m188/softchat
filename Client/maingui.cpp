#include "maingui.h"

MainGui::MainGui(QWidget *parent)
	: QWidget(parent), myInfo({}), friendList(new QListWidget(this)), framePlace(new QWidget(this)), chatFramePool({})
{
	setAttribute(Qt::WA_DeleteOnClose, true);
	setAttribute(Qt::WA_QuitOnClose, true);

	setWindowTitle(myInfo.name);
	resize(1300, 800);

	QRect rect = frameGeometry();
	rect.moveCenter(QApplication::desktop()->availableGeometry().center());
	move(rect.topLeft());

	//¿Ø¼þ
	friendList->setFont(QFont(u8"Î¢ÈíÑÅºÚ", 12));
	connect(friendList, &QListWidget::itemDoubleClicked, this, &MainGui::friendListItemDoubleClicked);

	//¶¥²ã²Ëµ¥
	QMenuBar *menuBar = new QMenuBar(this);
	menuBar->setStyleSheet("QMenuBar{background-color:rgb(240,240,240)}");

	QMenu *menu = new QMenu(menuBar);
	menu->setTitle(tr(u8"²Ëµ¥"));
	menuBar->addMenu(menu);

	QAction *addFriendAction = new QAction(menu);
	addFriendAction->setText(tr(u8"Ìí¼ÓºÃÓÑ"));
	connect(addFriendAction, &QAction::triggered, this, &MainGui::addFriendActionTriggered);
	menu->addAction(addFriendAction);

	QAction * delFriendAction = new QAction(menu);
	delFriendAction->setText(tr(u8"É¾³ýºÃÓÑ"));
	connect(delFriendAction, &QAction::triggered, this, &MainGui::delFriendActionTriggered);
	menu->addAction(delFriendAction);

	QAction *videoAction = new QAction(menu);
	videoAction->setText(tr(u8"ÊÓÆµÍ¨»°"));
	connect(videoAction, &QAction::triggered, this, &MainGui::videoActionTriggered);
	menu->addAction(videoAction);

	//²¼¾Ö
	QGridLayout *layout = new QGridLayout(this);
	layout->addWidget(friendList, 0, 0, -1, 4);
	layout->addWidget(framePlace, 0, 4, -1, 16);
	layout->setMenuBar(menuBar);
}

MainGui::~MainGui()
{

}

void MainGui::resizeEvent(QResizeEvent * event)
{
	for (ChatFrame *c : chatFramePool)
	{
		c->resize(framePlace->size());
	}
	return QWidget::resizeEvent(event);
}

void MainGui::friendListItemDoubleClicked(QListWidgetItem *item)
{
	for (ChatFrame *c : chatFramePool)
	{
		if (c->getID() == item->data(Qt::UserRole + FriendInfoNum::id).toString())
		{
			c->raise();
			setWindowTitle(tr(myInfo.name.toUtf8() + u8"-" + c->getName().toUtf8()));
			return;
		}
	}
	ChatFrame *chatFrame = new ChatFrame(framePlace, UserInfo(item->data(Qt::UserRole + FriendInfoNum::id).toString(), item->data(Qt::UserRole + FriendInfoNum::name).toString()));
	setWindowTitle(tr(myInfo.name.toUtf8() + u8"-" + item->data(Qt::UserRole + FriendInfoNum::name).toString().toUtf8()));
	chatFrame->resize(framePlace->size());
	connect(chatFrame, &ChatFrame::sendMsgSignal, this, &MainGui::sendMsgSignal);
	connect(this, &MainGui::getMsgSignal, chatFrame, &ChatFrame::getMsgSlot);
	chatFramePool.push_back(chatFrame);
	chatFrame->show();
}

void MainGui::getMyInfoSlot(QString id, QString name)
{
	myInfo.id = id;
	myInfo.name = name;
	setWindowTitle(name);
}

void MainGui::updateFriendList(QStringList friendList)
{
	for (QString s : friendList)
	{
		QStringList sList = s.split('_');
		QListWidgetItem *item = new QListWidgetItem(this->friendList);
		item->setText(sList[1]);
		item->setData(Qt::UserRole + FriendInfoNum::id, sList[0]);
		item->setData(Qt::UserRole + FriendInfoNum::name, sList[1]);
		this->friendList->addItem(item);
	}
}

void MainGui::getMsgSlot(QString msg, QString senderID)
{
	for (int i = 0; i < friendList->count(); i++)
	{
		if (friendList->item(i)->data(Qt::UserRole + FriendInfoNum::id).toString() == senderID)
		{
			friendListItemDoubleClicked(friendList->item(i));
			emit getMsgSignal(msg, senderID);
			break;
		}
	}
}

void MainGui::addFriendActionTriggered()
{
}

void MainGui::delFriendActionTriggered()
{
}

void MainGui::videoActionTriggered()
{
}