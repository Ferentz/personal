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
	m_TextureIndex{},
	m_Normal{}
{

}
Plane::Plane(Vector3f& position, Vector3f& width, Vector3f& height, int texIdx)
	:m_Location{ position },
	m_Width{ width },
	m_Height{ height },
	m_TextureIndex{ texIdx },
	m_Normal{ m_Width.CrossProduct(m_Height).x,m_Width.CrossProduct(m_Height).y }
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
Plane::~Plane()
{
}
void Plane::UpdatePlane(const DoomCamera& eyeObject, float elapsSec) {
	for (int index{}; index < 4; index++) {
		m_Points[index].point2D = eyeObject.PerspCalc(m_Points[index], m_Points[index].point3D);
	}
	
	if (m_Normal.DotProduct(eyeObject.GetLookAtVector()) < 0)
	{
		m_IsVisible = true;
	}
	else
	{
		m_IsVisible = false;
	}
}

void Plane::DrawPlane(const Texture& texture) {
	//the negine, open gl might not be set up to draw for 3D
	/*if (m_IsVisible) // no way to tell the normal when loading the level
	{
	}*/
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

	float top{ m_Location.z + m_Height.z };
	float bottom{ m_Location.z };
	float topUse{ top };
	float bottomUse{ bottom };
	if (top < bottom)
	{
		topUse = bottom;
		bottomUse = top;
	}
	
	if (eyeObject.m_NextPos.z > topUse || eyeObject.m_NextPos.z < bottomUse)
	{
		return false;
	}
		
	utils::HitInfo hitInfo{};
	Vector2f pos{ eyeObject.m_Pos.x, eyeObject.m_Pos.y };
	Vector2f nextPos{ eyeObject.m_NextPos.x, eyeObject.m_NextPos.y };
	Vector2f wallvert[]{ Vector2f( m_Points[0].point3D.x, m_Points[0].point3D.y ),
		Vector2f{ m_Points[2].point3D.x, m_Points[2].point3D.y } };
	
	
	if (utils::Raycast(wallvert, 2,
		pos, nextPos,
		hitInfo))
	{
		//std::cout << m_bBlock << std::endl;
		//std::cout << m_bBlockMonster << std::endl;
		//std::cout << m_bSecret << std::endl;
		//std::cout << m_bSoundBlock << std::endl;
		//std::cout << std::endl;
		if (m_bBlockMonster)
		{
			std::cout << "activate door" << std::endl;
		}
		const Vector2f RAY{ pos - nextPos };
		const Vector2f REFLECTION{ RAY.Reflect(hitInfo.normal) };

		Vector2f nextPoint2D = hitInfo.intersectPoint - REFLECTION * (hitInfo.lambda);
		eyeObject.m_NextPos.x = nextPoint2D.x;
		eyeObject.m_NextPos.y = nextPoint2D.y;
		return true;
	}
	return false;
}

bool Plane::PlaneCollision(const Vector3f& pos3D, const Vector3f& nextPos3D,
	Vector2f& returnPos, Vector2f& outNormal, const bool isPlayer) const
{
	float top = m_Location.z + m_Height.z;
	float bottom = m_Location.z;
	float topUse = std::max(top, bottom); // some walls are created upside down
	float bottomUse = std::min(top, bottom);
	//
	if(((pos3D.z > topUse && isPlayer) ||
		//(((bottomUse > pos3D.z + 40) && (topUse < pos3D.z + 20)) && !isPlayer )||
		pos3D.z < bottomUse)) // if the player is above or below, if a monster and the ledge is bigger than the step height
	{
		return false;
	}
		

	Vector2f pos{ pos3D.x, pos3D.y };
	Vector2f nextPos{ nextPos3D.x, nextPos3D.y };

	Vector2f wallVerts[2]{};
	GetWallVerts(wallVerts);

	utils::HitInfo hitInfo{};
	if (utils::Raycast(wallVerts, 2, pos, nextPos, hitInfo))
	{
		/*std::cout << m_bBlock << std::endl;
		std::cout << m_bBlockMonster << std::endl;
		std::cout << m_bSecret << std::endl;
		std::cout << m_bSoundBlock << std::endl;
		std::cout << std::endl;*/
		if (m_bBlock)
		{
			//std::cout << "activate door" << std::endl;
		}
		// Get approach vector
		Vector2f moveDir = (nextPos - pos).Normalized();

		// Make sure normal is pointing toward pos (against motion)
		Vector2f hitNormal = hitInfo.normal.Normalized();
		if (hitNormal.DotProduct(moveDir) > 0.0f)
		{
			hitNormal = -hitNormal;  // Flip
		}

		outNormal = hitNormal;

		// Calculate reflection result
		Vector2f RAY = pos - nextPos;
		Vector2f REFLECTION = RAY.Reflect(hitNormal);
		Vector2f nextPoint2D = hitInfo.intersectPoint - REFLECTION * hitInfo.lambda;

		returnPos = nextPoint2D;
		return true;
	}

	return false;
}

