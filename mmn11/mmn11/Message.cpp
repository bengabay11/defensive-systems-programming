#include "Message.h"
#include <iostream>

using namespace std;

Message::Message(string text)
{
    this->text = text;
}

string Message::getText()
{
    return this->text;
}

ostream& operator<<(ostream& out, Message& message)
{
    cout << message.getText() << endl;
    return out;
}
