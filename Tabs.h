#pragma once
#include "IDirect3DDevice9.h"
#include "Themes.h"

typedef void(*CL_FullUpdate_t) (void);
CL_FullUpdate_t CL_FullUpdate = nullptr;

void KnifeApplyCallbk()
{
	static auto CL_FullUpdate = reinterpret_cast<CL_FullUpdate_t>(U::FindPattern("engine.dll", reinterpret_cast<PBYTE>("\xA1\x00\x00\x00\x00\xB9\x00\x00\x00\x00\x56\xFF\x50\x14\x8B\x34\x85"), "x????x????xxxxxxx"));
	CL_FullUpdate();
}
void dankestSetClanTag(const char* tag, const char* name)
{
	static auto ClanTagOffset = U::FindPattern("engine.dll", (PBYTE)"\x53\x56\x57\x8B\xDA\x8B\xF9\xFF\x15", "xxxxxxxxx");
	if (ClanTagOffset)
	{
		if (strlen(name) > 0) {
			auto name_ = std::string(name);
			auto newline = name_.find("\\n");
			auto tab = name_.find("\\t");
			if (newline != std::string::npos) {
				name_.replace(newline, newline + 2, "\n");
			}
			if (tab != std::string::npos) {
				name_.replace(tab, tab + 2, "\t");
			}
		}
		auto tag_ = std::string(tag);
		if (strlen(tag) > 0) {
			auto newline = tag_.find("\\n");
			auto tab = tag_.find("\\t");
			if (newline != std::string::npos) {
				tag_.replace(newline, newline + 2, "\n");
			}
			if (tab != std::string::npos) {
				tag_.replace(tab, tab + 2, "\t");
			}
		}
		static auto dankesttSetClanTag = reinterpret_cast<void(__fastcall*)(const char*, const char*)>(ClanTagOffset);
		dankesttSetClanTag(tag_.data(), tag_.data());
	}
}

namespace ImGui
{
	class Tab
	{
	private:

		std::vector<std::string> labels;

	public:

		void add(std::string name)
		{
			labels.push_back(name);
		}

		void draw(int *selected)
		{
			ImGuiStyle &style = GetStyle();
			ImVec4 color = style.Colors[ImGuiCol_Button];
			ImVec4 colorActive = style.Colors[ImGuiCol_ButtonActive];
			ImVec4 colorHover = style.Colors[ImGuiCol_ButtonHovered];
			ImVec2 max = GetContentRegionMax();
			float size_x = max.x / labels.size() - 0.f;
			float size_y = max.y / labels.size() - 30.f;

			for (size_t i = 0; i < labels.size(); i++)
			{
				if (i == *selected)
				{
					style.Colors[ImGuiCol_Button] = colorActive;
					style.Colors[ImGuiCol_ButtonActive] = colorActive;
					style.Colors[ImGuiCol_ButtonHovered] = colorActive;
				}
				else
				{
					style.Colors[ImGuiCol_Button] = color;
					style.Colors[ImGuiCol_ButtonActive] = colorActive;
					style.Colors[ImGuiCol_ButtonHovered] = colorHover;
				}

				if (Button(labels.at(i).c_str(), { size_x, size_y }))
					*selected = i;
			}

			style.Colors[ImGuiCol_Button] = color;
			style.Colors[ImGuiCol_ButtonActive] = colorActive;
			style.Colors[ImGuiCol_ButtonHovered] = colorHover;
		}
	};
}

const char* hitsound[] =
{
	"off",
	"Default",
	"Trump",
	"Skeet"
};




void RenderMain()
{

	ImGui::BeginChild("Main", ImVec2(), true);
	{

	static float r = 1.0f;
	static float g = 0.f;
	static float b = 0.f;

	if (r == 1.f && g >= 0.f && b <= 0.f)
	{
		g += 0.005f;
		b = 0.f;
	}
	if (r <= 1.f && g >= 1.f && b == 0.f)
	{
		g = 1.f;
		r -= 0.005f;
	}
	if (r <= 0.f && g == 1.f && b >= 0.f)
	{
		r = 0.f;
		b += 0.005f;
	}
	if (r == 0.f && g <= 1.f && b >= 1.f)
	{
		b = 1.f;
		g -= 0.005f;
	}
	if (r >= 0.f && g <= 0.f && b == 1.f)
	{
		g = 0.f;
		r += 0.005f;
	}
	if (r >= 1.f && g >= 0.f && b <= 1.f)
	{
		r = 1.f;
		b -= 0.005f;
	}

	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(r, g, b, 1.f));

	ImGui::BeginChild("##rainbotcrapnignog", ImVec2(460.f, 1.f));
	ImGui::EndChild();
	ImGui::PopStyleColor();





	ImGui::Text("enjoy vac");
	ImGui::TextDisabled("ragebot not work");

	}ImGui::EndChild();
	ImGui::SameLine();


}


