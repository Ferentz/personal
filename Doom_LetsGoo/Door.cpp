#include "pch.h"
#include "Door.h"
#include <iostream>
#include "utils.h"

Door::Door(Vector3f& position, Vector3f& width, Vector3f& height, int colour, float upperHeight, float lowerHeight, bool isExit)
	:Plane{  position,  width,  height, colour },
	m_upperHeight {upperHeight},
	m_lowerHeight{lowerHeight},
	m_IsExit{ isExit }
{
}

bool Door::PlaneCollision(const Vector3f& pos3D, const Vector3f& nextPos3D, Vector2f& returnPos, Vector2f& outNormal, const bool isPlayer)
{
	if (Plane::PlaneCollision(pos3D, nextPos3D, returnPos, outNormal, isPlayer))
	{
		m_opening = true;
		return true;
	}
	return false;
}

void Door::UpdatePlane(const Player& eyeObject, float elapsSec)
{
	if (m_opening)
	{
		Opening();
	}
	Plane::UpdatePlane(eyeObject, elapsSec);
}

/// <summary>
/// returns if door is the level exit
/// </summary>
/// <returns>
/// </returns>
bool Door::Trigger()
{
	if (m_IsExit)
	{
		return true;
	}
	m_opening = true;

	//SOUND
	//m_Door
	bool DidPlay{ m_Door.Play(true) };
	return false;
}

void Door::Opening()
{

	float lowestHeight{ 1000 };
	for (int point{}; point < 4; point++)
	{
		m_Points[point].point3D.z += (m_speed * m_direction);
		m_Location.z += (m_speed * m_direction);
		if (m_Points[point].point3D.z < lowestHeight)
		{
			lowestHeight = m_Points[point].point3D.z;
		}
	}
	if (lowestHeight >= m_upperHeight)
	{
		m_direction = -1;
	}
	if (lowestHeight <= m_lowerHeight)
	{
		m_direction = 1;
		m_opening = false;
	}
}

