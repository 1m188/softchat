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

struct Head
{
	int length;

	Head::Head() :length(0) {}
};

#endif // !EXTERN_H
