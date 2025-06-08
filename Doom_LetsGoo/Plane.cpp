#include "pch.h"
#include "plane.h"

#include <iostream>
//#include <cmath>
//#include <vector>
#include <utils.h>

#include "Texture.h"
#include "Player.h"


Plane::Plane(Vector3f& position, Vector3f& width, Vector3f& height, int texIdx)
	:m_Location{ position },
	m_Width{ width },
	m_Height{ height },
	m_TextureIndex{ texIdx }
{
	Vector3f cross{ m_Width.CrossProduct(
		Vector3f(abs(m_Height.x), abs(m_Height.y), abs(m_Height.z))) };
	m_Normal = Vector2f{ cross.x,cross.y };

	for (int index{}; index < 4; index++) {
		m_Points[index] = {};
	}
	int pointIndex{};
	int mult{ 1 };
	int heightFactor{ 0 };
	for (int widthFactor{}; widthFactor < 2; widthFactor++) {

		for (int heightFactorPrev{}; heightFactorPrev < 2; heightFactorPrev++) {
			heightFactor += heightFactorPrev * mult;
			m_Points[pointIndex].point3D.x = position.x + width.x * widthFactor + height.x * heightFactor;
			m_Points[pointIndex].point3D.y = position.y + width.y * widthFactor + height.y * heightFactor;
			m_Points[pointIndex].point3D.z = position.z + width.z * widthFactor + height.z * heightFactor;
			pointIndex++;
		}
		mult = -1;
	}

}

#pragma region basics
void Plane::UpdatePlane(const Player& player, float elapsSec) {
	bool pointVisible{ false };
	for (int index{}; index < 4; index++) {
		m_Points[index].point2D = player.GetCamera().PerspectiveCalculation(m_Points[index]);
		if (m_Points[index].amIVisible)
		{
			pointVisible = true;
		}
	}

	for (int index{}; index < 4; index++) {
		bool vis1{ m_Points[index].amIVisible };
		int prevIndex{ utils::getArrIndex(4, index - 1) };
		bool vis2{ m_Points[prevIndex].amIVisible };
	}
	// clamp the dot result to reduce instances of large walls being visible whilst behind you
	if (m_Normal.Normalized().DotProduct(player.GetLookAtVector(true)) < 0.2f && pointVisible)
	{
		m_IsVisible = true;
	}
	else
	{
		m_IsVisible = false;
	}
}

void Plane::DrawPlane(const Texture& texture) const
{
	//the negine, open gl might not be set up to draw for 3D
	/*if (m_IsVisible) // no way to tell the normal when loading the level
	{
	}*/
	/*bool isVisible{ false };
	for (int index{}; index < 4; index++) {
		if (m_Points[index].amIVisible) {
			isVisible = true;
		}
	}*/
	if (m_IsVisible) {

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
#pragma endregion basics

#pragma region collision
bool Plane::PlaneCollision(const Vector3f& pos3D, const Vector3f& nextPos3D,
	Vector2f& returnPos, Vector2f& outNormal, const bool isPlayer) const
{
	float top = m_Location.z + m_Height.z;
	float bottom = m_Location.z;
	float topUse = std::max(top, bottom); // some walls are created upside down
	float bottomUse = std::min(top, bottom);
	//
	float highPoint = std::max(pos3D.z, nextPos3D.z); // some walls are created upside down
	float lowPoint = std::min(pos3D.z, nextPos3D.z);


	if (lowPoint > topUse || highPoint < bottomUse)
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
		// we wanna get at what distance it is, and then see if the height would be above or below the plane
		float percentage = hitInfo.lambda;
		float difference = (nextPos3D.z - pos3D.z) * percentage;
		float intersectionZ = pos3D.z + difference;

		const float epsilon = 0.01f;
		if (intersectionZ < bottomUse - epsilon || intersectionZ > topUse + epsilon)
		{
			return false;
		}
		/*std::cout << m_bBlock << std::endl;
		std::cout << m_bBlockMonster << std::endl;
		std::cout << m_bSecret << std::endl;
		std::cout << m_bSoundBlock << std::endl;
		std::cout << std::endl;*/

		Vector2f moveDir = (nextPos - pos).Normalized();
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

void Plane::GetWallVerts(Vector2f wallVerts[]) const
{
	wallVerts[0] = Vector2f{ m_Points[0].point3D.x, m_Points[0].point3D.y };
	wallVerts[1] = Vector2f{ m_Points[2].point3D.x, m_Points[2].point3D.y };
}
#pragma endregion collision

#pragma region interaction
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
bool Plane::Trigger()
{
	return false;
}
#pragma endregion interaction

void Plane::SetActionBools(uint16_t actionBit)
{
	m_bBlock = (actionBit & 0x0001) != 0;
	m_bBlockMonster = (actionBit & 0x0002) != 0;
	m_bTwoSided = (actionBit & 0x0004) != 0;
	m_bUnpeggedUpperTex = (actionBit & 0x0008) != 0;
	m_bUnpeggedLowerTex = (actionBit & 0x0010) != 0;
	m_bSecret = (actionBit & 0x0020) != 0;
	m_bSoundBlock = (actionBit & 0x0040) != 0;
	m_bNotOnAutoMap = (actionBit & 0x0080) != 0;
	m_bAlwaysOnAUtoMap = (actionBit & 0x0100) != 0;
}

