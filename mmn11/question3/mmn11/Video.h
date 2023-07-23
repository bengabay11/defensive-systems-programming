#pragma once

#include "Media.h"

class Video :
    public Media
{
public:
    void display() override;
    Media* clone() const override;
};
