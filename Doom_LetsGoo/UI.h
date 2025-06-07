#pragma once
#include <iostream>
class Texture;
class Player;

class UI final
{
public:
	explicit UI(const Rectf& window, Player* pPlayer);
	~UI();

	void UpdateUI(float elapsedSecs);
	void DrawOverlay();
	void DrawUI(const float width,
		int health, int armor, int amo) const;
	
private:
	void DrawRedNumbers() const;
	void DrawArms() const;
	void DrawAmo() const;

	//helpers
	int GetDigit(int number, int index) const;
	void PrintNumber(int number, bool percent, int numberSwitch) const;


	static const Player* s_pPlayer;
	static const int s_NormalFaceRows{ 3 };
	static const int s_FaceHurtLevels{ 5 };

	int m_HurtLevel{0};
	bool m_FaceSide{true};
	int m_FaceIndex{1};
	int m_PrevHealth{};
	int m_PrevAmo[2]{};
	bool m_PickedUp{};
	bool m_Hurt{};
	const float m_IntervalTime{.5f};
	float m_Timer{ 0.f };
	const float m_DiffFaceTime{ 2.f };
	float m_DiffFaceTimer{};
	bool HurtF_ShootT{};
	
	Texture* m_pBasisUI;
	Texture* m_pNumbersRed[12];
	Texture* m_pNumbersGreyFlat[10];
	Texture* m_pNumbersGreyShadow[10];
	Texture* m_pNumbersYellow[10];
	Texture* m_pBasisArms;
	
	Texture* m_pDoomguyFaceNormal[s_NormalFaceRows * s_FaceHurtLevels];
	Texture* m_pDoomguyFaceOuch[s_FaceHurtLevels];
	Texture* m_pDoomguyFaceKill[s_FaceHurtLevels];
	const Rectf& r_Window;

};

