# softchat

一个简单的IM程序。

1.Qt5.14.2

2.数据库使用QSQLITE

3.数据库中的acount表：

|字段|类型|长度|
|:-:|:-:|:-:|:-:|
|id（账号）|char|10|
|name（昵称）|char|50|
|password（密码）|char|50|

另有一表为好友表，为程序自动生成。

4.服务器连接为硬编码在代码之中，默认监听和连接本地。data.cpp里的init函数里的socket的连接操作。
