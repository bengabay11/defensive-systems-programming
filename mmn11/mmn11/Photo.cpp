#include "Photo.h"
#include <iostream>

void Photo::display()
{
	std::cout << "image" << std::endl;
}

Media* Photo::clone() const
{
	return new Photo();
}
