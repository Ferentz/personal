#include "pch.h"
#include "WeaponManager.h"

void WeaponManager::SwitchWeapon(int weaponIndex)
{
	switch (weaponIndex)
	{
	case 0 :
		if (m_pistolPickedUp)
		{

		}
		break;
	case 1:
		if (m_shotgunPickedUp)
		{

		}
		break;
	default:
		break;
	}
}

int WeaponManager::GetDamageOutput()
{
	switch (m_equipedWeapon)
	{
	case 0:
		return m_pistolDamage;
		break;
	case 1:
		return m_shotgunDamage;
		break;
	default:
		return 0;
		break;
	}
}
