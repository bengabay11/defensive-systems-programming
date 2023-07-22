#include "Post.h"
#include <iostream>

Post::Post(std::string text): text(text), media(nullptr) {}

Post::Post(std::string text, Media* media): text(text), media(media) {}

Post::~Post()
{
    delete this->media;
}

std::string Post::getText() const
{
    return this->text;
}

Media* Post::getMedia() const
{
    return this->media;
}

std::ostream& operator<<(std::ostream& out, const Post& post)
{
    out << "Text: " << post.getText() << std::endl;
    Media* media = post.getMedia();
    if (media != nullptr) {
        out << "Media: ";
        media->display();
    }
    return out;
}