void RenderColor()
{

	ImGui::BeginChild("Main", ImVec2(), true);
	{




		ImGui::TextDisabled("iks de nie wiem status");

		ImGui::Columns(2, NULL, false);

		ImGui::Text("Box");

		ImGui::NextColumn();

		ImGui::MyColorEdit3(XorStr("CT"), (float*)&g_Options.Colors.box_color_ct, 1 << 7);
		ImGui::SameLine();
		ImGui::MyColorEdit3(XorStr("TR"), (float*)&g_Options.Colors.box_color_t, 1 << 7);

		ImGui::Separator();

		ImGui::Columns(2, NULL, false);

		ImGui::Text("Skeleton");

		ImGui::NextColumn();

		ImGui::MyColorEdit3(XorStr("##04593"), (float*)&g_Options.Colors.color_skeleton, 1 << 7);

		ImGui::Separator();

		ImGui::Columns(2, NULL, false);

		ImGui::Text("Hand Color");

		ImGui::NextColumn();

		ImGui::MyColorEdit3(("##47ESP"), g_Options.Colors.hands_color, 1 << 7);
		ImGui::Separator();

		ImGui::Columns(2, NULL, false);

		ImGui::Text("Spread Crosshair");

		ImGui::NextColumn();

		ImGui::MyColorEdit3(XorStr("##23452"), (float*)&g_Options.Colors.color_spread, 1 << 7);

		ImGui::Separator();

		ImGui::Columns(2, NULL, false);

		ImGui::Text("Recoil Crosshair");

		ImGui::NextColumn();

		ImGui::MyColorEdit3(XorStr("##95340"), (float*)&g_Options.Colors.color_recoil, 1 << 7);


		ImGui::Separator();


		ImGui::Columns(2, NULL, false);

		ImGui::Text("Cham enemy ");

		ImGui::NextColumn();

		ImGui::MyColorEdit3("##42ESP", g_Options.Colors.EnemyChamsVis, 1 << 7);


		ImGui::Separator();

		ImGui::Text("Cham team ");

		ImGui::NextColumn();

		ImGui::MyColorEdit3("##E772SP", g_Options.Colors.TeamChamsVis, 1 << 7);


		ImGui::Separator();


		ImGui::Text("XQZ");

		ImGui::NextColumn();
		ImGui::MyColorEdit3("##E772SP", g_Options.Colors.TeamChamsVis, 1 << 7);
		ImGui::MyColorEdit3("##43ESP", g_Options.Colors.EnemyChamsNVis, 1 << 7);


		ImGui::Separator();






	



	}ImGui::EndChild();
	ImGui::SameLine();


}







void testy()
{
	

	







}

void RenderTrigger()
{

	ImGui::BeginChild("first", ImVec2(), true);
	{

	ImGui::Columns(2, NULL, false);

	ImGui::Checkbox(("Enable Triggerbot"), &g_Options.Legitbot.Triggerbot.Enabled);
	ImGui::PushItemWidth(80);
	ImGui::Hotkey("Key##3", &g_Options.Legitbot.Triggerbot.Key);
	ImGui::PushItemWidth(110);
	ImGui::SliderFloat(("Delay"), &g_Options.Legitbot.Triggerbot.Delay, 1.f, 150.f, "%.0f");
	// ImGui::Combo(("Triggerbot key"), &menu.Legitbot.Triggerbot.Key, keyNames, ARRAYSIZE(keyNames));
	ImGui::SliderFloat(("Hitchance"), &g_Options.Legitbot.Triggerbot.hitchance, 1.f, 100.f, "%.0f");



	ImGui::NextColumn();

	ImGui::Text(("Filter"));

	ImGui::BeginChild(("Filter"), ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 19 * 5));
	{
		ImGui::Selectable((" Head"), &g_Options.Legitbot.Triggerbot.Filter.Head);
		ImGui::Selectable((" Arms"), &g_Options.Legitbot.Triggerbot.Filter.Arms);
		ImGui::Selectable((" Chest"), &g_Options.Legitbot.Triggerbot.Filter.Chest);
		ImGui::Selectable((" Stomach"), &g_Options.Legitbot.Triggerbot.Filter.Stomach);
		ImGui::Selectable((" Legs"), &g_Options.Legitbot.Triggerbot.Filter.Legs);
	} ImGui::EndChild();





	}ImGui::EndChild();


}

void drawConfig()
{
	ImGui::PushItemWidth(10);
	ImGui::Combo("Config", &g_Options.Menu.ConfigFile, configFiles, ARRAYSIZE(configFiles));
	ImGui::PopItemWidth();
	if (ImGui::Button("Save")) Config->Save();
	ImGui::SameLine();
	if (ImGui::Button("Load")) Config->Load();
	ImGui::SameLine();
	if (ImGui::Button("Reset")) Config->Setup();
}

