#pragma once
#include "utils.h"
#include "Things.h"

class HealthManager;
class WeaponManager;
class PowerUp final : public Thing
{
public:
	PowerUp(Vector2f pos, float height, utils::ThingsEnum type, std::string sprite);
	virtual ~PowerUp() override;
	virtual void UpdateThing(const PLayer& eyeObject, float elapsSec) override;
	virtual void DrawThing(Texture* tex);

private:
	bool m_PickedUp{false};
	utils::ThingsEnum m_Type;
	Texture* m_pTex;
};

