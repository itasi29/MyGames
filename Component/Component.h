#pragma once
#include <iostream>

class Component abstract
{
public:
	Component() {}
	virtual ~Component() {}

	virtual void ShowName() const abstract;
};

class UI : public Component
{
public:
	UI() {}
	~UI() {}

	void ShowName() const override
	{
		std::cout << "UI" << std::endl;
	}
};

class Text : public Component
{
public:
	Text() {}
	~Text() {}

	void ShowName() const override
	{
		std::cout << "Text" << std::endl;
	}
};

class Pos : public Component
{
public:
	Pos() {}
	~Pos() {}

	void ShowName() const override
	{
		std::cout << "Pos" << std::endl;
	}
};