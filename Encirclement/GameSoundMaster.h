#pragma once

#include <string>
#include <queue>


namespace enc {
	class GameSoundMaster {
	private:
		static std::queue<std::string> soundRequests;
	public:
		static void AddSoundRequest(std::string soundName);
		static std::string GetFirstSoundRequest();
		static std::queue<std::string>& GetAllSoundRequests();
	};

}

