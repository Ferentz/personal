#include "pch.h"
#include <iostream>
#include "WeaponManager.h"
#include "Enemy.h"
#include "Plane.h"
#include "Navigator.h"
#include "HealthManager.h"


void WeaponManager::SwitchWeapon(WeaponType type)
{
	switch (type)
	{
	case WeaponType::pistol :
		if (m_pistolPickedUp)
		{
			m_equipedWeapon = type;
		}
		break;
	case WeaponType::shotgun:
		if (m_shotgunPickedUp)
		{
			m_equipedWeapon = type;
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
	case WeaponType::pistol:
	{
		int damage{ (rand() % 11) + 5 };
		//return m_pistolDamage;
		std::cout << damage;
		return damage;
		break;
	}
	case WeaponType::shotgun:
	{
		int damage{ ((rand() % 11) + 5) * ((rand() % 7)+1) };
		//return m_shotgunDamage;
		std::cout << damage;
		return damage;
		break;
	}
	default:
		return 0;
		break;
	}
}

int WeaponManager::GetAmo() const
{
	switch (m_equipedWeapon)
	{
	case WeaponType::pistol:
		return m_pistolAmo;
		break;
	case WeaponType::shotgun:
		return m_shotgunAmo;
		break;
	default:
		return 0;
		break;
	}
}

void WeaponManager::AddAmo(int amount, WeaponType type)
{
	switch (type)
	{
	case WeaponType::pistol:
		m_pistolAmo += amount;
		break;
	case WeaponType::shotgun:
		m_shotgunAmo += amount;
		break;
	default:
		break;
	}
}

void WeaponManager::PickUpWeapon(WeaponType type)
{
	int typeIndex{ int(type) };
	if (!m_UnlockedWeapons[typeIndex])
	{
		m_UnlockedWeapons[typeIndex] = true;
		m_shotgunPickedUp = true;
	}
	m_shotgunPickedUp = true;
}

void WeaponManager::Shoot(Shootable* object, float windowWidth)
{
	int shootRange{ 100 };
	int* p_UsedAmo{};
	switch (m_equipedWeapon)
	{
	case WeaponType::pistol:
		p_UsedAmo = &m_pistolAmo;
		break;
	case WeaponType::shotgun:
		p_UsedAmo = &m_shotgunAmo;
		break;
	default:
		return;
	}

	if (*p_UsedAmo > 0)
	{
		std::cout << "SHOOT" << std::endl;
		if (object->GetLocationPoint().point2D.x < windowWidth / 2 + shootRange && object->GetLocationPoint().point2D.x > windowWidth / 2 - shootRange)
		{
			object->BeShot(GetDamageOutput());
		}
		//(*p_UsedAmo)--;
	}
}

void WeaponManager::Shoot(std::vector<Shootable*> shootables, float windowWidth)
{

	int shootRange{ 100 };
	int* p_UsedAmo{};
	switch (m_equipedWeapon)
	{
	case WeaponType::pistol:
		p_UsedAmo = &m_pistolAmo;
		break;
	case WeaponType::shotgun:
		p_UsedAmo = &m_shotgunAmo;
		break;
	default:
		return;
	}

	if (*p_UsedAmo > 0)
	{

		float shootRangeDist{ 1000.f };

		std::vector<float> distances{};
		distances.reserve(shootables.size());
		switch (m_equipedWeapon)
		{
		case WeaponType::pistol:
		{
			(*p_UsedAmo)--;
			int closest{ -1 };
			float closestDist{ shootRangeDist };
			for (int index{}; index < shootables.size(); index++)
			{
				if (closestDist > shootables[index]->GetLocationPoint().Dist3D)
				{
					closest = index;
					closestDist = shootables[index]->GetLocationPoint().Dist3D;
				}
			}

			if (closest >= 0)
			{
				Shoot(shootables[closest], windowWidth);
			}
			break;
		}
		case WeaponType::shotgun:
		{
			(*p_UsedAmo)--;
			for (int bulletNum{}; bulletNum < 7; bulletNum++)
			{
				float bulletX{ rand() % (int(windowWidth / 3)) + windowWidth / 3 };

				int closest{ -1 };
				float closestDist{ shootRangeDist };
				for (int index{}; index < shootables.size(); index++)
				{
					float xDif{ shootables[index]->GetLocationPoint().point2D.x - bulletX };
					if (abs(xDif) < windowWidth / 24 &&
						closestDist > shootables[index]->GetLocationPoint().Dist3D)
					{
						closest = index;
						closestDist = shootables[index]->GetLocationPoint().Dist3D;
					}
				}

				if (closest >= 0)
				{
					Shoot(shootables[closest], windowWidth);
				}
			}

		break;
	}
	default:
		return;
	}
	}
}

int WeaponManager::GetPickedUpWeaponsNum() const
{
	if (m_shotgunPickedUp)
	{
		return 3;
	}
	return 2;
}

int WeaponManager::GetPistolAmo() const
{
	return m_pistolAmo;
}

int WeaponManager::GetMaxPistolAmo() const
{
	return m_pistolMaxAmo;
}

int WeaponManager::GetShotgunAmo() const
{
	return m_shotgunAmo;
}

int WeaponManager::GetMaxShotgunAmo() const
{
	return m_shotgunMaxAmo;
}
