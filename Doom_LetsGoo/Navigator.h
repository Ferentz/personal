#pragma once
class Level;
class Navigator
{
public:
	Navigator(Vector3f& pos);
	const int m_TimeInterval{ 2 };
	const float m_detectRadius{ 200 };
	Vector3f& m_Pos;
	float m_timer{};
	float m_Speed{50};
	float m_size{ 20 };
	Vector3f GetNextPos( const Vector3f& playerPos, const std::vector<BSPnode>& nodes, float elapsSec);
	void PushPosition(const Vector3f& playerPos);
	Vector2f m_Direction{};
};

