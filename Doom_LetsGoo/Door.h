#pragma once
#include "Plane.h"
class Door : public Plane
{
public:
	Door(Vector3f& position, Vector3f& width, Vector3f& height, int colour, float upperHeight, float lowerHeight);
	virtual ~Door();
	virtual void Triggered() override;
	bool PlaneCollision(const Vector3f& pos3D, const Vector3f& nextPos3D,
		Vector2f& returnPos, Vector2f& outNormal, const bool isPlayer);
	virtual void UpdatePlane(const DoomCamera& eyeObject, float elapsSec) override;
private:
	void Opening();

	float m_lowerHeight{};
	float m_upperHeight{};

	int m_direction{1};
	float m_speed{ 1 };
	bool m_opening{false};
};

