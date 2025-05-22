#pragma once
#include "Things.h"
#include "HealthManager.h"

enum class ShootableStates
{
	alive,
	harmed,
	dying,
	dead
};

class Shootable : public Thing
{
public:
	Shootable(Vector2f pos, float height, std::string sprite, 
		int startingHealth);
	Shootable(const Shootable& toCopy) = default;
	Shootable(Shootable&& rhs) = default;
	virtual ~Shootable() = default;
	Shootable& operator=(const Shootable& rhs) = default;
	Shootable& operator=(Shootable&& rhs) = default; // it has no movable members

	void BeShot(int damage);

protected:
	ShootableStates m_State{};
	HealthManager m_pHealthManager;
};

