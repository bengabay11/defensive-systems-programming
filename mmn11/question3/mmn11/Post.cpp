#include "Post.h"
#include <iostream>

Post::Post(std::string text): text(text), media(nullptr) {}

Post::Post(std::string text, Media* media): text(text), media(media) {}

Post::~Post()
{
    delete this->media;
}

Post::Post(const Post& other): text(other.text), media(nullptr)
{
    if (other.media) {
        this->media = other.media->clone();
    }
}

Post& Post::operator=(const Post& other)
{
    if (this == &other) {
        return *this;
    }

    delete this->media;
    this->media = nullptr;
    if (other.media) {
        this->media = other.media->clone();
    }

    this->text = other.text;

    return *this;
}

std::string Post::getText()
{
    return this->text;
}

Media* Post::getMedia()
{
    return this->media;
}

std::ostream& operator<<(std::ostream& out, const Post& post)
{
    out << "Text: " << post.text << std::endl;
    Media* media = post.media;
    if (media != nullptr) {
        out << "Media: ";
        media->display();
    }
    return out;
}
