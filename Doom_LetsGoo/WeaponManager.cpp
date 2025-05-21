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

		}
		break;
	case WeaponType::shotgun:
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
	case WeaponType::pistol:
		return m_pistolDamage;
		break;
	case WeaponType::shotgun:
		return m_shotgunDamage;
		break;
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
	}
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
		(*p_UsedAmo)--;
	}
}
