#include "pch.h"
#include "Shootable.h"
#include "HealthManager.h"

Shootable::Shootable(Vector2f pos, float height, std::string sprite,
	int startingHealth)
	:Thing(pos, height, sprite),
	m_pHealthManager{ new HealthManager(startingHealth) }
{
	m_SubClass = ThingSubclass::shootable;
}

Shootable::Shootable(const Shootable& toCopy)
	:Thing(Thing(toCopy)),
	m_pHealthManager{ new HealthManager(*toCopy.m_pHealthManager) }
{
	m_SubClass = toCopy.m_SubClass;
}

Shootable::Shootable(Shootable&& rhs) noexcept
	:Thing(Thing(rhs)),
	m_pHealthManager{ std::move(rhs.m_pHealthManager) }
{
	m_SubClass = rhs.m_SubClass;
}

Shootable::~Shootable() noexcept
{
	delete m_pHealthManager;
	m_pHealthManager = nullptr;
}

Shootable& Shootable::operator=(const Shootable& rhs)
{
	if (&rhs != this)
	{
		Thing::operator=(rhs);
		*m_pHealthManager = *rhs.m_pHealthManager; // fucking * is getting  the value of gdamnit
	}
	return *this;
}

Shootable& Shootable::operator=(Shootable&& rhs) noexcept
{
	if (&rhs != this)
	{
		Thing::operator=(rhs);
		m_pHealthManager = rhs.m_pHealthManager;
		rhs.m_pHealthManager = nullptr;
	}
	return *this;
}

void Shootable::BeShot(int damage)
{
	m_pHealthManager->Harm(damage);
}
