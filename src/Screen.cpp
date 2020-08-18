#include "Screen.hpp"
using namespace std;

inline void Folder::addMsg(Message* msg)
{
	messages.insert(msg);
}

inline void Folder::remMsg(Message* msg)
{
	messages.erase(msg);
}

inline void Message::add_to_Folders(const Message& msg)
{
	for (auto item : msg.folders)
	{
		item->addMsg(this);
	}
}

inline void Message::remove_from_Folders()
{
	for (auto item : folders)
	{
		item->remMsg(this);
	}
}

Message::Message(const Message& m)
	: contents(m.contents), folders(m.folders)
{
	add_to_Folders(m);
}

Message& Message::operator=(const Message& rhs)
{
	remove_from_Folders();
	contents = rhs.contents;
	folders = rhs.folders;
	add_to_Folders(rhs);
	return *this;
}

Message::~Message()
{
	remove_from_Folders();
}

void Message::save(Folder& f)
{
	folders.insert(&f);
	f.addMsg(this);
}

void Message::remove(Folder& f)
{
	folders.erase(&f);
	f.remMsg(this);
}

void swap(Message& lhs, Message& rhs)
{
	using std::swap;
	for (auto item : lhs.folders)
	{
		item->remMsg(&lhs);
	}
	for (auto item : rhs.folders)
	{
		item->remMsg(&rhs);
	}
	swap(lhs.contents, rhs.contents);
	swap(lhs.folders, rhs.folders);
	for (auto item : lhs.folders)
	{
		item->addMsg(&lhs);
	}
	for (auto item : rhs.folders)
	{
		item->addMsg(&rhs);
	}
}

Folder::Folder(const Folder& f)
	: messages(f.messages) 
{
	for(auto item : messages)
	{
		item->add_to_Folders(*item);
	}
}

Folder& Folder::operator=(const Folder& f)
{
	for(auto item : messages)
	{
		item->remove_from_Folders();
	}
	messages = f.messages;
	for(auto item : messages)
	{
		item->add_to_Folders(*item);
	}
	return *this;
}

Folder::~Folder()
{
	for(auto item : messages)
	{
		item->folders.erase(this);
	}
}

void Folder::save(Message *m)
{
	messages.insert(m);
	m->folders.insert(this);
}

void Folder::remove(Message *m)
{
	messages.erase(m);
	m->folders.erase(this);
}

void Folder::print()
{
	for(auto item : messages)
	{
		item->print();
	}
}
