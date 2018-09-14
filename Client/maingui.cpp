#include "maingui.h"
#include "QApplication"
#include "QDeskTopWidget"
#include "QMenuBar"
#include "QGridLayout"

MainGui::MainGui(QWidget *parent)
	: QWidget(parent), myInfo({}), friendList(new QListWidget(this)), framePlace(new QWidget(this)), chatFramePool({})
{
	//界面基本设置
	setAttribute(Qt::WA_DeleteOnClose, true);
	setAttribute(Qt::WA_QuitOnClose, true);

	//设置界面标题名和大小
	setWindowTitle(myInfo.id + "-" + myInfo.name);
	resize(1300, 800);
	setMinimumSize(size());

	//把界面放在屏幕中间
	QRect rect = frameGeometry();
	rect.moveCenter(QApplication::desktop()->availableGeometry().center());
	move(rect.topLeft());

	//控件
	friendList->setFont(QFont(u8"微软雅黑", 12));
	connect(friendList, &QListWidget::itemDoubleClicked, this, &MainGui::friendListItemDoubleClicked);

	//顶层菜单
	QMenuBar *menuBar = new QMenuBar(this);
	menuBar->setStyleSheet("QMenuBar{background-color:rgb(240,240,240)}");

	QMenu *menu = new QMenu(menuBar);
	menu->setTitle(tr(u8"菜单"));
	menuBar->addMenu(menu);

	QAction *addFriendAction = new QAction(menu);
	addFriendAction->setText(tr(u8"添加好友"));
	connect(addFriendAction, &QAction::triggered, this, &MainGui::addFriendActionTriggered);
	menu->addAction(addFriendAction);

	QAction * delFriendAction = new QAction(menu);
	delFriendAction->setText(tr(u8"删除好友"));
	connect(delFriendAction, &QAction::triggered, this, &MainGui::delFriendActionTriggered);
	menu->addAction(delFriendAction);

	QAction *videoAction = new QAction(menu);
	videoAction->setText(tr(u8"视频通话"));
	connect(videoAction, &QAction::triggered, this, &MainGui::videoActionTriggered);
	menu->addAction(videoAction);

	//布局
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
	//挨个在聊天界面列表中把每个聊天界面都改变大小
	for (ChatFrame *c : chatFramePool)
	{
		c->resize(framePlace->size());
	}
	return QWidget::resizeEvent(event);
}

void MainGui::friendListItemDoubleClicked(QListWidgetItem *item)
{
	//遍历聊天界面列表，看这个好友的聊天界面是否打开
	for (ChatFrame *c : chatFramePool)
	{
		//如果打开了
		if (c->getID() == item->data(Qt::UserRole + FriendInfoNum::id).toString())
		{
			//放到最顶层
			c->raise();
			//重置主界面标题
			setWindowTitle(windowTitle().split("---")[0] + "---" + c->getID() + "-" + c->getName());
			//结束
			return;
		}
	}

	//没有结束的话，那肯定是没有打开，则打开
	ChatFrame *chatFrame = new ChatFrame(framePlace, UserInfo(item->data(Qt::UserRole + FriendInfoNum::id).toString(), item->data(Qt::UserRole + FriendInfoNum::name).toString()));
	setWindowTitle(windowTitle().split("---")[0] + "---" + item->data(Qt::UserRole + FriendInfoNum::id).toString() + "-" + item->data(Qt::UserRole + FriendInfoNum::name).toString());
	chatFrame->resize(framePlace->size());
	connect(chatFrame, &ChatFrame::sendMsgSignal, this, &MainGui::sendMsgSignal); //发送聊天消息
	connect(this, &MainGui::getMsgSignal, chatFrame, &ChatFrame::getMsgSlot); //接收聊天消息
	chatFramePool.push_back(chatFrame); //加入聊天界面列表
	chatFrame->show();
}

void MainGui::getMyInfoSlot(QString id, QString name)
{
	//更新自己的用户信息
	myInfo.id = id;
	myInfo.name = name;
	//重置主界面标题
	setWindowTitle(myInfo.id + "-" + myInfo.name);
}

void MainGui::updateFriendList(QStringList friendList)
{
	//先清空原来的好友列表，防止出现重复好友项
	this->friendList->clear();
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
	//在好友列表中找到相应的发送方，模拟双击该项，这样无论这个好友的聊天界面是否打开最终都会打开
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