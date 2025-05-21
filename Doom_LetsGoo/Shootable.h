#pragma once
#include "Things.h"
class Shootable : public Thing
{
public:
	Shootable(Vector2f pos, float height, std::string sprite, 
		int startingHealth);
	Shootable(const Shootable& toCopy);
	Shootable(Shootable&& rhs) noexcept;
	virtual ~Shootable() noexcept;
	Shootable& operator=(const Shootable& rhs);
	Shootable& operator=(Shootable&& rhs) noexcept; // it has no movable members

	void BeShot(int damage);

protected:
	HealthManager* m_pHealthManager;
};

