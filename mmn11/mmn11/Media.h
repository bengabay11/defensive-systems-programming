#pragma once

class Media {
public:
	virtual void display() = 0;
	virtual Media* clone() const = 0;
};
