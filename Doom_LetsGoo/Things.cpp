#include "pch.h"
#include "Things.h"
#include "Texture.h"
#include "DoomCamera.h"

Thing::Thing(Vector2f pos, float height, utils::ThingsEnum type)
	:m_StartPos{ pos },
	m_Height{ height },
	m_Type{ type },
	m_location{ pos.x, pos.y, 0 },
	m_pNavigator{nullptr},
	m_pHealthInteractor{nullptr}
{
}

Thing::~Thing()
{
}

void Thing::UpdateThing(const DoomCamera& eyeObject, float elapsSec)
{
	m_location.point2D = eyeObject.PerspCalc(m_location, m_location.point3D);
}

void Thing::DrawThing(Texture* tex)
{
	float scale{ 0.001 };
	float width{ tex->GetWidth() / (m_location.Dist3D * scale)};
	float height{ m_Height / (m_location.Dist3D * scale) };
	Rectf dest{ m_location.point2D.x - width / 2, m_location.point2D.y, width, height };
	//utils::SetColor(1, 1, 1, 1);
	//utils::DrawRect(dest);
	tex->Draw(dest);
}
