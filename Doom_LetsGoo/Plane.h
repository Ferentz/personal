#pragma once
#include "structs.h"
#include <utils.h>


class Texture;
class Player;
class Plane
{
public:
	Plane();
	Plane(Vector3f& position, Vector3f& width, Vector3f& height, int texIdx);
	Plane(const Plane& toCopy) = default;
	Plane(Plane&& toMove) = default;
	Plane& operator=(const Plane& rhs) = default;
	Plane& operator=(Plane&& rhs) = default;
	virtual ~Plane() = default;
	
	virtual void UpdatePlane(const Player& eyeObject, float elapsSec);
	void DrawPlane(const Texture& texture) const;
	//verb in ervey function, plus, since we're already in plane class, no "plane" needed
	bool PlaneCollision(Player& eyeObject); // unused
	virtual bool PlaneCollision(const Vector3f& pos3D, const Vector3f& nextPos3D,
		Vector2f& returnPos, Vector2f& outNormal, const bool isPlayer) const ;
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

