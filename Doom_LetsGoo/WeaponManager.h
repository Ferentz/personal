#pragma once
class WeaponManager
{
public:
	void SwitchWeapon(int weaponIndex);
	int GetDamageOutput();
private:
	int m_equipedWeapon{0};
	int m_pistolDamage{ 2 }, m_shotgunDamage{ 5 };
	int m_pistolAmo{}, m_shotgunAmo{};
	bool m_pistolPickedUp{ true }, m_shotgunPickedUp{false};
};

