
#pragma once


class State
{
public:
	State(){}
	virtual ~State(){}

	virtual bool update() = 0;
	virtual void render() = 0;
};