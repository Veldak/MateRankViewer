#include "pch.h"
#include "MateRankViewer.h"




// Don't call this yourself, BM will call this function with a pointer to the current ImGui context
void MateRankViewer::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

std::string MateRankViewer::GetPluginName()
{
	return "MateRankViewer";
}

void MateRankViewer::RenderSettings()
{
	if (ImGui::Button("test"))
	{
		test();
	}

	if (ImGui::Button("add"))
	{
		addPlayer();
	}
	if (ImGui::Button("remove"))
	{
		removePlayer();
	}
	ImVec2 max = ImGui::GetItemRectMax();

	ImGui::SliderInt("height", &height, 0, 1000);
	ImGui::SliderFloat("width", &width, 0, 1000);
	ImGui::SliderFloat("width2", &width2, 0, 1000);

	
	if (PlayersList.size() > 0)
	{
		height = ((PlayersList.size() - 1) * 21) + 54;
	}



	if (ImGui::BeginChild("##PlayerList", ImVec2(337.f, height), true))
	{
		ImGui::Columns(4, 0, true);


		ImGui::SetColumnWidth(0, 208.f);
		ImGui::SetColumnWidth(1, 42.f);
		ImGui::SetColumnWidth(2, 42.f);
		ImGui::SetColumnWidth(3, 42.f);


		ImGui::Text("Name");
		ImGui::NextColumn();
		ImGui::Text("1v1");
		ImGui::NextColumn();
		ImGui::Text("2v2");
		ImGui::NextColumn();
		ImGui::Text("3v3");
		ImGui::NextColumn();

		ImGui::Separator();


		for (int i = 0; i < PlayersList.size(); i++)
		{
			ImGui::PushID(i);
			ImGui::BeginGroup();
			{
				std::string PlayerName = PlayersList.at(i).Name;
				std::string PlayerPlatform = PlayersList.at(i).Platform;

				ImGui::Text("%s(%s)", PlayerName, PlayerPlatform);

				ImGui::NextColumn();

				std::string mmr1v1 = PlayersList.at(i).MMR1v1;
				std::string mmr2v2 = PlayersList.at(i).MMR2V2;
				std::string mmr3v3 = PlayersList.at(i).MMR3V3;


				ImGui::Text(mmr1v1.c_str());
				ImGui::NextColumn();
				ImGui::Text(mmr2v2.c_str());
				ImGui::NextColumn();
				ImGui::Text(mmr3v3.c_str());
				ImGui::NextColumn();

				ImGui::EndGroup();
			}
			ImGui::PopID();

			ImGui::Separator();
		}
	}
	ImGui::EndChild();

}




/*
// Do ImGui rendering here
void MateRankViewer::Render()
{
	if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, ImGuiWindowFlags_None))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::End();

	if (!isWindowOpen_)
	{
		cvarManager->executeCommand("togglemenu " + GetMenuName());
	}
}

// Name of the menu that is used to toggle the window.
std::string MateRankViewer::GetMenuName()
{
	return "MateRankViewer";
}

// Title to give the menu
std::string MateRankViewer::GetMenuTitle()
{
	return menuTitle_;
}

// Don't call this yourself, BM will call this function with a pointer to the current ImGui context
void MateRankViewer::SetImGuiContext(uintptr_t ctx)
{
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}

// Should events such as mouse clicks/key inputs be blocked so they won't reach the game
bool MateRankViewer::ShouldBlockInput()
{
	return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

// Return true if window should be interactive
bool MateRankViewer::IsActiveOverlay()
{
	return true;
}

// Called when window is opened
void MateRankViewer::OnOpen()
{
	isWindowOpen_ = true;
}

// Called when window is closed
void MateRankViewer::OnClose()
{
	isWindowOpen_ = false;
}
*/
