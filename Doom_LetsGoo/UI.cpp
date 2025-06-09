#include "pch.h"
#include "UI.h"
#include "Texture.h"
#include "Player.h"
#include "utils.h"

const Player*  UI::s_pPlayer{};

UI::UI(const Rectf& window, Player* pPlayer)
	:m_pBasisUI{ new Texture{"UIBits/STBAR_converted.png"} },
	m_pNumbersRed{
	new Texture{"numRed/STTNUM0.png"} ,
	new Texture{"numRed/STTNUM1.png"},
	new Texture{"numRed/STTNUM2.png"},
	new Texture{"numRed/STTNUM3.png"},
	new Texture{"numRed/STTNUM4.png"},
	new Texture{"numRed/STTNUM5.png"},
	new Texture{"numRed/STTNUM6.png"},
	new Texture{"numRed/STTNUM7.png"},
	new Texture{"numRed/STTNUM8.png"},
	new Texture{"numRed/STTNUM9.png"},
	new Texture{"numRed/STTPRCNT.png"},
	new Texture{"numRed/STTMINUS.png"}
	},
	m_pNumbersGreyFlat{
	new Texture{"numGreyFlat/AMMNUM0.png"} ,
	new Texture{"numGreyFlat/AMMNUM1.png"},
	new Texture{"numGreyFlat/AMMNUM2.png"},
	new Texture{"numGreyFlat/AMMNUM3.png"},
	new Texture{"numGreyFlat/AMMNUM4.png"},
	new Texture{"numGreyFlat/AMMNUM5.png"},
	new Texture{"numGreyFlat/AMMNUM6.png"},
	new Texture{"numGreyFlat/AMMNUM7.png"},
	new Texture{"numGreyFlat/AMMNUM8.png"},
	new Texture{"numGreyFlat/AMMNUM9.png"}
	},
	m_pNumbersGreyShadow{
	new Texture{"numGreyShadow/STGNUM0.png"} ,
	new Texture{"numGreyShadow/STGNUM1.png"},
	new Texture{"numGreyShadow/STGNUM2.png"},
	new Texture{"numGreyShadow/STGNUM3.png"},
	new Texture{"numGreyShadow/STGNUM4.png"},
	new Texture{"numGreyShadow/STGNUM5.png"},
	new Texture{"numGreyShadow/STGNUM6.png"},
	new Texture{"numGreyShadow/STGNUM7.png"},
	new Texture{"numGreyShadow/STGNUM8.png"},
	new Texture{"numGreyShadow/STGNUM9.png"}
	},
	m_pNumbersYellow{
	new Texture{"numYellow/STYSNUM0.png"} ,
	new Texture{"numYellow/STYSNUM1.png"},
	new Texture{"numYellow/STYSNUM2.png"},
	new Texture{"numYellow/STYSNUM3.png"},
	new Texture{"numYellow/STYSNUM4.png"},
	new Texture{"numYellow/STYSNUM5.png"},
	new Texture{"numYellow/STYSNUM6.png"},
	new Texture{"numYellow/STYSNUM7.png"},
	new Texture{"numYellow/STYSNUM8.png"},
	new Texture{"numYellow/STYSNUM9.png"}
	},
	m_pBasisArms{new Texture{"UIBits/STARMS.png"}},
	m_pDoomguyFaceNormal{
	new Texture{"UIBits/STFST00.png"}, 
	new Texture{"UIBits/STFST01.png"}, 
	new Texture{"UIBits/STFST02.png"}, 
	new Texture{"UIBits/STFST10.png"},
	new Texture{"UIBits/STFST11.png"},
	new Texture{"UIBits/STFST12.png"}, 
	new Texture{"UIBits/STFST20.png"},
	new Texture{"UIBits/STFST21.png"},
	new Texture{"UIBits/STFST22.png"},
	new Texture{"UIBits/STFST30.png"},
	new Texture{"UIBits/STFST31.png"},
	new Texture{"UIBits/STFST32.png"},
	new Texture{"UIBits/STFST40.png"},
	new Texture{"UIBits/STFST41.png"},
	new Texture{"UIBits/STFST42.png"} },
	m_pDoomguyFaceOuch{
	new Texture{"UIBits/STFOUCH0.png"},
	new Texture{"UIBits/STFOUCH1.png"},
	new Texture{"UIBits/STFOUCH2.png"},
	new Texture{"UIBits/STFOUCH3.png"},
	new Texture{"UIBits/STFOUCH4.png"}
	},
	m_pDoomguyFaceKill{
	new Texture{"UIBits/STFKILL0.png"},
	new Texture{"UIBits/STFKILL1.png"},
	new Texture{"UIBits/STFKILL2.png"},
	new Texture{"UIBits/STFKILL3.png"},
	new Texture{"UIBits/STFKILL4.png"}
	},
	r_Window{ window }
{
	s_pPlayer = pPlayer;
	m_PrevHealth = s_pPlayer->m_HealthManager.GetHealth();
}

