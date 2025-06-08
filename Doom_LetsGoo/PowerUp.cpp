#include "pch.h"
#include <iostream>
#include "PowerUp.h"

#include "Interactor.h"
#include "HealthInteractor.h"
#include "WeaponInteractor.h"

#include "Player.h"
#include "Texture.h"

#include "SoundEffect.h"

SoundEffect* PowerUp::s_pPickupSound{nullptr};
int PowerUp::s_MemberCount{0};

PowerUp::PowerUp(Vector2f pos, float height, utils::ThingsEnum type, std::string sprite)
	:Thing{ pos , height  , sprite, true }//,
	//m_FilePath{ "pickUps/" + sprite + "A0.png" }
{
	if (s_pPickupSound == nullptr)
	{
		s_pPickupSound = new SoundEffect{ "sounds/DSITEMUP.wav" };
	}
	s_MemberCount++;

	//m_pTex = new Texture{ m_FilePath };
	m_SubClass = ThingSubclass::pickup;

	switch (type)
	{
	case utils::ThingsEnum::Shotgun:
		m_InteractorSwitch_HiF_WiT = true;
		m_pInteractor = new WeaponInteractor{
			4,
			WeaponType::shotgun,
			true };
		break;
	case utils::ThingsEnum::Chaingun:
		m_InteractorSwitch_HiF_WiT = true;
		m_pInteractor = new WeaponInteractor{
			20,
			WeaponType::chaingun,
			true };
		break;
	case utils::ThingsEnum::RocketLauncher:
		m_InteractorSwitch_HiF_WiT = true;
		m_pInteractor = new WeaponInteractor{
			2,
			WeaponType::rocket,
			true };
		break;
	case utils::ThingsEnum::Clip:
		m_InteractorSwitch_HiF_WiT = true;
		m_pInteractor = new WeaponInteractor{
			10,
			WeaponType::pistol};
		break;
	case utils::ThingsEnum::shotgunShells4:
		m_InteractorSwitch_HiF_WiT = true;
		m_pInteractor = new WeaponInteractor{
			4,
			WeaponType::shotgun };
		break;
	case utils::ThingsEnum::BoxOfRockets:
		m_InteractorSwitch_HiF_WiT = true;
		m_pInteractor = new WeaponInteractor{
			5,
			WeaponType::rocket };
		break;
	case utils::ThingsEnum::BoxOfBullets:
		m_InteractorSwitch_HiF_WiT = true;
		m_pInteractor = new WeaponInteractor{
			50,
			WeaponType::pistol };
		break;
	case utils::ThingsEnum::BoxOfShotgunShells:
		m_InteractorSwitch_HiF_WiT = true;
		m_pInteractor = new WeaponInteractor{
			20,
			WeaponType::shotgun };
		break;

	case utils::ThingsEnum::Stimpack:
		m_InteractorSwitch_HiF_WiT = false;
		m_pInteractor = new HealthInteractor{
			10};
		break;
	case utils::ThingsEnum::Medikit:
		m_InteractorSwitch_HiF_WiT = false;
		m_pInteractor = new HealthInteractor{
			25 };
		break;
	case utils::ThingsEnum::HealthBonus:
		m_InteractorSwitch_HiF_WiT = false;
		m_pInteractor = new HealthInteractor{
			1,
		false,
		true};
		break;
	case utils::ThingsEnum::ArmorBonus:
		m_InteractorSwitch_HiF_WiT = false;
		m_pInteractor = new HealthInteractor{
			1,
		true,
		true };
		break;
	case utils::ThingsEnum::Armor:
		m_InteractorSwitch_HiF_WiT = false;
		m_pInteractor = new HealthInteractor{
			1/3.f,
		true,
		false };
		break;
	case utils::ThingsEnum::Megaarmor:
		m_InteractorSwitch_HiF_WiT = false;
		m_pInteractor = new HealthInteractor{
			1/2.f,
		true,
		false };
		break;
	default:
		break;
	}
}

