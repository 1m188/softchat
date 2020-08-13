#pragma once

#include "QFrame"
#include "QTextEdit"

#include "extern.h"

//聊天面板
class ChatFrame : public QFrame
{
    Q_OBJECT

  public:
    ChatFrame(QWidget *parent, UserInfo friendInfo);

  private:
    UserInfo friendInfo;     //该聊天面板代表的好友的用户信息
    QTextEdit *sendTextEdit; //发送消息的textedit
    QTextEdit *recvTextEdit; //接收消息的textedit

  public:
    const QString getID() const //获取该好友id
    {
        return friendInfo.id;
    }
    const QString getName() const //获取该好友昵称
    {
        return friendInfo.name;
    }

  protected:
    bool eventFilter(QObject *watched, QEvent *event) override; //事件过滤器

  signals:
    void sendMsgSignal(QString msg, QString recverID); //发送聊天消息

  private slots:
    void sendButtonClicked(); //单击发送按钮

  public slots:
    void getMsgSlot(QString msg, QString senderID); //接收好友的聊天消息
};
