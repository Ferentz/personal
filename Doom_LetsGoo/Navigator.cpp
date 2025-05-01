#include "pch.h"
#include "Navigator.h"
//#include "Level.h"

Navigator::Navigator(Vector3f& pos)
	:m_Pos{pos}
{
}

Vector3f Navigator::GetNextPos( const Vector3f& playerPos,  const std::vector<BSPnode>& nodes, float elapsSec)
{
	Vector3f PlayDirec{ playerPos - m_Pos };
	/*if (PlayDirec.Length() < m_detectRadius)
	{
		Vector3f normalizedDirection{ PlayDirec.Normalized() };
		m_Direction.x = normalizedDirection.x * m_Speed;
		m_Direction.y = normalizedDirection.y * m_Speed;
	}*/
	if (m_timer <= 0)
	{
		m_Direction.x = ((rand() % 20) - 10) / 10.f;
		m_Direction.y = ((rand() % 20) - 10) / 10.f;
		m_timer = m_TimeInterval;
	}
	m_timer -= elapsSec;
	return Vector3f{ m_Pos.x + m_Direction.x, m_Pos.y + m_Direction.y, m_Pos.z };

}

void Navigator::PushPosition(const Vector3f& nextPos)
{
	m_Pos = nextPos;
}
