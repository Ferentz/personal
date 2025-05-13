#pragma once
#include <iostream>
class Texture;
class UI
{
public:
	UI();
	~UI();
	void DrawUI(const float width, float bottomOfset, float leftOffset, float scale, int health);
	int GetDigit(int number, int index);
	Texture* m_pUI;
	Texture* m_pNumbers[12];
};

