#include "MapChipField.h"
#include <assert.h>
#include <map>
#include <string>

namespace {

std::map<char, MapChipType> mapChipTypeTable = {
    {'B', MapChipType::kBlock},
    {'P', MapChipType::kPlayer},
    {'E', MapChipType::kEnemy},
};

}

void MapChipField::ResetMapChipData() {
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVertical);
	for (std::vector<MapChipDataUnit>& mapChipDataLine : mapChipData_.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCSV(const std::string& filePath) {
	ResetMapChipData();
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	std::stringstream mapChipCSV;
	mapChipCSV << file.rdbuf();
	file.close();

	// Reaad map chip data from CSV
	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {
		std::string line;
		getline(mapChipCSV, line);

		std::istringstream lineStream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			std::string word;
			std::getline(lineStream, word, ',');
			if (word.empty()) {
				continue;
			}
			if (!mapChipTypeTable.contains(word[kChipType])) {
				continue;
			}
			mapChipData_.data[i][j].type = mapChipTypeTable[word[kChipType]];
			if (word.size() <= kChipSubID) {
				continue;
			}
			mapChipData_.data[i][j].subID = static_cast<uint8_t>(word[kChipSubID] - '0');
		}
	}
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVertical - 1 < yIndex) {
		return MapChipType::kBlank;
	}
	return mapChipData_.data[yIndex][xIndex].type;
}

uint8_t MapChipField::GetMapChipSubIDByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex >= kNumBlockHorizontal || yIndex >= kNumBlockVertical) {
		return 0;
	}
	return mapChipData_.data[yIndex][xIndex].subID;
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) {
	return Vector3(kBlockWidth * (static_cast<float>(xIndex) + 0.5f), kBlockHeight * (static_cast<float>(kNumBlockVertical - 1 - yIndex) + 0.5f), 0.0f);
}

IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) {
	IndexSet index;
	index.xIndex = static_cast<uint32_t>(position.x / kBlockWidth);

	float yComputed = static_cast<float>(kNumBlockVertical - 1) - floor(position.y / kBlockHeight);

	if (yComputed < 0)
		yComputed = 0;
	index.yIndex = static_cast<uint32_t>(yComputed);

	if (index.xIndex >= kNumBlockHorizontal)
		index.xIndex = kNumBlockHorizontal - 1;
	if (index.yIndex >= kNumBlockVertical)
		index.yIndex = kNumBlockVertical - 1;
	return index;
}

Rect MapChipField::GetRectByIndex(uint32_t xIndex, uint32_t yIndex) {
	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);
	Rect rect;
	rect.left = center.x - kBlockWidth / 2.0f;
	rect.right = center.x + kBlockWidth / 2.0f;
	rect.bottom = center.y - kBlockHeight / 2.0f;
	rect.top = center.y + kBlockHeight / 2.0f;
	return rect;
}
