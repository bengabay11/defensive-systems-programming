#pragma once

#include <string>
#include <list>
#include "Post.h"
#include "Message.h"

class USocial;

class User {
protected:
	USocial* us;
	friend class USocial;
	unsigned long id;
	std::string name;
	std::list<unsigned long> friends;
	std::list<Post* > posts;
	std::list<Message*> receivedMsgs;
	User();
	~User();
	User(const User&);
	User& operator=(const User& other);
public:
	unsigned long getId();
	std::string getName();
	void addFriend(User* user);
	void removeFriend(User*);
	void post(std::string);
	void post(std::string, Media*);
	std::list<Post*> getPosts();
	void viewFriendsPosts();
	void receiveMessage(Message*);
	virtual void sendMessage(User*, Message*);
	void viewReceivedMessages();
};
