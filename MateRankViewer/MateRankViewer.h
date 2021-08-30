#pragma once


#pragma comment(lib, "libcurl.lib")
#pragma comment(lib, "zlib.lib")
#pragma comment(lib, "cpr.lib")
#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "crypt32")
#pragma comment (lib, "Wldap32.lib")
#pragma comment (lib, "jsoncpp.lib")

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"

#include <cpr/cpr.h>
#include <json/json.h>

constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);


struct Player
{
	std::string Name;
	std::string Platform;
	std::string MMR1v1;
	std::string MMR2V2;
	std::string MMR3V3;
};


class MateRankViewer : public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow
{

	//std::shared_ptr<bool> enabled;

	//Boilerplate
	virtual void onLoad();
	virtual void onUnload();

	int height;
	float width;
	float width2;


	void test();
	void addPlayer();
	void removePlayer();

	void MatchStart(std::string eventName);


	std::vector<Player> PlayersList;

	void GetPlayersInfos();


	ServerWrapper GetCurrentServer();
	std::vector<std::string> FindAllSubstringInAString(std::string texte, std::string beginString, std::string endString);

	//Render Settings Window
	virtual void RenderSettings() override;
	virtual std::string GetPluginName() override;
	virtual void SetImGuiContext(uintptr_t ctx) override;

	//Canvas
	virtual void RenderCanvas(CanvasWrapper canvas);



	// Inherited via PluginWindow
	/*

	bool isWindowOpen_ = false;
	bool isMinimized_ = false;
	std::string menuTitle_ = "MateRankViewer";

	virtual void Render() override;
	virtual std::string GetMenuName() override;
	virtual std::string GetMenuTitle() override;
	virtual void SetImGuiContext(uintptr_t ctx) override;
	virtual bool ShouldBlockInput() override;
	virtual bool IsActiveOverlay() override;
	virtual void OnOpen() override;
	virtual void OnClose() override;

	*/
};

