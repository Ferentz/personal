#pragma once
#include "Interactor.h"
class HealthManager;

class HealthInteractor final : public Interactor
{
public:
	HealthInteractor(float healAmount, bool affectsArmour = false, bool affectsMax = false);
	virtual ~HealthInteractor() override;
	virtual void Interact() override;
	static void AssignPlayer(HealthManager& playerHealth);
private:
	float m_HealAmount;
	bool m_AffectsArmour;
	bool m_AffectsMax;
	static HealthManager* m_pPlayerHealth;
};

