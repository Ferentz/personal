#pragma once
#include "Things.h"

#include "utils.h"

class SoundEffect;
class Interactor;

class PowerUp final : public Thing
{
public:
	explicit PowerUp(Vector2f pos, float height, utils::ThingsEnum type, std::string sprite);
	explicit PowerUp(point2D_3D Location);
	virtual ~PowerUp() override;
	
	//rule of 5
	PowerUp(const PowerUp& toCopy);
	PowerUp(PowerUp&& toMove) noexcept;
	PowerUp& operator=(const PowerUp& rhs);
	PowerUp& operator=(PowerUp&& rhs) noexcept;
	

	virtual void UpdateThing(const Player& eyeObject, float elapsSec) override;
	virtual void DrawThing(Texture* tex) override;

	Interactor* m_pInteractor{};

private:

	//std::string m_FilePath;
	//Texture* m_pTex;

	bool m_InteractorSwitch_HiF_WiT{};
	bool m_PickedUp{false};
	
	//sound is same for all pickups
	static SoundEffect* s_pPickupSound;
	static int s_MemberCount;
};