void RenderLegit()
{




		ImGui::BeginChild("general", ImVec2(180, 210), true);
		{

			ImGui::Checkbox(("Active"), &g_Options.Legitbot.MainSwitch);
			ImGui::Checkbox(("Enable Aimbot"), &g_Options.Legitbot.Aimbot.Enabled);
			ImGui::Checkbox(("Legit Backtrack"), &g_Options.Legitbot.backtrack);
			ImGui::SliderFloat(("Ticks"), &g_Options.Legitbot.backtrackTicks, 0, 12, "%1.f");
			ImGui::Checkbox(("Backtrack line"), &g_Options.Visuals.backtrackline);
			ImGui::SameLine();
			ImGui::MyColorEdit3(XorStr("##232gd"), (float*)&g_Options.Colors.backtrackdots_color, 1 << 7);
			ImGui::Checkbox(("Auto Pistol"), &g_Options.Legitbot.AutoPistol);
			/*ImGui::BeginChild(("HitboxFilter"), ImVec2(ImGui::GetWindowContentRegionWidth() * 0.5f, 19 * 5));

			ImGui::Selectable((" Head"), &g_Options.Legitbot.Filter.Head);
			ImGui::Selectable((" Arms"), &g_Options.Legitbot.Filter.Arms);
			ImGui::Selectable((" Chest"), &g_Options.Legitbot.Filter.Chest);
			ImGui::Selectable((" Stomach"), &g_Options.Legitbot.Filter.Stomach);
			ImGui::Selectable((" Legs"), &g_Options.Legitbot.Filter.Legs);


			*/


		}ImGui::EndChild();
		ImGui::SameLine();


		ImGui::BeginChild("drugie", ImVec2(310, 210), true);
		{



			static int aimtab = 0;

			if (ImGui::Button(XorStr("pistol")))
			{
				aimtab = 0;
			}

			ImGui::SameLine();

			if (ImGui::Button(XorStr("riffle")))
			{
				aimtab = 1;
			}

			ImGui::SameLine();

			if (ImGui::Button(XorStr("snipers")))
			{
				aimtab = 2;
			}




			if (aimtab == 0)
			{


				ImGui::PushItemWidth(80);
				ImGui::Hotkey("Key##1", &g_Options.Legitbot.PistolKey);
				// ImGui::Combo(("Key##1"), &menu.Legitbot.PistolKey, keyNames, ARRAYSIZE(keyNames));
				ImGui::PushItemWidth(110);
				ImGui::SliderFloat("Smooth##1", &g_Options.Legitbot.PistolSmooth, 1.00f, 100.00f, "%.2f");
				ImGui::SliderFloat("FOV##1", &g_Options.Legitbot.Pistolfov, 0.00f, 30.00f, "%.2f");
				ImGui::SliderFloat("Min RCS##1", &g_Options.Legitbot.pistol_recoil_min, 1.00f, 100.00f, "%.2f");
				ImGui::SliderFloat("Max RCS##1", &g_Options.Legitbot.pistol_recoil_max, 1.00f, 100.00f, "%.2f");



			}
			if (aimtab == 1)
			{


				ImGui::PushItemWidth(80);
				ImGui::Hotkey("Key##0", &g_Options.Legitbot.MainKey);
				// ImGui::Combo(("Key##0"), &menu.Legitbot.MainKey, keyNames, ARRAYSIZE(keyNames));
				ImGui::PushItemWidth(110);
				ImGui::SliderFloat("Smooth##0", &g_Options.Legitbot.MainSmooth, 1.00f, 100.00f, "%.2f");
				ImGui::SliderFloat("FOV##0", &g_Options.Legitbot.Mainfov, 0.00f, 30.00f, "%.2f");
				ImGui::SliderFloat("Min RCS##0", &g_Options.Legitbot.main_recoil_min, 1.00f, 100.00f, "%.2f");
				ImGui::SliderFloat("Max RCS##0", &g_Options.Legitbot.main_recoil_max, 1.00f, 100.00f, "%.2f");


			}
			if (aimtab == 2)
			{

				ImGui::PushItemWidth(80);
				ImGui::Hotkey("Key##2", &g_Options.Legitbot.SniperKey);
				// ImGui::Combo(("Key##2"), &menu.Legitbot.SniperKey, keyNames, ARRAYSIZE(keyNames));
				ImGui::PushItemWidth(110);
				ImGui::SliderFloat("Smooth##2", &g_Options.Legitbot.SniperSmooth, 1.00f, 100.00f, "%.2f");
				ImGui::SliderFloat("FOV##2", &g_Options.Legitbot.Sniperfov, 0.00f, 30.00f, "%.2f");
				ImGui::SliderFloat("Min RCS##2", &g_Options.Legitbot.sniper_recoil_min, 1.00f, 100.00f, "%.2f");
				ImGui::SliderFloat("Max RCS##2", &g_Options.Legitbot.sniper_recoil_max, 1.00f, 100.00f, "%.2f");

			}

		}ImGui::EndChild();
		ImGui::SameLine();






}