UI::~UI()
{
	for (int index{}; index < 12; index++)
	{
		delete m_pNumbersRed[index];
		m_pNumbersRed[index] = nullptr;
	}
	for (int index{}; index < 10; index++)
	{
		delete m_pNumbersGreyFlat[index];
		delete m_pNumbersGreyShadow[index];
		delete m_pNumbersYellow[index];

		m_pNumbersGreyFlat[index] = nullptr;
		m_pNumbersGreyShadow[index] = nullptr;
		m_pNumbersYellow[index] = nullptr;
	}
	delete m_pBasisArms;
	m_pBasisArms = nullptr;
	delete m_pBasisUI;
	m_pBasisUI = nullptr;

	for (int index{}; index < (s_NormalFaceRows * s_FaceHurtLevels); index++)
	{
		delete m_pDoomguyFaceNormal[index];
		m_pDoomguyFaceNormal[index] = nullptr;
	}
	for (int index{}; index < s_FaceHurtLevels; index++)
	{
		delete m_pDoomguyFaceOuch[index];
		delete m_pDoomguyFaceKill[index];

		m_pDoomguyFaceOuch[index] = nullptr;
		m_pDoomguyFaceKill[index] = nullptr;
	}

}



void UI::UpdateUI(float elapsedSecs)
{
	m_Timer += elapsedSecs;
	if (m_DiffFaceTimer > 0)
	{
		m_DiffFaceTimer -= elapsedSecs;
	}
	int curHealth{ s_pPlayer->m_HealthManager.GetHealth() };
	float HeathFrac = float(curHealth) / s_pPlayer->m_HealthManager.GetFullHealth();
	float HealthInccrement{ 1.f / s_FaceHurtLevels };
	m_HurtLevel = int(s_FaceHurtLevels - (HeathFrac / HealthInccrement));
	if (m_HurtLevel < 0)
	{
		m_HurtLevel = 0;
	}
	if (m_HurtLevel > s_FaceHurtLevels - 1)
	{
		m_HurtLevel = s_FaceHurtLevels - 1;
	}

	m_Hurt = false;
	if(curHealth < m_PrevHealth)
	{
		m_DiffFaceTimer = m_DiffFaceTime;
		HurtF_ShootT = false;
		m_Hurt = true;
		m_PrevHealth = curHealth;
	}
	else if (curHealth > m_PrevHealth)
	{
		m_PickedUp = true;
	}

	m_PickedUp = false;
	if (s_pPlayer->m_WeaponManager.GetPistolAmo() > m_PrevAmo[0])
	{
		m_PickedUp = true;
		m_PrevAmo[0] = s_pPlayer->m_WeaponManager.GetPistolAmo();
	}
	else if (s_pPlayer->m_WeaponManager.GetShotgunAmo() > m_PrevAmo[1])
	{
		m_PickedUp = true;
		m_PrevAmo[1] = s_pPlayer->m_WeaponManager.GetShotgunAmo();
	}
	else if(s_pPlayer->m_WeaponManager.GetPistolAmo() < m_PrevAmo[0]||
		s_pPlayer->m_WeaponManager.GetShotgunAmo() < m_PrevAmo[1])
	{
		m_PrevAmo[0] = s_pPlayer->m_WeaponManager.GetPistolAmo();
		m_PrevAmo[1] = s_pPlayer->m_WeaponManager.GetShotgunAmo();
		m_DiffFaceTimer = m_DiffFaceTime;
		HurtF_ShootT = true;
	}

	if (m_Timer >= m_IntervalTime)
	{
		
		if (m_FaceSide)
		{
			m_FaceIndex++;
		}
		else {
			m_FaceIndex--;
		}
		if (m_FaceIndex >= s_NormalFaceRows-1)
		{
			m_FaceSide = false;
		}
		else if(m_FaceIndex <= 0)
		{
			m_FaceSide = true;
		}
		m_Timer = 0;
	}
	
}

