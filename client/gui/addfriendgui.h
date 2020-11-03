#pragma once

#include "QLineEdit"
#include <QWidget>

#include "extern.h"

//添加好友界面
class AddFriendGui : public QWidget
{
    Q_OBJECT

public:
    AddFriendGui(QWidget *parent, UserInfo myInfo);

private:
    UserInfo myInfo;       //自己的用户信息
    QLineEdit *idLineEdit; //要添加的好友id输入框

protected:
    void closeEvent(QCloseEvent *event) override; //重写的关闭事件

signals:
    void closeSignal();                            //自身关闭的时候发出信号
    void addFriendRequestSignal(QString friendID); //添加好友信号

private slots:
    void addButtonClicked(); //添加好友按钮按下

public slots:
    void addFriendRepeatSlot(); //已经添加了这个好友
    void noThisUserSlot();      //添加好友的时候返回没有这个用户
};
