#include "pch.h"
#include "Navigator.h"
#include <iostream>
#include "Plane.h"
#include "Level.h"
//#include "Level.h"

Navigator::Navigator(Vector3f& pos)
	:m_Pos{pos},
	m_EyePos{ pos .x, pos.y, pos.z + m_charHeightr}
{
}

void Navigator::CalcNextPos( const Vector3f& playerPos,  const std::vector<Plane*>& r_pPlanes, float elapsSec)
{
	seesPlayer = false;
	Vector3f PlayDirec{ playerPos - m_Pos };
	if (PlayDirec.Length() < m_detectRadius)
	{
		if (HasLineOfSight(r_pPlanes, playerPos))
		{
			Vector3f normalizedDirection{ PlayDirec.Normalized() };
			m_Direction.x = normalizedDirection.x * m_Speed;
			m_Direction.y = normalizedDirection.y * m_Speed;
			std::cout << "following player" << std::endl;
			seesPlayer = true;
		}
	}

	if (m_timer <= 0 && !seesPlayer)
	{
		std::cout << "wandering" << std::endl;
		m_Direction.x = ((rand() % 20) - 10) / 10.f;
		m_Direction.y = ((rand() % 20) - 10) / 10.f;
		m_timer = m_TimeInterval;
	}
	m_timer -= elapsSec;
	m_NextPos = Vector3f{ m_Pos.x + m_Direction.x, m_Pos.y + m_Direction.y, m_Pos.z };

}

Vector3f Navigator::GetNextPos() const
{
	return m_NextPos;
}

Vector3f Navigator::GetPos() const
{
	return m_Pos;
}

int Navigator::ColideWLevel(const Level* pLevel)
{
	return pLevel->ColideWLevel(m_Pos, m_NextPos, m_NextPos.z, m_charDiameter, true);
}

void Navigator::PushPosition()
{
	m_Pos = m_NextPos;
	m_EyePos = Vector3f{ m_NextPos.x, m_NextPos.y, m_NextPos.z + m_charHeightr };
}

bool Navigator::HasLineOfSight(const std::vector <Plane*>  r_pPlanes, const Vector3f& PlayerPos)
{
	bool sightBlocked{ false };
	for (int wallIndex{}; wallIndex < r_pPlanes.size(); wallIndex++)
	{
		const Plane* wall{ r_pPlanes[wallIndex] };
		Vector2f returnPos{};
		Vector2f outNormal{};
		if (wall->PlaneCollision(m_EyePos, PlayerPos, returnPos, outNormal, true))
		{
			sightBlocked = true;
		}
	}
	return !sightBlocked;
}
