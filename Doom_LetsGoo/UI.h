#pragma once
#include <iostream>
class Texture;
class UI final
{
public:
	UI(const Rectf& window);
	~UI();
	void DrawUI(const float width, float bottomOfset, float leftOffset, float scale,
		int health, int armor, int amo);
private:
	int GetDigit(int number, int index);
	void PrintNumber(int number, bool percent);

	const Rectf& m_Window;

	Texture* m_pUI;
	Texture* m_pNumbers[12];

	/*int m_mainStatBottomOffset{ 38 };
	
	int m_MainStatScale{2};
	int m_AmoLeftOffset{ -266 };

	int m_HealthLeftOffset{ 268 };
	int m_ArmourLeftOffset{ 186 };*/

	float m_mainStatBottomOffset{ 0.04f };

	float m_MainStatScale{ 0.0023 };

	float m_AmoLeftOffset{0.12f };

	float m_HealthLeftOffset{ 0.182f };
	float m_ArmourLeftOffset{ 0.408f };
};

