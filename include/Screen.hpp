#include <cctype>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <string>
#include <vector>

class Folder;
class Message
{
	friend class Folder;
	friend void swap(Message& lhs, Message& rhs);

public:
	explicit Message(const std::string& s = "")
		: contents(s) {}
	Message(const Message&);			 // 拷贝构造函数
	Message& operator=(const Message&);	 // 拷贝赋值运算符
	void print()
	{
		std::cout << contents << std::endl;
	}
	~Message();							 // 析构函数

	void save(Folder&);	   // 从给定Folder中添加本Message
	void remove(Folder&);  // 从给定Folder中删除本Message

private:
	std::string contents;		// 实际消息文本
	std::set<Folder*> folders;	// 包含本Message的Folder

	// 将本Message添加到指向参数的Folder中
	void add_to_Folders(const Message&);
	// 从folders中的每个Folder删除本Message
	void remove_from_Folders();
};

class Folder
{
	friend Message;
	friend void swap(Message&, Message&);

public:
	Folder() = default;
	Folder(const Folder& f);
	Folder& operator=(const Folder& f);
	~Folder();

	void save(Message* m);
	void remove(Message* m);
	void print();

private:
	std::set<Message*> messages;  // 本Folder所包含的Message指针的集合
	// 将指定Message加入到本Folder中
	void addMsg(Message*);
	// 将指定Message从本Folder中移除
	void remMsg(Message*);
};

void swap(Message&, Message&);
