#pragma once
#include "structs.h"
#include <utils.h>


class Texture;
class DoomCamera;
class Plane
{
public:
	Plane();
	Plane(Vector3f& position, Vector3f& width, Vector3f& height, int texIdx);
	virtual ~Plane();
	virtual void UpdatePlane(const DoomCamera& eyeObject, float elapsSec);
	void DrawPlane(const Texture& texture);
	//verb in ervey function, plus, since we're already in plane class, no "plane" needed
	bool PlaneCollision(DoomCamera& eyeObject); // unused
	virtual bool PlaneCollision(const Vector3f& pos3D, const Vector3f& nextPos3D,
		Vector2f& returnPos, Vector2f& outNormal, const bool isPlayer) const;
	void SetActionBools(uint16_t actionBit);
	float planeOfset2{ 50.f };
	float planeOfset{ 400.f };
	int m_colour{};
	bool IsDoor();
	int GetTexIdx();
	virtual bool Triggered();
	void GetWallVerts(Vector2f wallVerts[]) const;
protected:
	Vector3f m_Location;
	point2D_3D m_Points[4];
private:
	bool m_bBlock{};
	bool m_bBlockMonster{};
	bool m_bTwoSided{};
	bool m_bUnpeggedUpperTex{};
	bool m_bUnpeggedLowerTex{};
	bool m_bSecret{};
	bool m_bSoundBlock{};
	bool m_bNotOnAutoMap{};
	bool m_bAlwaysOnAUtoMap{};
	int m_TextureIndex;

	bool m_IsVisible{};
	
	Vector3f m_Width;
	Vector3f m_Height;
	Vector2f m_Normal;
	

};

