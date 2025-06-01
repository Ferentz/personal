#pragma once
class Level;
class Plane;

class Navigator final
{
public:
	explicit Navigator(Vector3f& pos);
	
	void CalcNextPos( const Vector3f& playerPos, const std::vector<Plane*>& r_pPlanes, float elapsSec); // should not be used in level, just update thing
	Vector3f GetNextPos() const;
	Vector3f GetPos() const;
	void ColideWLevel(const Level* pLevel); //should be in update thing
	void PushPosition(); // again, update
	bool HasLineOfSight( const std::vector <Plane*>&  r_pPlanes, const Vector3f& PlayerPos);
	
	static bool HasLineOfSight( const std::vector<Plane*>& r_pPlanes, const Vector3f& PlayerPos, const Vector3f& objectPos);
	static void SetLevel(const Level* pLevel);

	bool m_hasLineOfSight{false};
	bool m_Mobile{ true };

private:
	static const Level* pLevel;
	const float m_TimeIntervalSecs{ 2.f }; // specify time unit
	int m_charHeight{ 50 };
	Vector3f& r_Pos;
	Vector3f m_EyePos;
	Vector3f m_NextPos;
	const float m_detectRadius{ 1000.f };
	const float m_minPlayerDist{ 200.f };
	const float m_charDiameter{ 20.f };
	
	
	float m_timer{};
	float m_Speed{ 1 };
	float m_size{ 20 }; //remove?

	Vector2f m_Direction{};
	bool m_SeesPlayer{ false };
};

