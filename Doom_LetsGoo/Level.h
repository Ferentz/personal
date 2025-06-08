#pragma once
#include "BaseGame.h"
#include <vector>

#include "SoundStream.h"

class Player;
class Plane;
class PlaneIregular;
class Thing;
class Texture;

class Level final
{
public:
	explicit Level(Player& player);
	~Level();

	//rule of 5
	
	Level(Level&& toMove) noexcept;
	Level& operator=(Level&& rhs) noexcept;

	Level(const Level& toCopy) = delete; // there should only be one instance of a given level + the level's are created by the level generator, which can be called again if this is desired
	Level& operator=(const Level& rhs) = delete;
	

	//basics
	void DrawLevel(Vector2f pos);
	void UpdateLevel( Player& eyeObject, float elapsedSec);
	
	//collision
	void ColideWLevel(const Vector3f& pos, Vector3f& nextPos,
		float& floorHeight, const float charDiameter,
		const bool ignoreLedges, bool& foundExit) const;

	//interactions
	const std::vector <Thing*>& GetThings() const;
	const std::vector <Plane*>& GetWalls() const;
	bool GetCleared() const;
	void SpawnWeapon(point2D_3D Location);

	
	SoundStream m_LevelMusic{
		"sounds/D_E1M1.mid"
	};
	
	

private:
	//rule of 5 helper
	void MoveLevelMembers(Level&& toMove) noexcept;

	//basics helpers
	void DrawLevelBSP(int leaf, Vector2f pos);
	void DrawLeaf(int leafIndex);
	void UpdateThings(Player& eyeObject, float elapsedSec);
	void SwapThingSubsector(const int originalThingSubSector, const int subSector2, const int ThingLevelIndex);
	const std::vector<BSPnode>& GetBSPtree() const;

	//collision helpers
	float GetFloorOfLeaf(const int leaf) const;
	int GetLeafWallAmount(const int leaf) const;
	Plane* GetWallFromLeaf(int leaf, int wallIdx) const;
	bool TriggerDoor(const int leaf, const int wallIdx) const;


	bool m_isCleared{false};

	Texture* m_pThingTexture{}; //temp untill Thing class is finalized   TODO:: include texture with Thing class
	
	std::vector<BSPnode> m_BSP{};
	std::vector<BSPleafeNode> m_BSPleafs{};
	std::vector <Plane*> m_pPlanes{};
	std::vector <PlaneIregular> m_pFloorsNCeilings{};
	std::vector <Thing*> m_pThings{};
	std::vector<Texture> m_Textures{};
	
	
};

