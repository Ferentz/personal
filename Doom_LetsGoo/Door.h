#pragma once
#include "Plane.h"
class Door final: public Plane
{
public:
	explicit Door(Vector3f& position, Vector3f& width, Vector3f& height, int colour,
		float upperHeight, float lowerHeight, bool isExit);
	bool PlaneCollision(const Vector3f& pos3D, const Vector3f& nextPos3D,
		Vector2f& returnPos, Vector2f& outNormal, const bool isPlayer);
	virtual void UpdatePlane(const Player& eyeObject, float elapsSec) override;
	virtual bool Trigger() override;
	
private:
	void Opening();

	int m_direction{ 1 };
	bool m_opening{ false };
	const float m_speed{ 1 };
	
	const float m_upperHeight;
	const float m_lowerHeight;
	const bool m_IsExit;
};

