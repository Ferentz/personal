#pragma once
#include <vector>
#include "SoundEffect.h"
class Shootable;
class Texture;
class Weapon
{
public:

	Weapon(int startAmo, int maxAmo, float range, std::string soundName);
	virtual ~Weapon() = default;

	//rule of 5
	Weapon(const Weapon& toCopy) = delete;
	Weapon(Weapon&& rhs) = delete;
	Weapon& operator=(const Weapon& rhs) = delete;
	Weapon& operator=(Weapon&& rhs) = delete;

	//basics
	virtual void DrawWeapon(float windowWidth) const = 0;
	virtual void UpdateWeapon(float elapsedsecs) = 0;

	//shoot
	virtual void Shoot(std::vector<Shootable*>& shootables, float windowWidth);

	//interactions
	int GetMaxAmo() const;
	int GetAmo() const;
	void ChangeAmoBy(int amount);
	void IncreaseMaxAmo(int amount);

	
protected:
	//shoot helpers
	void Shoot(Shootable* object);
	virtual int GetDamageOutput() const;
	float GetRange() const;
	float GetScreenRange() const;
	bool GetIsShooting() const;
	

	int m_ShootOverlayNumFrames{1};
	int m_AnimationFrames{5};
	static const int s_WeaponTexAmount{6};

	Texture* m_WeaponImages[s_WeaponTexAmount]{ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr };
	int m_TextureOffsets[s_WeaponTexAmount][2]{ {0,0}, {0,0}, {0,0}, {0,0}, {0,0}, {0,0} };
	
	bool m_IsShooting{ false };

	SoundEffect m_GunShootSound;

private:
	
	int m_MaxAmo;
	int m_Amo;
	float m_Range;
	const float m_ScreenRange{ 50 };
};

