#pragma once
#include <iostream>
class Texture;
class UI final
{
public:
	UI();
	~UI();
	void DrawUI(const float width, float bottomOfset, float leftOffset, float scale,
		int health, int armor, int amo);
private:
	int GetDigit(int number, int index);
	void PrintNumber(int number, bool percent);

	Texture* m_pUI;
	Texture* m_pNumbers[12];

	int m_mainStatBottomOffset{ 38 };
	
	int m_MainStatScale{2};
	int m_AmoLeftOffset{ -266 };

	int m_HealthLeftOffset{ 268 };
	int m_ArmourLeftOffset{ 186 };
};

