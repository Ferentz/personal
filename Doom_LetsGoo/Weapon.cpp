#include "pch.h"
#include "Weapon.h"
#include "Enemy.h"
#include "Texture.h"
#include <iostream>

Weapon::Weapon(int startAmo, int maxAmo, float range, std::string soundFile)
	:m_MaxAmo{ maxAmo },
	m_Amo{startAmo},
	m_Range{range},
	m_GunShootSound{ soundFile }
{
}

Weapon::~Weapon()
{
	if (m_WeaponImages[0] != nullptr)
	{
		for (int i{}; i < 6; i++)
		{
			delete m_WeaponImages[i];
			m_WeaponImages[i] = nullptr;
		}
	}
}

void Weapon::Shoot(std::vector<Shootable*> shootables, float windowWidth)
{
	m_GunShootSound.Play(false);
	m_IsShooting = true;
	int closest{ -1 };
	float closestDist{ GetRange() };
	for (int index{}; index < shootables.size(); index++)
	{
		float xDiff = shootables[index]->GetLocationPoint().point2D.x - windowWidth / 2;
		if (closestDist > shootables[index]->GetLocationPoint().Dist3D
			&& abs(xDiff) < GetScreenRange())
		{
			closest = index;
			closestDist = shootables[index]->GetLocationPoint().Dist3D;
		}
	}

	if (closest >= 0)
	{
		Weapon::Shoot(shootables[closest]);
	}
}

int Weapon::GetMaxAmo() const
{
    return m_MaxAmo;
}

int Weapon::GetAmo() const
{
    return m_Amo;
}

void Weapon::ChangeAmoBy(int amount)
{
	m_Amo += amount;
	if (m_Amo < 0)
	{
		m_Amo = 0;
	}
	else if (m_Amo > m_MaxAmo)
	{
		m_Amo = m_MaxAmo;
	}
}

void Weapon::IncreaseMaxAmo(int amount)
{
	m_MaxAmo += amount;
}

//void Weapon::DrawWeapon(float windowWidth) const
//{
//	glPushMatrix();
//	{
//		int indexToUse{ m_ShootOverayNum };
//		if (m_IsShooting)
//		{
//			indexToUse = m_ImageIndex + m_ShootOverayNum;
//		}
//		if (!m_IsShooting)
//		{
//			indexToUse = m_ShootOverayNum ;
//		}
//
//		//glTranslatef(windowWidth / 2, 100.f, 0.f); //moves the image space 
//		glScalef(windowWidth / 300, windowWidth / 300, 0.f);
//		glTranslatef(0.f, 200, 0.f);
//		if (m_IsShooting && !m_Reverse && m_ImageIndex == m_FireFrame)
//		{
//			glPushMatrix();
//			{
//				//glTranslatef(0.f, m_WeaponImages[m_ImageIndex]->GetHeight()/2, 0.f); //moves the image space 
//				glTranslatef(-m_TextureOffsets[0][0], m_TextureOffsets[0][1], 0.f);
//				m_WeaponImages[0]->Draw(
//					Rectf{
//				0.f,
//				-m_WeaponImages[0]->GetHeight(),
//				m_WeaponImages[0]->GetWidth(),
//				m_WeaponImages[0]->GetHeight()
//					}
//				);
//			}
//			glPopMatrix();
//		}
//		
//		glTranslatef(-m_TextureOffsets[indexToUse][0], m_TextureOffsets[indexToUse][1], 0.f);
//
//		std::cout << indexToUse << std::endl;
//		m_WeaponImages[indexToUse]->Draw(
//			Rectf{
//				0.f,
//			-m_WeaponImages[indexToUse]->GetHeight(),
//			m_WeaponImages[indexToUse]->GetWidth(),
//			m_WeaponImages[indexToUse]->GetHeight()
//			}
//		);
//		
//	}
//	glPopMatrix();
//}
//
//void Weapon::UpdateWeapon(float elapsedsecs)
//{
//	if (m_IsShooting)
//	{
//		m_timer += elapsedsecs;
//		if (m_timer >= m_ImageRefresh)
//		{
//			m_timer = 0;
//			if (!m_Reverse)
//			{
//				m_ImageIndex++;
//
//				if (m_ImageIndex == m_FireFrame)
//				{
//					//m_DrawFire = true;
//				}
//				if (m_DrawFire && m_ImageIndex > m_FireFrame+ m_ShootOverayNum -1)
//				{
//					m_ImageIndex = 0;
//					
//					m_DrawFire = false;
//					std::cout << "stop drawing fire-------------------" << std::endl;
//				}
//				else if (m_ImageIndex >= m_AnimationFrames-1)
//				{
//					m_Reverse = true;
//					std::cout << "reverse animation----------------------" << std::endl;
//				}
//			}
//			else
//			{
//				m_ImageIndex--;
//				if (m_ImageIndex <= 0)
//				{
//					std::cout << "done shooting" << std::endl;
//					m_IsShooting = false;
//					m_ImageIndex = 0;
//					m_Reverse = false;
//				}
//			}
//			
//		}
//	}
//}


int Weapon::GetDamageOutput() const
{
	return (rand() % 11) + 5;
}

void Weapon::Shoot(Shootable* object)
{
	if (m_Amo > 0)
	{
		object->BeShot(GetDamageOutput());
	
	}
}

float Weapon::GetRange() const
{
	return m_Range;
}

float Weapon::GetScreenRange() const
{
	return m_ScreenRange;
}

bool Weapon::GetIsShooting() const
{
	return m_IsShooting;
}

