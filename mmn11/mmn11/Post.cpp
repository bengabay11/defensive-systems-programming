#include "Post.h"
#include <iostream>

Post::Post(string text): text(text), media(nullptr) {}

Post::Post(string text, Media* media): text(text), media(media) {}

Post::~Post()
{
    delete this->media;
}

string Post::getText() const
{
    return this->text;
}

Media* Post::getMedia() const
{
    return this->media;
}

ostream& operator<<(ostream& out, const Post& post)
{
    out << "Text: " << post.getText() << endl;
    Media* media = post.getMedia();
    if (media != nullptr) {
        out << "Media: ";
        media->display();
    }
    return out;
}
