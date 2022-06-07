#include "GameStats.h"

namespace enc {
	std::map<std::string, int> GameStats::_tagHitCount = std::map<std::string, int>();
	int GameStats::arenaRotations = 0;
	void GameStats::IncreaseTagHitCount(std::string tagName) {
		_tagHitCount[tagName]++;
	}

	int GameStats::GetTagHitCount(std::string tagName) {
		return _tagHitCount[tagName];
	}
	void GameStats::ResetStats() {
		_tagHitCount.clear();
		arenaRotations = 0;
	}
	GameStats::~GameStats() {
		for (auto& stat : _tagHitCount) {
			std::cout << stat.first << " tag was hit " << stat.second << " times.\n";
		}
		std::cout << "Arena rotation count is " << arenaRotations << "\n";
	}
}
