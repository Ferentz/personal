#pragma once
#include "utils.h"

class DoomCamera;
class Texture;
class Navigator;
class Thing
{
public:
	Thing(Vector2f pos, float height, utils::ThingsEnum type);
	virtual ~Thing();
	virtual void UpdateThing(const DoomCamera& eyeObject, float elapsSec);
	virtual void DrawThing(Texture* tex);
	Vector2f m_StartPos;
	float m_Height;
	int m_SubsectorIndex{};
	utils::ThingsEnum m_Type;
	point2D_3D m_location;
	Navigator* m_pNavigator;
};

