#pragma once
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
	void SwitchWeapon(WeaponType type);
	int GetDamageOutput();
	int GetAmo() const;
	void AddAmo(int amout, WeaponType type);
	void PickUpWeapon(WeaponType type);
	void Shoot(Shootable* object, float windowWidth);
	void Shoot(std::vector<Shootable*> shootables, float windowWidth);
	
	int GetPickedUpWeaponsNum() const;
	int GetPistolAmo() const;
	int GetMaxPistolAmo() const;
	int GetShotgunAmo() const;
	int GetMaxShotgunAmo() const;
private:
	WeaponType m_equipedWeapon{};
	bool m_UnlockedWeapons[4]{true, false, false, false}; // pistol is the starting weapon, still working on this system
	int m_pistolDamage{ 2 }, m_shotgunDamage{ 5 };
	int m_pistolAmo{50}, m_shotgunAmo{}; //starting amo
	int m_pistolMaxAmo{ 200 }, m_shotgunMaxAmo{50}; //starting amo

	bool m_pistolPickedUp{ true }, m_shotgunPickedUp{false}; // hence why i have booleans twice
};

