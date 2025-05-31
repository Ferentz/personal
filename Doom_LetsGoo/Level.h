#pragma once
#include "BaseGame.h"
#include <vector>
#include "Texture.h"
#include "Utils.h"
#include "structs.h"
#include <set>

#include "SoundEffect.h"
#include "SoundStream.h"

class Player;
class Plane;
class PlaneIregular;
class Thing;
class Enemy;
class Level final
{
public:
	Level(Player& player);
	Level(const Level& toCopy) = delete;
	Level(Level&& toMove) noexcept;
	Level& operator=(const Level& rhs) = delete;
	Level& operator=(Level&& rhs) noexcept;
	~Level();

	void DrawLevelBSP(int leaf, Vector2f pos);
	void UpdateLevel( Player& eyeObject, float elapsedSec);
	int ColideWLevel(const Vector3f& pos, Vector3f& nextPos, float& floorHeight, const float charDiameter,
		bool isPlayer, bool& exit)const;
	const std::vector<BSPnode>& GetBSPtree() const;
	std::vector <Thing*>& GetThings();
	const std::vector <Plane*>& GetWalls();
	bool GetCleared();
	void SpawnWeapon(point2D_3D Location);
	SoundStream m_LevelMusic{
		"sounds/D_E1M1.mid"
	};
	

private:
	void DrawLeaf(int leafIndex);
	void UpdateThings(Player& eyeObject, float elapsedSec);
	void SwapThingSubsector(const int originalThingSubSector, const int subSector2, const int ThingLevelIndex);
	float GetFloorOfLeaf(const int leaf) const;
	int GetLeafWallAmount(const int leaf) const;
	Plane* GetWallFromLeaf(int leaf, int wallIdx) const;
	bool TriggerDoor(const int leaf, const int wallIdx) const;

	bool m_isCleared{false};
	//added vars
	Texture* m_pThingTexture{};
	std::vector<Texture> m_Textures{};
	std::map<std::string, int> m_NameMap{};

	std::vector<BSPnode> m_BSP{};
	std::vector<BSPleafeNode> m_BSPleafs{};
	int m_PlaneAmount{}; //remove
	std::vector <Plane*> m_pPlanes{};
	// so now, 
	int m_floorsNCeilingsAmount{}; // remove
	std::vector <PlaneIregular> m_pFloorsNCeilings{};
	std::vector <Thing*> m_pThings{};

	// we need a vector to store what line a wall belongs to
	std::vector<int> m_LinePerWall;
	std::set<int> m_tags[2]; // name unlear

	int m_EnemyToDraw{};
	
	SoundEffect m_Door{
		"sounds/DSDOROPN.wav"
	};

};

