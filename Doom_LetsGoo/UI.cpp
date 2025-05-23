#include "pch.h"
#include "UI.h"
#include "Texture.h"
#include "Player.h"

Player* UI::s_pPlayer{};

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
	}
	for (int index{}; index < 10; index++)
	{
		delete m_pNumbersGreyFlat[index];
		delete m_pNumbersGreyShadow[index];
		delete m_pNumbersYellow[index];
	}
	delete m_pUI;

}

void UI::DrawUI(const float width, float bottomOfset, float leftOffset, float scale,
	int health, int armor, int amo)
{
	//float width{ GetWindow().width };
	float uiWidth{ m_pUI->GetWidth() };
	float ratio{ m_Window.width / uiWidth };

	//Rectf hi{ 0, 0, width, m_UI->GetHeight() / ratio };

	glPushMatrix(); // adds a clean matrix for the image to exist in
	{
		DrawRedNumbers();
		
		DrawArms();

		DrawAmo();


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
}

void UI::AssignPlayer(Player* pPlayer)
{
	s_pPlayer = pPlayer;
}

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
	glPushMatrix(); // adds a clean matrix for the image to exist in
	{
		//float numOffset{};
		glTranslatef(m_Window.width * m_AmoLeftOffset, m_Window.width * m_mainStatBottomOffset, 0.f);
		glRotatef(0.f, 0.f, 0.f, 1.f);

		glPushMatrix(); // adds a clean matrix for the image to exist in
		{
			glScalef(m_Window.width * m_MainStatScale, m_Window.width * m_MainStatScale, 0.f);
			PrintNumber(s_pPlayer->m_WeaponManager.GetAmo(), false, 1);
		}
		glPopMatrix();

		glTranslatef(m_Window.width * m_HealthLeftOffset, 0, 0.f); //moves the image space 
		glPushMatrix(); // adds a clean matrix for the image to exist in
		{
			glScalef(m_Window.width * m_MainStatScale, m_Window.width * m_MainStatScale, 0.f);
			PrintNumber(s_pPlayer->m_HealthManager.GetHeath(), true, 1);
		}
		glPopMatrix();


		glTranslatef(m_Window.width * m_ArmourLeftOffset, 0, 0.f);
		glPushMatrix(); // adds a clean matrix for the image to exist in
		{
			glScalef(m_Window.width * m_MainStatScale, m_Window.width * m_MainStatScale, 0.f);
			PrintNumber(s_pPlayer->m_HealthManager.GetArmour(), true, 1);
		}
		glPopMatrix();


	}
	glPopMatrix(); // goe out of imge space

}

void UI::DrawArms()
{
	glPushMatrix(); 
	{
		glTranslatef(m_Window.width * 0.323f, m_Window.width * 0.00f, 0.f);
		glRotatef(0.f, 0.f, 0.f, 1.f);

		glScalef(m_Window.width * 0.0031, m_Window.width * 0.0031, 0.f);
		int count{ 2 };
		int pickedUpArms{ s_pPlayer->m_WeaponManager.GetPickedUpWeaponsNum()};
		glPushMatrix();
		{
			glTranslatef(7, 22, 0.f);
			for (int i{ 0 }; i < 2; i++)
			{
			
				glPushMatrix();
				{
					glTranslatef( 0, - i * 10, 0.f);
					for (int j{ 0 }; j < 3; j++)
					{
						glPushMatrix();
						{
							glTranslatef(j * 12, 0.f,  0.f);
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
	glPushMatrix();
	{
		glTranslatef(m_Window.width * 0.85f, m_Window.width * 0.066f, 0.f);
		glRotatef(0.f, 0.f, 0.f, 1.f);
		glScalef(m_Window.width * 0.0031, m_Window.width * 0.0031, 0.f);

		const float rowOffset{ -m_Window.width * 0.007f };
		PrintNumber(s_pPlayer->m_WeaponManager.GetPistolAmo(), false, 4);
		glPushMatrix();
		{
			glTranslatef(0, rowOffset, 0.f);
			PrintNumber(s_pPlayer->m_WeaponManager.GetShotgunAmo(), false, 4);
			glTranslatef(0, rowOffset, 0.f);
			PrintNumber(0, false, 4);
			glTranslatef(0, rowOffset, 0.f);
			PrintNumber(0, false, 4);
		}
		glPopMatrix();
		glTranslatef(m_Window.width * 0.025f, 0, 0.f);
		PrintNumber(s_pPlayer->m_WeaponManager.GetMaxPistolAmo(), false, 4);
		glPushMatrix();
		{
			glTranslatef(0, rowOffset, 0.f);
			PrintNumber(s_pPlayer->m_WeaponManager.GetMaxShotgunAmo(), false, 4);
			glTranslatef(0, rowOffset, 0.f);
			PrintNumber(50, false, 4);
			glTranslatef(0, rowOffset, 0.f);
			PrintNumber(300, false, 4);
		}
		glPopMatrix();
	}
	glPopMatrix();
}
