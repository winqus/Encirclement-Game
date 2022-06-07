#pragma once

#include "GameObject.h"
#include <memory>
#include <map>
#include <string>
#include <vector>
#include <queue>

namespace enc {
	class GameStats	{
	public:
		static int arenaRotations;
	private:
		static std::map<std::string, int> _tagHitCount;
	public:
		static void IncreaseTagHitCount(std::string tagName);
		static int GetTagHitCount(std::string tagName);
		static void ResetStats();
		~GameStats();
	};

}