void RenderVisuals()
{
	
	ImGui::Checkbox(("Active"), &g_Options.Visuals.Enabled);
	ImGui::SameLine();
	ImGui::Checkbox(("Visible"), &g_Options.Visuals.Filter.Players);
	ImGui::SameLine();
	ImGui::Checkbox(("EnemyOnly"), &g_Options.Visuals.Filter.EnemyOnly);


	ImGui::SameLine();

	static int esptab = 0;

	if (ImGui::Button(XorStr("general"), ImVec2(70, 20)))
	{
		esptab = 0;
	}

	ImGui::SameLine();

	if (ImGui::Button(XorStr("visual"), ImVec2(70, 20)))
	{
		esptab = 1;
	}

	ImGui::SameLine();

	if (ImGui::Button(XorStr("render"), ImVec2(70, 20)))
	{
		esptab = 2;
	}

	ImGui::Separator();

	if (esptab == 0)
	{

		



			ImGui::BeginChild("first", ImVec2(180, 210), true);
			{


			ImGui::Columns(1, NULL, false);

			ImGui::PushItemWidth(150);
			ImGui::SliderFloat(("fov"), &g_Options.Visuals.FOVChanger, 0.f, 60.f, "%.0f");


			ImGui::Columns(1, NULL, false);

			ImGui::PushItemWidth(150);
			ImGui::SliderFloat(("vfov"), &g_Options.Visuals.viewmodelChanger, 0.f, 120.f, "%.0f");

			ImGui::Separator();

			ImGui::Text("hand ");
			ImGui::SameLine();
			ImGui::PushItemWidth(90);
			ImGui::SliderFloat(XorStr("alpha"), &g_Options.Colors.hands_alpha, 0.f, 1.f);



			ImGui::PushItemWidth(80);
			ImGui::Combo(XorStr(""), &g_Options.Visuals.Hands, handd, ARRAYSIZE(handd));
			ImGui::SameLine();
			ImGui::MyColorEdit3(("##47ESP"), g_Options.Colors.hands_color, 1 << 7);


			ImGui::Separator();

			ImGui::Checkbox(("HasDefuser"), &g_Options.Visuals.HasDefuser);
			ImGui::Checkbox(("IsScoped"), &g_Options.Visuals.IsScoped);
			ImGui::Checkbox(("IsFlashed"), &g_Options.Visuals.Flashed);

			ImGui::Separator();

			ImGui::Checkbox(("nFlash"), &g_Options.Visuals.NoFlash);
			ImGui::Checkbox(("nSmoke"), &g_Options.Visuals.NoSmoke);
			ImGui::Checkbox(("nVisRecoil"), &g_Options.Visuals.NoVisualRecoil);
			ImGui::Checkbox(("nScope"), &g_Options.Visuals.noscopeborder);



			}ImGui::EndChild();
			ImGui::SameLine();


		ImGui::BeginChild("second", ImVec2(155, 180),  true);
		{

			ImGui::Checkbox(("Box  "), &g_Options.Visuals.Box);
			ImGui::SameLine();
			ImGui::MyColorEdit3(XorStr("CT"), (float*)&g_Options.Colors.box_color_ct, 1 << 7);
			ImGui::SameLine();
			ImGui::MyColorEdit3(XorStr("TT"), (float*)&g_Options.Colors.box_color_t, 1 << 7);

			ImGui::Checkbox(("Name"), &g_Options.Visuals.Name);
			ImGui::Checkbox(("HP"), &g_Options.Visuals.HP);
			ImGui::Checkbox(("Skeleton"), &g_Options.Visuals.Skeleton);
			ImGui::SameLine();
			ImGui::MyColorEdit3(XorStr("##04593"), (float*)&g_Options.Colors.color_skeleton, 1 << 7);

			ImGui::Checkbox(("C4"), &g_Options.Visuals.C4);
			ImGui::Checkbox(("Weapon"), &g_Options.Visuals.Weapon);
			ImGui::Checkbox(("Money"), &g_Options.Visuals.money);
	
		}ImGui::EndChild();
		ImGui::SameLine();

		ImGui::BeginChild("third", ImVec2(145, 180), true);
		{

			ImGui::Text("Weapons world ");
			ImGui::Separator();
			ImGui::Checkbox(("icon"), &g_Options.Visuals.WeaponsWorldicon);
			ImGui::SameLine();
			ImGui::Checkbox(("text"), &g_Options.Visuals.WeaponsWorld);
			ImGui::Separator();
			ImGui::Checkbox(("World c4"), &g_Options.Visuals.C4World);
			ImGui::Checkbox(("Radar"), &g_Options.Misc.Radar);
			ImGui::Checkbox(("Spr Crossh"), &g_Options.Visuals.SpreadCrosshair);
			ImGui::SameLine();
			ImGui::MyColorEdit3(XorStr("##23452"), (float*)&g_Options.Colors.color_spread, 1 << 7);
			ImGui::Checkbox(("Rec Crossh"), &g_Options.Visuals.RecoilCrosshair);
			ImGui::SameLine();
			ImGui::MyColorEdit3(XorStr("##95340"), (float*)&g_Options.Colors.color_recoil, 1 << 7);



		}ImGui::EndChild();
	}
	if (esptab == 1)
	{

		ImGui::BeginChild("pierwszy", ImVec2(160, 180), true);
		{



			ImGui::Text("Chams");

			ImGui::Separator();

			ImGui::PushItemWidth(130);
			ImGui::Checkbox(XorStr("enemy"), &g_Options.Visuals.Chams);
			ImGui::SameLine();
			ImGui::MyColorEdit3("##42ESP", g_Options.Colors.EnemyChamsVis, 1 << 7);

			ImGui::Checkbox(XorStr("Team"), &g_Options.Visuals.Teamchams);

			ImGui::SameLine();
			ImGui::MyColorEdit3("##E772SP", g_Options.Colors.TeamChamsVis, 1 << 7);

			ImGui::Checkbox(XorStr("XQZ"), &g_Options.Visuals.XQZ);

			ImGui::SameLine();
			ImGui::MyColorEdit3("##43ESP", g_Options.Colors.EnemyChamsNVis, 1 << 7);

			ImGui::Separator();

			ImGui::PushItemWidth(120);
			ImGui::SliderInt(XorStr("Chams Alpha"), &g_Options.Visuals.champlayeralpha, 0, 100);
			ImGui::Combo("##sdgl", &g_Options.Visuals.matierial, "Normal\0\rFlat\0\rWireframe\0\rGlass\0\rGold\0\rPlatinum\0\rPlastic\0\rCrystal\0\0", -1);
			ImGui::PushItemWidth(80);




		}ImGui::EndChild();

		ImGui::SameLine();


		ImGui::BeginChild("drugi", ImVec2(160, 180), true);
		{

			ImGui::Text("glow");

			ImGui::Separator();

			ImGui::Checkbox(XorStr("enemy"), &g_Options.Visuals.glowplayer);
			ImGui::SameLine();
			ImGui::MyColorEdit3("##glow", g_Options.Colors.EnemyGlow, 1 << 7);

			ImGui::Checkbox(XorStr("Team"), &g_Options.Visuals.teamglow);
			ImGui::SameLine();
			ImGui::MyColorEdit3("##teamglow", g_Options.Colors.TeamGlow, 1 << 7);
			ImGui::PushItemWidth(130);

			ImGui::SliderInt(XorStr(" ##pga"), &g_Options.Visuals.playerglowalpha, 0, 255);

			ImGui::Separator();

			ImGui::Checkbox(XorStr("Weapons"), &g_Options.Visuals.weaponglow);
			ImGui::SameLine();
			ImGui::MyColorEdit3("##ESPWeapon", g_Options.Colors.WeaponGlow, 1 << 7);
			ImGui::PushItemWidth(130);
			ImGui::SliderInt(XorStr(" ##wga"), &g_Options.Visuals.weaponglowalpha, 0, 255);

			ImGui::Separator();

			ImGui::Checkbox(XorStr("Bomb"), &g_Options.Visuals.bombglow);
			ImGui::SameLine();
			ImGui::MyColorEdit3("##ESPC4", g_Options.Colors.C4glow, 1 << 7);
			ImGui::PushItemWidth(130);
			ImGui::SliderInt(XorStr(" ##bga"), &g_Options.Visuals.bombglowalpha, 0, 255);


		/*	ImGui::Text("glow");

			ImGui::Separator();

			ImGui::Checkbox("team", &g_Options.Visuals.Glow);
			ImGui::SameLine();
			ImGui::MyColorEdit3("##43ESP", g_Options.Colors.EnemyChamsNVis, 1 << 7);
			ImGui::Checkbox("enemy", &g_Options.Visuals.Glow);
			ImGui::SameLine();
			ImGui::MyColorEdit3("##43ESP", g_Options.Colors.EnemyChamsNVis, 1 << 7);
			ImGui::Checkbox("weapon", &g_Options.Visuals.Glow);
			ImGui::SameLine();
			ImGui::MyColorEdit3("##43ESP", g_Options.Colors.EnemyChamsNVis, 1 << 7);
			ImGui::Checkbox("bomb", &g_Options.Visuals.Glow);
			ImGui::SameLine();
			ImGui::MyColorEdit3("##43ESP", g_Options.Colors.EnemyChamsNVis, 1 << 7);*/
			
		}ImGui::EndChild();
		ImGui::SameLine();


		ImGui::SameLine();


		ImGui::BeginChild("trzecie", ImVec2(160, 180), true);
		{

			ImGui::Text("other");

			ImGui::Separator();

			ImGui::Checkbox(("DLight"), &g_Options.Visuals.DLight);
			ImGui::SameLine();
			ImGui::MyColorEdit3(XorStr("##63456"), (float*)&g_Options.Colors.dlight_color, 1 << 7);
			ImGui::Checkbox(("GrenadeESP"), &g_Options.Visuals.GrenadeESP);
			ImGui::Checkbox(("nadePredict"), &g_Options.Visuals.GrenadePrediction);
			ImGui::SameLine();
			ImGui::MyColorEdit3(XorStr("##1234"), (float*)&g_Options.Colors.tracer_color, 1 << 7);










		}ImGui::EndChild();
		ImGui::SameLine();






	}
	if (esptab == 2)
	{

		ImGui::BeginChild("Main", ImVec2(), true);
		{



			ImGui::Columns(2, NULL, false);

			ImGui::PushItemWidth(80);
			ImGui::Checkbox(XorStr("Thirdperson"), &g_Options.Visuals.ThirdPerson);
			ImGui::PushItemWidth(125);
			ImGui::SliderFloat(XorStr("##tpdist"), &g_Options.Visuals.tpdist, 50.f, 450.f, "%.0f");
			ImGui::Hotkey(XorStr("Key##273"), &g_Options.Visuals.TPKey);



			ImGui::Text("Custom-Skybox:");
			ImGui::PushItemWidth(146);
			ImGui::Combo(("##aadk342"), &g_Options.Misc.Skybox, skybox_items, _ARRAYSIZE(skybox_items));



			ImGui::NextColumn();

			ImGui::Checkbox(("NightMode"), &g_Options.Misc.nightMode);
			ImGui::Checkbox(("GrayMode"), &g_Options.Misc.Gray);
			ImGui::Checkbox(("LsdMode"), &g_Options.Misc.crack);
			ImGui::Checkbox(("ChromeMode"), &g_Options.Misc.chromatic);
			ImGui::Checkbox(("No Post-Processing"), &g_Options.Misc.fps);

		}ImGui::EndChild();
		ImGui::SameLine();


	}

	

}



