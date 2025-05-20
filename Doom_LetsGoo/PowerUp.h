#pragma once
#include "utils.h"
#include "Things.h"

class HealthManager;
class WeaponManager;
class PowerUp final : public Thing
{
public:
	PowerUp(Vector2f pos, float height, utils::ThingsEnum type, std::string sprite);
	PowerUp(const PowerUp& toCopy);
	PowerUp(PowerUp&& toMove) noexcept;
	PowerUp& operator=(const PowerUp& rhs);
	PowerUp& operator=(PowerUp&& rhs) noexcept;
	virtual ~PowerUp() override;
	virtual void UpdateThing(const PLayer& eyeObject, float elapsSec) override;
	virtual void DrawThing(Texture* tex);

private:
	bool m_PickedUp{false};
	utils::ThingsEnum m_Type;
	std::string m_FilePath;
	Texture* m_pTex;
};

