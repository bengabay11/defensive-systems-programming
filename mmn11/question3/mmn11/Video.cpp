#include "Video.h"
#include <iostream>

void Video::display()
{
	std::cout << "video" << std::endl;
}

Media* Video::clone() const
{
	return new Video();
}
