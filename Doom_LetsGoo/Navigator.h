#pragma once
class Level;
class Plane;
class Navigator final
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
	int m_charHeightr{ 50 };
	float m_timer{};
	float m_Speed{1};
	float m_size{ 20 };
	bool m_hasLineOfSight{};
	bool m_IsShooting{ false };
	bool m_CanShootAgain{ true };
	void CalcNextPos( const Vector3f& playerPos, const std::vector<Plane*>& r_pPlanes, float elapsSec);
	Vector3f GetNextPos() const;
	Vector3f GetPos() const;
	int ColideWLevel(const Level* pLevel);
	void PushPosition();
	bool HasLineOfSight(const std::vector <Plane*>&  r_pPlanes, const Vector3f& PlayerPos);
	static bool HasLineOfSight(const std::vector<Plane*>& r_pPlanes, const Vector3f& PlayerPos, const Vector3f& objectPos);
	Vector2f m_Direction{};
	bool seesPlayer{false};
	bool m_Alive{ true };
	bool m_Dying{ false };
};

