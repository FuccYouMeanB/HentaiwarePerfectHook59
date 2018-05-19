#pragma once
#include "Configuration.hpp"
#include "dropboxes.h"
#include "Variables.h"
#include "Tabs.h"
#include <chrono>
#include "Themes.h"

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))


void RenderInterface() {
	// Place all item settings under a collapsing header.


		static int page = 6;

		switch(g_Options.Menu.Theme)
		{
		case 0:
			RenderDefault();
			break;
		case 1:
			RenderOld();
			break;
		}

		ImGui::SetNextWindowSize(ImVec2(510, 280), ImGuiSetCond_Always);

		if (ImGui::Begin(XorStr("s1x44"), &g_Options.Menu.Opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_ShowBorders | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | G::extra_flags))
		{



			ImGui::Columns(1, NULL, false);


			const char* tabs[] = {
				"RAGE",
				"LEGIT",
				"TRIGGER",
				"ESP",
				"MISC",
                "SKINS"
			};

			for (int i = 0; i < IM_ARRAYSIZE(tabs); i++)
			{

				//ImGui::GetStyle().Colors[ImGuiCol_Button] = ImColor(54, 54, 54, 255);

				if (ImGui::Button(tabs[i], ImVec2(ImGui::GetWindowSize().x / IM_ARRAYSIZE(tabs) - 9, 0)))
					page = i;

				//ImGui::GetStyle().Colors[ImGuiCol_Button] = ImColor(54, 54, 54, 255);

				if (i < IM_ARRAYSIZE(tabs) - 1)
					ImGui::SameLine();
			}

			ImGui::Separator();

			switch (page)
			{
			case 0:
				testy();
				break; 
			case 1:
				RenderLegit();
				break;
			case 2:
				RenderTrigger();
				break;
			case 3:
				RenderVisuals();
				break;
			case 4:
				RenderMisc();
				break;
			case 5:
				RenderSkinChanger();
				break;
			case 6:
				RenderMain();
				break;
			}
			ImGui::End();
		}
}
