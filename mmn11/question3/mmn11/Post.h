#pragma once

#include <string>
#include "Media.h"

class Post {
private:
	std::string text;
	Media* media;
public:
	Post(std::string text);
	Post(std::string text, Media* media);
	~Post();
	Post(const Post& other);
	Post& operator=(const Post& other);
	std::string getText();
	Media* getMedia() ;
	friend std::ostream& operator<< (std::ostream& out, const Post& post);
};
