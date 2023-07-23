#include "Audio.h"
#include <iostream>

void Audio::display()
{
	std::cout << "audio" << std::endl;
}

Media* Audio::clone() const
{
	return new Audio();
}
