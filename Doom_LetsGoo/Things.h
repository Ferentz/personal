#pragma once

class Player;
class Texture;


enum class ThingSubclass
{
	decoration,
	pickup,
	shootable,
	enemy
};

class Thing
{
public:
	Thing( Vector2f pos, float height, std::string sprite, bool singleTexture = false );
	virtual ~Thing();

	//rule of 5
	Thing( const Thing& toCopy );
	Thing( Thing&& rhs ) noexcept;
	Thing& operator=(const Thing& rhs);
	Thing& operator=(Thing&& rhs);

	// basics
	virtual void UpdateThing(const Player& eyeObject, float elapsSec);
	virtual void DrawThing(Texture* tex);

	//interactions
	Vector2f GetStartPos() const;
	int GetSubsectorIndex() const;
	void SetSubsectorIndex(int newSubsector);
	virtual const point2D_3D& GetLocationPoint() const;
	void SetPosHeight(float height);
	const ThingSubclass GetSubClass() const;


protected:
	Vector2f m_StartPos;
	float m_Height;
	point2D_3D m_location;

	ThingSubclass m_SubClass;
	

	int m_SubsectorIndex{};
	float m_ScreenRatio{};

private:
	std::string m_FilePath;
	Texture* m_pTex{nullptr};
	
};