void UI::DrawOverlay()
{
	glDepthFunc(GL_ALWAYS);
	if (m_Hurt)
	{
		utils::SetColor(1.f, 0.f, 0.f, .5f);
		utils::FillRect(0.f, 0.f, r_Window.width, r_Window.height);
	}
	else if (m_PickedUp)
	{
		utils::SetColor(1.f, 1.f, 0.f, .5f);
		utils::FillRect(0.f, 0.f, r_Window.width, r_Window.height);
	}
}

void UI::DrawUI(const float width,
	int health, int armor, int amo) const
{
	//float width{ GetWindow().width };
	float uiWidth{ m_pBasisUI->GetWidth() };
	float ratio{ r_Window.width / uiWidth };

	


	//Rectf hi{ 0, 0, width, m_UI->GetHeight() / ratio };
	glPushMatrix(); // adds a clean matrix for the image to exist in
	{
		glTranslatef((r_Window.width/2) - m_pDoomguyFaceNormal[0]->GetWidth()*1.3f, 0.f, 0.f); //moves the image space 
		glScalef(ratio, ratio, 0.f);
		glRotatef(0.f, 0.f, 0.f, 1.f);
		int FaceDisplayIndex = m_FaceIndex + m_HurtLevel * s_NormalFaceRows;
		if (m_DiffFaceTimer > 0)
		{
			if (HurtF_ShootT)
			{
				m_pDoomguyFaceKill[m_HurtLevel]->Draw(
					Rectf{ 0.f,
					0.f,
					m_pDoomguyFaceKill[m_HurtLevel]->GetWidth(),
					m_pDoomguyFaceKill[m_HurtLevel]->GetHeight()

					}
				);
			}
			else
			{
				m_pDoomguyFaceOuch[m_HurtLevel]->Draw(
					Rectf{ 0.f,
					0.f,
					m_pDoomguyFaceOuch[m_HurtLevel]->GetWidth(),
					m_pDoomguyFaceOuch[m_HurtLevel]->GetHeight()

					}
				);
			}
		}
		else
		{
			m_pDoomguyFaceNormal[FaceDisplayIndex]->Draw(
				Rectf{ 0.f,
				0.f,
				m_pDoomguyFaceNormal[FaceDisplayIndex]->GetWidth(),
				m_pDoomguyFaceNormal[FaceDisplayIndex]->GetHeight()

				}
			);
		}
		
	}
	glPopMatrix(); // goe out of imge space
	
	
	DrawRedNumbers();
		
	DrawArms();

	DrawAmo();

	glPushMatrix(); // adds a clean matrix for the image to exist in
	{
		glTranslatef(0.f, 0.f, 0.f); //moves the image space 
		glScalef(ratio, ratio, 0.f);
		glRotatef(0.f, 0.f, 0.f, 1.f);
		m_pBasisUI->Draw(
			Rectf{ 0.f,
			0.f,
			m_pBasisUI->GetWidth(),
			m_pBasisUI->GetHeight()
			}
		);

		
	}
	glPopMatrix(); // goe out of imge space

	s_pPlayer->m_WeaponManager.DrawWeapon(r_Window.width);

}

