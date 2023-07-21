#include "Post.h"
#include <iostream>

Post::Post(string text): text(text), media(nullptr) {}

Post::Post(string text, Media* media): text(text), media(media) {}

string Post::getText()
{
    return this->text;
}

Media* Post::getMedia()
{
    return this->media;
}

ostream& operator<<(ostream& out, Post& post)
{
    out << post.getText() << endl;
    Media* media = post.getMedia();
    if (media != nullptr) {
        media->display();
    }
    return out;
}
