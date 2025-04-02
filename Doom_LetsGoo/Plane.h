#pragma once
#include "structs.h"
#include <utils.h>


class Texture;
class DoomCamera;
class Plane
{
public:
	Plane();
	Plane(Vector3f& position, Vector3f& width, Vector3f& height, int colour);
	void UpdatePlane(const DoomCamera& eyeObject);
	void DrawPlane(const Texture& texture);
	//verb in ervey function, plus, since we're already in plane class, no "plane" needed
	bool PlaneCollision(DoomCamera& eyeObject);
	float planeOfset2{ 50.f };
	float planeOfset{ 400.f };
	int m_colour;
private:

	Vector3f m_Location;
	Vector3f m_Width;
	Vector3f m_Height;
	point2D_3D m_Points[4];

};

