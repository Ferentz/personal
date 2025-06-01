#pragma once
#include <iostream>
class Texture;
class Player;

class UI final
{
public:
	UI(const Rectf& window);
	~UI();

	void DrawUI(const float width,
		int health, int armor, int amo);
	static const Player*  s_pPlayer;
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

};

