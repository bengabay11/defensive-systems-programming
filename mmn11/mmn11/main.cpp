#include "USocial.h"
#include "User.h"
#include <exception>
#include <iostream>

void example() {
	USocial us;
	User* u1 = us.registerUser("Liron");
	User* u2 = us.registerUser("Yahav");
	User* u3 = us.registerUser("Shachaf");
	User* u4 = us.registerUser("Tsur", true);
	User* u5 = us.registerUser("Elit");
	u1->post("Hello world!");
	u2->post("I'm having a great time here :)", new Audio());
	u3->post("This is awesome!", new Photo());
	u5->addFriend(u1);
	u5->addFriend(u2);
	cout << "All friends posts of " << u5->getName() << ":" << endl;
	u5->viewFriendsPosts(); // should see only u1, u2 s' posts
	u4->sendMessage(u5, new Message("Buy Falafel!"));
	cout << "All received messages of " << u5->getName() << ":" << endl;
	u5->viewReceivedMessages();
	try
	{
		u3->sendMessage(u5, new Message("All your base are belong to us"));
	}
	catch (const std::exception& e)
	{
		std::cout << "error: " << e.what() << std::endl;
	}
	cout << "All received messages of " << u5->getName() << ":" << endl;
	u5->viewReceivedMessages();
	u3->addFriend(u5);
	u3->sendMessage(u5, new Message("All your base are belong to us"));
	cout << "All received messages of " << u5->getName() << ":" << endl;
	u5->viewReceivedMessages();
}

int main()
{
	example();
	return 0;
}
