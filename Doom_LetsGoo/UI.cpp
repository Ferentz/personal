#include "pch.h"
#include "UI.h"
#include "Texture.h"
#include "Player.h"

const Player*  UI::s_pPlayer{};

UI::UI(const Rectf& window)
	:m_pUI{ new Texture{"STBAR_converted.png"} },
	m_pNumbersRed{ new Texture{"numRed/STTNUM0.png"} ,
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
	m_pNumbersGreyFlat{ new Texture{"numGreyFlat/AMMNUM0.png"} ,
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
	m_pNumbersGreyShadow{ new Texture{"numGreyShadow/STGNUM0.png"} ,
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
	m_pNumbersYellow{ new Texture{"numYellow/STYSNUM0.png"} ,
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
	m_pArmsBase{new Texture{"UIBits/STARMS.png"}},
	m_Window{ window }
{
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
	delete m_pArmsBase;
	m_pArmsBase = nullptr;
	delete m_pUI;
	m_pUI = nullptr;

}

void UI::DrawUI(const float width,
	int health, int armor, int amo)
{
	//float width{ GetWindow().width };
	float uiWidth{ m_pUI->GetWidth() };
	float ratio{ m_Window.width / uiWidth };

	//Rectf hi{ 0, 0, width, m_UI->GetHeight() / ratio };

	
	DrawRedNumbers();
		
	DrawArms();

	DrawAmo();

	glPushMatrix(); // adds a clean matrix for the image to exist in
	{
		glTranslatef(0.f, 0.f, 0.f); //moves the image space 
		glScalef(ratio, ratio, 0.f);
		glRotatef(0.f, 0.f, 0.f, 1.f);
		m_pUI->Draw(
			Rectf{ 0.f,
			0.f,
			m_pUI->GetWidth(),
			m_pUI->GetHeight()
			}
		);

		
	}
	glPopMatrix(); // goe out of imge space

	s_pPlayer->m_WeaponManager.DrawWeapon(m_Window.width);

}

void UI::AssignPlayer(Player* pPlayer)
{
	s_pPlayer = pPlayer;
}

/// <summary>
/// gets the individual digits of number based on "index" where index is for example: 100 -> get the degit representing the hunderds
/// </summary>
/// <param name="number"></param>
/// <param name="index"></param>
/// <returns></returns>
int UI::GetDigit(int number, int index) {
	int devisor{ number / index };
	if (devisor >= 10)
	{
		return devisor % 10;
	}
	return devisor;
}

void UI::PrintNumber(int number, bool percent, int numberSwitch)
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

void UI::DrawRedNumbers()
{
	const float mainStatBottomOffset{ 0.04f };

	const float mainStatScale{ 0.0023f };

	const float amoLeftOffset{ -0.003f };

	const float healthLeftOffset{ 0.145f };
	const float armourLeftOffset{ 0.408f };
	glPushMatrix(); 
	{
		glTranslatef(m_Window.width * amoLeftOffset, m_Window.width * mainStatBottomOffset, 0.f);

		glPushMatrix(); 
		{
			glScalef(m_Window.width * mainStatScale, m_Window.width * mainStatScale, 0.f);
			PrintNumber(s_pPlayer->m_WeaponManager.GetAmo(), false, 1);
		}
		glPopMatrix();

		glTranslatef(m_Window.width * healthLeftOffset, 0, 0.f);  
		glPushMatrix(); 
		{
			glScalef(m_Window.width * mainStatScale, m_Window.width * mainStatScale, 0.f);
			PrintNumber(s_pPlayer->m_HealthManager.GetHealth(), true, 1);
		}
		glPopMatrix();


		glTranslatef(m_Window.width * armourLeftOffset, 0, 0.f);
		glPushMatrix(); 
		{
			glScalef(m_Window.width * mainStatScale, m_Window.width * mainStatScale, 0.f);
			PrintNumber(s_pPlayer->m_HealthManager.GetArmourHealth(), true, 1);
		}
		glPopMatrix();
	}
	glPopMatrix(); 

}

void UI::DrawArms()
{
	glPushMatrix(); 
	{
		const float armsNumScale{ 0.0031f };
		//get to the panel 
		glTranslatef(m_Window.width * 0.323f, m_Window.width * 0.00f, 0.f);
		glScalef(m_Window.width * armsNumScale, m_Window.width * armsNumScale, 0.f); //TODO:: magic number spotted
		
		int count{ 2 };
		int pickedUpArms{ s_pPlayer->m_WeaponManager.GetPickedUpWeaponsNum()}; // goes up when the next weapon progression is picked up (curently just shotgun)
		glPushMatrix();
		{
			//move to the startPos of the numbers
			glTranslatef(7, 22, 0.f);
			//rows
			for (int row{ 0 }; row < 2; row++)
			{
				glPushMatrix();
				{
					glTranslatef( 0.f, - row * 10.f, 0.f);
					//colums
					for (int collum{ 0 }; collum < 3; collum++)
					{
						glPushMatrix();
						{
							glTranslatef(collum * 12.f, 0.f,  0.f);
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
		m_pArmsBase->Draw(
			Rectf{ 0.f,
				0.f,
				m_pArmsBase->GetWidth(),
				m_pArmsBase->GetHeight()

			}
		);
	}
	glPopMatrix();
}

void UI::DrawAmo()
{
	const int AmoTexSwitch{ 4 };
	glPushMatrix();
	{
		// move to available amo start pos
		const float armsNumScale{ 0.0031f };
		glTranslatef(m_Window.width * 0.85f, m_Window.width * 0.066f, 0.f);
		glScalef(m_Window.width * armsNumScale, m_Window.width * armsNumScale, 0.f); //TODO:: magic number again

		const float rowOffset{ -m_Window.width * 0.007f };
		
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
		glTranslatef(m_Window.width * 0.025f, 0, 0.f);
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
