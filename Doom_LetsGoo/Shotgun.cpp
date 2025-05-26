#include "pch.h"
#include "Shotgun.h"
#include "Shootable.h"
#include "Texture.h"

Shotgun::Shotgun(int startAmo, int maxAmo, float range)
	:Weapon(startAmo, maxAmo, range, "sounds/DSSHOTGN.wav")
{
	m_ShootOverayNum = 2;
	m_AnimationFrames = 4;

	m_WeaponImages[0] = new Texture("Shotgun/SHTFA0.png");
	m_WeaponImages[1] = new Texture("Shotgun/SHTFB0.png");
	m_WeaponImages[2] = new Texture("Shotgun/SHTGA0.png");
	m_WeaponImages[3] = new Texture("Shotgun/SHTGB0.png");
	m_WeaponImages[4] = new Texture("Shotgun/SHTGC0.png");
	m_WeaponImages[5] = new Texture("Shotgun/SHTGD0.png");

	m_TextureOffsets[0][0] = -141; m_TextureOffsets[0][1] = -95;
	m_TextureOffsets[1][0] = -136; m_TextureOffsets[1][1] = -86;
	m_TextureOffsets[2][0] = -121; m_TextureOffsets[2][1] = -108;
	m_TextureOffsets[3][0] = -43; m_TextureOffsets[3][1] = -47;
	m_TextureOffsets[4][0] = -30; m_TextureOffsets[4][1] = -17;
	m_TextureOffsets[5][0] = -29; m_TextureOffsets[5][1] = -37;

}

void Shotgun::Shoot(std::vector<Shootable*> shootables, float windowWidth)
{
	if (GetAmo() > 0 && !GetIsShooting())
	{
		m_DrawFire = true;
		for (int bulletNum{}; bulletNum < 7; bulletNum++)
		{
			float bulletX{ rand() % (int(windowWidth / 3)) + windowWidth / 3 };

			Weapon::Shoot(shootables, bulletX * 2);
		}
		ChangeAmoBy(-1);
	}
}

void Shotgun::UpdateWeapon(float elapsedsecs)
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

void Shotgun::DrawWeapon(float windowWidth) const
{
	glPushMatrix();
	{
		int indexFire{ m_ImageIndex };
		int indexGun{ m_ShootOverayNum };
		if (m_IsShooting && ! m_DrawFire)
		{
			indexGun = m_ImageIndex + m_ShootOverayNum;
		}
	/*	if (!m_IsShooting)
		{
			indexGun = m_ShootOverayNum;
		}*/

		//glTranslatef(windowWidth / 2, 100.f, 0.f); //moves the image space 
		glScalef(windowWidth / 300, windowWidth / 300, 0.f);
		glTranslatef(0.f, 200, 0.f);
		if (m_IsShooting && !m_Reverse && m_DrawFire)
		{
			glPushMatrix();
			{
				//glTranslatef(0.f, m_WeaponImages[m_ImageIndex]->GetHeight()/2, 0.f); //moves the image space 
				glTranslatef(-m_TextureOffsets[indexFire][0], m_TextureOffsets[indexFire][1], 0.f);
				m_WeaponImages[indexFire]->Draw(
					Rectf{
				0.f,
				-m_WeaponImages[indexFire]->GetHeight(),
				m_WeaponImages[indexFire]->GetWidth(),
				m_WeaponImages[indexFire]->GetHeight()
					}
				);
			}
			glPopMatrix();
		}

		glTranslatef(-m_TextureOffsets[indexGun][0], m_TextureOffsets[indexGun][1], 0.f);

		m_WeaponImages[indexGun]->Draw(
			Rectf{
				0.f,
			-m_WeaponImages[indexGun]->GetHeight(),
			m_WeaponImages[indexGun]->GetWidth(),
			m_WeaponImages[indexGun]->GetHeight()
			}
		);

	}
	glPopMatrix();
}

