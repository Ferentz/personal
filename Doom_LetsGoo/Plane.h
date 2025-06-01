#pragma once


class Texture;
class Player;
class Plane
{
public:
	//Plane();
	explicit Plane(Vector3f& position, Vector3f& width, Vector3f& height, int texIdx);
	virtual ~Plane() = default;

	//rule of 5
	Plane(const Plane& toCopy) = default;
	Plane(Plane&& toMove) = default;
	Plane& operator=(const Plane& rhs) = default;
	Plane& operator=(Plane&& rhs) = default;
	
	//basics
	virtual void UpdatePlane(const Player& eyeObject, float elapsSec);
	void DrawPlane(const Texture& texture) const;
	
	//collision
	virtual bool PlaneCollision(const Vector3f& pos3D, const Vector3f& nextPos3D,
		Vector2f& returnPos, Vector2f& outNormal, const bool isPlayer) const ;
	
	//interaction
	bool IsDoor();
	int GetTexIdx();
	virtual bool Trigger();

	//not realy used yet
	void SetActionBools(uint16_t actionBit);
	

protected:
	Vector3f m_Location;
	point2D_3D m_Points[4];

private:
	//collision helper
	void GetWallVerts(Vector2f wallVerts[]) const;

	int m_TextureIndex;
	bool m_IsVisible{};

	Vector3f m_Width;
	Vector3f m_Height;
	Vector2f m_Normal{};

	//booleans for future functionality, not realy used yet
	bool m_bBlock{};
	bool m_bBlockMonster{};
	bool m_bTwoSided{};
	bool m_bUnpeggedUpperTex{};
	bool m_bUnpeggedLowerTex{};
	bool m_bSecret{};
	bool m_bSoundBlock{};
	bool m_bNotOnAutoMap{};
	bool m_bAlwaysOnAUtoMap{};
};

