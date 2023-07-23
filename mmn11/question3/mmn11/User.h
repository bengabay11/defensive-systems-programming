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
	User(USocial* us, unsigned long id, std::string name);
	~User();
	User(const User& other);
	User& operator=(const User& other);
public:
	unsigned long getId();
	std::string getName();
	void addFriend(User* user);
	void removeFriend(User* user);
	void post(std::string text);
	void post(std::string text, Media* media);
	std::list<Post*> getPosts();
	void viewFriendsPosts();
	void receiveMessage(Message* message);
	virtual void sendMessage(User* user, Message* message);
	void viewReceivedMessages();
};
