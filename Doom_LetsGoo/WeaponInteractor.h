#pragma once
#include "Interactor.h"

class WeaponManager;
enum class WeaponType;

class WeaponInteractor final : public Interactor
{
public:
	WeaponInteractor(int pickedupAmount, WeaponType type, bool isWeapon = false);
	
	virtual ~WeaponInteractor() override;
	virtual void Interact() override;
	static void AssignPlayer(WeaponManager& playerWeapon);

private:
	int m_PickedupAmount;
	WeaponType m_Type;
	bool m_IsWeapon;
	static WeaponManager* m_pPlayerWeapons;
};

