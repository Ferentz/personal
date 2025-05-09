#include "pch.h"
#include "UI.h"
#include "Texture.h"

UI::UI()
	:m_pUI{ new Texture{"STBAR_converted.png"} },
	//numRed/STTMINUS.png
	m_pStripe{ new Texture{"numRed/STTMINUS.png"} },
	m_pNumbers{ new Texture{"numRed/STTNUM0.png"} ,
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
	m_pOne{ new Texture{"numRed/STTNUM1.png"} },
	m_pTwo{ new Texture{"numRed/STTNUM2.png"} },
	m_pThree{ new Texture{"numRed/STTNUM3.png"} },
	m_pFour{ new Texture{"numRed/STTNUM4.png"} },
	m_pFive{ new Texture{"numRed/STTNUM5.png"} },
	m_pSix{ new Texture{"numRed/STTNUM6.png"} },
	m_pSeven{ new Texture{"numRed/STTNUM7.png"} },
	m_pEight{ new Texture{"numRed/STTNUM8.png"} },
	m_pNine{ new Texture{"numRed/STTNUM9.png"} },
	m_pPercent{ new Texture{"numRed/STTPRCNT.png"} }
{
}

UI::~UI()
{
	for (int index{}; index < 12; index++)
	{
		delete m_pNumbers[index];
	}
	delete m_pUI;
	delete m_pStripe;
	//delete m_pZero;
	delete m_pOne;
	delete m_pTwo;
	delete m_pThree;
	delete m_pFour;
	delete m_pFive;
	delete m_pSix;
	delete m_pSeven;
	delete m_pEight;
	delete m_pNine;
	delete m_pPercent;
}

void UI::DrawUI(const float width, float bottomOfset, float leftOffset, float scale, int health)
{
	//float width{ GetWindow().width };
	float uiWidth{ m_pUI->GetWidth() };
	float ratio{ width / uiWidth };

	//Rectf hi{ 0, 0, width, m_UI->GetHeight() / ratio };

	glPushMatrix(); // adds a clean matrix for the image to exist in
	{
		glPushMatrix(); // adds a clean matrix for the image to exist in
		{
			float numOffset{};
			glTranslatef(leftOffset, bottomOfset, 0.f); //moves the image space 
			glScalef(scale, scale, 0.f);
			glRotatef(0.f, 0.f, 0.f, 1.f);

			numOffset -= m_pPercent->GetWidth();
			m_pPercent->Draw(
				Rectf{ numOffset,
				0.f,
				m_pPercent->GetWidth(),
				m_pPercent->GetHeight()
				}
			);
			for (int devisor{ 1 }; devisor <= 150; devisor *= 10)
			{
				int index{ GetDigit(health, devisor) };
				//m_pNumbers[index]
				numOffset -= m_pNumbers[index]->GetWidth();
				m_pNumbers[index]->Draw(
					Rectf{ numOffset,
					0.f,
					m_pNumbers[index]->GetWidth(),
					m_pNumbers[index]->GetHeight()
					}
				);
			}
			//int index{ GetDigit(health, 1) };
			////m_pNumbers[index]
			//numOffset -= m_pNumbers[index]->GetWidth();
			//m_pNumbers[index]->Draw(
			//	Rectf{ numOffset,
			//	0.f,
			//	m_pNumbers[index]->GetWidth(),
			//	m_pNumbers[index]->GetHeight()
			//	}
			//);
			//numOffset -= m_pZero->GetWidth();
			//m_pZero->Draw(
			//	Rectf{ numOffset,
			//	0.f,
			//	m_pZero->GetWidth(),
			//	m_pZero->GetHeight()
			//	}
			//);
			//numOffset -= m_pZero->GetWidth();
			//m_pZero->Draw(
			//	Rectf{ numOffset,
			//	0.f,
			//	m_pZero->GetWidth(),
			//	m_pZero->GetHeight()
			//	}
			//);
		}
		glPopMatrix(); // goe out of imge space


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


int UI::GetDigit(int number, int index) {
	int devisor{ number / index };
	if (devisor >= 10)
	{
		return devisor % 10;
	}
	return devisor;
}