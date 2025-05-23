#pragma once
#include <iostream>
class Texture;
class Player;

class UI final
{
public:
	UI(const Rectf& window);
	~UI();
	void DrawUI(const float width, float bottomOfset, float leftOffset, float scale,
		int health, int armor, int amo);
	static Player* s_pPlayer;
	static void AssignPlayer(Player* pPlayer);

private:
	int GetDigit(int number, int index);
	void PrintNumber(int number, bool percent, int numberSwitch);

	void DrawRedNumbers();
	void DrawArms();
	void DrawAmo();

	const Rectf& m_Window;

	Texture* m_pUI;
	Texture* m_pNumbersRed[12];
	Texture* m_pNumbersGreyFlat[10];
	Texture* m_pNumbersGreyShadow[10];
	Texture* m_pNumbersYellow[10];
	Texture* m_pArmsBase;

	/*int m_mainStatBottomOffset{ 38 };
	
	int m_MainStatScale{2};
	int m_AmoLeftOffset{ -266 };

	int m_HealthLeftOffset{ 268 };
	int m_ArmourLeftOffset{ 186 };*/

	float m_mainStatBottomOffset{ 0.04f };

	float m_MainStatScale{ 0.0023f };

	float m_AmoLeftOffset{-0.003f };

	float m_HealthLeftOffset{ 0.145f };
	float m_ArmourLeftOffset{ 0.408f };
};