void UI::DrawRedNumbers() const
{
	const float mainStatBottomOffset{ 0.04f };

	const float mainStatScale{ 0.0023f };

	const float amoLeftOffset{ -0.003f };

	const float healthLeftOffset{ 0.145f };
	const float armourLeftOffset{ 0.408f };
	glPushMatrix();
	{
		glTranslatef(r_Window.width * amoLeftOffset, r_Window.width * mainStatBottomOffset, 0.f);

		glPushMatrix();
		{
			glScalef(r_Window.width * mainStatScale, r_Window.width * mainStatScale, 0.f);
			PrintNumber(s_pPlayer->m_WeaponManager.GetAmo(), false, 1);
		}
		glPopMatrix();

		glTranslatef(r_Window.width * healthLeftOffset, 0, 0.f);
		glPushMatrix();
		{
			glScalef(r_Window.width * mainStatScale, r_Window.width * mainStatScale, 0.f);
			PrintNumber(s_pPlayer->m_HealthManager.GetHealth(), true, 1);
		}
		glPopMatrix();


		glTranslatef(r_Window.width * armourLeftOffset, 0, 0.f);
		glPushMatrix();
		{
			glScalef(r_Window.width * mainStatScale, r_Window.width * mainStatScale, 0.f);
			PrintNumber(s_pPlayer->m_HealthManager.GetArmourHealth(), true, 1);
		}
		glPopMatrix();
	}
	glPopMatrix();

}

void UI::DrawArms() const
{
	glPushMatrix();
	{
		const float armsNumScale{ 0.0031f };
		//get to the panel 
		glTranslatef(r_Window.width * 0.323f, r_Window.width * 0.00f, 0.f);
		glScalef(r_Window.width * armsNumScale, r_Window.width * armsNumScale, 0.f); //TODO:: magic number spotted

		int count{ 2 };
		int pickedUpArms{ s_pPlayer->m_WeaponManager.GetPickedUpWeaponsNum() }; // goes up when the next weapon progression is picked up (curently just shotgun)
		glPushMatrix();
		{
			//move to the startPos of the numbers
			glTranslatef(7, 22, 0.f);
			//rows
			for (int row{ 0 }; row < 2; row++)
			{
				glPushMatrix();
				{
					glTranslatef(0.f, -row * 10.f, 0.f);
					//colums
					for (int collum{ 0 }; collum < 3; collum++)
					{
						glPushMatrix();
						{
							glTranslatef(collum * 12.f, 0.f, 0.f);
							if (count <= pickedUpArms)
							{
								m_pNumbersYellow[count]->Draw(
									Rectf{ 0.f,
									0.f,
									m_pNumbersYellow[count]->GetWidth(),
									m_pNumbersYellow[count]->GetHeight()

									}
								);
							}
							else
							{
								m_pNumbersGreyShadow[count]->Draw(
									Rectf{ 0.f,
									0.f,
									m_pNumbersGreyShadow[count]->GetWidth(),
									m_pNumbersGreyShadow[count]->GetHeight()

									}
								);
							}

						}
						glPopMatrix();
						count++;
					}
				}
				glPopMatrix();
			}

		}
		glPopMatrix();
		m_pBasisArms->Draw(
			Rectf{ 0.f,
				0.f,
				m_pBasisArms->GetWidth(),
				m_pBasisArms->GetHeight()

			}
		);
	}
	glPopMatrix();
}

