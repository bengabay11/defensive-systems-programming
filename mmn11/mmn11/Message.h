#pragma once
#include <string>

using namespace std;

class Message {
private:
	string text;
public:
	Message(string text);
	string getText() const;
};

ostream& operator << (ostream& out, const Message& message);
