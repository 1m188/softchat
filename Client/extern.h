#ifndef EXTERN_H
#define EXTERN_H

#include "QString"

//好友项相关信息的索引枚举
const enum FriendInfoNum
{
	id,
	name,
};

//用户信息
struct UserInfo
{
	QString id; //id
	QString name; //昵称

	UserInfo::UserInfo() :id(""), name("") {}
	UserInfo::UserInfo(QString id, QString name) : id(id), name(name) {}
};

#endif // !EXTERN_H