bool memesaredank;

void RenderMisc()
{

	//ImGui::SameLine();



	static int tabbmisc = 0;

	if (ImGui::Button(XorStr("GENERAL"), ImVec2(75, 20)))
	{
		tabbmisc = 0;
	}

	ImGui::SameLine();

	if (ImGui::Button(XorStr("OTHER"), ImVec2(75, 20)))
	{
		tabbmisc = 1;
	}

	ImGui::SameLine();

	if (ImGui::Button(XorStr(""), ImVec2(75, 20)))
	{
		tabbmisc = 2;
	}




	ImGui::SameLine();

	ImGui::PushItemWidth(60);
	ImGui::Combo("", &g_Options.Menu.ConfigFile, configFiles, ARRAYSIZE(configFiles));
	ImGui::SameLine();
	ImGui::PopItemWidth();
	if (ImGui::Button("Save")) Config->Save();
	ImGui::SameLine();
	if (ImGui::Button("Load")) Config->Load();
	ImGui::SameLine();
	if (ImGui::Button("Reset")) Config->Setup();
	ImGui::SameLine();
	if (ImGui::Button(XorStr("Unhook")))
	{
		g_Engine->ClientCmd_Unrestricted("cl_mouseenable 1");
		unload = true;
	}

	ImGui::Columns(1, NULL, false);
	//ImGui::Separator();

	if (tabbmisc == 0)
	{

		ImGui::BeginChild("pierwsze child", ImVec2(), true);
		{


			ImGui::Columns(3, NULL, false);

			ImGui::Checkbox(("Watermark"), &g_Options.Misc.Watermark);
			ImGui::Checkbox(("Time"), &g_Options.Visuals.Time);
			ImGui::Checkbox(("SpecList"), &g_Options.Misc.SpecList);
			ImGui::Checkbox(XorStr("Show rank"), &g_Options.Misc.ServerRankRevealAll);
			ImGui::Checkbox(("Auto-Accept"), &g_Options.Misc.AutoAccept);


			ImGui::NextColumn();

			ImGui::Checkbox(("Bunny Hop"), &g_Options.Misc.Bhop);
			ImGui::Checkbox(("AutoStrafe"), &g_Options.Misc.AutoStrafe);

			ImGui::Checkbox(("Hitmarker"), &g_Options.Misc.Hitmarker);
			ImGui::SameLine();
			ImGui::MyColorEdit3(("##fd5432"), g_Options.Colors.hitmark_color, 1 << 7);
			ImGui::PushItemWidth(60);
			ImGui::Combo(XorStr("HitSound"), &g_Options.Misc.hitsound, hitsound, _ARRAYSIZE(hitsound));

			ImGui::NextColumn();

			ImGui::Checkbox("nameStealer", &g_Options.Misc.silentstealer);
			ImGui::Checkbox("NameSpammer", &g_Options.Misc.namespam);
			//ImGui::Checkbox("Clantag", &g_Options.Misc.syncclantag);
			ImGui::PushItemWidth(60);
			ImGui::Combo(("chatSpam"), &g_Options.Misc.spammer, spammers, ARRAYSIZE(spammers));





		}ImGui::EndChild();
		ImGui::SameLine();


	}
	if (tabbmisc == 1)
	{

		ImGui::BeginChild("drugie child", ImVec2(), true);
		{





			ImGui::PushItemWidth(150);
			static char buf1[128] = ""; ImGui::InputText(("##Name"), buf1, 128, ImGuiInputTextFlags_AllowTabInput);
			ImGui::SameLine();
			ImGui::PushItemWidth(50);
			if (ImGui::Button(("Change Name")))
			{
				ConVar* name = g_CVar->FindVar("name");
				if (name)
				{
					*(int*)((DWORD)&name->fnChangeCallback + 0xC) = NULL;
					name->SetValue(buf1);
				}
			}

			ImGui::PushItemWidth(150);
			static char buf2[128] = ""; ImGui::InputText(("##Clantag"), buf2, 16, ImGuiInputTextFlags_AllowTabInput);
			ImGui::SameLine();
			ImGui::PushItemWidth(50);

			if (ImGui::Button(("Change Clantag")))
			{
				dankestSetClanTag(buf2, buf2);
			}

			ImGui::PushItemWidth(150);
			ImGui::InputText(("##AnimatedClantag"), G::AnimatedClantag, 16, ImGuiInputTextFlags_AllowTabInput);
			ImGui::SameLine();
			ImGui::PushItemWidth(50);
			if (ImGui::Button(("Start")))
			{
				g_Options.Misc.animatedclantag = true;
			}
			ImGui::SameLine();
			ImGui::PushItemWidth(85);
			if (ImGui::Button(("Stop")))
			{
				g_Options.Misc.animatedclantag = false;
			}







			if (ImGui::Button("Silent exploit"))
			{
				static ConVar* name = g_CVar->FindVar("name");
				if (name)
				{
					*(int*)((DWORD)&name->fnChangeCallback + 0xC) = NULL;
					name->SetValue("\n");
				}
			}


			if (ImGui::Button("crash name"))
			{
				char name[151];
				char fucked_char = static_cast<char>(-1);
				for (auto i = 0; i <= 150; i++)
					name[i] = fucked_char;

				const char nick[128] = "™B™1™G ™C™R™A™S™™H™E™R ™C™Y™A ™A™Y™Y™W™A™R™E ™™™™™™™™™™™™™™™™™™™™™™™™™™™™™™™™™™™™™™™B™1™G ™C™R™A™S™™H™E™R ™C™Y™A ™A™Y™Y™W™A™R™E";
				memcpy(name, nick, 11);

				SetName(name);
			}
















		}ImGui::EndChild();
		ImGui::SameLine();
	}
	if (tabbmisc == 2)
	{

		ImGui::BeginChild("render child", ImVec2(), true);
		{


			//ImGui::Text("Themes");
			ImGui::PushItemWidth(65);
			ImGui::Combo(("Themes"), &g_Options.Menu.Theme, themes, ARRAYSIZE(themes));


		}ImGui::EndChild();
		ImGui::SameLine();
	}
		/*ImGui::PushItemWidth(100);
		ImGui::Combo(("File"), &g_Options.Menu.ConfigFile, configFiles, ARRAYSIZE(configFiles));
		if (ImGui::Button("Save Config")) Config->Save();
		ImGui::SameLine();
		if (ImGui::Button("Load Config")) Config->Load();
		ImGui::SameLine();

		*/
}

