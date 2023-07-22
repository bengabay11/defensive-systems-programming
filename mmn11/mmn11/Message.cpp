#include "Message.h"
#include <iostream>

using namespace std;

Message::Message(string text)
{
    this->text = text;
}

string Message::getText() const
{
    return this->text;
}

ostream& operator<<(ostream& out, const Message& message)
{
    cout << message.getText() << endl;
    return out;
}
