#include "pch.h"
#include "iostream"
#include "HealthInteractor.h"
#include "HealthManager.h"
#include <assert.h>

HealthInteractor::HealthInteractor(float healAmount, bool affectsArmour, bool affectsMax)
	:m_HealAmount{healAmount},
	m_AffectsArmour{affectsArmour},
	m_AffectsMax{affectsMax}
{
}

void HealthInteractor::Interact()
{
	assert(m_pPlayerHealth != nullptr && "HealthInteractor::AssignPlayer must be called before Interact()");
	
	if (m_AffectsArmour)
	{
		if (m_AffectsMax)
		{
			m_pPlayerHealth->IncreaseMaxArmourHealth(static_cast<int>(m_HealAmount));
			return;
		}
			m_pPlayerHealth->SetArmourAbsorption(m_HealAmount);
			return;
	}
	if (m_AffectsMax)
	{
		m_pPlayerHealth->IncreaseMaxHealth(static_cast<int>(m_HealAmount));
		return;
	}
	m_pPlayerHealth->Heal(static_cast<int>(m_HealAmount));
}

void HealthInteractor::AssignPlayer(HealthManager& playerHealth)
{
	m_pPlayerHealth = &playerHealth;
}

HealthManager* HealthInteractor::m_pPlayerHealth{};
