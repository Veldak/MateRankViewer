#include "pch.h"
#include "MateRankViewer.h"


BAKKESMOD_PLUGIN(MateRankViewer, "write a plugin description here", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void MateRankViewer::onLoad()
{
	_globalCvarManager = cvarManager;
	cvarManager->log("Plugin loaded!");

	height = 50;
	width = 50;
	width2 = 400;


	this->gameWrapper->HookEvent("Function GameEvent_TA.Countdown.BeginState", std::bind(&MateRankViewer::MatchStart, this, std::placeholders::_1));

	gameWrapper->RegisterDrawable(std::bind(&MateRankViewer::RenderCanvas, this, std::placeholders::_1));
}


void MateRankViewer::MatchStart(std::string eventName)
{


	std::thread t1(&MateRankViewer::GetPlayersInfos, this);
	t1.detach();

}

void MateRankViewer::GetPlayersInfos()
{
	auto server = GetCurrentServer();
	if (server.IsNull()) { cvarManager->log("SERVER IS NULL : "); return; }

	auto pris = server.GetPRIs();
	if (pris.IsNull()) { cvarManager->log("PRIS ARE NULL"); return; }

	PlayersList.clear();

	for (int i = 0; i < pris.Count(); i++)
	{
		Player player;
		PriWrapper playerPri = pris.Get(i);
		std::string playerName = playerPri.GetPlayerName().ToString();
		std::string playerPlatform = std::to_string(playerPri.GetPlatform());


		cvarManager->log("Name : " + playerName);

		std::string request_url;

		if (playerPlatform == "11")
		{
			cvarManager->log("Platform : epic");
			player.Platform = "Epic";
			request_url = "https://api.tracker.gg/api/v2/rocket-league/standard/profile/epic/" + playerName;
		}
		else if (playerPlatform == "2")
		{
			cvarManager->log("Platform : playstation");
			player.Platform = "PSN";
			request_url = "https://api.tracker.gg/api/v2/rocket-league/standard/profile/psn/" + playerName;
		}
		else if (playerPlatform == "1")
		{
			cvarManager->log("Platform : steam");
			player.Platform = "Steam";

			SteamID ID = playerPri.GetUniqueId();
			cvarManager->log("steam unique id : " + std::to_string(ID.ID));
			request_url = "https://api.tracker.gg/api/v2/rocket-league/standard/profile/steam/" + std::to_string(ID.ID);
		}
		else if (playerPlatform == "4")
		{
			cvarManager->log("Platform : xbox");
			player.Platform = "Xbox";
			request_url = "https://api.tracker.gg/api/v2/rocket-league/standard/profile/xbl/" + playerName;
		}
		else
		{
			cvarManager->log("Unknown Platform");
			player.Platform = "/";
			return;
		}
		cpr::Response request_response = cpr::Get(cpr::Url{ request_url });

		//Parse response json
		Json::Value actualJson;
		Json::Reader reader;

		reader.parse(request_response.text, actualJson);


		const Json::Value segments = actualJson["data"]["segments"];

		for (int index = 0; index < segments.size(); ++index)
		{
			std::string PlaylistName = segments[index]["metadata"]["name"].asString();
			std::string RatingValue = segments[index]["stats"]["rating"]["value"].asString();

			if (PlaylistName == "Ranked Duel 1v1")
			{
				player.MMR1v1 = RatingValue;
			}
			if (PlaylistName == "Ranked Doubles 2v2")
			{
				player.MMR2V2 = RatingValue;
			}
			if (PlaylistName == "Ranked Standard 3v3")
			{
				player.MMR3V3 = RatingValue;
			}
		}
		player.Name = playerName;

		PlayersList.push_back(player);
	}
}

void MateRankViewer::test()
{
	std::string request_url = "https://api.tracker.gg/api/v2/rocket-league/standard/profile/epic/Vync62";
	cpr::Response request_response = cpr::Get(cpr::Url{ request_url });

	Json::Value actualJson;
	Json::Reader reader;

	reader.parse(request_response.text, actualJson);

	const Json::Value segments = actualJson["data"]["segments"];

	for (int index = 0; index < segments.size(); ++index)
	{
		cvarManager->log(segments[index]["metadata"]["name"].asString());
		cvarManager->log(segments[index]["stats"]["rating"]["value"].asString());
		cvarManager->log("");
	}
}


void MateRankViewer::RenderCanvas(CanvasWrapper canvas)
{
	if (gameWrapper->IsInOnlineGame())
	{
		auto server = GetCurrentServer();
		if (server.IsNull()) { return; }

		auto pris = server.GetPRIs();
		if (pris.IsNull()) { return; }

		Vector2 pos = { 0, 0 };
		for (int i = 0; i < pris.Count(); i++)
		{
			PriWrapper player = pris.Get(i);
			canvas.SetColor(255, 255, 255, 255);
			canvas.SetPosition(Vector2{ 0, 0 });
			canvas.DrawString(player.GetPlayerName().ToString(), 3.f, 3.f);
			pos.Y += 50;
		}

	}
}

ServerWrapper MateRankViewer::GetCurrentServer()
{
	if (this->gameWrapper->IsInReplay())
		return this->gameWrapper->GetGameEventAsReplay().memory_address;
	else if (this->gameWrapper->IsInOnlineGame())
		return this->gameWrapper->GetOnlineGame();
	else if (this->gameWrapper->IsInFreeplay())
		return this->gameWrapper->GetGameEventAsServer();
	else if (this->gameWrapper->IsInCustomTraining())
		return this->gameWrapper->GetGameEventAsServer();
	else if (this->gameWrapper->IsSpectatingInOnlineGame())
		return this->gameWrapper->GetOnlineGame();
	else
		return NULL;
}


std::vector<std::string> MateRankViewer::FindAllSubstringInAString(std::string texte, std::string beginString, std::string endString)
{
	std::vector<std::string> List;
	std::vector<std::size_t> IndexPos;

	int occurrences = 0;
	std::string::size_type posBegin = 0;
	std::string s = texte;
	std::string BeginTarget = beginString;

	while ((posBegin = s.find(BeginTarget, posBegin)) != std::string::npos)
	{
		++occurrences;
		IndexPos.push_back(posBegin);
		posBegin += BeginTarget.length();
	}

	for (int i = 0; i < IndexPos.size(); i++)
	{
		s = texte;
		s.erase(0, IndexPos.at(i));

		std::string resultString = s.substr(beginString.length(), s.find(endString) - beginString.length());
		//std::cout << "Resultat : " + resultString << std::endl;
		List.push_back(resultString);
		//std::cout << std::endl;
	}
	return List;
}

void MateRankViewer::addPlayer()
{
	Player player;
	int nb = PlayersList.size();
	player.Name = "wsh" + std::to_string(nb);
	player.Platform = "Epic";
	player.MMR1v1 = "1111";
	player.MMR2V2 = "2222";
	player.MMR3V3 = "3333";

	PlayersList.push_back(player);
}

void MateRankViewer::removePlayer()
{
	std::vector<Player>::iterator begin = PlayersList.begin();
	std::advance(begin, PlayersList.size() - 1);
	PlayersList.erase(begin);
}

void MateRankViewer::onUnload()
{
}