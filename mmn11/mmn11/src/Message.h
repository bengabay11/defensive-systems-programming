#pragma once
#include <string>

using namespace std;

class Message {
private:
	string text;
public:
	Message(string text);
	string getText();
};

ostream& operator << (ostream& out, Message& message);
