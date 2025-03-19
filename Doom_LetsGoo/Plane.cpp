#include "pch.h"
#include "plane.h"
#include <iostream>
#include <cmath>
#include <vector> // For vector operations if needed
#include <utils.h>
#include "Texture.h"
#include "DoomCamera.h"

const Color4f RED{ 1.0f, 0.0f, 0.0f, 1.0f };
const Color4f WHITE{ 1.0f, 1.0f, 1.0f, 1.0f };
const Color4f YELLOW{ 1.0f, 1.0f, 0.0f, 1.0f };
const Color4f BLUE{ 0.0f, 0.0f, 1.0f, 1.0f };
const Color4f GREEN{ 0.0f, 1.0f, .0f, 1.0f };
const Color4f TEAL{ 0.0f, 1.0f, 1.0f, 1.0f };

Plane::Plane()
	:m_Location{},
	m_Width{},
	m_Height{},
	m_colour{}
{

}
Plane::Plane(Vector3f& position, Vector3f& width, Vector3f& height, int colour)
	:m_Location{ position },
	m_Width{ width },
	m_Height{ height },
	m_colour{ colour }
{
	for (int index{}; index < 4; index++) {
		m_Points[index] = {};
	}
	int pointIndex{};
	int mult{ 1 };
	int heightFactor{ 0 };
	for (int widthFactor{}; widthFactor < 2; widthFactor++) {

		for (int heightFactorPrev{}; heightFactorPrev < 2; heightFactorPrev++) {
			// first 0 0 , then 0 1, then 1 1, then 1 0

			// so, 0 0 1 1 is good
			//then 0 1 1 0
			// 0 
			heightFactor += heightFactorPrev * mult;
			m_Points[pointIndex].point3D.x = position.x + width.x * widthFactor + height.x * heightFactor;
			m_Points[pointIndex].point3D.y = position.y + width.y * widthFactor + height.y * heightFactor;
			m_Points[pointIndex].point3D.z = position.z + width.z * widthFactor + height.z * heightFactor;
			pointIndex++;
		}
		mult = -1;
	}

}
void Plane::UpdatePlane(const DoomCamera& eyeObject) {
	for (int index{}; index < 4; index++) {
		m_Points[index].point2D = eyeObject.PerspCalc(m_Points[index], m_Points[index].point3D);
		if (m_Points[index].Angle[2] > M_PI * 0.5f || m_Points[index].Angle[2] < - M_PI * 0.5f) {
			m_Points[index].amIVisible = false;
		}
		else {
			m_Points[index].amIVisible = true;
		}
	}

}
void Plane::DrawPlane(const Texture& texture) {
	//the negine, open gl might not be set up to draw for 3D
	bool isVisible{ false };
	for (int index{}; index < 4; index++) {
		if (m_Points[index].amIVisible) {
			isVisible = true;
		}
	}
	if (isVisible) {

		Vector3f planesCompArr3D[4];
		for (int index{}; index < 4; index++) {
			planesCompArr3D[index] = Vector3f{ 
				m_Points[index].point2D.x, m_Points[index].point2D.y,
				-m_Points[index].Dist3D * 0.0001f 
			}; // need to explicitly call it a vector3f
		}

		texture.DrawTexturePersp(planesCompArr3D);
	}
}

bool Plane::PlaneCollision(DoomCamera& eyeObject) {
	Vector3f normalWidth{ m_Width.Normalized()};
	Vector3f normalHeight{ m_Height.Normalized() };
	Vector3f normal{ m_Width.CrossProduct(m_Height).Normalized() }; //cross(height).normalize() };

	Vector3f relativeEyeLocation_moved{ eyeObject.m_Pos.x - m_Location.x,
		eyeObject.m_Pos.y - m_Location.y,
		eyeObject.m_Pos.z - m_Location.z };



	float newXpos = relativeEyeLocation_moved.DotProduct(normalWidth) * relativeEyeLocation_moved.DotProduct(normalWidth);//dot(normalWidth), 2)); // all power 2 to avoid using the square root later
	float newYpos = relativeEyeLocation_moved.DotProduct(normalHeight) * relativeEyeLocation_moved.DotProduct(normalHeight);
	float newZpos = relativeEyeLocation_moved.DotProduct(normal) * relativeEyeLocation_moved.DotProduct(normal);

	float planeWidth = m_Width.DotProduct(m_Width);  // Squared length of width vector
	float planeHeight = m_Height.DotProduct(m_Height);  // Squared length of height vector

	while (newXpos >= 0 - planeOfset && newXpos <= planeWidth + planeOfset &&
		newYpos >= 0 - planeOfset && newYpos <= planeHeight + planeOfset &&
		newZpos >= -planeOfset && newZpos <= planeOfset) //planeOfset is the power 2 of the actual planeoffset
	{
		float sideCheck = relativeEyeLocation_moved.DotProduct(normal);
		int mult{ -1 };
		if (sideCheck > 0) {
			mult = 1;
		}
		eyeObject.m_Pos.x += normal.x * mult;
		eyeObject.m_Pos.y += normal.y * mult;
		eyeObject.m_Pos.z += normal.z * mult;
		return true;
	}

	return false;
}

