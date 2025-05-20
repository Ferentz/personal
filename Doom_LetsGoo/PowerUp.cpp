#include "pch.h"
#include <iostream>
#include "PowerUp.h"
#include "Interactor.h"
#include "HealthInteractor.h"
#include "WeaponInteractor.h"
#include "WeaponManager.h"
#include "PLayer.h"
#include "Texture.h"

PowerUp::PowerUp(Vector2f pos, float height, utils::ThingsEnum type, std::string sprite)
	:m_Type{ type },
	Thing{ pos , height , type , sprite }
{
	//m_pInteractor = new Interactor{};
	// assigns the interactor
	/*std::string path{ entry.path().string() };
	std::string name{ entry.path().filename().string() };
	std::string suffic{ entry.path().filename().string().erase(name.size() - 4, name.size() - 1) };
	suffic.erase(0, 4);*/
	m_pTex = new Texture{ "pickUps/" + sprite + "A0.png" };

	switch (type)
	{
	case utils::ThingsEnum::Shotgun:
		m_pInteractor = new WeaponInteractor{
			4,
			WeaponType::shotgun,
			true };
		break;
	case utils::ThingsEnum::Chaingun:
		m_pInteractor = new WeaponInteractor{
			20,
			WeaponType::chaingun,
			true };
		break;
	case utils::ThingsEnum::RocketLauncher:
		m_pInteractor = new WeaponInteractor{
			2,
			WeaponType::rocket,
			true };
		break;
	case utils::ThingsEnum::Clip:
		m_pInteractor = new WeaponInteractor{
			10,
			WeaponType::pistol};
		break;
	case utils::ThingsEnum::shotgunShells4:
		m_pInteractor = new WeaponInteractor{
			4,
			WeaponType::shotgun };
		break;
	case utils::ThingsEnum::BoxOfRockets:
		m_pInteractor = new WeaponInteractor{
			5,
			WeaponType::rocket };
		break;
	case utils::ThingsEnum::BoxOfBullets:
		m_pInteractor = new WeaponInteractor{
			50,
			WeaponType::pistol };
		break;
	case utils::ThingsEnum::BoxOfShotgunShells:
		m_pInteractor = new WeaponInteractor{
			20,
			WeaponType::shotgun };
		break;

	case utils::ThingsEnum::Stimpack:
		m_pInteractor = new HealthInteractor{
			10};
		break;
	case utils::ThingsEnum::Medikit:
		m_pInteractor = new HealthInteractor{
			25 };
		break;
	case utils::ThingsEnum::HealthBonus:
		m_pInteractor = new HealthInteractor{
			1,
		false,
		true};
		break;
	case utils::ThingsEnum::ArmorBonus:
		m_pInteractor = new HealthInteractor{
			1,
		true,
		true };
		break;
	case utils::ThingsEnum::Armor:
		m_pInteractor = new HealthInteractor{
			1/3.f,
		true,
		false };
		break;
	case utils::ThingsEnum::Megaarmor:
		m_pInteractor = new HealthInteractor{
			1/2.f,
		true,
		false };
		break;
	default:
		break;
	}
}

PowerUp::~PowerUp()
{
	delete m_pTex;
	delete m_pInteractor;
	m_pInteractor = nullptr;
}

void PowerUp::UpdateThing(const PLayer& eyeObject, float elapsSec)
{
	if (!m_PickedUp && (eyeObject.m_Pos - m_location.point3D).Length() < 100)
	{
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
		Thing::DrawThing(m_pTex);
	}
}

