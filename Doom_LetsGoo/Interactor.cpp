#include "pch.h"
#include "Interactor.h"
#include "HealthInteractor.h"
#include "WeaponInteractor.h"
#include "Player.h"

void Interactor::AssignPlayer(Player& player)
{
	HealthInteractor::AssignPlayer(player.m_HealthManager);
	WeaponInteractor::AssignPlayer(player.m_WeaponManager);
}
