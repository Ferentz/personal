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
	Texture* m_pStripe;
	Texture* m_pZero;
	Texture* m_pOne;
	Texture* m_pTwo;
	Texture* m_pThree;
	Texture* m_pFour;
	Texture* m_pFive;
	Texture* m_pSix;
	Texture* m_pSeven;
	Texture* m_pEight;
	Texture* m_pNine;
	Texture* m_pPercent;
};

