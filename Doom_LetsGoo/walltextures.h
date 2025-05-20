#pragma once
#include <vector>
#include <string>
struct Patch {
	Patch(std::string name, int x, int y) {};
};
struct WallTexture {
	WallTexture(std::string name, int x, int y) {}
	std::vector<Patch> patches{};
};

void WallTextureData() {

	std::vector<WallTexture> walltextures{};
	walltextures.reserve(150);


	walltextures.emplace_back(WallTexture{ "AASTINKY", 24, 72 });

	walltextures.back().patches.emplace_back(Patch{ "WALL00_3", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL00_3", 12, -6 });


	walltextures.emplace_back(WallTexture{ "BIGDOOR1", 128, 96 });

	walltextures.back().patches.emplace_back(Patch{ "W13_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "W13_1", 0, 24 });
	walltextures.back().patches.emplace_back(Patch{ "DOOR2_1", 17, 0 });
	walltextures.back().patches.emplace_back(Patch{ "W13_1", 113, 0 });
	walltextures.back().patches.emplace_back(Patch{ "W13_1", 113, 25 });


	walltextures.emplace_back(WallTexture{ "BIGDOOR2", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "DOOR2_4", 0, 0 });


	walltextures.emplace_back(WallTexture{ "BIGDOOR4", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "DOOR9_1", 0, 0 });


	walltextures.emplace_back(WallTexture{ "BRNBIGC", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "W113_1", 0, 0 });


	walltextures.emplace_back(WallTexture{ "BRNBIGL", 32, 128 });

	walltextures.back().patches.emplace_back(Patch{ "W113_2", 0, 0 });


	walltextures.emplace_back(WallTexture{ "BRNBIGR", 32, 128 });

	walltextures.back().patches.emplace_back(Patch{ "W113_3", 0, 0 });


	walltextures.emplace_back(WallTexture{ "BRNPOIS", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL62_2", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "PS20A0", 1, 60 });


	walltextures.emplace_back(WallTexture{ "BRNPOIS2", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL62_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "PS20A0", 0, 57 });


	walltextures.emplace_back(WallTexture{ "BRNSMAL1", 64, 64 });

	walltextures.back().patches.emplace_back(Patch{ "W111_2", 0, 0 });


	walltextures.emplace_back(WallTexture{ "BRNSMAL2", 64, 64 });

	walltextures.back().patches.emplace_back(Patch{ "W111_3", 0, 0 });


	walltextures.emplace_back(WallTexture{ "BRNSMALC", 64, 64 });

	walltextures.back().patches.emplace_back(Patch{ "W112_1", 0, 0 });


	walltextures.emplace_back(WallTexture{ "BRNSMALL", 32, 64 });

	walltextures.back().patches.emplace_back(Patch{ "W112_3", 0, 0 });


	walltextures.emplace_back(WallTexture{ "BRNSMALR", 32, 64 });

	walltextures.back().patches.emplace_back(Patch{ "W112_2", 0, 0 });


	walltextures.emplace_back(WallTexture{ "BROWN1", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL02_2", 0, 56 });
	walltextures.back().patches.emplace_back(Patch{ "WALL02_2", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL02_3", 64, 56 });
	walltextures.back().patches.emplace_back(Patch{ "WALL02_1", 88, 56 });
	walltextures.back().patches.emplace_back(Patch{ "WALL02_3", 64, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL02_1", 88, 0 });


	walltextures.emplace_back(WallTexture{ "BROWN144", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL00_5", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL00_6", 16, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL00_7", 32, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL00_8", 48, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL00_6", 64, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL00_6", 112, -16 });
	walltextures.back().patches.emplace_back(Patch{ "WALL00_7", 96, -1 });
	walltextures.back().patches.emplace_back(Patch{ "WALL00_6", 80, 0 });


	walltextures.emplace_back(WallTexture{ "BROWN96", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL62_1", 0, 0 });


	walltextures.emplace_back(WallTexture{ "BROWNGRN", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL62_2", 0, 0 });


	walltextures.emplace_back(WallTexture{ "BROWNHUG", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL03_4", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL03_4", 0, 72 });


	walltextures.emplace_back(WallTexture{ "BROWNPIP", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "TP2_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "STEP07", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "STEP07", 32, 64 });
	walltextures.back().patches.emplace_back(Patch{ "STEP07", 64, 64 });
	walltextures.back().patches.emplace_back(Patch{ "STEP07", 96, 64 });
	walltextures.back().patches.emplace_back(Patch{ "WALL05_2", 0, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL05_2", 64, 72 });
	walltextures.back().patches.emplace_back(Patch{ "STEP07", 0, 120 });
	walltextures.back().patches.emplace_back(Patch{ "STEP07", 32, 120 });
	walltextures.back().patches.emplace_back(Patch{ "STEP07", 64, 120 });
	walltextures.back().patches.emplace_back(Patch{ "STEP07", 96, 120 });


	walltextures.emplace_back(WallTexture{ "COMP2", 256, 128 });

	walltextures.back().patches.emplace_back(Patch{ "COMP02_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "COMP02_2", 64, 0 });
	walltextures.back().patches.emplace_back(Patch{ "COMP02_4", 128, 0 });
	walltextures.back().patches.emplace_back(Patch{ "COMP02_3", 192, 0 });
	walltextures.back().patches.emplace_back(Patch{ "COMP02_6", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "COMP02_8", 64, 64 });
	walltextures.back().patches.emplace_back(Patch{ "COMP02_7", 128, 64 });
	walltextures.back().patches.emplace_back(Patch{ "COMP02_5", 192, 64 });
	walltextures.back().patches.emplace_back(Patch{ "STEP08", 0, 56 });
	walltextures.back().patches.emplace_back(Patch{ "STEP08", 32, 56 });
	walltextures.back().patches.emplace_back(Patch{ "STEP08", 64, 56 });
	walltextures.back().patches.emplace_back(Patch{ "STEP08", 96, 56 });
	walltextures.back().patches.emplace_back(Patch{ "STEP08", 128, 56 });
	walltextures.back().patches.emplace_back(Patch{ "STEP08", 160, 56 });
	walltextures.back().patches.emplace_back(Patch{ "STEP08", 192, 56 });
	walltextures.back().patches.emplace_back(Patch{ "STEP08", 224, 56 });
	walltextures.back().patches.emplace_back(Patch{ "STEP08", 0, 120 });
	walltextures.back().patches.emplace_back(Patch{ "STEP08", 32, 120 });
	walltextures.back().patches.emplace_back(Patch{ "STEP08", 64, 120 });
	walltextures.back().patches.emplace_back(Patch{ "STEP08", 96, 120 });
	walltextures.back().patches.emplace_back(Patch{ "STEP08", 128, 120 });
	walltextures.back().patches.emplace_back(Patch{ "STEP08", 160, 120 });
	walltextures.back().patches.emplace_back(Patch{ "STEP08", 192, 120 });
	walltextures.back().patches.emplace_back(Patch{ "STEP08", 224, 120 });


	walltextures.emplace_back(WallTexture{ "COMPSPAN", 32, 128 });

	walltextures.back().patches.emplace_back(Patch{ "COMP03_4", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "COMP03_4", 0, 64 });


	walltextures.emplace_back(WallTexture{ "COMPSTA1", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "TOMW2_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "AG128_2", 0, 72 });
	walltextures.back().patches.emplace_back(Patch{ "AG128_2", 64, 72 });


	walltextures.emplace_back(WallTexture{ "COMPSTA2", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "TOMW2_2", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "AG128_2", 0, 72 });
	walltextures.back().patches.emplace_back(Patch{ "AG128_2", 64, 72 });


	walltextures.emplace_back(WallTexture{ "COMPTALL", 256, 128 });

	walltextures.back().patches.emplace_back(Patch{ "COMP04_5", 32, 0 });
	walltextures.back().patches.emplace_back(Patch{ "COMP04_8", 96, 0 });
	walltextures.back().patches.emplace_back(Patch{ "COMP03_8", 160, 0 });
	walltextures.back().patches.emplace_back(Patch{ "COMP04_6", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "COMP04_7", 64, 64 });
	walltextures.back().patches.emplace_back(Patch{ "COMP04_2", 128, 64 });
	walltextures.back().patches.emplace_back(Patch{ "COMP03_5", 192, 0 });
	walltextures.back().patches.emplace_back(Patch{ "COMP04_1", 192, 64 });
	walltextures.back().patches.emplace_back(Patch{ "COMP03_8", 0, 0 });


	walltextures.emplace_back(WallTexture{ "COMPTILE", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "COMP03_7", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "COMP03_1", 32, 0 });
	walltextures.back().patches.emplace_back(Patch{ "COMP03_6", 96, 0 });
	walltextures.back().patches.emplace_back(Patch{ "COMP03_7", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "COMP03_1", 32, 64 });
	walltextures.back().patches.emplace_back(Patch{ "COMP03_6", 96, 64 });


	walltextures.emplace_back(WallTexture{ "COMPUTE1", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "COMP01_5", 26, 64 });
	walltextures.back().patches.emplace_back(Patch{ "COMP01_1", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 0, 56 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 64, 56 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 0, 120 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 64, 120 });
	walltextures.back().patches.emplace_back(Patch{ "COMP01_5", 28, 0 });
	walltextures.back().patches.emplace_back(Patch{ "COMP1C_6", 72, 64 });
	walltextures.back().patches.emplace_back(Patch{ "COMP01_6", 72, 0 });
	walltextures.back().patches.emplace_back(Patch{ "COMP1B_4", 0, 0 });


	walltextures.emplace_back(WallTexture{ "COMPUTE2", 256, 56 });

	walltextures.back().patches.emplace_back(Patch{ "COMP02_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "COMP02_2", 64, 0 });
	walltextures.back().patches.emplace_back(Patch{ "COMP02_3", 128, 0 });
	walltextures.back().patches.emplace_back(Patch{ "COMP02_7", 192, 0 });


	walltextures.emplace_back(WallTexture{ "COMPUTE3", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "SW11_4", 0, 4 });
	walltextures.back().patches.emplace_back(Patch{ "SW11_5", 32, 4 });
	walltextures.back().patches.emplace_back(Patch{ "SW11_4", 64, 4 });
	walltextures.back().patches.emplace_back(Patch{ "COMP02_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW11_5", 96, 4 });
	walltextures.back().patches.emplace_back(Patch{ "COMP02_2", 64, 0 });
	walltextures.back().patches.emplace_back(Patch{ "STEP08", 0, 56 });
	walltextures.back().patches.emplace_back(Patch{ "STEP08", 32, 56 });
	walltextures.back().patches.emplace_back(Patch{ "STEP08", 64, 56 });
	walltextures.back().patches.emplace_back(Patch{ "STEP08", 96, 56 });


	walltextures.emplace_back(WallTexture{ "DOOR1", 64, 72 });

	walltextures.back().patches.emplace_back(Patch{ "WALL03_1", 0, 0 });


	walltextures.emplace_back(WallTexture{ "DOOR3", 64, 72 });

	walltextures.back().patches.emplace_back(Patch{ "DOOR2_5", 0, 0 });


	walltextures.emplace_back(WallTexture{ "DOORBLU", 8, 128 });

	walltextures.back().patches.emplace_back(Patch{ "W46_37", 0, 112 });
	walltextures.back().patches.emplace_back(Patch{ "W46_37", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "W46_37", 0, 96 });
	walltextures.back().patches.emplace_back(Patch{ "W46_37", 0, 80 });
	walltextures.back().patches.emplace_back(Patch{ "W46_37", 0, 16 });
	walltextures.back().patches.emplace_back(Patch{ "W46_37", 0, 32 });
	walltextures.back().patches.emplace_back(Patch{ "W46_37", 0, 48 });
	walltextures.back().patches.emplace_back(Patch{ "W46_37", 0, 64 });


	walltextures.emplace_back(WallTexture{ "DOORRED", 8, 128 });

	walltextures.back().patches.emplace_back(Patch{ "W46_38", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "W46_38", 0, 16 });
	walltextures.back().patches.emplace_back(Patch{ "W46_38", 0, 32 });
	walltextures.back().patches.emplace_back(Patch{ "W46_38", 0, 48 });
	walltextures.back().patches.emplace_back(Patch{ "W46_38", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "W46_38", 0, 80 });
	walltextures.back().patches.emplace_back(Patch{ "W46_38", 0, 96 });
	walltextures.back().patches.emplace_back(Patch{ "W46_38", 0, 112 });


	walltextures.emplace_back(WallTexture{ "DOORSTOP", 8, 128 });

	walltextures.back().patches.emplace_back(Patch{ "TTALL1_2", 0, 0 });


	walltextures.emplace_back(WallTexture{ "DOORTRAK", 8, 128 });

	walltextures.back().patches.emplace_back(Patch{ "DOORTRAK", 0, 0 });


	walltextures.emplace_back(WallTexture{ "DOORYEL", 8, 128 });

	walltextures.back().patches.emplace_back(Patch{ "W46_39", 0, 112 });
	walltextures.back().patches.emplace_back(Patch{ "W46_39", 0, 80 });
	walltextures.back().patches.emplace_back(Patch{ "W46_39", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "W46_39", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "W46_39", 0, 16 });
	walltextures.back().patches.emplace_back(Patch{ "W46_39", 0, 48 });
	walltextures.back().patches.emplace_back(Patch{ "W46_39", 0, 32 });
	walltextures.back().patches.emplace_back(Patch{ "W46_39", 0, 96 });


	walltextures.emplace_back(WallTexture{ "EXITDOOR", 128, 72 });

	walltextures.back().patches.emplace_back(Patch{ "DOOR3_6", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "DOOR3_4", 64, 0 });
	walltextures.back().patches.emplace_back(Patch{ "DOOR3_5", 88, 0 });
	walltextures.back().patches.emplace_back(Patch{ "T14_5", 112, 0 });


	walltextures.emplace_back(WallTexture{ "EXITSIGN", 64, 16 });

	walltextures.back().patches.emplace_back(Patch{ "EXIT1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "EXIT2", 32, 0 });
	walltextures.back().patches.emplace_back(Patch{ "EXIT2", 40, 0 });
	walltextures.back().patches.emplace_back(Patch{ "EXIT2", 48, 0 });
	walltextures.back().patches.emplace_back(Patch{ "EXIT2", 56, 0 });


	walltextures.emplace_back(WallTexture{ "GRAY4", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "W33_5", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "W33_5", 0, 64 });


	walltextures.emplace_back(WallTexture{ "GRAY5", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "W33_7", 0, 72 });
	walltextures.back().patches.emplace_back(Patch{ "W33_7", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 0, 64 });


	walltextures.emplace_back(WallTexture{ "GRAY7", 256, 128 });

	walltextures.back().patches.emplace_back(Patch{ "W32_4", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "W32_1", 64, 0 });
	walltextures.back().patches.emplace_back(Patch{ "W32_4", 128, 0 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 0, 120 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 64, 120 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 128, 120 });
	walltextures.back().patches.emplace_back(Patch{ "W32_4", 192, 0 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 192, 120 });
	walltextures.back().patches.emplace_back(Patch{ "W32_4", 64, 56 });
	walltextures.back().patches.emplace_back(Patch{ "W32_1", 128, 56 });
	walltextures.back().patches.emplace_back(Patch{ "W32_4", 192, 56 });
	walltextures.back().patches.emplace_back(Patch{ "W32_4", 0, 56 });


	walltextures.emplace_back(WallTexture{ "GRAYTALL", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL00_1", 16, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL00_3", 80, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL00_2", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL00_1", 96, 0 });


	walltextures.emplace_back(WallTexture{ "LITE2", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL02_2", 0, 56 });
	walltextures.back().patches.emplace_back(Patch{ "WALL02_2", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "FLAMP", 5, 0 });
	walltextures.back().patches.emplace_back(Patch{ "FLAMP", 35, 0 });


	walltextures.emplace_back(WallTexture{ "LITE3", 32, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 8, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 16, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 24, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 8 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 8, 8 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 16, 8 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 24, 8 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 16 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 8, 16 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 16, 16 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 24, 16 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 24 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 8, 24 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 16, 24 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 24, 24 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 32 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 8, 32 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 16, 32 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 24, 32 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 40 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 8, 40 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 16, 40 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 24, 40 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 48 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 8, 48 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 16, 48 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 24, 48 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 56 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 8, 56 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 16, 56 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 24, 56 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 8, 64 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 16, 64 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 24, 64 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 8, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 16, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 24, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 80 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 8, 80 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 16, 80 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 24, 80 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 88 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 8, 88 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 16, 88 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 24, 88 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 96 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 8, 96 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 16, 96 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 24, 96 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 104 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 8, 104 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 16, 104 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 24, 104 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 112 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 8, 112 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 16, 112 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 24, 112 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 120 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 8, 120 });
	walltextures.back().patches.emplace_back(Patch{ "WLITB0", 16, 120 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 24, 120 });


	walltextures.emplace_back(WallTexture{ "LITE4", 16, 128 });

	walltextures.back().patches.emplace_back(Patch{ "T14_5", 0, 56 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 8, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 48 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 8, 48 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 40 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 8, 40 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 32 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 8, 32 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 24 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 8, 24 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 16 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 8, 16 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 8 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 8, 8 });


	walltextures.emplace_back(WallTexture{ "LITE5", 16, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 8, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 8 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 8, 8 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 16 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 8, 16 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 24 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 8, 24 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 32 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 8, 32 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 40 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 8, 40 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 48 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 8, 48 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 56 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 8, 56 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 8, 64 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 8, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 80 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 8, 80 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 88 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 8, 88 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 96 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 8, 96 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 104 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 8, 104 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 112 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 8, 112 });
	walltextures.back().patches.emplace_back(Patch{ "WLITA0", 0, 120 });
	walltextures.back().patches.emplace_back(Patch{ "WLITC0", 8, 120 });


	walltextures.emplace_back(WallTexture{ "LITEBLU1", 8, 128 });

	walltextures.back().patches.emplace_back(Patch{ "AGB128_1", 0, 0 });


	walltextures.emplace_back(WallTexture{ "LITEBLU2", 16, 72 });

	walltextures.back().patches.emplace_back(Patch{ "W13_A", 0, 0 });


	walltextures.emplace_back(WallTexture{ "LITEBLU3", 16, 128 });

	walltextures.back().patches.emplace_back(Patch{ "W13_8", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "W13_8", 0, 56 });


	walltextures.emplace_back(WallTexture{ "LITEBLU4", 16, 128 });

	walltextures.back().patches.emplace_back(Patch{ "BLITA0", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "BLITB0", 8, 0 });
	walltextures.back().patches.emplace_back(Patch{ "BLITC0", 8, 0 });
	walltextures.back().patches.emplace_back(Patch{ "BLITA0", 0, 8 });
	walltextures.back().patches.emplace_back(Patch{ "BLITC0", 8, 8 });
	walltextures.back().patches.emplace_back(Patch{ "BLITA0", 0, 16 });
	walltextures.back().patches.emplace_back(Patch{ "BLITC0", 8, 16 });
	walltextures.back().patches.emplace_back(Patch{ "BLITA0", 0, 24 });
	walltextures.back().patches.emplace_back(Patch{ "BLITC0", 8, 24 });
	walltextures.back().patches.emplace_back(Patch{ "BLITA0", 0, 32 });
	walltextures.back().patches.emplace_back(Patch{ "BLITC0", 8, 32 });
	walltextures.back().patches.emplace_back(Patch{ "BLITA0", 0, 40 });
	walltextures.back().patches.emplace_back(Patch{ "BLITC0", 8, 40 });
	walltextures.back().patches.emplace_back(Patch{ "BLITA0", 0, 48 });
	walltextures.back().patches.emplace_back(Patch{ "BLITC0", 8, 48 });
	walltextures.back().patches.emplace_back(Patch{ "BLITA0", 0, 56 });
	walltextures.back().patches.emplace_back(Patch{ "BLITC0", 8, 56 });
	walltextures.back().patches.emplace_back(Patch{ "BLITA0", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "BLITC0", 8, 64 });
	walltextures.back().patches.emplace_back(Patch{ "BLITA0", 0, 72 });
	walltextures.back().patches.emplace_back(Patch{ "BLITC0", 8, 72 });
	walltextures.back().patches.emplace_back(Patch{ "BLITA0", 0, 80 });
	walltextures.back().patches.emplace_back(Patch{ "BLITC0", 8, 80 });
	walltextures.back().patches.emplace_back(Patch{ "BLITA0", 0, 88 });
	walltextures.back().patches.emplace_back(Patch{ "BLITC0", 8, 88 });
	walltextures.back().patches.emplace_back(Patch{ "BLITA0", 0, 96 });
	walltextures.back().patches.emplace_back(Patch{ "BLITC0", 8, 96 });
	walltextures.back().patches.emplace_back(Patch{ "BLITA0", 0, 104 });
	walltextures.back().patches.emplace_back(Patch{ "BLITC0", 8, 104 });
	walltextures.back().patches.emplace_back(Patch{ "BLITA0", 0, 112 });
	walltextures.back().patches.emplace_back(Patch{ "BLITC0", 8, 112 });
	walltextures.back().patches.emplace_back(Patch{ "BLITA0", 0, 120 });
	walltextures.back().patches.emplace_back(Patch{ "BLITC0", 8, 120 });


	walltextures.emplace_back(WallTexture{ "METAL1", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL03_7", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL03_7", 0, 64 });


	walltextures.emplace_back(WallTexture{ "NUKE24", 64, 24 });

	walltextures.back().patches.emplace_back(Patch{ "NUKEDGE", 0, 0 });


	walltextures.emplace_back(WallTexture{ "NUKEDGE1", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL04_3", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL04_4", 16, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL04_5", 32, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL04_2", 48, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL04_2", 64, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL04_5", 96, 32 });
	walltextures.back().patches.emplace_back(Patch{ "WALL04_3", 80, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL04_5", 112, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL04_7", 96, 0 });
	walltextures.back().patches.emplace_back(Patch{ "NUKEDGE", 0, 104 });
	walltextures.back().patches.emplace_back(Patch{ "NUKEDGE", 64, 104 });
	walltextures.back().patches.emplace_back(Patch{ "WALL04_5", 112, 32 });
	walltextures.back().patches.emplace_back(Patch{ "WALL04_3", 0, 33 });
	walltextures.back().patches.emplace_back(Patch{ "WALL04_4", 16, 33 });
	walltextures.back().patches.emplace_back(Patch{ "WALL04_5", 32, 33 });
	walltextures.back().patches.emplace_back(Patch{ "WALL04_2", 48, 33 });
	walltextures.back().patches.emplace_back(Patch{ "WALL04_2", 64, 33 });
	walltextures.back().patches.emplace_back(Patch{ "WALL04_3", 80, 33 });


	walltextures.emplace_back(WallTexture{ "NUKESLAD", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL57_1", 0, 0 });


	walltextures.emplace_back(WallTexture{ "PIPE2", 256, 128 });

	walltextures.back().patches.emplace_back(Patch{ "TP2_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "TP2_2", 128, 0 });


	walltextures.emplace_back(WallTexture{ "PLANET1", 256, 128 });

	walltextures.back().patches.emplace_back(Patch{ "AG128_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "TSCRN2", 4, 69 });
	walltextures.back().patches.emplace_back(Patch{ "AG128_1", 64, 0 });
	walltextures.back().patches.emplace_back(Patch{ "TSCRN3", 68, 69 });
	walltextures.back().patches.emplace_back(Patch{ "AG128_1", 128, 0 });
	walltextures.back().patches.emplace_back(Patch{ "TSCRN4", 132, 69 });
	walltextures.back().patches.emplace_back(Patch{ "TSCRN5", 68, 7 });
	walltextures.back().patches.emplace_back(Patch{ "TSCRN6", 4, 7 });
	walltextures.back().patches.emplace_back(Patch{ "TSCRN8", 132, 7 });
	walltextures.back().patches.emplace_back(Patch{ "AG128_1", 192, 0 });


	walltextures.emplace_back(WallTexture{ "PLAT1", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "PLAT2_1", 0, 0 });


	walltextures.emplace_back(WallTexture{ "REDWALL1", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "W15_4", 0, 56 });
	walltextures.back().patches.emplace_back(Patch{ "W15_5", 64, 56 });
	walltextures.back().patches.emplace_back(Patch{ "W15_4", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "W15_5", 64, 0 });


	walltextures.emplace_back(WallTexture{ "SHAWN2", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "AG128_1", 0, 0 });


	walltextures.emplace_back(WallTexture{ "SKY1", 256, 128 });

	walltextures.back().patches.emplace_back(Patch{ "SKY1", 0, -8 });


	walltextures.emplace_back(WallTexture{ "SLADPOIS", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WLA128_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "PS20A0", 1, 49 });


	walltextures.emplace_back(WallTexture{ "SLADRIP1", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL57_2", 0, 0 });


	walltextures.emplace_back(WallTexture{ "SLADRIP2", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL57_3", 0, 0 });


	walltextures.emplace_back(WallTexture{ "SLADRIP3", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL57_4", 0, 0 });


	walltextures.emplace_back(WallTexture{ "SLADWALL", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WLA128_1", 0, 0 });


	walltextures.emplace_back(WallTexture{ "STARG1", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "SW12_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW12_2", 32, 0 });


	walltextures.emplace_back(WallTexture{ "STARG3", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "SW19_3", 64, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW19_4", 0, 0 });


	walltextures.emplace_back(WallTexture{ "STARGR1", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "SW11_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW11_2", 32, 0 });


	walltextures.emplace_back(WallTexture{ "STARTAN1", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "SW12_4", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW12_5", 32, 0 });


	walltextures.emplace_back(WallTexture{ "STARTAN2", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "SW17_4", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW17_5", 32, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW17_6", 64, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW18_7", 96, 0 });


	walltextures.emplace_back(WallTexture{ "STARTAN3", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "SW19_1", 64, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW19_2", 0, 0 });


	walltextures.emplace_back(WallTexture{ "STEP1", 32, 8 });

	walltextures.back().patches.emplace_back(Patch{ "STEP04", 0, 0 });


	walltextures.emplace_back(WallTexture{ "STEP2", 32, 8 });

	walltextures.back().patches.emplace_back(Patch{ "STEP03", 0, 0 });


	walltextures.emplace_back(WallTexture{ "STEP3", 32, 8 });

	walltextures.back().patches.emplace_back(Patch{ "STEP05", 0, 0 });


	walltextures.emplace_back(WallTexture{ "STEP4", 32, 16 });

	walltextures.back().patches.emplace_back(Patch{ "STEP06", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "STEP06", 0, 8 });


	walltextures.emplace_back(WallTexture{ "STEP5", 32, 16 });

	walltextures.back().patches.emplace_back(Patch{ "STEP09", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "STEP09", 0, 8 });


	walltextures.emplace_back(WallTexture{ "STEP6", 32, 16 });

	walltextures.back().patches.emplace_back(Patch{ "STEP10", 0, 8 });
	walltextures.back().patches.emplace_back(Patch{ "STEP07", 0, 0 });


	walltextures.emplace_back(WallTexture{ "STONE", 256, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL01_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_2", 16, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_3", 32, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_4", 48, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_5", 64, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_6", 80, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_7", 96, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_8", 112, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_9", 128, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_A", 144, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_B", 160, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_C", 176, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_3", 192, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_1", 208, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_6", 224, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_A", 240, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_1", 0, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_2", 16, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_3", 32, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_4", 48, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_5", 64, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_6", 80, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_7", 96, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_8", 112, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_9", 128, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_A", 144, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_B", 160, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_C", 176, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_3", 192, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_1", 208, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_6", 224, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_A", 240, 72 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 64, 64 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 128, 64 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 192, 64 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 0, 120 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 64, 120 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 128, 120 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 192, 120 });


	walltextures.emplace_back(WallTexture{ "STONE2", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "W28_8", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "W28_8", 64, 0 });
	walltextures.back().patches.emplace_back(Patch{ "W28_5", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "W28_5", 64, 64 });


	walltextures.emplace_back(WallTexture{ "STONE3", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "W28_7", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "W28_6", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "W28_7", 64, 64 });
	walltextures.back().patches.emplace_back(Patch{ "W28_6", 64, 0 });


	walltextures.emplace_back(WallTexture{ "STONPOIS", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL01_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_2", 16, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_3", 32, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_4", 48, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_1", 0, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_2", 16, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_3", 32, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_4", 48, 72 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 0, 120 });
	walltextures.back().patches.emplace_back(Patch{ "PS20A0", 1, 43 });


	walltextures.emplace_back(WallTexture{ "SUPPORT2", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "SUPPORT2", 19, 72 });
	walltextures.back().patches.emplace_back(Patch{ "SUPPORT2", 19, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SUPPORT2", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SUPPORT2", 0, 72 });
	walltextures.back().patches.emplace_back(Patch{ "SUPPORT2", 40, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SUPPORT2", 40, 72 });


	walltextures.emplace_back(WallTexture{ "SW1BRCOM", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL62_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW1S0", 48, 72 });


	walltextures.emplace_back(WallTexture{ "SW1BRN1", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL02_2", 0, 56 });
	walltextures.back().patches.emplace_back(Patch{ "WALL02_2", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW3S0", 16, 75 });


	walltextures.emplace_back(WallTexture{ "SW1BRN2", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL02_2", 0, 56 });
	walltextures.back().patches.emplace_back(Patch{ "WALL02_2", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW4S0", 20, 79 });


	walltextures.emplace_back(WallTexture{ "SW1BRNGN", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL62_2", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW4S0", 20, 80 });


	walltextures.emplace_back(WallTexture{ "SW1BROWN", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL62_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW3S1", 48, 72 });


	walltextures.emplace_back(WallTexture{ "SW1COMM", 64, 72 });

	walltextures.back().patches.emplace_back(Patch{ "W13_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW1S0", 15, 18 });


	walltextures.emplace_back(WallTexture{ "SW1COMP", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "COMP03_4", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "COMP04_5", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "COMP03_4", 32, 64 });
	walltextures.back().patches.emplace_back(Patch{ "SW2S0", 16, 72 });


	walltextures.emplace_back(WallTexture{ "SW1DIRT", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL00_7", 32, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL00_6", 16, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW1S0", 16, 20 });
	walltextures.back().patches.emplace_back(Patch{ "WALL00_6", 0, -16 });
	walltextures.back().patches.emplace_back(Patch{ "WALL00_7", 48, 0 });


	walltextures.emplace_back(WallTexture{ "SW1EXIT", 32, 72 });

	walltextures.back().patches.emplace_back(Patch{ "W32_4", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW2S0", 0, 16 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 0, 64 });


	walltextures.emplace_back(WallTexture{ "SW1GRAY", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "W31_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "W31_1", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "SW2S0", 16, 70 });


	walltextures.emplace_back(WallTexture{ "SW1GRAY1", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "W31_1", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "W31_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW4S0", 19, 79 });


	walltextures.emplace_back(WallTexture{ "SW1METAL", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL03_7", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL03_7", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "SW4S0", 20, 68 });


	walltextures.emplace_back(WallTexture{ "SW1PIPE", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "TP2_2", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW3S0", 48, 76 });


	walltextures.emplace_back(WallTexture{ "SW1SLAD", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WLA128_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WARNB0", 24, 73 });


	walltextures.emplace_back(WallTexture{ "SW1STARG", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "SW19_3", 64, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW19_4", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW3S0", 48, 76 });


	walltextures.emplace_back(WallTexture{ "SW1STON1", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "W28_8", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "W28_8", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW1S0", 16, 78 });


	walltextures.emplace_back(WallTexture{ "SW1STON2", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "W28_5", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "W28_8", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "WARNB0", 24, 73 });


	walltextures.emplace_back(WallTexture{ "SW1STONE", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL01_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_2", 16, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_3", 32, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_4", 48, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_1", 0, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_2", 16, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_3", 32, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_4", 48, 72 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 0, 120 });
	walltextures.back().patches.emplace_back(Patch{ "SW1S0", 16, 72 });


	walltextures.emplace_back(WallTexture{ "SW1STRTN", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "SW12_4", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW12_5", 32, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW1S0", 16, 72 });


	walltextures.emplace_back(WallTexture{ "SW2BRCOM", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL62_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW1S1", 48, 72 });


	walltextures.emplace_back(WallTexture{ "SW2BRN1", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "STEP07", 32, 72 });
	walltextures.back().patches.emplace_back(Patch{ "STEP07", 0, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL02_2", 0, 56 });
	walltextures.back().patches.emplace_back(Patch{ "WALL02_2", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW3S1", 16, 75 });


	walltextures.emplace_back(WallTexture{ "SW2BRN2", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL02_2", 0, 56 });
	walltextures.back().patches.emplace_back(Patch{ "WALL02_2", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW4S1", 20, 79 });


	walltextures.emplace_back(WallTexture{ "SW2BRNGN", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL62_2", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW4S1", 20, 80 });


	walltextures.emplace_back(WallTexture{ "SW2BROWN", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL62_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW3S0", 48, 72 });


	walltextures.emplace_back(WallTexture{ "SW2COMM", 64, 72 });

	walltextures.back().patches.emplace_back(Patch{ "W13_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW1S1", 15, 18 });


	walltextures.emplace_back(WallTexture{ "SW2COMP", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "COMP03_4", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "COMP04_5", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "COMP03_4", 32, 64 });
	walltextures.back().patches.emplace_back(Patch{ "SW2S1", 16, 72 });


	walltextures.emplace_back(WallTexture{ "SW2DIRT", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL00_8", 48, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL00_7", 32, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL00_6", 16, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL00_5", 0, -1 });
	walltextures.back().patches.emplace_back(Patch{ "SW1S1", 16, 20 });


	walltextures.emplace_back(WallTexture{ "SW2EXIT", 32, 72 });

	walltextures.back().patches.emplace_back(Patch{ "W32_4", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW2S1", 0, 16 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 0, 64 });


	walltextures.emplace_back(WallTexture{ "SW2GRAY", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "W31_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "W31_1", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "SW2S1", 16, 70 });


	walltextures.emplace_back(WallTexture{ "SW2GRAY1", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "W31_1", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "W31_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW4S1", 19, 79 });


	walltextures.emplace_back(WallTexture{ "SW2METAL", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL03_7", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL03_7", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "SW4S1", 20, 68 });


	walltextures.emplace_back(WallTexture{ "SW2PIPE", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "TP2_2", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW3S1", 48, 76 });


	walltextures.emplace_back(WallTexture{ "SW2SLAD", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WLA128_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WARNA0", 24, 73 });


	walltextures.emplace_back(WallTexture{ "SW2STARG", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "SW19_3", 64, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW19_4", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW3S1", 48, 76 });


	walltextures.emplace_back(WallTexture{ "SW2STON1", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "W28_8", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "W28_8", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW1S1", 16, 78 });


	walltextures.emplace_back(WallTexture{ "SW2STON2", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "W28_8", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "WARNA0", 24, 73 });
	walltextures.back().patches.emplace_back(Patch{ "W28_5", 0, 0 });


	walltextures.emplace_back(WallTexture{ "SW2STONE", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL01_1", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_2", 16, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_3", 32, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_4", 48, 0 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_1", 0, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_2", 16, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_3", 32, 72 });
	walltextures.back().patches.emplace_back(Patch{ "WALL01_4", 48, 72 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 0, 64 });
	walltextures.back().patches.emplace_back(Patch{ "W33_8", 0, 120 });
	walltextures.back().patches.emplace_back(Patch{ "SW1S1", 16, 72 });


	walltextures.emplace_back(WallTexture{ "SW2STRTN", 64, 128 });

	walltextures.back().patches.emplace_back(Patch{ "SW12_4", 0, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW12_5", 32, 0 });
	walltextures.back().patches.emplace_back(Patch{ "SW1S1", 16, 72 });


	walltextures.emplace_back(WallTexture{ "TEKWALL1", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "W17_1", 0, -27 });
	walltextures.back().patches.emplace_back(Patch{ "W17_1", 0, 16 });


	walltextures.emplace_back(WallTexture{ "TEKWALL2", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL24_1", 0, 0 });


	walltextures.emplace_back(WallTexture{ "TEKWALL3", 128, 112 });

	walltextures.back().patches.emplace_back(Patch{ "W17_1", -104, 0 });


	walltextures.emplace_back(WallTexture{ "TEKWALL4", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "W94_1", 0, 0 });


	walltextures.emplace_back(WallTexture{ "TEKWALL5", 128, 128 });

	walltextures.back().patches.emplace_back(Patch{ "WALL24_1", -120, -8 });


	// End of texture definitions});


}

//WallTexture "AASTINKY", 24, 72
//{
//	NullTexture
//		Patch "WALL00_3", 0, 0
//		Patch "WALL00_3", 12, -6
//}