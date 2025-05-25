#include "pch.h"
#include "Pistol.h"
#include "Shootable.h"
#include "Texture.h"

Pistol::Pistol(int startAmo, int maxAmo, float range)
	:Weapon(startAmo, maxAmo, range)

{
	m_WeaponImages[0] = new Texture("Pistol/PISFA0.png");
	m_WeaponImages[1] = new Texture("Pistol/PISGA0.png");
	m_WeaponImages[2] = new Texture("Pistol/PISGB0.png");
	m_WeaponImages[3] = new Texture("Pistol/PISGC0.png");
	m_WeaponImages[4] = new Texture("Pistol/PISGD0.png");
	m_WeaponImages[5] = new Texture("Pistol/PISGE0.png");

	m_TextureOffsets[0][0] = -140; m_TextureOffsets[0][1] = -66;
	m_TextureOffsets[1][0] = -126; m_TextureOffsets[1][1] = -106;
	m_TextureOffsets[2][0] = -104; m_TextureOffsets[2][1] = -86;
	m_TextureOffsets[3][0] = -119; m_TextureOffsets[3][1] = -87;
	m_TextureOffsets[4][0] = -125; m_TextureOffsets[4][1] = -87;
	m_TextureOffsets[5][0] = -106; m_TextureOffsets[5][1] = -65;
	
}

void Pistol::Shoot(std::vector<Shootable*> shootables, float windowWidth)
{
	if (GetAmo() > 0 && !GetIsShooting())
	{
		Weapon::Shoot(shootables, windowWidth);
		ChangeAmoBy(-1);
	}
}

void Pistol::UpdateWeapon(float elapsedsecs)
{
	if (m_IsShooting)
	{
		m_timer += elapsedsecs;
		if (m_timer >= m_ImageRefresh)
		{
			m_timer = 0;
			if (!m_Reverse)
			{
				m_ImageIndex++;

				if (m_ImageIndex == m_FireFrame)
				{
					//m_DrawFire = true;
				}
				if (m_DrawFire && m_ImageIndex > m_FireFrame + m_ShootOverayNum - 1)
				{
					m_ImageIndex = 0;

					m_DrawFire = false;
				}
				else if (m_ImageIndex >= m_AnimationFrames - 1)
				{
					m_Reverse = true;
				}
			}
			else
			{
				m_ImageIndex--;
				if (m_ImageIndex <= 0)
				{
					m_IsShooting = false;
					m_ImageIndex = 0;
					m_Reverse = false;
				}
			}

		}
	}
}

void Pistol::DrawWeapon(float windowWidth) const
{
	glPushMatrix();
	{
		int indexToUse{ m_ShootOverayNum };
		if (m_IsShooting)
		{
			indexToUse = m_ImageIndex + m_ShootOverayNum;
		}
		if (!m_IsShooting)
		{
			indexToUse = m_ShootOverayNum;
		}

		//glTranslatef(windowWidth / 2, 100.f, 0.f); //moves the image space 
		glScalef(windowWidth / 300, windowWidth / 300, 0.f);
		glTranslatef(0.f, 200, 0.f);
		if (m_IsShooting && !m_Reverse && m_ImageIndex == m_FireFrame)
		{
			glPushMatrix();
			{
				//glTranslatef(0.f, m_WeaponImages[m_ImageIndex]->GetHeight()/2, 0.f); //moves the image space 
				glTranslatef(-m_TextureOffsets[0][0], m_TextureOffsets[0][1], 0.f);
				m_WeaponImages[0]->Draw(
					Rectf{
				0.f,
				-m_WeaponImages[0]->GetHeight(),
				m_WeaponImages[0]->GetWidth(),
				m_WeaponImages[0]->GetHeight()
					}
				);
			}
			glPopMatrix();
		}

		glTranslatef(-m_TextureOffsets[indexToUse][0], m_TextureOffsets[indexToUse][1], 0.f);

		m_WeaponImages[indexToUse]->Draw(
			Rectf{
				0.f,
			-m_WeaponImages[indexToUse]->GetHeight(),
			m_WeaponImages[indexToUse]->GetWidth(),
			m_WeaponImages[indexToUse]->GetHeight()
			}
		);

	}
	glPopMatrix();
}
