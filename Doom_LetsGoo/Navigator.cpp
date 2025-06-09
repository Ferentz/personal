#include "pch.h"
#include "Navigator.h"
#include <iostream>
#include "Plane.h"
#include "Level.h"
//#include "Level.h"

const Level* Navigator::pLevel{};

Navigator::Navigator(Vector3f& pos)
	:r_Pos{pos},
	m_EyePos{ pos .x, pos.y, pos.z + m_charHeight},
	m_NextPos{pos}
{
}

void Navigator::CalcNextPos( const Vector3f& playerPos, const std::vector<Plane*>& r_pPlanes, float elapsSec)
{
	if (!m_Mobile)
	{
		return;
	}
	m_SeesPlayer = false;
	Vector3f PlayDirec{ playerPos - r_Pos };
	if (PlayDirec.Length() < m_detectRadius)
	{
		if (HasLineOfSight(r_pPlanes, playerPos))
		{
			Vector3f normalizedDirection{ PlayDirec.Normalized() };
			m_Direction.x = normalizedDirection.x * m_Speed;
			m_Direction.y = normalizedDirection.y * m_Speed;
			m_SeesPlayer = true;
		}
	}

	if (m_timer <= 0 && !m_SeesPlayer)
	{
		float angle = float(rand() / RAND_MAX * 2 * M_PI);
		m_Direction.x = cosf(angle) * m_Speed;
		m_Direction.y = sinf(angle) * m_Speed;

		//m_Direction.x = ((rand() % 20) - 10) / 10.f;
		//m_Direction.y = ((rand() % 20) - 10) / 10.f;
		m_timer = m_TimeIntervalSecs;
	}
	m_timer -= elapsSec;
	m_NextPos = Vector3f{ r_Pos.x + m_Direction.x, r_Pos.y + m_Direction.y, r_Pos.z };

}

Vector3f Navigator::GetNextPos() const
{
	return m_NextPos;
}

Vector3f Navigator::GetPos() const
{
	return r_Pos;
}

void Navigator::ColideWLevel(const Level* pLevel)
{
	if (!m_Mobile)
	{
		return;
	}
	bool exit{ false };
	pLevel->ColideWLevel(r_Pos, m_NextPos, m_NextPos.z, m_charDiameter, false, exit);
}

void Navigator::PushPosition()
{
	if (!m_Mobile)
	{
		return;
	}
	r_Pos = m_NextPos;
	m_EyePos = Vector3f{ m_NextPos.x, m_NextPos.y, m_NextPos.z + m_charHeight };
}

bool Navigator::HasLineOfSight( const std::vector <Plane*>&  r_pPlanes, const Vector3f& PlayerPos)
{
	if (!m_Mobile)
	{
		return false;
	}
	m_hasLineOfSight = HasLineOfSight(r_pPlanes, PlayerPos, m_EyePos);
	/*if (m_hasLineOfSight)
	{
		std::cout << "i can see through walls";
	}*/
	return m_hasLineOfSight;
}

bool Navigator::HasLineOfSight( const std::vector <Plane*>&  r_pPlanes, const Vector3f& PlayerPos, const Vector3f& objectPos)
{
	for (int wallIndex{}; wallIndex < r_pPlanes.size(); wallIndex++)
	{
		Plane* wall{ r_pPlanes[wallIndex] };
		Vector2f returnPos{};
		Vector2f outNormal{};
		if (wall->Plane::PlaneCollision(objectPos, PlayerPos, returnPos, outNormal, true))
		{
			return false;
		}
	}
	return true;
}

void Navigator::SetLevel(const Level* level)
{
	pLevel = level;
}
