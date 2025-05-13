#include "pch.h"
#include "UI.h"
#include "Texture.h"

UI::UI()
	:m_pUI{ new Texture{"STBAR_converted.png"} },
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
	}
{
}

UI::~UI()
{
	for (int index{}; index < 12; index++)
	{
		delete m_pNumbers[index];
	}
	delete m_pUI;

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

			numOffset -= m_pNumbers[10]->GetWidth();
			m_pNumbers[10]->Draw(
				Rectf{ numOffset,
				0.f,
				m_pNumbers[10]->GetWidth(),
				m_pNumbers[10]->GetHeight()
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