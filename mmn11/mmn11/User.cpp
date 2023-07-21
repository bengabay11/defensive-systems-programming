#include "User.h"
#include <iostream>
#include "USocial.h"

list<Post*> User::getPosts()
{
	return this->posts;
}

void User::viewFriendsPosts()
{
	for (unsigned long friend_id : this->friends) {
		User* user = this->us->getUserById(friend_id);
		for (Post* post : user->posts) {
			cout << post << endl;
		}
	}
}

void User::receiveMessage(Message* message)
{
	this->receivedMsgs.push_back(message);
}

void User::sendMessage(User* user, Message* message)
{
	user->receiveMessage(message);
}

void User::viewReceivedMessages()
{
	for (Message* message : this->receivedMsgs) {
		cout << message << endl;
	}
}

User::User()
{
}

User::~User()
{
}

unsigned long User::getId()
{
	return this->id;
}

string User::getName()
{
	return this->name;
}

void User::addFriend(User* user)
{
	this->friends.push_back(user->getId());
}

void User::removeFriend(User* user)
{
	this->friends.remove(user->getId());
}

void User::post(string text)
{
	Post* new_post = new Post(text);
	this->posts.push_back(new_post);
}

void User::post(string text, Media* media)
{
	Post* new_post = new Post(text, media);
	this->posts.push_back(new_post);
}
