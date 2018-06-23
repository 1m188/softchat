#ifndef EXTERN_H
#define EXTERN_H

#include "QString"

const enum FriendInfoNum
{
	id,
	name,
};

struct UserInfo
{
	QString id;
	QString name;

	UserInfo::UserInfo() :id(""), name("") {}
	UserInfo::UserInfo(QString id, QString name) : id(id), name(name) {}
};

#endif // !EXTERN_H
