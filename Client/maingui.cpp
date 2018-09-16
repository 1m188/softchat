#include "maingui.h"
#include "QApplication"
#include "QDeskTopWidget"
#include "QMenuBar"
#include "QGridLayout"
#include "addfriendgui.h"
#include "QMessageBox"

MainGui::MainGui(QWidget *parent)
	: QWidget(parent), myInfo({}), friendList(new QListWidget(this)), framePlace(new QWidget(this)), chatFramePool({}), isAddFriendGuiOpen(false)
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
	for (int i = 0; i < this->friendList->count(); i++)
	{
		delete this->friendList->item(i);
	}
	this->friendList->clear();

	//挨个添加
	for (QString s : friendList)
	{
		QStringList sList = s.split('_');
		QListWidgetItem *item = new QListWidgetItem(this->friendList);
		item->setText(sList[1]);
		item->setData(Qt::UserRole + FriendInfoNum::id, sList[0]);
		item->setData(Qt::UserRole + FriendInfoNum::name, sList[1]);
		this->friendList->addItem(item);
	}

	//如果是被删除好友的一方，被更新好友列表，则要把相关的界面删除
	for (int i = 0; i < chatFramePool.count(); i++)
	{
		bool isExist = false; //打开的聊天面板的id是否在好友列表中

		//检测打开的聊天面板的id是否存在好友列表中
		for (int j = 0; j < this->friendList->count(); j++)
		{
			if (this->friendList->item(j)->data(Qt::UserRole + FriendInfoNum::id).toString() == chatFramePool[i]->getID())
			{
				//如果存在
				isExist = true;
				break;
			}
		}

		//如果不在
		if (!isExist)
		{
			//不然的话就关闭并删除这个面板
			chatFramePool[i]->close();
			chatFramePool[i]->deleteLater();
			chatFramePool.removeAt(i);
		}
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
	//使添加好友界面出现但是其他的界面依旧可以操作，且添加好友界面只出现一个
	if (!isAddFriendGuiOpen)
	{
		AddFriendGui *addFriendGui = new AddFriendGui(nullptr, myInfo);
		isAddFriendGuiOpen = true;
		connect(addFriendGui, &AddFriendGui::closeSignal, this, [&]() {isAddFriendGuiOpen = false; }); //添加好友界面唯一
		connect(addFriendGui, &AddFriendGui::addFriendRequestSignal, this, &MainGui::addFriendRequestSlot); //添加好友
		connect(this, &MainGui::addFriendRepeatSignal, addFriendGui, &AddFriendGui::addFriendRepeatSlot); //已经添加了这个好友
		connect(this, &MainGui::noThisUserSignal, addFriendGui, &AddFriendGui::noThisUserSlot); //添加好友的时候返回没有这个用户
		addFriendGui->show();
	}
}

void MainGui::addFriendRequestSlot(QString friendID)
{
	//如果好友列表里有这个申请的id，驳回请求，结束
	for (int i = 0; i < friendList->count(); i++)
	{
		if (friendID == friendList->item(i)->data(Qt::UserRole + FriendInfoNum::id).toString())
		{
			emit addFriendRepeatSignal();
			return;
		}
	}

	//否则，发送好友添加请求
	emit addFriendRequestSignal(friendID);
}

void MainGui::delFriendActionTriggered()
{
	//获取当前选中项
	QList<QListWidgetItem *> items = friendList->selectedItems();
	//如果有
	if (items.count())
	{
		//判断是否要删除好友
		if (QMessageBox::warning(this, tr(u8"警告"), tr((QString(u8"你真的要删除你的好友 %1 吗？").arg(items[0]->data(Qt::UserRole + FriendInfoNum::name).toString()).toUtf8())), QMessageBox::Yes | QMessageBox::Cancel) == QMessageBox::Yes)
		{
			//是则发送删除好友请求
			emit delFriendRequestSignal(items[0]->data(Qt::UserRole + FriendInfoNum::id).toString());
			//如果聊天面板打开了的话就要关闭
			for (int i = 0; i < chatFramePool.count(); i++)
			{
				if (chatFramePool[i]->getID() == items[0]->data(Qt::UserRole + FriendInfoNum::id).toString())
				{
					chatFramePool[i]->close();
					chatFramePool[i]->deleteLater();
					chatFramePool.removeAt(i);
					break;
				}
			}
			//并且删除好友列表里的项
			friendList->removeItemWidget(items[0]);
			delete items[0];
		}
	}
}

void MainGui::videoActionTriggered()
{
}