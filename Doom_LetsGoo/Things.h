#pragma once
#include "utils.h"

class PLayer;
class Texture;
class Navigator;
class HealthManager;
class Interactor;

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
	Thing( Vector2f pos, float height, std::string sprite );
	Thing( const Thing& toCopy );
	Thing( Thing&& rhs ) noexcept;
	virtual ~Thing() = default;
	Thing& operator=(const Thing& rhs);
	Thing& operator=(Thing&& rhs) = delete; // it has no movable members

	virtual void UpdateThing(const PLayer& eyeObject, float elapsSec);
	virtual void DrawThing(Texture* tex);
	Vector2f GetStartPos() const;
	int GetSubsectorIndex() const;
	void SetSubsectorIndex(int newSubsector);
	virtual const point2D_3D& GetLocationPoint() const;
	void SetPosHeight(float height);
	const ThingSubclass GetSubClass() const;
	//prefered enumerator +static cast over checking for nullptr
	//Navigator* m_pNavigator{};
	//HealthManager* m_pHealthManager{};
	//Interactor* m_pInteractor{};

protected:
	Vector2f m_StartPos;
	float m_Height;
	int m_SubsectorIndex{};
	point2D_3D m_location;
	ThingSubclass m_SubClass;
};

