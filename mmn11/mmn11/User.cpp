#include "User.h"
#include <iostream>
#include "USocial.h"

User::User() {}

User::~User()
{
	delete this->us;
	for (auto post : this->posts) {
		delete post;
	}
	for (auto message : this->receivedMsgs) {
		delete message;
	}
}

unsigned long User::getId()
{
	return this->id;
}

string User::getName()
{
	return this->name;
}

list<Post*> User::getPosts()
{
	return this->posts;
}

/**
 * @brief Print all posts of the user's friends
 */
void User::viewFriendsPosts()
{
	for (unsigned long friend_id : this->friends) {
		User* user = this->us->getUserById(friend_id);
		cout << user->getName() << "'s posts:" << endl;
		for (Post* post : user->posts) {
			cout << *post << endl;
		}
	}
}

/**
 * @brief Receive a new message.
 * 
 * The function will add the given message to the received messages list.
 * @param message the message that the user will receive.
 */
void User::receiveMessage(Message* message)
{
	this->receivedMsgs.push_back(message);
}

/**
 * @brief Send a message to other user.
 * 
 * if the given user is not in the friends list, runtime_error will be thrown.
 * @param user The user that receives the message.
 * @param message The message to be sent.
 */
void User::sendMessage(User* user, Message* message)
{
	auto it = find(this->friends.begin(), this->friends.end(), user->getId());
	if (it != this->friends.end()) {
		user->receiveMessage(message);
	}
	else {
		throw runtime_error("Failed sending a message: "  + user->getName() + " is not a friend of  " + this->getName() + ", so the message cannot be sent");
	}
}

/**
 * @brief Print all the messages of the user
 */
void User::viewReceivedMessages()
{
	for (Message* message : this->receivedMsgs) {
		cout << *message << endl;
	}
}

/**
 * @brief Add friend to the list of friends.
 * @param user The friend user to add.
 */
void User::addFriend(User* user)
{
	auto it = std::find(this->friends.begin(), this->friends.end(), user->getId());
	if (it != this->friends.end()) {
		throw runtime_error("Add friend failed: " + user->getName() + " is already a friend of " + this->getName());
	}
	else {
		this->friends.push_back(user->getId());
	}
}

/**
 * @brief Remove friend from the list of friends.
 * @param user The friend user to remove.
 */
void User::removeFriend(User* user)
{
	auto it = std::find(this->friends.begin(), this->friends.end(), user->getId());
	if (it != this->friends.end()) {
		this->friends.remove(user->getId());
	}
	else {
		throw runtime_error("Remove friend failed: " + user->getName() + " is not a friend of " + this->getName());
	}
	this->friends.remove(user->getId());
}

/**
 * @brief Add post for the user.
 * @param text The text of the post.
 */
void User::post(string text)
{
	Post* new_post = new Post(text);
	this->posts.push_back(new_post);
}

/**
 * @brief Add post for the user.
 * @param text The text of the post.
 * @param media The media of the post.
 */
void User::post(string text, Media* media)
{
	Post* new_post = new Post(text, media);
	this->posts.push_back(new_post);
}
