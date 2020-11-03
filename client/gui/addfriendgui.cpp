#include "addfriendgui.h"

#include "QApplication"
#include "QScreen"
#include "QLabel"
#include "QMessageBox"
#include "QPushButton"
#include "QRegExpValidator"

AddFriendGui::AddFriendGui(QWidget *parent, UserInfo myInfo)
    : QWidget(parent), myInfo(myInfo), idLineEdit(new QLineEdit(this))
{
    //界面基本设置
    setAttribute(Qt::WA_DeleteOnClose, true);
    setAttribute(Qt::WA_QuitOnClose, false);

    //设置界面标题和大小
    setWindowTitle(tr(u8"添加好友"));
    setFixedSize(600, 300);

    //移动到屏幕中央位置
    QRect rect = frameGeometry();
    rect.moveCenter(qApp->primaryScreen()->availableGeometry().center());
    move(rect.topLeft());

    //控件+布局
    QLabel *infoLabel = new QLabel(this);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setFont(QFont(u8"微软雅黑", 15));
    infoLabel->setText(tr(u8"请输入要添加的好友的ID"));
    infoLabel->resize(infoLabel->sizeHint());
    infoLabel->move(width() / 2 - infoLabel->width() / 2, height() / 2 / 2 - infoLabel->height() / 2);

    QLabel *idLabel = new QLabel(this);
    idLabel->setAlignment(Qt::AlignCenter);
    idLabel->setFont(QFont(u8"微软雅黑", 10));
    idLabel->setText(tr(u8"用户ID"));
    idLabel->resize(idLabel->sizeHint());
    idLabel->move(width() / 2 - idLabel->width() - 80, infoLabel->y() + infoLabel->height() + 65);

    idLineEdit->setValidator(new QRegExpValidator(QRegExp("[0-9]+$"), idLineEdit)); ///限制id框输入类型
    idLineEdit->resize(idLineEdit->sizeHint());
    idLineEdit->move(width() / 2 + width() / 2 - idLabel->x() - idLabel->width() - 50,
                     idLabel->y() + idLabel->height() / 2 - idLineEdit->height() / 2);
    connect(idLineEdit, &QLineEdit::returnPressed, this, &AddFriendGui::addButtonClicked);

    QPushButton *addButton = new QPushButton(this);
    addButton->setFont(QFont(u8"微软雅黑", 9));
    addButton->setText(tr(u8"添加"));
    addButton->resize(addButton->sizeHint());
    addButton->move(width() - addButton->width() - 10, height() - addButton->height() - 10);
    connect(addButton, &QPushButton::clicked, this, &AddFriendGui::addButtonClicked);
}

void AddFriendGui::closeEvent(QCloseEvent *event)
{
    //发送关闭信号，告诉主界面该界面已经关闭，可以重新打开
    emit closeSignal();
    return QWidget::closeEvent(event);
}

void AddFriendGui::addButtonClicked()
{
    //获取要添加的好友id
    QString friendID = idLineEdit->text();
    if (friendID == "") //如果为空
        QMessageBox::critical(this, tr(u8"错误"), tr(u8"好友id不可为空！"), QMessageBox::Ok);
    else if (friendID == myInfo.id)
        QMessageBox::critical(this, tr(u8"错误"), tr(u8"不可添加自己为好友！"), QMessageBox::Ok);
    else //否则发送添加好友信号
        emit addFriendRequestSignal(friendID);
}

void AddFriendGui::addFriendRepeatSlot()
{
    QMessageBox::information(this, tr(u8"重复添加"), tr(u8"您已经添加了这个好友！"), QMessageBox::Ok);
}

void AddFriendGui::noThisUserSlot()
{
    QMessageBox::critical(this, tr(u8"错误"), tr(u8"没有此用户！"), QMessageBox::Ok);
}