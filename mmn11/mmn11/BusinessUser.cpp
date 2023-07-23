#include "BusinessUser.h"

BusinessUser::BusinessUser(USocial* us, unsigned long id, std::string name): User(us, id, name) {}

/**
 * @brief Send a message to other user.
 *
 * A business user can send messages for anyone, 
 * so the function will not check if the given user is in the friends list.
 * @param user The user that receives the message.
 * @param message The message to be sent.
 */
void BusinessUser::sendMessage(User* user, Message* message)
{
	user->receiveMessage(message);
}
