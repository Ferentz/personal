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
	if (m_Health > m_MaxHealth)
	{
		m_Health = m_MaxHealth;
	}
}

void HealthManager::Harm(int damage)
{
	float absorbedAmount{ damage * m_Armour_DamageAbsorption };
	m_Health -= damage - absorbedAmount;
	m_ArmourHealth -= absorbedAmount;
	if (m_ArmourHealth<0)
	{
		m_ArmourHealth = 0;
	}
}

int HealthManager::GetHeath() const
{
	return m_Health;
}

int HealthManager::GetArmour() const
{
	return m_ArmourHealth;
}

void HealthManager::FullHealth()
{
	m_Health = m_FullHealth;
}

void HealthManager::SetArmour(float damageAbsorption)
{

	if (m_Armour_DamageAbsorption < damageAbsorption)
	{
		m_Armour_DamageAbsorption = damageAbsorption;
	}
	if (m_ArmourHealth != m_MaxArmour)
	{
		m_ArmourHealth = m_MaxArmour;
	}
}

void HealthManager::IncreaseMaxArmour(int amount)
{
	m_MaxArmour += amount;
}

void HealthManager::IncreaseMaxHealth(int amount)
{
	m_MaxHealth += amount;
}
