#include "pch.h"
#include "Player.h"
#include <iostream>
#include "Level.h"
#include "Plane.h"
#include "Enemy.h"
#include "Navigator.h"


Player::Player(const Rectf& window)
	:m_HealthManager{500},
	m_WeaponManager{},
	m_Camera{ window , m_Pos}
{
}


#pragma region basics
bool Player::UpdatePlayer(float elapsedsecs, Level* pLevel)
{
	m_WeaponManager.UpdateWeapon(elapsedsecs);

	if (ColideWLevel(pLevel))
	{
		return true;
	}
	VelocCalc(elapsedsecs);
	pushPosition();
	return false;
}

#pragma region basics helpers
void Player::pushPosition()
{
	m_Pos = m_NextPos;
}

bool Player::ColideWLevel(const Level* pLevel)
{
	bool exit{ false };
	pLevel->ColideWLevel(m_Pos, m_NextPos, m_FLoorHeigt, m_charDIameter, true, exit);
	return exit;
}

//TODO :: footsteps soud + velocity bob & sound when fall
void Player::VelocCalc(float elapsedSec)
{
	m_VericalVelocity -= (m_Gravity * elapsedSec * m_JumpnFallSpeed);
	m_NextPos.z = m_Pos.z + (m_VericalVelocity * elapsedSec * m_JumpnFallSpeed);
	if (m_NextPos.z <= m_FLoorHeigt + m_CharHeight)
	{
		m_VericalVelocity = 0;
		m_NextPos.z = m_FLoorHeigt + m_CharHeight;
		m_Standing = true;
		return;
	}
	
}


#pragma endregion basics helpers
#pragma endregion basics

#pragma region input
void Player::Shoot(const std::vector<Thing*> thingsToShoot, const std::vector<Plane*>& r_pPlanes, float windowWidth)
{
	const int maxAmout{ 20 };
	int count{ 1 };
	std::vector<Shootable*> shootables{};
	std::vector<float> distances{};
	shootables.reserve(maxAmout);
	distances.reserve(maxAmout);
	for (Thing* object : thingsToShoot)
	{
		if ((object->GetSubClass() == ThingSubclass::enemy || object->GetSubClass() == ThingSubclass::shootable) )
		{
			Shootable* pShootable{ static_cast<Shootable*>(object) };
			if (pShootable->GetState() != ShootableStates::dead)
			{
				bool lineOfSight{ false };
				if (pShootable->GetSubClass() == ThingSubclass::enemy)
				{
					lineOfSight = static_cast <Enemy*> (pShootable)->GetpNavigator()->m_hasLineOfSight;
				}

				if (m_Camera.GetDistanceFromMidle(pShootable->GetLocationPoint().point2D.x) < m_Camera.GetWindowWidth() / 6 && // if the shootable is in front of you
					(lineOfSight || Navigator::HasLineOfSight(r_pPlanes, m_Pos, pShootable->GetLocationPoint().point3D))) // and if there is a line of sight
				{
					shootables.push_back(pShootable);
					distances.push_back(pShootable->GetLocationPoint().Dist3D);
					count++;
					if (count == maxAmout)
					{
						break;
					}
					//m_WeaponManager.Shoot(pShootable, windowWidth);

				}
			}
		}
	}
	m_WeaponManager.Shoot(shootables, m_Camera.GetWindowWidth());
}

void Player::Jump()
{
	if (m_Standing)
	{
		m_Standing = false;
		m_VericalVelocity = m_JumpVelocity;
	}
}

void Player::MoveForward()
{
	//m_NextPos.x += GetLookAtVector().x;
	//m_NextPos.y -= GetLookAtVector().y;
	Move(GetLookAtVector().x, -GetLookAtVector().y);
}

void Player::MoveBackward()
{
	//m_NextPos.x -= GetLookAtVector().x;
	//m_NextPos.y += GetLookAtVector().y;
	Move(-GetLookAtVector().x, GetLookAtVector().y);
}

void Player::MoveLeft()
{
	//m_NextPos.x += -GetLookAtVector().y;
	//m_NextPos.y -= GetLookAtVector().x;
	Move(-GetLookAtVector().y, -GetLookAtVector().x);
}

void Player::MoveRight()
{
	//m_NextPos.x += GetLookAtVector().y;
	//m_NextPos.y -= -GetLookAtVector().x;
	Move(GetLookAtVector().y, GetLookAtVector().x);
}

void Player::Move(float x, float y)
{
	m_NextPos.x += x * m_Speed;
	m_NextPos.y += y * m_Speed;
}
void Player::Turn(float offset)
{
	m_Camera.TurnCamera(offset);
	m_lookVector = m_Camera.MakeLookAtVector();
	m_InvertedLookVector = m_Camera.MakeLookAtVector(true);
}

void Player::SwitchWeapon(int weaponIndex)
{
	WeaponType weapon{};
	switch (weaponIndex)
	{
	case 1:
		weapon = WeaponType::pistol;
		break;
	case 2:
		weapon = WeaponType::shotgun;
		break;
	}
	m_WeaponManager.SwitchWeapon(weapon);
}

#pragma endregion input

#pragma region other
void Player::ResetPos()
{
	m_Pos.x = m_StartPos.x;
	m_Pos.y = m_StartPos.y;
	m_Pos.z = m_StartPos.z;

}

const Vector2f& Player::GetLookAtVector( bool invert) const {
	if (invert)
	{
		return m_InvertedLookVector;
	}
	return m_lookVector;
}

const Camera& Player::GetCamera() const
{
	return m_Camera;
}

const Vector3f& Player::GetPos() const
{
	return m_Pos;
}
#pragma endregion other