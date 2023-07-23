#pragma once

#include "Media.h"

class Audio :
    public Media
{
public:
    void display() override;
    Media* clone() const override;
};
