#pragma once
class HealthManager final
{
public:
	explicit HealthManager(int startingHealth);
	
	void Heal(int healAmount);
	void Harm(int damage);

	int GetHealth() const;
	int GetFullHealth() const;
	int GetArmourHealth() const;

	void FullHealth();
	void SetArmourAbsorption(float damageAbsorption);
	
	void IncreaseMaxArmourHealth(int amount);
	void IncreaseMaxHealth(int amount);

private:
	int m_FullHealth{100};
	int m_Health;
	int m_MaxHealth{ m_FullHealth };

	float m_Armour_DamageAbsorption{0};
	int m_ArmourHealth{0};
	int m_MaxArmour{100};

};

