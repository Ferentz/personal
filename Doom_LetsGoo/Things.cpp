#include "pch.h"
#include "Things.h"

#include "Player.h"
#include "Texture.h"



Thing::Thing(Vector2f pos, float height, std::string sprite, bool singleTexture)
	:m_StartPos{ pos },
	m_Height{ height },
	m_location{ pos.x, pos.y, 0 },
	m_SubClass{ ThingSubclass::decoration },
	m_FilePath{ "pickUps/" + sprite + "A0.png" }
{
	if (singleTexture)
	{
		m_pTex = new Texture{ m_FilePath };
	}
	
}

Thing::~Thing()
{
	if (m_pTex != nullptr)
	{
		delete m_pTex;
	}
	
}

#pragma region rule of 5
Thing::Thing(const Thing& toCopy) 
	:m_StartPos{ toCopy.m_StartPos },
	m_Height{ toCopy.m_Height },
	m_location{ toCopy.m_location },
	m_SubClass{ toCopy.m_SubClass },
	m_FilePath{ toCopy.m_FilePath }
{
	m_SubsectorIndex = toCopy.m_SubsectorIndex;
	m_pTex = new Texture{ toCopy.m_FilePath };
}

Thing::Thing(Thing&& rhs) noexcept
	:m_StartPos{ rhs.m_StartPos },
	m_Height{ rhs.m_Height },
	m_location{ rhs.m_location },
	m_SubClass{ rhs.m_SubClass },
	m_FilePath{ std::move(rhs.m_FilePath) }
{
	m_SubsectorIndex = rhs.m_SubsectorIndex;
	m_pTex = rhs.m_pTex;
	rhs.m_pTex = nullptr;
}

Thing& Thing::operator=(const Thing& rhs)
{
	if (&rhs != this)
	{
		m_StartPos = rhs.m_StartPos;
		m_Height = rhs.m_Height;
		m_location = rhs.m_location;
		m_SubsectorIndex = rhs.m_SubsectorIndex;
		m_SubClass = rhs.m_SubClass;

		m_FilePath = rhs.m_FilePath;
		if (m_pTex != nullptr)
		{
			delete m_pTex;
		}
		m_pTex = new Texture{ m_FilePath };
	}
	return *this;
}

Thing& Thing::operator=(Thing&& rhs) noexcept
{
	if (&rhs != this)
	{
		m_FilePath = std::move(rhs.m_FilePath);
		m_pTex = rhs.m_pTex;
		rhs.m_pTex = nullptr;
	}
	return *this;
}

#pragma endregion rule of 5

#pragma region basics
void Thing::UpdateThing(const Player& player, float elapsSec)
{
	m_location.point2D = player.GetCamera().PerspectiveCalculation(m_location);
	m_ScreenRatio = player.GetCamera().GetWindowWidth()/800;
}

void Thing::DrawThing()
{
	float scale{ 0.001f };
	float width{ m_pTex->GetWidth() * m_ScreenRatio / (m_location.Dist3D * scale)};
	float height{ m_pTex->GetHeight() * m_ScreenRatio / (m_location.Dist3D * scale) };
	Rectf dest{ m_location.point2D.x - width / 2, m_location.point2D.y, width, height };
	m_pTex->Draw(dest);
}

#pragma endregion basics

#pragma region interactions
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
#pragma endregion interactions