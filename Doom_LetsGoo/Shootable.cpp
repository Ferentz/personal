#include "pch.h"
#include "Shootable.h"
#include "HealthManager.h"

Shootable::Shootable(Vector2f pos, float height, std::string sprite,
	int startingHealth)
	:Thing(pos, height, sprite),
	m_pHealthManager{startingHealth}
{
	m_SubClass = ThingSubclass::shootable;
}

void Shootable::BeShot(int damage)
{
	if (!(m_State == ShootableStates::dead || m_State == ShootableStates::dying))
	{
		m_State = ShootableStates::harmed;
		m_pHealthManager.Harm(damage);
	}
}

ShootableStates Shootable::GetState() const
{
	return m_State;
}
