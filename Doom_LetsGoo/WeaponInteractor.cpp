#include "pch.h"
#include "WeaponInteractor.h"
#include "WeaponManager.h"

WeaponManager* WeaponInteractor::m_pPlayerWeapons{};

WeaponInteractor::WeaponInteractor(int pickedupAmount, WeaponType type, bool isWeapon)
	:m_PickedupAmountAmo{pickedupAmount},
	m_Type{type},
	m_IsWeapon{ isWeapon }
{
}


void WeaponInteractor::Interact()
{
	if (m_IsWeapon)
	{
		m_pPlayerWeapons->PickUpWeapon(m_Type);
	}
	m_pPlayerWeapons->AddAmo(m_PickedupAmountAmo, m_Type);
}

void WeaponInteractor::AssignPlayer(WeaponManager& playerWeapon)
{
		m_pPlayerWeapons = &playerWeapon;
	
}