void RenderSkinChanger()
{
	{
		ImGui::Checkbox(("Enabled"), &g_Options.Skinchanger.Enabled);
		ImGui::SameLine();
		ImGui::PushItemWidth(150);
		if (ImGui::Button(("Force Update")))
		{
			KnifeApplyCallbk();
		}


		ImGui::Separator();

		ImGui::Text("General");
		ImGui::Combo(("Knife Model"), &g_Options.Skinchanger.Knife, knives, ARRAYSIZE(knives));
		ImGui::Combo(("Knife Skin"), &g_Options.Skinchanger.KnifeSkin, knifeskins, ARRAYSIZE(knifeskins));
		ImGui::Combo(("Gloves"), &g_Options.Skinchanger.gloves, gloves, _ARRAYSIZE(gloves));


		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::Text("Rifles");
		ImGui::Columns(2, NULL, false);
		ImGui::Combo(("AK-47"), &g_Options.Skinchanger.AK47Skin, ak47, ARRAYSIZE(ak47));
		ImGui::Combo(("M4A1-S"), &g_Options.Skinchanger.M4A1SSkin, m4a1s, ARRAYSIZE(m4a1s));
		ImGui::Combo(("M4A4"), &g_Options.Skinchanger.M4A4Skin, m4a4, ARRAYSIZE(m4a4));
		ImGui::Combo(("Galil AR"), &g_Options.Skinchanger.GalilSkin, galil, ARRAYSIZE(galil));
		ImGui::NextColumn();
		ImGui::Combo(("AUG"), &g_Options.Skinchanger.AUGSkin, aug, ARRAYSIZE(aug));
		ImGui::Combo(("FAMAS"), &g_Options.Skinchanger.FAMASSkin, famas, ARRAYSIZE(famas));
		ImGui::Combo(("Sg553"), &g_Options.Skinchanger.Sg553Skin, sg553, ARRAYSIZE(sg553));

		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::Text("Snipers");
		ImGui::Columns(2, NULL, false);
		ImGui::Combo(("AWP"), &g_Options.Skinchanger.AWPSkin, awp, ARRAYSIZE(awp));
		ImGui::Combo(("SSG08"), &g_Options.Skinchanger.SSG08Skin, ssg08, ARRAYSIZE(ssg08));
		ImGui::NextColumn();
		ImGui::Combo(("SCAR20"), &g_Options.Skinchanger.SCAR20Skin, scar20, ARRAYSIZE(scar20));
		ImGui::Combo(("G3SG1"), &g_Options.Skinchanger.G3sg1Skin, g3sg1, ARRAYSIZE(g3sg1));

		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::Text("SMG's");
		ImGui::Columns(2, NULL, false);
		ImGui::Combo(("P90"), &g_Options.Skinchanger.P90Skin, p90, ARRAYSIZE(p90));
		ImGui::Combo(("MP7"), &g_Options.Skinchanger.Mp7Skin, mp7, ARRAYSIZE(mp7));
		ImGui::Combo(("MP9"), &g_Options.Skinchanger.Mp9Skin, mp9, ARRAYSIZE(mp9));
		ImGui::NextColumn();
		ImGui::Combo(("UMP45"), &g_Options.Skinchanger.UMP45Skin, ump45, ARRAYSIZE(ump45));
		ImGui::Combo(("MAC-10"), &g_Options.Skinchanger.Mac10Skin, mac10, ARRAYSIZE(mac10));
		ImGui::Combo(("PP-Bizon"), &g_Options.Skinchanger.BizonSkin, bizon, ARRAYSIZE(bizon));

		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::Text("Pistols");
		ImGui::Columns(2, NULL, false);
		ImGui::Combo(("Glock-18"), &g_Options.Skinchanger.GlockSkin, glock, ARRAYSIZE(glock));
		ImGui::Combo(("USP-S"), &g_Options.Skinchanger.USPSkin, usp, ARRAYSIZE(usp));
		ImGui::Combo(("Deagle"), &g_Options.Skinchanger.DeagleSkin, deagle, ARRAYSIZE(deagle));
		ImGui::Combo(("Five-Seven"), &g_Options.Skinchanger.FiveSkin, five, ARRAYSIZE(five));
		ImGui::Combo(("Revolver"), &g_Options.Skinchanger.RevolverSkin, revolver, ARRAYSIZE(revolver));

		ImGui::NextColumn();
		ImGui::Combo(("TEC-9"), &g_Options.Skinchanger.tec9Skin, tec9, ARRAYSIZE(tec9));
		ImGui::Combo(("P2000"), &g_Options.Skinchanger.P2000Skin, p2000, ARRAYSIZE(p2000));
		ImGui::Combo(("P250"), &g_Options.Skinchanger.P250Skin, p250, ARRAYSIZE(p250));
		ImGui::Combo(("Dual-Barettas"), &g_Options.Skinchanger.DualSkin, dual, ARRAYSIZE(dual));
		ImGui::Combo(("Cz75-Auto"), &g_Options.Skinchanger.Cz75Skin, cz75, ARRAYSIZE(cz75));

		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::Text("Shotguns");
		ImGui::Columns(2, NULL, false);
		ImGui::Combo(("Nova"), &g_Options.Skinchanger.NovaSkin, nova, ARRAYSIZE(nova));
		ImGui::Combo(("Sawed-Off"), &g_Options.Skinchanger.SawedSkin, sawed, ARRAYSIZE(sawed));

		ImGui::NextColumn();
		ImGui::Combo(("Mag-7"), &g_Options.Skinchanger.MagSkin, mag, ARRAYSIZE(mag));
		ImGui::Combo(("XM1014"), &g_Options.Skinchanger.XmSkin, xm, ARRAYSIZE(xm));

		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::Text("Machine Guns");
		ImGui::Columns(2, NULL, false);
		ImGui::Combo(("Negev"), &g_Options.Skinchanger.NegevSkin, negev, ARRAYSIZE(negev));

		ImGui::NextColumn();
		ImGui::Combo(("M249"), &g_Options.Skinchanger.M249Skin, m249, ARRAYSIZE(m249));
	}
}

