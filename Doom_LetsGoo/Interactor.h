#pragma once
class PLayer;

class Interactor
{
public:
	Interactor();
	virtual ~Interactor();
	virtual void Interact() = 0;
	static void AssignPlayer(PLayer& player);
private:
};

