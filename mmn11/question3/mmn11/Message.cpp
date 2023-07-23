#include "Message.h"
#include <iostream>

Message::Message(std::string text)
{
    this->text = text;
}

std::string Message::getText() const
{
    return this->text;
}

std::ostream& operator<<(std::ostream& out, const Message& message)
{
    std::cout << message.getText() << std::endl;
    return out;
}
