#pragma once
#include "Weapon.h"
class Pistol : public Weapon
{
public:
	Pistol(int startAmo, int maxAmo, float range);
	virtual void Shoot(std::vector<Shootable*> shootables, float windowWidth) override;
	virtual void UpdateWeapon(float elapsedsecs) override;
	virtual void DrawWeapon(float windowWidth) const override;
private:
	int m_FireFrame{ 1 };

	bool m_DrawFire{ false };
	bool m_Reverse{ false };
	float m_timer{ 0.f };
	int m_ImageIndex{ 0 };
	float m_ImageRefresh{ 0.1f };
};

