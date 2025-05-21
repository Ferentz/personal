#include "pch.h"
#include "PLayer.h"
#include <iostream>
#include "Level.h"
#include "Plane.h"
//#include "Things.h"
#include "Enemy.h"
#include "Navigator.h"
//#include "game.h"

// next we need to work on jump

// get the trigger event in game to shoot.
//then conect to get the level things ad check if smth gets hit

PLayer::PLayer(const Rectf& window)
	:
	m_NextPos{ m_Pos },
	m_lookVector{ 0,0 },
	m_HealthManager{500},
	m_WeaponManager{},
	m_Camera{ window , m_Pos}
{
}

void PLayer::pushPosition()
{
	m_Pos = m_NextPos;
}

bool PLayer::ColideWLevel(const Level* pLevel)
{
	bool exit{ false };
	pLevel->ColideWLevel(m_Pos, m_NextPos, m_FLoorHeigt, m_charDIameter, true, exit);
	return exit;
}

void PLayer::VelocCalc(float elapsedSec)
{
	m_vericalVelocity -= (m_GRAVITY * elapsedSec);
	m_NextPos.z = m_Pos.z + m_vericalVelocity;
	if (m_NextPos.z <= m_FLoorHeigt + m_charHeight)
	{
		m_vericalVelocity = 0;
		m_NextPos.z = m_FLoorHeigt + m_charHeight;
		m_Standing = true;
		return;
	}
	
}


void PLayer::Shoot(std::vector<Thing*> thingsToShoot, const std::vector<Plane*>& r_pPlanes, float windowWidth)
{
	std::vector<Thing*> thingsICanShoot{};
	const int maxAmout{ 5 };
	int count{1};
	thingsICanShoot.reserve(maxAmout);
	for (Thing*& object : thingsToShoot)
	{
		if (object->GetSubClass() == ThingSubclass::enemy || object->GetSubClass() == ThingSubclass::shootable)
		{
			bool lineOfSight{ false };
			if (object->GetSubClass() == ThingSubclass::enemy)
			{
				lineOfSight = static_cast <Enemy*> (object)->m_pNavigator->m_hasLineOfSight;
			}

			if(lineOfSight || Navigator::HasLineOfSight(r_pPlanes, m_Pos, object->GetLocationPoint().point3D))
			{
				thingsICanShoot.push_back(object);
				count++;
				if (count == maxAmout)
				{
					break;
				}
				m_WeaponManager.Shoot(static_cast <Shootable*> (object), windowWidth);
				
			}
		}
	}
}

void PLayer::Jump()
{
	if (m_Standing)
	{
		m_Standing = false;
		m_vericalVelocity = 4;
	}
}

void PLayer::MoveForward()
{
	m_NextPos.x += GetLookAtVector().x;
	m_NextPos.y -= GetLookAtVector().y;
}

void PLayer::MoveBackward()
{
	m_NextPos.x -= GetLookAtVector().x;
	m_NextPos.y += GetLookAtVector().y;
}

void PLayer::MoveLeft()
{
	m_NextPos.x += -GetLookAtVector().y;
	m_NextPos.y -= GetLookAtVector().x;
}

void PLayer::MoveRight()
{
	m_NextPos.x += GetLookAtVector().y;
	m_NextPos.y -= -GetLookAtVector().x;
}

void PLayer::Turn(float offset)
{
	m_Camera.TurnCamera(offset);
	m_lookVector = m_Camera.MakeLookAtVector();
}


bool PLayer::IsLookingAtMe(const Vector3f crosspoints[])const {

	Vector3f crosA{crosspoints[1].CrossProduct(crosspoints[0])}; 
	Vector3f crosb{ crosspoints[2].CrossProduct(crosspoints[0]) };

	Vector3f crosResult{crosA.CrossProduct(crosb)};
	Vector3f eyeVec{ m_Pos.CrossProduct(crosspoints[0]) }; 

	float dotResult = crosResult.DotProduct(eyeVec); 

	if (dotResult < 0) {
		return false;
	}
	return true;
}

void PLayer::ResetPos()
{
	m_Pos.x = m_StartPos.x;
	m_Pos.y = m_StartPos.y;
	m_Pos.z = m_StartPos.z;

}

const Vector2f& PLayer::GetLookAtVector() const {
	return m_lookVector;
}

const Camera& PLayer::GetCamera() const
{
	return m_Camera;
}
