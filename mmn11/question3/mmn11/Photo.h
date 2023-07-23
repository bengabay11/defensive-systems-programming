#pragma once

#include "Media.h"

class Photo: public Media
{
public:
	void display() override;
	Media* clone() const override;
};
