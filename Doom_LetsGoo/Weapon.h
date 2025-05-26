#pragma once
#include <vector>
#include "SoundEffect.h"
class Shootable;
class Texture;
class Weapon
{
public:

	Weapon(int startAmo, int maxAmo, float range, std::string soundName);
	~Weapon();

	Weapon(const Weapon& toCopy) = delete;
	Weapon(Weapon&& rhs) = delete;
	Weapon& operator=(const Weapon& rhs) = delete;
	Weapon& operator=(Weapon&& rhs) = delete;

	virtual void Shoot(std::vector<Shootable*> shootables, float windowWidth);

	int GetMaxAmo() const;
	int GetAmo() const;

	void ChangeAmoBy(int amount);
	void IncreaseMaxAmo(int amount);

	virtual void DrawWeapon(float windowWidth) const = 0;
	virtual void UpdateWeapon(float elapsedsecs) = 0;
protected:
	int m_ShootOverayNum{1};
	int m_AnimationFrames{5};
	//int m_FireFrame{ 1 };
	virtual int GetDamageOutput() const;
	void Shoot(Shootable* object);
	float GetRange() const;
	float GetScreenRange() const;
	Texture* m_WeaponImages[6]{nullptr,nullptr, nullptr, nullptr, nullptr, nullptr};
	int m_TextureOffsets[6][2]{ {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0}};
	bool GetIsShooting() const;
	bool m_IsShooting{ false };

	SoundEffect m_GunShootSound;

private:
	/*bool m_IsShooting{ false };
	bool m_DrawFire{false};
	bool m_Reverse{ false };
	float m_timer{0.f};
	int m_ImageIndex{ 0 };
	float m_ImageRefresh{ 0.1f };*/

	int m_MaxAmo;
	int m_Amo;
	float m_Range;
	float m_ScreenRange{ 50 };
};

