#pragma once

#include "QDialog"
#include "QLineEdit"

//登陆界面
class LoginGui : public QDialog
{
    Q_OBJECT

private:
    QLineEdit *idLineEdit;       // id输入框
    QLineEdit *passwordLineEdit; //密码输入框

public:
    LoginGui(QWidget *parent);

signals:
    void loginRequestSignal(QString acountInfo);    //登陆请求
    void registerRequestSignal(QString acountInfo); //注册请求
    void registerSuccessSignal(QString id);         //注册成功

private slots:
    void loginButtonClicked();    //登陆按钮按下
    void registerButtonClicked(); //注册按钮按下

public slots:
    void loginFailedSlot(); //登陆失败
    void loginRepeatSlot(); //重复登陆
};
