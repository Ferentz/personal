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
	void DrawUI(const float width,
		int health, int armor, int amo) const;
	void DrawOverlay();
	
private:
	//update helper
	void UpdateOverlay(float elapsedSecs, int curHealth);

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
	int m_PrevArmourHealth{};
	int m_PrevAmo[2]{};
	bool m_PickedUp{false};
	bool m_Hurt{false};

	const float m_OverlayTime{ 0.05f };
	float m_TimerOverlay{ 0.f };
	const float m_IntervalTime{.5f};

	float m_Timer{ 0.f };
	const float m_DiffFaceTime{ 2.f };
	float m_DiffFaceTimer{};
	bool HurtFalse_ShootTrue{};
	
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

