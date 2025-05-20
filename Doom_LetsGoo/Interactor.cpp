#include "pch.h"
#include "Interactor.h"
#include "HealthInteractor.h"
#include "WeaponInteractor.h"
#include "PLayer.h"

Interactor::Interactor()
{
}

void Interactor::AssignPlayer(PLayer& player)
{
	HealthInteractor::AssignPlayer(player.m_HealthManager);
	WeaponInteractor::AssignPlayer(player.m_WeaponManager);
}
