#pragma once
class HealthManager final
{
public:
	HealthManager(int startingHealth);
	void Heal(int healAmount);
	void Harm(int damage);
	int GetHeath() const;
	int GetArmour() const;
	void FullHealth();
	void SetArmour(float damageAbsorption);
	void IncreaseMaxArmour(int amount);
	void IncreaseMaxHealth(int amount);

	HealthManager& operator=(const HealthManager& rhs);
private:
	int m_Health;
	int m_FullHealth{100};
	int m_MaxHealth{100};

	float m_Armour_DamageAbsorption{0};
	int m_ArmourHealth{0};
	int m_MaxArmour{100};

};

