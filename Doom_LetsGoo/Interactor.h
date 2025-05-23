#pragma once
class Player;

class Interactor
{
public:
	Interactor();
	virtual ~Interactor() = default;
	Interactor(const Interactor& toCopy) = default;
	Interactor(Interactor&& toMove) = default;
	Interactor& operator=(const Interactor& rhs) = default;
	Interactor& operator=(Interactor&& rhs) = default;
	virtual void Interact() = 0;
	static void AssignPlayer(Player& player);
private:
};

