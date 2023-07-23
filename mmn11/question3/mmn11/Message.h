#pragma once

#include <string>

class Message {
private:
	std::string text;
public:
	Message(std::string text);
	std::string getText() const;
};

std::ostream& operator << (std::ostream& out, const Message& message);
