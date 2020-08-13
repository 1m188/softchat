#pragma once

#include "QString"

//好友项相关信息的索引枚举
enum FriendInfoNum
{
    id,
    name,
};

//用户信息
struct UserInfo
{
    QString id;   // id
    QString name; //昵称

    UserInfo() : id(""), name("")
    {
    }
    UserInfo(QString id, QString name) : id(id), name(name)
    {
    }
};
