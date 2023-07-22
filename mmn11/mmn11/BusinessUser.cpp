#include "BusinessUser.h"

BusinessUser::BusinessUser(unsigned long id, USocial* us, string name): User(id, us, name) {}

void BusinessUser::sendMessage(User* user, Message* message)
{
	user->receiveMessage(message);
}
