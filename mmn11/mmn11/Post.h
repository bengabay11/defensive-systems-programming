#pragma once
#include <string>
#include "Media.h"

using namespace std;

class Post {
private:
	string text;
	Media* media;
public:
	Post(string text);
	Post(string text, Media* media);
	~Post();
	string getText() const;
	Media* getMedia() const;
};

ostream& operator<< (ostream& out, const Post& post);
