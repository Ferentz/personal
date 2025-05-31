#include "pch.h"
#include "Things.h"
#include "Texture.h"
#include "PLayer.h"



Thing::Thing(Vector2f pos, float height, std::string sprite)
	:m_StartPos{ pos },
	m_Height{ height },
	m_location{ pos.x, pos.y, 0 },
	//m_pNavigator{nullptr},
	//m_pHealthManager{nullptr},
	//m_pInteractor{nullptr},
	m_SubClass{ ThingSubclass::decoration }
{
}

Thing::Thing(const Thing& toCopy) 
	:m_StartPos{ toCopy.m_StartPos },
	m_Height{ toCopy.m_Height },
	m_location{ toCopy.m_location },
	m_SubClass{ toCopy.m_SubClass }
{
	m_SubsectorIndex = toCopy.m_SubsectorIndex;
}

Thing::Thing(Thing&& rhs) noexcept
	:m_StartPos{ rhs.m_StartPos },
	m_Height{ rhs.m_Height },
	m_location{ rhs.m_location },
	m_SubClass{ rhs.m_SubClass }
{
	m_SubsectorIndex = rhs.m_SubsectorIndex;
}

Thing& Thing::operator=(const Thing& rhs)
{
	m_StartPos = rhs.m_StartPos;
	m_Height = rhs.m_Height;
	m_location = rhs.m_location;
	m_SubsectorIndex = rhs.m_SubsectorIndex;
	m_SubClass = rhs.m_SubClass;
	return *this;
}


void Thing::UpdateThing(const Player& player, float elapsSec)
{
	m_location.point2D = player.GetCamera().PerspCalc(m_location);
}

void Thing::DrawThing(Texture* tex)
{
	float scale{ 0.001f };
	float width{ tex->GetWidth() / (m_location.Dist3D * scale)};
	float height{ m_Height / (m_location.Dist3D * scale) };
	Rectf dest{ m_location.point2D.x - width / 2, m_location.point2D.y, width, height };
	//utils::SetColor(1, 1, 1, 1);
	//utils::DrawRect(dest);
	tex->Draw(dest);
}

Vector2f Thing::GetStartPos() const
{
	return m_StartPos;
}

int Thing::GetSubsectorIndex() const
{
	return m_SubsectorIndex;
}

void Thing::SetSubsectorIndex(int newSubsector)
{
	m_SubsectorIndex = newSubsector;
}

const point2D_3D& Thing::GetLocationPoint() const
{
	// TODO: insert return statement here
	return m_location;
}

void Thing::SetPosHeight(float height)
{
	m_location.point3D.z = height;
}

const ThingSubclass Thing::GetSubClass() const
{
	return m_SubClass;
}
