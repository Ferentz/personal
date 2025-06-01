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
	virtual ~Shootable() = default;

	//rule of 5
	Shootable(const Shootable& toCopy) = default;
	Shootable(Shootable&& rhs) = default;
	Shootable& operator=(const Shootable& rhs) = default;
	Shootable& operator=(Shootable&& rhs) = default; // it has no movable members

	void BeShot(int damage);
	ShootableStates GetState() const;

protected:
	ShootableStates m_State{};
	HealthManager m_HealthManager;
};

