#pragma once
#include "utils.h"
#include "Things.h"
class SoundEffect;

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

	PowerUp(point2D_3D Location);
	virtual void UpdateThing(const Player& eyeObject, float elapsSec) override;
	virtual void DrawThing(Texture* tex) override;

	Interactor* m_pInteractor{};

private:

	bool m_PickedUp{false};
	std::string m_FilePath;
	Texture* m_pTex;

	static SoundEffect* s_PickupSound;
	static int s_MemberCount;

};