void Plane::SetActionBools(uint16_t actionBit)
{
	m_bBlock = (actionBit & 0x0001) != 0 ;
	m_bBlockMonster = (actionBit & 0x0002) != 0 ;
	m_bTwoSided = (actionBit & 0x0004) != 0 ;
	m_bUnpeggedUpperTex = (actionBit & 0x0008) != 0;
	m_bUnpeggedLowerTex = (actionBit & 0x0010) != 0 ;
	m_bSecret = (actionBit & 0x0020) != 0 ;
	m_bSoundBlock = (actionBit & 0x0040) != 0 ;
	m_bNotOnAutoMap = (actionBit & 0x0080) != 0 ;
	m_bAlwaysOnAUtoMap = (actionBit & 0x0100) != 0 ;
}
bool Plane::IsDoor()
{
	if (m_bBlock)
	{
		return true;
	}
	return false;
}
int Plane::GetTexIdx()
{
	return m_TextureIndex;
}
void Plane::Triggered()
{
}
//
//bool Plane::PlaneCollision(DoomCamera& eyeObject) {
//	
//	Vector3f normalWidth{ m_Width.Normalized()};
//	Vector3f normalHeight{ m_Height.Normalized() };
//	Vector3f normal{ m_Width.CrossProduct(m_Height).Normalized() }; //cross(height).normalize() };
//
//	Vector3f relativeEyeLocation_moved{ eyeObject.m_Pos.x - m_Location.x,
//		eyeObject.m_Pos.y - m_Location.y,
//		eyeObject.m_Pos.z - m_Location.z };
//
//	//std::cout << relativeEyeLocation_moved << std::endl;
//
//	float newXpos = relativeEyeLocation_moved.DotProduct(normalWidth) * relativeEyeLocation_moved.DotProduct(normalWidth);//dot(normalWidth), 2)); // all power 2 to avoid using the square root later
//	float newYpos = relativeEyeLocation_moved.DotProduct(normalHeight) * relativeEyeLocation_moved.DotProduct(normalHeight);
//	float newZpos = relativeEyeLocation_moved.DotProduct(normal) * relativeEyeLocation_moved.DotProduct(normal);
//
//	float planeWidth = m_Width.DotProduct(m_Width);  // Squared length of width vector
//	float planeHeight = m_Height.DotProduct(m_Height);  // Squared length of height vector
//
//	while (newXpos >= 0 - planeOfset && newXpos <= planeWidth + planeOfset &&
//		newYpos >= 0 - planeOfset && newYpos <= planeHeight + planeOfset &&
//		newZpos >= -planeOfset && newZpos <= planeOfset) //planeOfset is the power 2 of the actual planeoffset
//	{
//		float sideCheck = relativeEyeLocation_moved.DotProduct(normal);
//		int mult{ -1 };
//		if (sideCheck > 0) {
//			mult = 1;
//		}
//		std::cout << "collide" << std::endl;
//		eyeObject.m_Pos.x += normal.x * mult;
//		eyeObject.m_Pos.y += normal.y * mult;
//		eyeObject.m_Pos.z += normal.z * mult;
//		return true;
//	}
//
//	return false;
//}

void Plane::GetWallVerts(Vector2f wallVerts[]) const 
{
	wallVerts[0] = Vector2f{ m_Points[0].point3D.x, m_Points[0].point3D.y };
	wallVerts[1] = Vector2f{ m_Points[2].point3D.x, m_Points[2].point3D.y };
}