void UI::DrawAmo() const
{
	const int AmoTexSwitch{ 4 };
	glPushMatrix();
	{
		// move to available amo start pos
		const float armsNumScale{ 0.0031f };
		glTranslatef(r_Window.width * 0.85f, r_Window.width * 0.066f, 0.f);
		glScalef(r_Window.width * armsNumScale, r_Window.width * armsNumScale, 0.f); //TODO:: magic number again

		const float rowOffset{ -r_Window.width * 0.007f };

		glPushMatrix();
		{
			// print available amo, only pistol and shotgun are implemented
			PrintNumber(s_pPlayer->m_WeaponManager.GetPistolAmo(), false, 4);
			glTranslatef(0, rowOffset, 0.f);
			PrintNumber(s_pPlayer->m_WeaponManager.GetShotgunAmo(), false, 4);
			glTranslatef(0, rowOffset, 0.f);
			PrintNumber(0, false, AmoTexSwitch);
			glTranslatef(0, rowOffset, 0.f);
			PrintNumber(0, false, AmoTexSwitch);
		}
		glPopMatrix();

		// move to max amo start pos
		glTranslatef(r_Window.width * 0.025f, 0, 0.f);
		glPushMatrix();
		{
			//print max amo vals, again, just pistol and shotgun are curntly implemented
			PrintNumber(s_pPlayer->m_WeaponManager.GetMaxPistolAmo(), false, 4);
			glTranslatef(0, rowOffset, 0.f);
			PrintNumber(s_pPlayer->m_WeaponManager.GetMaxShotgunAmo(), false, 4);
			glTranslatef(0, rowOffset, 0.f);
			PrintNumber(50, false, AmoTexSwitch);
			glTranslatef(0, rowOffset, 0.f);
			PrintNumber(300, false, AmoTexSwitch);
		}
		glPopMatrix();
	}
	glPopMatrix();
}


/// <summary>
/// gets the individual digits of number based on "index" where index is for example: 100 -> get the degit representing the hunderds
/// </summary>
/// <param name="number"></param>
/// <param name="index"></param>
/// <returns></returns>
int UI::GetDigit(int number, int index) const {
	int devisor{ number / index };
	if (devisor >= 10)
	{
		return devisor % 10;
	}
	return devisor;
}

void UI::PrintNumber(int number, bool percent, int numberSwitch) const
{
	float numOffset{};
	bool numStart{ false };
	
	
	for (int devisor{ 100 }; devisor > 0; devisor /= 10)
	{
		int index{ GetDigit(number, devisor) };
		if (index > 0 || devisor < 10)
		{
			numStart = true;
		}
		switch (numberSwitch)
		{
		case 1:
			numOffset += m_pNumbersRed[index]->GetWidth();
			break;
		case 2:
			numOffset += m_pNumbersGreyFlat[index]->GetWidth();
			break;
		case 3:
			numOffset += m_pNumbersGreyShadow[index]->GetWidth();
			break;
		case 4:
			numOffset += m_pNumbersYellow[index]->GetWidth();
			break;
		default:
			break;
		}
		if (numStart)
		{
			switch (numberSwitch)
			{
			case 1:
				m_pNumbersRed[index]->Draw(
					Rectf{ numOffset,
					0.f,
					m_pNumbersRed[index]->GetWidth(),
					m_pNumbersRed[index]->GetHeight()
					}
				);
				break;
			case 2:
				m_pNumbersGreyFlat[index]->Draw(
					Rectf{ numOffset,
					0.f,
					m_pNumbersGreyFlat[index]->GetWidth(),
					m_pNumbersGreyFlat[index]->GetHeight()
					}
				);
				break;
			case 3:
				m_pNumbersGreyShadow[index]->Draw(
					Rectf{ numOffset,
					0.f,
					m_pNumbersGreyShadow[index]->GetWidth(),
					m_pNumbersGreyShadow[index]->GetHeight()
					}
				);
				break;
			case 4:
				m_pNumbersYellow[index]->Draw(
					Rectf{ numOffset,
					0.f,
					m_pNumbersYellow[index]->GetWidth(),
					m_pNumbersYellow[index]->GetHeight()
					}
				);
				break;
			default:
				break;
			}
		}
		
	}
	if (percent)
	{
		numOffset += m_pNumbersRed[10]->GetWidth();
		m_pNumbersRed[10]->Draw(
			Rectf{ numOffset,
			0.f,
			m_pNumbersRed[10]->GetWidth(),
			m_pNumbersRed[10]->GetHeight()
			}
		);
	}
}

