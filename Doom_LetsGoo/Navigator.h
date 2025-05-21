#pragma once
class Level;
class Plane;
class Navigator final
{
public:
	Navigator(Vector3f& pos);
	
	void CalcNextPos( const Vector3f& playerPos, const std::vector<Plane*>& r_pPlanes, float elapsSec); // should not be used in level, just update thing
	Vector3f GetNextPos() const;
	Vector3f GetPos() const;
	int ColideWLevel(const Level* pLevel); //should be in update thing
	void PushPosition(); // again, update
	bool HasLineOfSight(const std::vector <Plane*>&  r_pPlanes, const Vector3f& PlayerPos);
	static bool HasLineOfSight(const std::vector<Plane*>& r_pPlanes, const Vector3f& PlayerPos, const Vector3f& objectPos);

	static void SetLevel(const Level* pLevel);

	bool m_hasLineOfSight{};
	bool m_IsShooting{ false };
	bool m_CanShootAgain{ true };
	bool m_Alive{ true };
	bool m_Dying{ false };

private:
	static const Level* pLevel;
	const int m_TimeIntervalSecs{ 2 }; // specify time unit
	Vector3f& m_Pos;
	Vector3f m_EyePos;
	Vector3f m_NextPos{};
	const float m_detectRadius{ 1000 };
	const float m_minPlayerDist{ 200 };
	int m_charDiameter{ 20 };
	int m_charHeightr{ 50 };

	float m_timer{};
	float m_Speed{ 1 };
	float m_size{ 20 }; //remove?

	Vector2f m_Direction{};
	bool seesPlayer{ false };
};

