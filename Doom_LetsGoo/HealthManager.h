#pragma once
class HealthManager
{
public:
	HealthManager(int startingHealth);
	void Heal(int healAmount);
	void Harm(int damage);
	int GetHeath() const;
	void FullHealth();
private:
	int m_Health;
	int m_FullHealth;

};

