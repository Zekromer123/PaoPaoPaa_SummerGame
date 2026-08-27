#pragma once
#include "KamataEngine.h"
#include <cstdint>
#include <fstream>
#include <sstream>
#include <vector>

using namespace KamataEngine;

enum class MapChipType {
	kBlank,
	kBlock,
	kPlayer,
	kEnemy,
};
enum MapChipCharIndex {
	kChipType = 0,
	kChipSubID = 1,
};

struct MapChipDataUnit {
	MapChipType type;
	uint8_t subID;
};

struct MapChipData {
	std::vector<std::vector<MapChipDataUnit>> data;
};
struct IndexSet {
	uint32_t xIndex;
	uint32_t yIndex;
};
struct Rect {
	float left;
	float right;
	float bottom;
	float top;
};

class MapChipField {

public:
	void ResetMapChipData();
	void LoadMapChipCSV(const std::string& filePath);
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	uint8_t GetMapChipSubIDByIndex(uint32_t xIndex, uint32_t yIndex);
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	const uint32_t GetNumBlockVertical() { return kNumBlockVertical; }
	const uint32_t GetNumBlockHorizontal() { return kNumBlockHorizontal; }
	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);
	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

public:
	MapChipData mapChipData_;

	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	static inline const uint32_t kNumBlockVertical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;
};