PowerUp::PowerUp(point2D_3D Location)
	:Thing{ Vector2f{Location.point3D.x, Location.point3D.y} , 20  , "SHOT" , true}//,
	//m_FilePath{ "pickUps/SHOTA0.png" }
{
	//m_pTex = new Texture{ m_FilePath };
	m_SubClass = ThingSubclass::pickup;
	m_InteractorSwitch_HiF_WiT = true;
	m_pInteractor = new WeaponInteractor{
			4,
			WeaponType::shotgun,
			true };
}

PowerUp::~PowerUp()
{
	if (s_MemberCount == 1)
	{
		delete s_pPickupSound;
		s_pPickupSound = nullptr;
	}
	s_MemberCount--;
	//delete m_pTex;
	delete m_pInteractor;
	m_pInteractor = nullptr;
}

#pragma region rule of 5
PowerUp::PowerUp(const PowerUp& toCopy)
	:Thing( Thing(toCopy))//,
	//m_FilePath{ toCopy.m_FilePath }
{
	m_PickedUp = toCopy.m_PickedUp;
	//m_pTex = new Texture{ m_FilePath };
	m_InteractorSwitch_HiF_WiT = toCopy.m_InteractorSwitch_HiF_WiT;
	if (toCopy.m_InteractorSwitch_HiF_WiT)
	{
		m_pInteractor = new WeaponInteractor{
			*static_cast<WeaponInteractor*>(toCopy.m_pInteractor)
		};
	}
	else
	{
		m_pInteractor = new HealthInteractor{
			*static_cast<HealthInteractor*>(toCopy.m_pInteractor)
		};
	}
}

PowerUp::PowerUp(PowerUp&& toMove) noexcept
	:Thing(Thing(toMove))//,
	//m_FilePath{ std::move(toMove.m_FilePath )}
{
	m_PickedUp = std::move(toMove.m_PickedUp);
	//m_pTex = toMove.m_pTex;
	//toMove.m_pTex = nullptr;
	m_InteractorSwitch_HiF_WiT = toMove.m_InteractorSwitch_HiF_WiT;
	m_pInteractor = std::move(toMove.m_pInteractor);
	toMove.m_pInteractor = nullptr;
}

PowerUp& PowerUp::operator=(const PowerUp& rhs)
{
	if (&rhs != this)
	{
		Thing::operator=(rhs);
		m_PickedUp = rhs.m_PickedUp;
		/*m_FilePath = rhs.m_FilePath;
		if (m_pTex != nullptr)
		{
			delete m_pTex;
		}
		m_pTex = new Texture{ m_FilePath };*/
		m_InteractorSwitch_HiF_WiT = rhs.m_InteractorSwitch_HiF_WiT;
		delete m_pInteractor;
		if (rhs.m_InteractorSwitch_HiF_WiT)
		{
			m_pInteractor = new WeaponInteractor{
				*static_cast<WeaponInteractor*>(rhs.m_pInteractor)
			};
		}
		else
		{
			m_pInteractor = new HealthInteractor{
				*static_cast<HealthInteractor*>(rhs.m_pInteractor)
			};
		}
	}
	return *this;
}

PowerUp& PowerUp::operator=(PowerUp&& rhs) noexcept
{
	if (&rhs != this)
	{
		Thing::operator=(rhs);
		m_PickedUp = std::move(rhs.m_PickedUp);
		/*m_FilePath = std::move(rhs.m_FilePath);
		m_pTex = rhs.m_pTex;
		rhs.m_pTex = nullptr;*/
		m_InteractorSwitch_HiF_WiT = rhs.m_InteractorSwitch_HiF_WiT;
		m_pInteractor = std::move(rhs.m_pInteractor);
		rhs.m_pInteractor = nullptr;
	}
	return *this;
}

#pragma endregion rule of 5


void PowerUp::UpdateThing(const Player& eyeObject, float elapsSec)
{
	if (!m_PickedUp && (eyeObject.GetPos() - m_location.point3D).Length() < 100)
	{
		//SOUND s_pPickupSound
		std::cout << "pickup" << std::endl;
		m_PickedUp = true;
		m_pInteractor->Interact();
	}
	Thing::UpdateThing(eyeObject, elapsSec);
}

void PowerUp::DrawThing(Texture* tex)
{
	if (!m_PickedUp)
	{
		Thing::DrawThing(tex);
	}
}

