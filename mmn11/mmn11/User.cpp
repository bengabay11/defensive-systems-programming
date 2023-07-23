#include "User.h"
#include <iostream>
#include "USocial.h"
#include "FriendAlreadyExistsException.h"
#include "NotAFriendException.h"

User::User(USocial* us, unsigned long id, std::string name) : us(us), id(id), name(name) {}

User::~User()
{
	for (auto post : this->posts) {
		delete post;
	}
	for (auto message : this->receivedMsgs) {
		delete message;
	}
}

User::User(const User& other): us(other.us), id(other.id), name(other.name), friends(), posts(), receivedMsgs()
{
	for (unsigned long friendId : other.friends) {
		this->friends.push_back(friendId);
	}
	for (auto post : other.posts) {
		this->posts.push_back(new Post(*post));
	}
	for (auto message : other.receivedMsgs) {
		this->receivedMsgs.push_back(new Message(*message));
	}
}

User& User::operator=(const User& other)
{
	if (this == &other) {
		return *this;
	}

	for (auto post : this->posts) {
		delete post;
	}
	for (auto message : this->receivedMsgs) {
		delete message;
	}

	this->friends.clear();
	this->posts.clear();
	this->receivedMsgs.clear();
	for (unsigned long friendId : other.friends) {
		this->friends.push_back(friendId);
	}
	for (auto post : other.posts) {
		this->posts.push_back(new Post(*post));
	}
	for (auto message : this->receivedMsgs) {
		this->receivedMsgs.push_back(new Message(*message));
	}

	this->id = other.id;
	this->name = other.name;
	this->us = other.us;
}

unsigned long User::getId()
{
	return this->id;
}

std::string User::getName()
{
	return this->name;
}

std::list<Post*> User::getPosts()
{
	return this->posts;
}

/**
 * @brief Print all posts of the user's friends
 */
void User::viewFriendsPosts()
{
	for (unsigned long friendId : this->friends) {
		User* user = this->us->getUserById(friendId);
		std::cout << user->getName() << "'s posts:" << std::endl;
		for (Post* post : user->posts) {
			std::cout << *post << std::endl;
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
 * if the given user is not in the friends list, NotAFriendException will be thrown.
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
		throw NotAFriendException(this->getName(), user->getName());
	}
}

/**
 * @brief Print all the messages of the user
 */
void User::viewReceivedMessages()
{
	for (Message* message : this->receivedMsgs) {
		std::cout << *message << std::endl;
	}
}

/**
 * @brief Add friend to the list of friends.
 * 
 * if the given user is in the friends list, FriendAlreadyExistsException will be thrown.
 * @param user The friend user to add.
 */
void User::addFriend(User* user)
{
	auto it = std::find(this->friends.begin(), this->friends.end(), user->getId());
	if (it != this->friends.end()) {
		throw FriendAlreadyExistsException(this->getName(), user->getName());
	}
	else {
		this->friends.push_back(user->getId());
	}
}

/**
 * @brief Remove friend from the list of friends.
 * 
 * if the given user is not in the friends list, NotAFriendException will be thrown.
 * @param user The friend user to remove.
 */
void User::removeFriend(User* user)
{
	auto it = std::find(this->friends.begin(), this->friends.end(), user->getId());
	if (it != this->friends.end()) {
		this->friends.remove(user->getId());
	}
	else {
		throw NotAFriendException(this->getName(), user->getName());
	}
	this->friends.remove(user->getId());
}

/**
 * @brief Add post for the user.
 * @param text The text of the post.
 */
void User::post(std::string text)
{
	Post* newPost = new Post(text);
	this->posts.push_back(newPost);
}

/**
 * @brief Add post for the user.
 * @param text The text of the post.
 * @param media The media of the post.
 */
void User::post(std::string text, Media* media)
{
	Post* new_post = new Post(text, media);
	this->posts.push_back(new_post);
}
