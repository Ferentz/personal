#pragma once
class Thing;
enum class WeaponType {
	pistol,
	shotgun,
	chaingun,
	rocket
};

class WeaponManager final
{
public:
	void SwitchWeapon(WeaponType type);
	int GetDamageOutput();
	int GetAmo() const;
	void AddAmo(int amout, WeaponType type);
	void PickUpWeapon(WeaponType type);
	void Shoot(Thing* object, float windowWidth);

private:
	WeaponType m_equipedWeapon{};
	bool m_UnlockedWeapons[4]{true, false, false, false}; // pistol is the starting weapon, still working on this system
	int m_pistolDamage{ 2 }, m_shotgunDamage{ 5 };
	int m_pistolAmo{50}, m_shotgunAmo{}; //starting amo
	bool m_pistolPickedUp{ true }, m_shotgunPickedUp{false}; // hence why i have booleans twice
};

