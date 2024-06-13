#include <iostream>
#include "GameObject.h"
#include "Component.h"

int main()
{
	GameObject test;

	std::cout << "yAddz" << std::endl;
	test.AddComponent<UI>();
	test.AddComponent<UI>();
	test.AddComponent<UI>();
	test.AddComponent<Text>();
	test.ListShow();

	std::cout << "yGetz" << std::endl;
	auto text = test.GetComponent<Text>();
	const auto& ui = test.GetComponent<UI>();
	text->ShowName();

	std::cout << "yDelz" << std::endl;
	test.DelComponent<UI>();
	test.ListShow();

	return 0;
}
