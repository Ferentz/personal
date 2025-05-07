#include "pch.h"
#include "Door.h"
#include <iostream>

Door::Door(Vector3f& position, Vector3f& width, Vector3f& height, int colour, float upperHeight, float lowerHeight)
	:Plane{  position,  width,  height, colour },
	m_upperHeight {upperHeight},
	m_lowerHeight{lowerHeight}
{
	std::cout << "door made" << std::endl;
}

Door::~Door()
{
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

void Door::Triggered()
{
	m_opening = true;
}

bool Door::PlaneCollision(const Vector3f& pos3D, const Vector3f& nextPos3D, Vector2f& returnPos, Vector2f& outNormal, const bool isPlayer)
{
	if (Plane::PlaneCollision(pos3D, nextPos3D, returnPos, outNormal, isPlayer))
	{
		//triggered();
		m_opening = true;
		return true;
	}
	return false;
}

void Door::UpdatePlane(const DoomCamera& eyeObject, float elapsSec)
{
	if (m_opening)
	{
		Opening();
	}
	Plane::UpdatePlane(eyeObject, elapsSec);
}

