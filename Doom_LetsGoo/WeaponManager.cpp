#include "pch.h"
#include <iostream>
#include "WeaponManager.h"
#include "Enemy.h"
#include "utils.h"

#pragma region basics
void WeaponManager::DrawWeapon(float windowWidth) const
{
	glPushMatrix();
	{
		glScalef(windowWidth / 300, windowWidth / 300, 0.f);
		glTranslatef(0.f, 180, 0.f);
		m_pEquipedWeapon->DrawWeapon(windowWidth);
	}
	glPopMatrix();
}

void WeaponManager::UpdateWeapon(float elapsedsecs)
{
	m_pEquipedWeapon->UpdateWeapon(elapsedsecs);
}

#pragma endregion basics

#pragma region general
void WeaponManager::SwitchWeapon(WeaponType type)
{
	switch (type)
	{
	case WeaponType::pistol :
			m_pEquipedWeapon = &m_Pistol;
		break;
	case WeaponType::shotgun:
		if (m_shotgunPickedUp)
		{
			m_pEquipedWeapon = &m_Shotgun;
		}
		break;
	default:
		break;
	}
}

int WeaponManager::GetAmo() const
{
	return m_pEquipedWeapon->GetAmo();
}

void WeaponManager::AddAmo(int amount, WeaponType type)
{
	switch (type)
	{
	case WeaponType::pistol:
		m_Pistol.ChangeAmoBy(amount);
		break;
	case WeaponType::shotgun:
		m_Shotgun.ChangeAmoBy(amount);
		break;
	default:
		break;
	}
}

void WeaponManager::PickUpWeapon(WeaponType type)
{
	//SOOUND m_WeaponPickupSound
	utils::PlaySoundwPriority(m_WeaponPickupSound, 8, false);
	int typeIndex{ int(type) };
	if (!m_UnlockedWeapons[typeIndex])
	{
		m_UnlockedWeapons[typeIndex] = true;
		m_shotgunPickedUp = true;
	}
	m_shotgunPickedUp = true;
}

void WeaponManager::Shoot(std::vector<Shootable*> shootables, float windowWidth)
{
	m_pEquipedWeapon->Shoot(shootables, windowWidth);
}

int WeaponManager::GetPickedUpWeaponsNum() const
{
	if (m_shotgunPickedUp)
	{
		return 3;
	}
	return 2;
}

#pragma endregion general

#pragma region weapon specific

int WeaponManager::GetPistolAmo() const
{
	return m_Pistol.GetAmo();
}

int WeaponManager::GetMaxPistolAmo() const
{
	return m_Pistol.GetMaxAmo();
}

int WeaponManager::GetShotgunAmo() const
{
	return m_Shotgun.GetAmo();
}

int WeaponManager::GetMaxShotgunAmo() const
{
	return m_Shotgun.GetMaxAmo();
}

#pragma endregion weapon specific
