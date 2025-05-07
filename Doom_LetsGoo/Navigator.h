#pragma once
class Level;
class Plane;
class Navigator
{
public:
	Navigator(Vector3f& pos);
	const int m_TimeInterval{ 2 };
	const float m_detectRadius{ 1000 };
	const float m_minPlayerDist{ 200 };
	Vector3f& m_Pos;
	Vector3f m_EyePos;
	Vector3f m_NextPos{};
	int m_charDiameter{ 20 };
	int m_charHeightr{ 40 };
	int m_HP{ 10 };
	float m_timer{};
	float m_Speed{1};
	float m_size{ 20 };
	void CalcNextPos( const Vector3f& playerPos, const std::vector<Plane*>& r_pPlanes, float elapsSec);
	Vector3f GetNextPos() const;
	Vector3f GetPos() const;
	int ColideWLevel(const Level* pLevel);
	void PushPosition();
	bool HasLineOfSight(const std::vector <Plane*>  r_pPlanes, const Vector3f& PlayerPos);
	Vector2f m_Direction{};
	bool seesPlayer{false};
};

