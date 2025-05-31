#include "pch.h"
#include "iostream"
#include "HealthInteractor.h"
#include "HealthManager.h"

HealthManager* HealthInteractor::m_pPlayerHealth{};

HealthInteractor::HealthInteractor(float healAmount, bool affectsArmour, bool affectsMax)
	:m_HealAmount{healAmount},
	m_AffectsArmour{affectsArmour},
	m_AffectsMax{affectsMax}
{
}


void HealthInteractor::Interact()
{
	if (m_AffectsArmour)
	{
		if (m_AffectsMax)
		{
			std::cout << "up armpur max" << std::endl;
			m_pPlayerHealth->IncreaseMaxArmour(int(m_HealAmount));
			return;
		}
			std::cout << "up armpur durability" << std::endl;
			m_pPlayerHealth->SetArmour(m_HealAmount);
			return;
	}
	if (m_AffectsMax)
	{
		std::cout << "up health max" << std::endl;
		m_pPlayerHealth->IncreaseMaxHealth(int(m_HealAmount));
		return;
	}
	std::cout << "up health" << std::endl;
	m_pPlayerHealth->Heal(int(m_HealAmount));
}

void HealthInteractor::AssignPlayer(HealthManager& playerHealth)
{
	m_pPlayerHealth = &playerHealth;
}
