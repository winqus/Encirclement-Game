#include "GameSoundMaster.h"

namespace enc {
	std::queue<std::string> GameSoundMaster::soundRequests;
	void GameSoundMaster::AddSoundRequest(std::string soundName) {
		soundRequests.push(soundName);
	}

	std::string GameSoundMaster::GetFirstSoundRequest() {
		std::string soundName = soundRequests.front();
		soundRequests.pop();
		return soundName;
	}

	std::queue<std::string>& GameSoundMaster::GetAllSoundRequests()
	{
		return soundRequests;
	}

}
