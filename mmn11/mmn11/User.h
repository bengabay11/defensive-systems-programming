#pragma once
#include <string>
#include <list>
#include "Post.h"
#include "Message.h"

using namespace std;

class USocial;

class User {
protected:
	USocial* us;
	friend class USocial;
	unsigned long id;
	string name;
	list<unsigned long> friends;
	list<Post* > posts;
	list<Message*> receivedMsgs;
	User();
	~User();
public:
	unsigned long getId();
	string getName();
	void addFriend(User* user);
	void removeFriend(User*);
	void post(string);
	void post(string, Media*);
	list<Post*> getPosts();
	void viewFriendsPosts();
	void receiveMessage(Message*);
	virtual void sendMessage(User*, Message*);
	void viewReceivedMessages();
};
