#include "pch.h"
#include "HealthManager.h"

HealthManager::HealthManager(int startingHealth)
	:m_Health{ startingHealth },
	m_FullHealth{ startingHealth }
{
}

void HealthManager::Heal(int healAmount)
{
	m_Health += healAmount;
}

void HealthManager::Harm(int damage)
{
	m_Health -= damage;
}

int HealthManager::GetHeath() const
{
	return m_Health;
}

void HealthManager::FullHealth()
{
	m_Health = m_FullHealth;
}
