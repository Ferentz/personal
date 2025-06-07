#pragma once
#include "Shotgun.h"
#include "Pistol.h"
#include <vector>

class Shootable;
enum class WeaponType {
	pistol,
	shotgun,
	chaingun,
	rocket
};
// can also make teh weapon type a class weapen, and then teh manager has objects of that class
class WeaponManager final
{
public:

	//basics
	void DrawWeapon(float windowWidth) const;
	void UpdateWeapon(float elapsedsecs);

	//general
	void SwitchWeapon(WeaponType type);
	int GetAmo() const;
	void AddAmo(int amout, WeaponType type);
	void PickUpWeapon(WeaponType type);
	void Shoot(std::vector<Shootable*> shootables, float windowWidth);
	int GetPickedUpWeaponsNum() const;

	//weapon specific
	int GetPistolAmo() const;
	int GetMaxPistolAmo() const;
	int GetShotgunAmo() const;
	int GetMaxShotgunAmo() const;

private:
	Shotgun m_Shotgun{ 0, 50, 1000.f };
	Pistol m_Pistol{ 50, 200, 1000.f };
	Weapon* m_pEquipedWeapon{ &m_Pistol };
	// basicaly redundant, but  the game at large has more weapons, afer which having seperate bools would be clunky
	bool m_UnlockedWeapons[4]{true, false, false, false}; // pistol is the starting weapon, still working on this system

	bool m_pistolPickedUp{ true }, m_shotgunPickedUp{false}; // hence why i have booleans twice
	SoundEffect m_WeaponPickupSound{ "sounds/DSSGCOCK.wav" };

};

