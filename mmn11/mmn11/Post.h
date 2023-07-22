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
	std::string getText() const;
	Media* getMedia() const;
};

std::ostream& operator<< (std::ostream& out, const Post& post);
