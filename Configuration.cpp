#include "Configuration.hpp"
#include "Variables.h"
#include <winerror.h>
#pragma warning( disable : 4091)
#include <ShlObj.h>
#include <string>
#include <sstream>


void CConfig::Setup()
{
	SetupValue(g_Options.Legitbot.MainSwitch, false, ("Legitbot"), ("LegitToggle"));
	SetupValue(g_Options.Legitbot.Aimbot.Enabled, false, ("Legitbot"), ("Enabled"));
	SetupValue(g_Options.Legitbot.backtrack, false, ("Legitbot"), ("Backtrack"));
	SetupValue(g_Options.Legitbot.backtrackTicks, 0, ("Legitbot"), ("BacktrackTicks"));

	SetupValue(g_Options.Legitbot.AutoPistol, false, ("Legitbot"), ("AutoPistol"));
	SetupValue(g_Options.Legitbot.MainKey, 1, ("Legitbot"), ("Key"));
	SetupValue(g_Options.Legitbot.Mainfov, 0.f, ("Legitbot"), ("FOV"));
	SetupValue(g_Options.Legitbot.MainSmooth, 1.f, ("Legitbot"), ("Speed"));
	SetupValue(g_Options.Legitbot.main_recoil_min, 0, ("Legitbot"), ("RCS-min"));
	SetupValue(g_Options.Legitbot.main_recoil_max, 0, ("Legitbot"), ("RCS-max"));

	SetupValue(g_Options.Legitbot.PistolKey, 6, ("Legitbot"), ("Key-Pistol"));
	SetupValue(g_Options.Legitbot.Pistolfov, 0.f, ("Legitbot"), ("FOV-Pistol"));
	SetupValue(g_Options.Legitbot.PistolSmooth, 1.f, ("Legitbot"), ("Speed-Pistol"));
	SetupValue(g_Options.Legitbot.pistol_recoil_min, 0, ("Legitbot"), ("RCS-min-pistol"));
	SetupValue(g_Options.Legitbot.pistol_recoil_max, 0, ("Legitbot"), ("RCS-max-pistol"));

	SetupValue(g_Options.Legitbot.SniperKey, 6, ("Legitbot"), ("Key-Sniper"));
	SetupValue(g_Options.Legitbot.Sniperfov, 0.f, ("Legitbot"), ("FOV-Sniper"));
	SetupValue(g_Options.Legitbot.SniperSmooth, 0.f, ("Legitbot"), ("Speed-Sniper"));
	SetupValue(g_Options.Legitbot.sniper_recoil_min, 0, ("Legitbot"), ("RCS-min-sniper"));
	SetupValue(g_Options.Legitbot.sniper_recoil_max, 0, ("Legitbot"), ("RCS-max-sniper"));

	SetupValue(g_Options.Legitbot.Triggerbot.Enabled, false, ("Legitbot"), ("TriggerBot"));
	SetupValue(g_Options.Legitbot.Triggerbot.Delay, 0.f, ("Legitbot"), ("TriggerBot delay"));
	SetupValue(g_Options.Legitbot.Triggerbot.Key, 6, ("Legitbot"), ("Key-Trigger"));
	SetupValue(g_Options.Legitbot.Triggerbot.hitchance, 0.f, ("LegitBot"), ("Trigger hitchance"));
	SetupValue(g_Options.Legitbot.Triggerbot.Filter.Head, false, ("LegitBot"), ("triggerHead"));
	SetupValue(g_Options.Legitbot.Triggerbot.Filter.Arms, false, ("LegitBot"), ("triggerArms"));
	SetupValue(g_Options.Legitbot.Triggerbot.Filter.Chest, false, ("LegitBot"), ("triggerChest"));
	SetupValue(g_Options.Legitbot.Triggerbot.Filter.Stomach, false, ("LegitBot"), ("triggerStomach"));
	SetupValue(g_Options.Legitbot.Triggerbot.Filter.Legs, false, ("LegitBot"), ("triggerLegs"));

	SetupValue(g_Options.Visuals.Skeleton, false, ("Visuals"), ("Skeleton"));
	SetupValue(g_Options.Visuals.HasDefuser, false, ("Visuals"), ("HasDefuser"));
	SetupValue(g_Options.Visuals.IsScoped, false, ("Visuals"), ("IsScoped"));
	SetupValue(g_Options.Visuals.Enabled, false, ("Visuals"), ("VisualsEnabled"));
	SetupValue(g_Options.Visuals.Box, false, ("Visuals"), ("Box"));
	SetupValue(g_Options.Visuals.Name, false, ("Visuals"), ("Name"));
	SetupValue(g_Options.Visuals.HP, false, ("Visuals"), ("HP"));
	SetupValue(g_Options.Visuals.Weapon, false, ("Visuals"), ("Weapon"));
	SetupValue(g_Options.Visuals.backtrackline, false, ("Visuals"), ("backtrackline"));
	SetupValue(g_Options.Visuals.Glow, false, ("Visuals"), ("Glow"));
	SetupValue(g_Options.Visuals.GrenadeESP, false, ("Visuals"), ("GranadeESP"));
	SetupValue(g_Options.Visuals.GrenadePrediction, false, ("Visuals"), ("Granade Prediction"));
	SetupValue(g_Options.Visuals.RecoilCrosshair, false, ("Visuals"), ("RecoilCrosshair"));
	SetupValue(g_Options.Visuals.SpreadCrosshair, false, ("Visuals"), ("SpreadCrosshair"));
	SetupValue(g_Options.Visuals.NoVisualRecoil, false, ("Visuals"), ("NoVisualRecoil"));
	SetupValue(g_Options.Visuals.FOVChanger, 0.f, ("Visuals"), ("fovchanger"));
	SetupValue(g_Options.Visuals.viewmodelChanger, 68.f, ("Visuals"), ("viewmodel_fov"));
	SetupValue(g_Options.Visuals.Time, false, ("Visuals"), ("Time"));
	SetupValue(g_Options.Visuals.DLight, false, ("Visuals"), ("DLight"));
	SetupValue(g_Options.Visuals.C4, false, ("Visuals"), ("C4"));
	SetupValue(g_Options.Visuals.money, false, ("Visuals"), ("Money"));
	SetupValue(g_Options.Visuals.NoFlash, false, ("Visuals"), ("NoFlash"));
	SetupValue(g_Options.Visuals.NoSmoke, false, ("Visuals"), ("NoSmoke"));
	SetupValue(g_Options.Visuals.noscopeborder, false, ("Visuals"), ("1tapNoScope360"));
	
	SetupValue(g_Options.Colors.hands_alpha, 1.f, ("Visuals"), ("HandsAlpha"));
	SetupValue(g_Options.Visuals.Hands, 0, ("Visuals"), ("Hands"));
	
	SetupValue(g_Options.Visuals.glowplayer, false, ("Visuals"), ("PlayerGlow"));
	SetupValue(g_Options.Visuals.teamglow, false, ("Visuals"), ("PlayerGlow Team"));
	SetupValue(g_Options.Visuals.playerglowalpha, 0, ("Visuals"), ("PlayerGlow Alpha"));
	SetupValue(g_Options.Visuals.weaponglow, false, ("Visuals"), ("WeaponGlow"));
	SetupValue(g_Options.Visuals.weaponglowalpha, 0, ("Visuals"), ("WeaponGlow Alpha"));
	SetupValue(g_Options.Visuals.bombglow, false, ("Visuals"), ("BombGlow"));
	SetupValue(g_Options.Visuals.bombglowalpha, 0, ("Visuals"), ("BombGlow Alpha"));

	SetupValue(g_Options.Colors.hands_color[0], 1.f, ("Colors"), ("HandsChams1Color"));
	SetupValue(g_Options.Colors.hands_color[1], 1.f, ("Colors"), ("HandsChams2Color"));
	SetupValue(g_Options.Colors.hands_color[2], 1.f, ("Colors"), ("HandsChams3Color"));

	SetupValue(g_Options.Visuals.Chams, false, ("Visuals"), ("PlayerChams"));
	SetupValue(g_Options.Visuals.Teamchams, false, ("Visuals"), ("Teamchams"));
	SetupValue(g_Options.Visuals.Chamweapon, false, ("Visuals"), ("WeaponChams"));
	SetupValue(g_Options.Visuals.XQZ, false, ("Visuals"), ("XQZ Chams"));
	SetupValue(g_Options.Visuals.champlayeralpha, 0, ("Visuals"), ("PlayerCham Alpha"));
	SetupValue(g_Options.Visuals.matierial, 0, ("Visuals"), ("PlayerCham Material"));
	SetupValue(g_Options.Visuals.weaponviewcham, false, ("Visuals"), ("Weapon Chams"));
	SetupValue(g_Options.Visuals.weaponhandalpha, 0, ("Visuals"), ("Weapon Chams Alpha"));
	
	SetupValue(g_Options.Colors.EnemyChamsNVis[0], 1.f, ("Colors"), ("EnemyChamsNVisRed"));
	SetupValue(g_Options.Colors.EnemyChamsNVis[1], 0.f, ("Colors"), ("EnemyChamsNVisGreen"));
	SetupValue(g_Options.Colors.EnemyChamsNVis[2], 0.f, ("Colors"), ("EnemyChamsNVisBlue"));

	SetupValue(g_Options.Colors.EnemyChamsVis[0], 0.f, ("Colors"), ("EnemyChamsVisRed"));
	SetupValue(g_Options.Colors.EnemyChamsVis[1], 1.f, ("Colors"), ("EnemyChamsVisGreen"));
	SetupValue(g_Options.Colors.EnemyChamsVis[2], 0.f, ("Colors"), ("EnemyChamsVisBlue"));
	
	SetupValue(g_Options.Colors.color_skeleton[0], 1.f, ("Colors"), ("Skeleton1Color"));
	SetupValue(g_Options.Colors.color_skeleton[1], 1.f, ("Colors"), ("Skeleton2Color"));
	SetupValue(g_Options.Colors.color_skeleton[2], 1.f, ("Colors"), ("Skeleton3Color"));
	
	SetupValue(g_Options.Colors.tracer_color[0], 0.f, ("Colors"), ("Tracer1Color"));
	SetupValue(g_Options.Colors.tracer_color[1], 1.f, ("Colors"), ("Tracer2Color"));
	SetupValue(g_Options.Colors.tracer_color[2], 0.f, ("Colors"), ("Tracer3Color"));
	
	SetupValue(g_Options.Colors.box_color_t[0], 1.f, ("Colors"), ("ColorBoxTR1Color"));
	SetupValue(g_Options.Colors.box_color_t[1], 0.f, ("Colors"), ("ColorBoxTR2Color"));
	SetupValue(g_Options.Colors.box_color_t[2], 0.f, ("Colors"), ("ColorBoxTR3Color"));

	SetupValue(g_Options.Colors.box_color_ct[0], 0.f, ("Colors"), ("ColorBoxCT1Color"));
	SetupValue(g_Options.Colors.box_color_ct[1], 0.f, ("Colors"), ("ColorBoxCT2Color"));
	SetupValue(g_Options.Colors.box_color_ct[2], 1.f, ("Colors"), ("ColorBoxCT3Color"));

	SetupValue(g_Options.Colors.color_spread[0], 0.f, ("Colors"), ("SpreadCrosshair1Color"));
	SetupValue(g_Options.Colors.color_spread[1], 1.f, ("Colors"), ("SpreadCrosshair2Color"));
	SetupValue(g_Options.Colors.color_spread[2], 0.f, ("Colors"), ("SpreadCrosshair3Color"));

	SetupValue(g_Options.Colors.color_recoil[0], 1.f, ("Colors"), ("RecoilCrosshair1Color"));
	SetupValue(g_Options.Colors.color_recoil[1], 0.f, ("Colors"), ("RecoilCrosshair2Color"));
	SetupValue(g_Options.Colors.color_recoil[2], 0.f, ("Colors"), ("RecoilCrosshair3Color"));

	SetupValue(g_Options.Colors.dlight_color[0], 0.f, ("Colors"), ("DynamicLight1Color"));
	SetupValue(g_Options.Colors.dlight_color[1], 1.f, ("Colors"), ("DynamicLight2Color"));
	SetupValue(g_Options.Colors.dlight_color[2], 0.f, ("Colors"), ("DynamicLight3Color"));

	SetupValue(g_Options.Colors.hitmark_color[0], 0.f, ("Colors"), ("hitmarker1Color"));
	SetupValue(g_Options.Colors.hitmark_color[1], 1.f, ("Colors"), ("hitmarker2Color"));
	SetupValue(g_Options.Colors.hitmark_color[2], 0.f, ("Colors"), ("hitmarker3Color"));

	SetupValue(g_Options.Colors.backtrackdots_color[0], 1.f, ("Colors"), ("backtrackdots1Color"));
	SetupValue(g_Options.Colors.backtrackdots_color[1], 0.f, ("Colors"), ("backtrackdots2Color"));
	SetupValue(g_Options.Colors.backtrackdots_color[2], 0.f, ("Colors"), ("backtrackdots3Color"));

	SetupValue(g_Options.Visuals.tpdist, false, ("Visuals"), ("thirdperson distance"));
	SetupValue(g_Options.Visuals.TPKey, false, ("Visuals"), ("thirdperson key"));
	SetupValue(g_Options.Visuals.ThirdPerson, false, ("Visuals"), ("thirdperson switch"));

	SetupValue(g_Options.Misc.fps, false, ("Visuals"), ("fpsboost"));
	SetupValue(g_Options.Misc.Gray, false, ("Visuals"), ("GrayMode"));
	SetupValue(g_Options.Misc.crack, false, ("Visuals"), ("lsdMode"));
	SetupValue(g_Options.Misc.chromatic, false, ("Visuals"), ("ChromeMode"));

	SetupValue(g_Options.Visuals.Filter.Players, false, ("Visuals"), ("Players"));
	SetupValue(g_Options.Visuals.Filter.EnemyOnly, false, ("Visuals"), ("EnemyOnly"));
	SetupValue(g_Options.Visuals.WeaponsWorld, false, ("Visuals"), ("WeaponsWorld"));
	SetupValue(g_Options.Visuals.C4World, false, ("Visuals"), ("C4World"));
	SetupValue(g_Options.Visuals.Flashed, false, ("Visuals"), ("EnemyState"));

	SetupValue(g_Options.Misc.Hitmarker, false, ("Misc"), ("Hitmarker"));
	SetupValue(g_Options.Misc.hitsound, 0, ("Misc"), ("Hitmarker Sound"));

	SetupValue(g_Options.Misc.Skybox, 0, ("Misc"), ("Skybox"));
	SetupValue(g_Options.Misc.Watermark, true, ("Misc"), ("Watermark"));
	SetupValue(g_Options.Menu.Theme, 0, ("themes"), ("themes"));
	SetupValue(g_Options.Misc.AutoStrafe, false, ("Misc"), ("AutoStafe"));
	SetupValue(g_Options.Misc.nightMode, false, ("Misc"), ("NightMode"));
	SetupValue(g_Options.Misc.Bhop, false, ("Misc"), ("Bhop"));
	SetupValue(g_Options.Misc.ServerRankRevealAll, false, ("Misc"), ("Reveal Ranks"));
	SetupValue(g_Options.Misc.AutoAccept, false, ("Misc"), ("AutoAccept"));
	SetupValue(g_Options.Misc.bought, false, ("Misc"), ("Show enemies purchases"));
	SetupValue(g_Options.Misc.SpecList, false, ("Misc"), ("SpecList"));
	SetupValue(g_Options.Misc.Radar, false, ("Misc"), ("Radar"));

	SetupValue(g_Options.Skinchanger.Enabled, false, ("Skinchanger"), ("Enabled"));
	SetupValue(g_Options.Skinchanger.Knife, 0, ("SkinChanger"), ("Knife"));
	SetupValue(g_Options.Skinchanger.KnifeSkin, 0, ("SkinChanger"), ("KnifeSkin"));
	SetupValue(g_Options.Skinchanger.gloves, 0, ("SkinChanger"), ("gloves"));

	SetupValue(g_Options.Skinchanger.AK47Skin, 0, ("SkinChanger"), ("AK47Skin"));
	SetupValue(g_Options.Skinchanger.M4A1SSkin, 0, ("SkinChanger"), ("M4A1SSkin"));
	SetupValue(g_Options.Skinchanger.M4A4Skin, 0, ("SkinChanger"), ("M4A4Skin"));
	SetupValue(g_Options.Skinchanger.AUGSkin, 0, ("SkinChanger"), ("AUGSkin"));
	SetupValue(g_Options.Skinchanger.FAMASSkin, 0, ("SkinChanger"), ("FAMASSkin"));
	SetupValue(g_Options.Skinchanger.GalilSkin, 0, ("SkinChanger"), ("GalilSkin"));
	SetupValue(g_Options.Skinchanger.Sg553Skin, 0, ("SkinChanger"), ("Sg553Skin"));

	SetupValue(g_Options.Skinchanger.AWPSkin, 0, ("SkinChanger"), ("AWPSkin"));
	SetupValue(g_Options.Skinchanger.SSG08Skin, 0, ("SkinChanger"), ("SSG08Skin"));
	SetupValue(g_Options.Skinchanger.SCAR20Skin, 0, ("SkinChanger"), ("SCAR20Skin"));
	SetupValue(g_Options.Skinchanger.G3sg1Skin, 0, ("SkinChanger"), ("G3sg1Skin"));

	SetupValue(g_Options.Skinchanger.P90Skin, 0, ("SkinChanger"), ("P90Skin"));
	SetupValue(g_Options.Skinchanger.UMP45Skin, 0, ("SkinChanger"), ("UMP45Skin"));
	SetupValue(g_Options.Skinchanger.Mp7Skin, 0, ("SkinChanger"), ("Mp7Skin"));
	SetupValue(g_Options.Skinchanger.Mac10Skin, 0, ("SkinChanger"), ("Mac10Skin"));
	SetupValue(g_Options.Skinchanger.Mp9Skin, 0, ("SkinChanger"), ("Mp9Skin"));
	SetupValue(g_Options.Skinchanger.BizonSkin, 0, ("SkinChanger"), ("BizonSkin"));

	SetupValue(g_Options.Skinchanger.GlockSkin, 0, ("SkinChanger"), ("GlockSkin"));
	SetupValue(g_Options.Skinchanger.USPSkin, 0, ("SkinChanger"), ("USPSkin"));
	SetupValue(g_Options.Skinchanger.DeagleSkin, 0, ("SkinChanger"), ("DeagleSkin"));
	SetupValue(g_Options.Skinchanger.RevolverSkin, 0, ("SkinChanger"), ("RevolverSkin"));
	SetupValue(g_Options.Skinchanger.DualSkin, 0, ("SkinChanger"), ("DualSkin"));

	SetupValue(g_Options.Skinchanger.MagSkin, 0, ("SkinChanger"), ("MagSkin"));
	SetupValue(g_Options.Skinchanger.NovaSkin, 0, ("SkinChanger"), ("NovaSkin"));
	SetupValue(g_Options.Skinchanger.SawedSkin, 0, ("SkinChanger"), ("SawedSkin"));
	SetupValue(g_Options.Skinchanger.XmSkin, 0, ("SkinChanger"), ("XmSkin"));

	SetupValue(g_Options.Skinchanger.Cz75Skin, 0, ("SkinChanger"), ("Cz75Skin"));
	SetupValue(g_Options.Skinchanger.tec9Skin, 0, ("SkinChanger"), ("tec9Skin"));
	SetupValue(g_Options.Skinchanger.P2000Skin, 0, ("SkinChanger"), ("P2000Skin"));
	SetupValue(g_Options.Skinchanger.P250Skin, 0, ("SkinChanger"), ("P250Skin"));
	SetupValue(g_Options.Skinchanger.FiveSkin, 0, ("SkinChanger"), ("FiveSkin"));

	SetupValue(g_Options.Skinchanger.NegevSkin, 0, ("SkinChanger"), ("NegevSkin"));
	SetupValue(g_Options.Skinchanger.M249Skin, 0, ("SkinChanger"), ("M249Skin"));
}

void CConfig::SetupValue(int &value, int def, std::string category, std::string name)
{
	value = def;
	ints.push_back(new ConfigValue<int>(category, name, &value));
}

void CConfig::SetupValue(float &value, float def, std::string category, std::string name)
{
	value = def;
	floats.push_back(new ConfigValue<float>(category, name, &value));
}

void CConfig::SetupValue(bool &value, bool def, std::string category, std::string name)
{
	value = def;
	bools.push_back(new ConfigValue<bool>(category, name, &value));
}

void CConfig::Save()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path)))
	{


		folder = std::string(path) + ("\\s1x44\\");
		switch (g_Options.Menu.ConfigFile)
		{
		case 0:
			file = std::string(path) + ("\\s1x44\\legit.ini");
			break;
		case 1:
			file = std::string(path) + ("\\s1x44\\legit2.ini");
			break;
		case 2:
			file = std::string(path) + ("\\s1x44\\rage.ini");
			break;
		case 3:
			file = std::string(path) + ("\\s1x44\\other.ini");
			break;


		}
	}

	CreateDirectory(folder.c_str(), NULL);

	for (auto value : ints)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : floats)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), std::to_string(*value->value).c_str(), file.c_str());

	for (auto value : bools)
		WritePrivateProfileString(value->category.c_str(), value->name.c_str(), *value->value ? "true" : "false", file.c_str());
}

void CConfig::Load()
{
	static TCHAR path[MAX_PATH];
	std::string folder, file;

	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, 0, path)))
	{
		folder = std::string(path) + ("\\s1x44\\");
		switch (g_Options.Menu.ConfigFile)
		{
		case 0:
			file = std::string(path) + ("\\s1x44\\legit.ini");
			break;
		case 1:
			file = std::string(path) + ("\\s1x44\\legit2.ini");
			break;
		case 2:
			file = std::string(path) + ("\\s1x44\\rage.ini");
			break;
		case 3:
			file = std::string(path) + ("\\s1x44\\other.ini");
			break;
		}
	}

	CreateDirectory(folder.c_str(), NULL);

	char value_l[32] = { '\0' };

	for (auto value : ints)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = atoi(value_l);
	}

	for (auto value : floats)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = (float)atof(value_l);
	}

	for (auto value : bools)
	{
		GetPrivateProfileString(value->category.c_str(), value->name.c_str(), "", value_l, 32, file.c_str());
		*value->value = !strcmp(value_l, "true");
	}
}


CConfig* Config = new CConfig();
Variables g_Options;

namespace junk6783084 {
	void junk10405709() {
		return;
		float uuwkpa;
		float t9i6y;
		float sadjgo;
		float jvrf6;
		float m6ypu5;
		float jzhjv;
		float lsdum9;
		float ic0k3;
		float fjbqyt;
		float jt3p2;
		while (jt3p2 == 8022147.3092)
		{
			jt3p2 = 626939.6361;
		}
		fjbqyt = 889981.5504;
		uuwkpa = 7769658.2165;
		while (sadjgo == 8692217.0950)
		{
			sadjgo = 7265347.0142;
		}
		sadjgo = 985994.0859;
		lsdum9 = 8581265.3070;
		if (jzhjv == 10528566.5815)
			jzhjv = 6045555.8067;
		jt3p2 = 8529980.9211;
		if (lsdum9 == 8166901.7247)
			lsdum9 = 850477.9634;
		fjbqyt = 7060792.6905;
		if (fjbqyt == 973446.2684)
			fjbqyt = 6842775.5905;
		lsdum9 = 4090296.2802;
		if (jzhjv == 6974741.7106)
			jzhjv = 9214891.8038;
		ic0k3 = 6869053.4500;
		m6ypu5 = 10102425.6314;
		while (uuwkpa == 2647391.5573)
		{
			uuwkpa = 9655455.2323;
		}
		uuwkpa = 8223758.6809;
	}
	void junk4963896() {
		return;
		float zk2aar;
		float eoeuj;
		float y01bmym;
		float zi23j;
		float i2hv09;
		float v0q6l3;
		float nm0dut;
		float ijjvli;
		float ofrfkd;
		float nwpe8;
		while (v0q6l3 == 7000439.4449)
		{
			v0q6l3 = 3014020.9161;
		}
		ijjvli = 9073920.3318;
		if (v0q6l3 == 6106756.2141)
			v0q6l3 = 9176090.7518;
		i2hv09 = 1512185.3227;
		for (int sos2vj = 0; sos2vj > 100; sos2vj++)
		{
			y01bmym = 4989164.9691;
		}
		ofrfkd = 2413187.5091;
		nm0dut = 6618993.9217;
		for (int qdsvqp = 0; qdsvqp > 100; qdsvqp++)
		{
			zi23j = 4483118.9858;
		}
		ofrfkd = 7229987.7755;
		if (ofrfkd == 3760675.1931)
			ofrfkd = 4170918.8562;
		zi23j = 5835287.7440;
		if (i2hv09 == 1613138.9394)
			i2hv09 = 6191054.9182;
		nm0dut = 7090923.0389;
		v0q6l3 = 6801189.0862;
		for (int ncn5k = 0; ncn5k > 100; ncn5k++)
		{
			zk2aar = 1966369.9226;
		}
		zk2aar = 2920367.8117;
		while (y01bmym == 6284195.3516)
		{
			y01bmym = 9260602.4024;
		}
		ijjvli = 9065086.0244;
	}
	void junk6328666() {
		return;
		float f8c9zk;
		float jm953f;
		float kocqec;
		float wjhke;
		float rlasjl;
		float bt0o4l;
		float sp44k;
		float kywo5a;
		float kixwt;
		float u6nn05;
		for (int sq0495 = 0; sq0495 > 100; sq0495++)
		{
			kywo5a = 4137742.2988;
		}
		jm953f = 2197226.1510;
		if (kixwt == 7609092.6695)
			kixwt = 9042823.5759;
		sp44k = 3861141.7623;
		if (wjhke == 10520257.0486)
			wjhke = 9353991.2479;
		u6nn05 = 7119032.5822;
		if (jm953f == 2894687.7267)
			jm953f = 9786566.5183;
		sp44k = 8845941.1267;
		if (kocqec == 6360789.9422)
			kocqec = 3576234.4710;
		u6nn05 = 2229882.6540;
		if (kixwt == 1607131.5568)
			kixwt = 2058176.2298;
		rlasjl = 455245.3425;
		for (int sbts99 = 0; sbts99 > 100; sbts99++)
		{
			kocqec = 5530743.4184;
		}
		bt0o4l = 3438171.9410;
		for (int bct3utg = 0; bct3utg > 100; bct3utg++)
		{
			bt0o4l = 8980018.4319;
		}
		jm953f = 7559552.2276;
		while (f8c9zk == 5182314.8355)
		{
			f8c9zk = 8983612.4195;
		}
		rlasjl = 6886418.5638;
		while (f8c9zk == 7712478.4618)
		{
			f8c9zk = 7328756.0187;
		}
		kixwt = 6148208.8670;
	}
	void junk8581361() {
		return;
		float enwi4;
		float i7cse8;
		float pv39n;
		float wfxxkh;
		float gvyh4;
		float aaaqoc;
		float vxoq5f;
		float dhotfm;
		float hjjxhy;
		float fgdn2e;
		while (pv39n == 5679981.9674)
		{
			pv39n = 1559305.5165;
		}
		gvyh4 = 1892504.4802;
		if (wfxxkh == 6273812.5504)
			wfxxkh = 5571452.9435;
		dhotfm = 7044822.7098;
		if (fgdn2e == 6924520.6232)
			fgdn2e = 8048150.4835;
		pv39n = 714205.2275;
		pv39n = 2299686.7962;
		for (int iv2han = 0; iv2han > 100; iv2han++)
		{
			dhotfm = 1440096.0344;
		}
		i7cse8 = 5753562.9818;
		for (int j0ikts = 0; j0ikts > 100; j0ikts++)
		{
			enwi4 = 1331211.7617;
		}
		fgdn2e = 307967.3652;
		if (fgdn2e == 888995.2744)
			fgdn2e = 9013860.0823;
		vxoq5f = 10323498.6919;
		if (vxoq5f == 5638223.3765)
			vxoq5f = 9727179.0350;
		pv39n = 6098872.5279;
		if (fgdn2e == 10457251.9943)
			fgdn2e = 4989936.8190;
		aaaqoc = 8975521.1551;
		while (enwi4 == 9261865.7542)
		{
			enwi4 = 9451747.9960;
		}
		pv39n = 8220596.2255;
	}
	void junk580188() {
		return;
		float b4navt;
		float qr2l5o;
		float m31olq;
		float ks3zr;
		float at7uz;
		float p5hm8g;
		float tih5p;
		float tw5j6j;
		float jlpvw;
		float zy0tag;
		for (int eholta = 0; eholta > 100; eholta++)
		{
			zy0tag = 10523868.7535;
		}
		at7uz = 3252826.1197;
		for (int ex1nf = 0; ex1nf > 100; ex1nf++)
		{
			at7uz = 5439302.2132;
		}
		tih5p = 9888195.7289;
		while (qr2l5o == 2722036.2270)
		{
			qr2l5o = 7027804.0337;
		}
		tih5p = 8005930.9262;
		if (tw5j6j == 4256816.2470)
			tw5j6j = 1209440.9184;
		m31olq = 4724604.3165;
		if (m31olq == 4114502.4150)
			m31olq = 3794831.7609;
		zy0tag = 7245935.8919;
		zy0tag = 6148589.4347;
		if (b4navt == 7642861.5448)
			b4navt = 5247097.8725;
		zy0tag = 6389998.7734;
		while (qr2l5o == 10159502.6967)
		{
			qr2l5o = 1872460.0892;
		}
		qr2l5o = 10015259.5760;
		ks3zr = 7906191.8049;
		if (ks3zr == 10191746.1367)
			ks3zr = 5986134.1407;
		zy0tag = 297713.3595;
	}
	void junk912397() {
		return;
		float dnkoko;
		float w032sa;
		float x9oq29;
		float cpk0ed;
		float ywheq;
		float u01qwr;
		float wafte;
		float vpv347;
		float sitc8r;
		float cs8iti;
		wafte = 3284141.0329;
		while (wafte == 1560319.9291)
		{
			wafte = 7176902.2057;
		}
		wafte = 10414109.2817;
		sitc8r = 2603750.2374;
		if (wafte == 3133146.3262)
			wafte = 926115.3271;
		wafte = 5584701.0652;
		for (int l2hqex = 0; l2hqex > 100; l2hqex++)
		{
			ywheq = 6663923.6171;
		}
		wafte = 8457490.8204;
		for (int eft8su = 0; eft8su > 100; eft8su++)
		{
			sitc8r = 5501624.0409;
		}
		ywheq = 6487317.3537;
		vpv347 = 4829641.6270;
		if (u01qwr == 2063833.5048)
			u01qwr = 7792376.0672;
		cpk0ed = 1016956.1219;
		for (int yswmqp = 0; yswmqp > 100; yswmqp++)
		{
			ywheq = 3829447.9416;
		}
		dnkoko = 5650567.9365;
		for (int x19zz = 0; x19zz > 100; x19zz++)
		{
			vpv347 = 2552725.0313;
		}
		wafte = 3586582.9383;
	}
	void junk5541023() {
		return;
		float cr0jqh;
		float dxrmgl;
		float pmkizs;
		float e97hlg;
		float ag14yz;
		float m908v;
		float v9fih;
		float r4skso;
		float x8ddpo;
		float gwoe4;
		while (v9fih == 2985306.0010)
		{
			v9fih = 5376260.3643;
		}
		x8ddpo = 5244241.3461;
		dxrmgl = 4144575.5216;
		while (cr0jqh == 5088800.5374)
		{
			cr0jqh = 8332049.1586;
		}
		ag14yz = 6172669.7254;
		for (int rxltjo = 0; rxltjo > 100; rxltjo++)
		{
			cr0jqh = 2333642.1065;
		}
		ag14yz = 9796187.2668;
		x8ddpo = 1909943.9365;
		dxrmgl = 2029826.7047;
		if (pmkizs == 6819439.2720)
			pmkizs = 2061088.5874;
		ag14yz = 5412133.3905;
		ag14yz = 2527632.4029;
		for (int fv7kmg = 0; fv7kmg > 100; fv7kmg++)
		{
			x8ddpo = 6377558.2182;
		}
		x8ddpo = 7584158.1843;
		for (int maqo4g = 0; maqo4g > 100; maqo4g++)
		{
			gwoe4 = 4649249.1722;
		}
		e97hlg = 5391091.7250;
	}
	void junk4679009() {
		return;
		float o2dqmh;
		float r85oh8;
		float eh07u;
		float iec8x;
		float dtfw4k;
		float sd00hc;
		float hsnelv;
		float vka8u9;
		float fd1n34;
		float akxfq;
		if (akxfq == 7883471.0737)
			akxfq = 10024440.2505;
		vka8u9 = 2616551.3629;
		if (r85oh8 == 7643829.8175)
			r85oh8 = 6128028.1438;
		eh07u = 6030803.9961;
		for (int japt78 = 0; japt78 > 100; japt78++)
		{
			o2dqmh = 5923287.6603;
		}
		r85oh8 = 5059151.1179;
		if (vka8u9 == 8468964.1130)
			vka8u9 = 4225822.3674;
		iec8x = 767101.4487;
		eh07u = 4613166.8075;
		while (r85oh8 == 7320762.8455)
		{
			r85oh8 = 6230750.2326;
		}
		eh07u = 4432158.3810;
		for (int x72bwn = 0; x72bwn > 100; x72bwn++)
		{
			dtfw4k = 5334743.4034;
		}
		fd1n34 = 5993006.0329;
		for (int g2i1gv = 0; g2i1gv > 100; g2i1gv++)
		{
			r85oh8 = 2412906.2866;
		}
		eh07u = 7744713.1135;
		while (fd1n34 == 5012567.5205)
		{
			fd1n34 = 9579292.9238;
		}
		hsnelv = 6769937.7719;
		o2dqmh = 3388878.6373;
	}
	void junk1865498() {
		return;
		float qptu46;
		float wba7zt;
		float tnrcyk;
		float jv6x3;
		float a4r6jo;
		float ff1wv;
		float n18voa;
		float fyi5i9;
		float czkfj8;
		float ktuz5;
		fyi5i9 = 6572829.7233;
		for (int ek8j3w = 0; ek8j3w > 100; ek8j3w++)
		{
			qptu46 = 1062424.9411;
		}
		czkfj8 = 9540317.5427;
		fyi5i9 = 4927841.3230;
		for (int osc1un = 0; osc1un > 100; osc1un++)
		{
			ktuz5 = 8097073.3254;
		}
		tnrcyk = 7098739.9423;
		tnrcyk = 6705989.1535;
		for (int k23bsx = 0; k23bsx > 100; k23bsx++)
		{
			a4r6jo = 7769335.0427;
		}
		fyi5i9 = 10090127.2238;
		while (czkfj8 == 5161934.9069)
		{
			czkfj8 = 730266.9864;
		}
		czkfj8 = 9427067.2769;
		if (qptu46 == 7463945.3374)
			qptu46 = 2893006.9461;
		czkfj8 = 8768437.5388;
		if (ktuz5 == 8818029.4004)
			ktuz5 = 9431945.2716;
		a4r6jo = 9946729.8923;
		n18voa = 8345586.3412;
	}
	void junk9346024() {
		return;
		float vwnsu;
		float kpoxj;
		float y1jux;
		float s8pm2;
		float k6o4g;
		float poz38p;
		float ko1ks8;
		float uti4lm;
		float mrr1bu;
		float g5xcrf;
		if (y1jux == 6604057.5886)
			y1jux = 6531482.1002;
		y1jux = 8523441.3283;
		while (k6o4g == 6347717.7089)
		{
			k6o4g = 104905.7356;
		}
		k6o4g = 3138422.3944;
		for (int u9tuw5 = 0; u9tuw5 > 100; u9tuw5++)
		{
			ko1ks8 = 3492346.9183;
		}
		k6o4g = 8367636.2285;
		g5xcrf = 2036884.3317;
		if (poz38p == 9591980.5868)
			poz38p = 7376611.5401;
		g5xcrf = 2276738.3856;
		for (int nkxqov = 0; nkxqov > 100; nkxqov++)
		{
			mrr1bu = 8349994.7127;
		}
		g5xcrf = 9947293.3669;
		if (kpoxj == 10285089.3959)
			kpoxj = 3983551.0109;
		k6o4g = 5586938.6511;
		if (uti4lm == 3270653.3023)
			uti4lm = 8344499.0906;
		ko1ks8 = 4968012.2038;
		y1jux = 5913136.1463;
		if (y1jux == 6942326.1456)
			y1jux = 9474560.6728;
		uti4lm = 9387272.1701;
	}
	void doJunk() {
		junk6783084::junk10405709();
		junk6783084::junk4963896();
		junk6783084::junk6328666();
		junk6783084::junk8581361();
		junk6783084::junk580188();
		junk6783084::junk912397();
		junk6783084::junk5541023();
		junk6783084::junk4679009();
		junk6783084::junk1865498();
		junk6783084::junk9346024();
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class akrpmde {
public:
	int hgcjjlbtbv;
	string hemnslrfsqkutxo;
	int ljyfuigjcftqcmm;
	bool ypnkb;
	akrpmde();
	bool kszuefnwfcfmsgnhqdne(bool dlkzdaxzgej, int ekdjymklzot, string jrdmoqgtl, bool cptzcpcbryrpwas, double lmjxmbag);

protected:
	bool czjxblyawjktf;
	double tiwaj;
	bool ttflpfrpotrhu;
	int pziiceibusolycs;

	double yvulobejjfbuuj(double issnh, bool uyqxsz, double udtikg, string uttag, double yairixhwhxnekcb);
	void jdlpnwpjnkyjbzjmezfoc(string vcavjpylviv, double kzqoywoi, double rvzhftzeg, string fxocxmwv, string hdfsfxvsecs, double monuppwznpdcso, bool kotpn, string cckngpgjaz, int nthkqyvueqxbhd, double rmfpyvgbmgqvohp);
	double unlhjlpdes(string dkvkai, double pdhrsatqtwcyes);
	int onixgkmeqydfmhyf(double nwezrtb, int czuhrmcfpigalvl, string reawzaqp, double xcsofy, bool pcmkrtba, double onnfmpzmh, int bvnur);
	void ucjbhblrdgwmedpau(int boywvvi, bool zsoxrzqnfail);
	int pjnwrpnhalcv();
	double rhpmrcvbqfcmhbribrtlz(string mdaryrytdzvxusu, string kyxgx, bool nffhern);

private:
	double rwismkgxvmat;
	string zckcwityaiia;
	double nctfpyy;

	void cvjrvtnpdnfzcnffxiqqx(int kirfe, string kaqyuoni, double jqmfxlhp);
	int avujvndtei(string vvbwbqdyqiqjgoh, double jhqbbfzgkxrrp, string wsbpkqqirpheqr, double gfidjssy, double oqfxmkdiiax, int dffofzavzw);
	void pehpzovfsd(double rutvvczrdgmy, bool jiyvkexcduush, string qlblvtzyd);
	void claltapojsltbi();
	void ydaqwkyjihtx(string saoblomoi, int fixqfuyuexsdxiu, int ymaymmey, double yrtkvvh, bool iezqhkvog, bool veoculcxnl, int wsmlgpsh, string nifpr);
	int yteptcinmuunm(string mftzi, string zyioxggghtil, string uplyotzreujdzl, double lgxgiug);
	double efxdkqivnbhoadpqmzxlxow(double bdfgyt, string tebtfihkqmm);
	double rgjnsaacxtqdopplllyaopz(int pudmelkqz);

};


void akrpmde::cvjrvtnpdnfzcnffxiqqx(int kirfe, string kaqyuoni, double jqmfxlhp) {

}

int akrpmde::avujvndtei(string vvbwbqdyqiqjgoh, double jhqbbfzgkxrrp, string wsbpkqqirpheqr, double gfidjssy, double oqfxmkdiiax, int dffofzavzw) {
	return 16288;
}

void akrpmde::pehpzovfsd(double rutvvczrdgmy, bool jiyvkexcduush, string qlblvtzyd) {
	int rhqqueb = 2483;
	int sifspqtwdlno = 293;
	int vlxdcsrf = 800;
	bool pkifmwdlbrswnwv = true;
	bool wepckhmswbjrp = true;
	double zegkktqfwljc = 17857;
	int swemzauafaaiidg = 2702;
	bool lhaxgoiogtj = false;
	if (2702 == 2702) {
		int jysslzocu;
		for (jysslzocu = 95; jysslzocu > 0; jysslzocu--) {
			continue;
		}
	}
	if (2483 == 2483) {
		int ijnu;
		for (ijnu = 63; ijnu > 0; ijnu--) {
			continue;
		}
	}
	if (17857 == 17857) {
		int jwek;
		for (jwek = 75; jwek > 0; jwek--) {
			continue;
		}
	}

}

void akrpmde::claltapojsltbi() {
	int qngqskvu = 1877;
	string dbxawnoenlgjwi = "qggnkjdaxywhasnnqczsohioerywfolaqtjcnicvkqmzzntayyvpnotxaobwjcrgfosiqfkqkpfmhwbrrdcsmmz";
	int hbkhlbboft = 1325;
	double xjpvgjref = 67797;
	double imcaiesviralz = 1620;
	bool gheda = false;
	if (false == false) {
		int uutvgegju;
		for (uutvgegju = 21; uutvgegju > 0; uutvgegju--) {
			continue;
		}
	}

}

void akrpmde::ydaqwkyjihtx(string saoblomoi, int fixqfuyuexsdxiu, int ymaymmey, double yrtkvvh, bool iezqhkvog, bool veoculcxnl, int wsmlgpsh, string nifpr) {
	string ppiuvxbboxitvjr = "saffclhqhqupcbstjqlh";
	bool crlndab = false;
	string yqnilgx = "xtgitxqyzpownpopjfziaavdgvlnkaduxnroidovmftfzqzon";
	bool kjgyikvc = true;
	if (string("saffclhqhqupcbstjqlh") != string("saffclhqhqupcbstjqlh")) {
		int incg;
		for (incg = 10; incg > 0; incg--) {
			continue;
		}
	}

}

int akrpmde::yteptcinmuunm(string mftzi, string zyioxggghtil, string uplyotzreujdzl, double lgxgiug) {
	int agugclfzyceph = 4156;
	string baclbybcbomamw = "jfzxcrzcggkomvowhwrijzzyijbygkkclxladlpixstnkgwvlkveknqcrnhjnx";
	bool jqwgkfqbnfvh = true;
	int ogfktsrvb = 1066;
	if (4156 == 4156) {
		int xgpwjbl;
		for (xgpwjbl = 44; xgpwjbl > 0; xgpwjbl--) {
			continue;
		}
	}
	if (1066 == 1066) {
		int dfht;
		for (dfht = 42; dfht > 0; dfht--) {
			continue;
		}
	}
	return 83687;
}

double akrpmde::efxdkqivnbhoadpqmzxlxow(double bdfgyt, string tebtfihkqmm) {
	string kxfbpqotmxwyn = "driuvroovloclugknhlghahlmxlcapwrqjedouldrxjytgkbysnmiuudtzwxbxmyzjxvopuiaakppujmldalqckwac";
	int cehdhirogt = 2521;
	int tuqzyvkufznfhgi = 3636;
	if (3636 == 3636) {
		int nrpyjt;
		for (nrpyjt = 82; nrpyjt > 0; nrpyjt--) {
			continue;
		}
	}
	if (string("driuvroovloclugknhlghahlmxlcapwrqjedouldrxjytgkbysnmiuudtzwxbxmyzjxvopuiaakppujmldalqckwac") == string("driuvroovloclugknhlghahlmxlcapwrqjedouldrxjytgkbysnmiuudtzwxbxmyzjxvopuiaakppujmldalqckwac")) {
		int vqzfqbt;
		for (vqzfqbt = 48; vqzfqbt > 0; vqzfqbt--) {
			continue;
		}
	}
	if (3636 != 3636) {
		int ucvihfzyi;
		for (ucvihfzyi = 45; ucvihfzyi > 0; ucvihfzyi--) {
			continue;
		}
	}
	if (string("driuvroovloclugknhlghahlmxlcapwrqjedouldrxjytgkbysnmiuudtzwxbxmyzjxvopuiaakppujmldalqckwac") != string("driuvroovloclugknhlghahlmxlcapwrqjedouldrxjytgkbysnmiuudtzwxbxmyzjxvopuiaakppujmldalqckwac")) {
		int vsenodjxce;
		for (vsenodjxce = 21; vsenodjxce > 0; vsenodjxce--) {
			continue;
		}
	}
	if (2521 == 2521) {
		int coxpkzvshn;
		for (coxpkzvshn = 86; coxpkzvshn > 0; coxpkzvshn--) {
			continue;
		}
	}
	return 91432;
}

double akrpmde::rgjnsaacxtqdopplllyaopz(int pudmelkqz) {
	string ozfhppxprfk = "hfemkpwmseocepmiadp";
	int rhcxvrusvfnh = 747;
	bool xpegurcjax = false;
	int dtqyzxghbcc = 4646;
	double mpaqsersyhaq = 11156;
	bool ebriwpxhenfffwh = true;
	int fljsensluy = 825;
	double lzkthchraownv = 25377;
	string ybzdfinlxexxb = "zrbgdvzqpvyldlbrsagnguvfjlhazhcxndcbivvilfpmzgqjricxkvnvsnfrxelypibjykazmkgosdellnsmowb";
	double osqpgwhkkqvthnu = 5775;
	if (5775 == 5775) {
		int dslvqrnilv;
		for (dslvqrnilv = 55; dslvqrnilv > 0; dslvqrnilv--) {
			continue;
		}
	}
	if (11156 != 11156) {
		int lvlvfiphf;
		for (lvlvfiphf = 89; lvlvfiphf > 0; lvlvfiphf--) {
			continue;
		}
	}
	if (true == true) {
		int zay;
		for (zay = 23; zay > 0; zay--) {
			continue;
		}
	}
	return 82074;
}

double akrpmde::yvulobejjfbuuj(double issnh, bool uyqxsz, double udtikg, string uttag, double yairixhwhxnekcb) {
	return 51582;
}

void akrpmde::jdlpnwpjnkyjbzjmezfoc(string vcavjpylviv, double kzqoywoi, double rvzhftzeg, string fxocxmwv, string hdfsfxvsecs, double monuppwznpdcso, bool kotpn, string cckngpgjaz, int nthkqyvueqxbhd, double rmfpyvgbmgqvohp) {

}

double akrpmde::unlhjlpdes(string dkvkai, double pdhrsatqtwcyes) {
	int qgyupoe = 131;
	double rjudphuillw = 19003;
	bool qidprmkfxdns = false;
	int jihatuo = 254;
	bool dshrspxskr = true;
	int jirwasyvexb = 1030;
	bool etapxaxywgu = true;
	if (1030 == 1030) {
		int pxczdxo;
		for (pxczdxo = 32; pxczdxo > 0; pxczdxo--) {
			continue;
		}
	}
	if (1030 != 1030) {
		int tsinru;
		for (tsinru = 75; tsinru > 0; tsinru--) {
			continue;
		}
	}
	if (false != false) {
		int spmg;
		for (spmg = 51; spmg > 0; spmg--) {
			continue;
		}
	}
	if (true == true) {
		int txmxqrnw;
		for (txmxqrnw = 34; txmxqrnw > 0; txmxqrnw--) {
			continue;
		}
	}
	if (1030 != 1030) {
		int yq;
		for (yq = 95; yq > 0; yq--) {
			continue;
		}
	}
	return 22524;
}

int akrpmde::onixgkmeqydfmhyf(double nwezrtb, int czuhrmcfpigalvl, string reawzaqp, double xcsofy, bool pcmkrtba, double onnfmpzmh, int bvnur) {
	return 87326;
}

void akrpmde::ucjbhblrdgwmedpau(int boywvvi, bool zsoxrzqnfail) {
	int wznbxrjzciylnmd = 3791;
	if (3791 != 3791) {
		int sthdxifl;
		for (sthdxifl = 63; sthdxifl > 0; sthdxifl--) {
			continue;
		}
	}
	if (3791 == 3791) {
		int pgjjnslke;
		for (pgjjnslke = 33; pgjjnslke > 0; pgjjnslke--) {
			continue;
		}
	}
	if (3791 == 3791) {
		int bhjgog;
		for (bhjgog = 96; bhjgog > 0; bhjgog--) {
			continue;
		}
	}
	if (3791 != 3791) {
		int samcz;
		for (samcz = 62; samcz > 0; samcz--) {
			continue;
		}
	}
	if (3791 != 3791) {
		int mcyvhls;
		for (mcyvhls = 6; mcyvhls > 0; mcyvhls--) {
			continue;
		}
	}

}

int akrpmde::pjnwrpnhalcv() {
	bool smjqkqyz = false;
	int vdtixhqkps = 4555;
	double kkicmzjbjc = 15982;
	string qfwplpkldwfzbkr = "mfyhvllqfpcmsbskhhjayuanxkbfqfwpbafkzxlmirvdtyzfogukjsfdbpzorqfstfbvi";
	int mruolhrdnk = 1705;
	int tpmljvyjaobggi = 721;
	bool uljepcx = true;
	if (1705 == 1705) {
		int guzcofrttc;
		for (guzcofrttc = 9; guzcofrttc > 0; guzcofrttc--) {
			continue;
		}
	}
	if (false != false) {
		int hforsl;
		for (hforsl = 15; hforsl > 0; hforsl--) {
			continue;
		}
	}
	if (1705 == 1705) {
		int fbndhym;
		for (fbndhym = 65; fbndhym > 0; fbndhym--) {
			continue;
		}
	}
	if (false == false) {
		int yasmcuzva;
		for (yasmcuzva = 22; yasmcuzva > 0; yasmcuzva--) {
			continue;
		}
	}
	return 96276;
}

double akrpmde::rhpmrcvbqfcmhbribrtlz(string mdaryrytdzvxusu, string kyxgx, bool nffhern) {
	int bahaqk = 163;
	bool dfuhctmmkxbia = false;
	int umoroyhucdi = 4454;
	double jqfybmvmoks = 47446;
	double hwlqk = 6906;
	string gpdphfhilbya = "lhf";
	if (false != false) {
		int owvdx;
		for (owvdx = 66; owvdx > 0; owvdx--) {
			continue;
		}
	}
	if (6906 != 6906) {
		int obhunmumh;
		for (obhunmumh = 18; obhunmumh > 0; obhunmumh--) {
			continue;
		}
	}
	return 9564;
}

bool akrpmde::kszuefnwfcfmsgnhqdne(bool dlkzdaxzgej, int ekdjymklzot, string jrdmoqgtl, bool cptzcpcbryrpwas, double lmjxmbag) {
	bool bvlkw = true;
	string epixos = "ubgghgplbyvqjufluokzucuondtwkhbzmgypxdcebststwgbfqoelrjybmysumajxdrbgniaeyxjllonkbxfywb";
	double kpajfc = 50438;
	string jogkymw = "kwcwecnccffbktglyscyzrrrwz";
	string lswwgcdlhf = "nwdaxmjxqmpxskgfldndavyjdwwblzgochdiswnamguqssogekosfefjiiearcx";
	double epomekc = 49161;
	if (string("nwdaxmjxqmpxskgfldndavyjdwwblzgochdiswnamguqssogekosfefjiiearcx") != string("nwdaxmjxqmpxskgfldndavyjdwwblzgochdiswnamguqssogekosfefjiiearcx")) {
		int bol;
		for (bol = 83; bol > 0; bol--) {
			continue;
		}
	}
	if (string("nwdaxmjxqmpxskgfldndavyjdwwblzgochdiswnamguqssogekosfefjiiearcx") == string("nwdaxmjxqmpxskgfldndavyjdwwblzgochdiswnamguqssogekosfefjiiearcx")) {
		int mvlrpkdulb;
		for (mvlrpkdulb = 36; mvlrpkdulb > 0; mvlrpkdulb--) {
			continue;
		}
	}
	if (string("kwcwecnccffbktglyscyzrrrwz") == string("kwcwecnccffbktglyscyzrrrwz")) {
		int wtexhrbpx;
		for (wtexhrbpx = 61; wtexhrbpx > 0; wtexhrbpx--) {
			continue;
		}
	}
	if (50438 == 50438) {
		int glbwfxujqj;
		for (glbwfxujqj = 45; glbwfxujqj > 0; glbwfxujqj--) {
			continue;
		}
	}
	return false;
}

akrpmde::akrpmde() {
	this->kszuefnwfcfmsgnhqdne(false, 2717, string("auffzhzgdqwpaufvhrynwlkdiknphmhkkz"), false, 5006);
	this->yvulobejjfbuuj(10502, false, 9439, string("qtbxt"), 5834);
	this->jdlpnwpjnkyjbzjmezfoc(string("tpsiriqddgkglkzjympmizhfqparzzfjxpndkkipunafdhdrveimxtnmqegvqwkntoqvoyyehbkeeeqxxpwndnoohyiwkenq"), 12058, 23954, string("irgbucxfkraivcsfxdqaznkqjnxxcjtufike"), string("oohfjqithlrmbpwncildstodhiiantgschibvzxtbyqgyxoxghmnvzwfqntmc"), 85635, true, string("ppisvfgambfslrspldqakemtbfffzoaawckyjdroezxisporgobfkdhwhpkagozrsfxlwakuyblrteqejolsedygtbfjxatym"), 2601, 64657);
	this->unlhjlpdes(string("eehuduoripxzbxiccrnjbbeierjmstigrkvikwnoikccheimfclbydq"), 11813);
	this->onixgkmeqydfmhyf(24205, 1546, string("turxngidjtgzoinlqicqmyrnzwufwhqwvclvydnjptoyyqpvcdhcewcpheaxkbmrcjfqkgjeyfrfmqxqhrtpo"), 18886, true, 14008, 1091);
	this->ucjbhblrdgwmedpau(2604, false);
	this->pjnwrpnhalcv();
	this->rhpmrcvbqfcmhbribrtlz(string("pdhaytmjelqzenwhpkvkqwnihdbvufydyrxhrxxopkzzuhwebsqygjhczzewxhlsflcqzbmaaupupjhjba"), string("mopgm"), false);
	this->cvjrvtnpdnfzcnffxiqqx(6837, string("omcdczgwcxjaypchbepdrdmhgqtgmayoodhb"), 16370);
	this->avujvndtei(string("hwsvwweqzrcpdjtzxdxddqgfsxjucvvylhxsxfnbcxvkaaffnugd"), 67226, string("tbljuhqmuyeoyzlailbavbbkomiryifxzgcrmsxxrvormujzowletajfwatfsasutvpcapjsmaozhiu"), 32817, 10186, 1186);
	this->pehpzovfsd(8505, false, string("pjnunuwjimffysbbybuexpemnxcakfbxxacneqbwcqtnnzartstgtpeejugtssnmivfrfiaiteioa"));
	this->claltapojsltbi();
	this->ydaqwkyjihtx(string("kzpfdradvgzkvctjhdhlydudipnmgapumgkhejbhnqrayqapqsrwqflquxzlqjaxbaxeglntvwxwmkhbmzusistqssavpquqcun"), 3353, 2584, 78476, false, false, 1575, string("dteixrcpxxnntqhdzuzynurdvmbjkbfiwiijxtpzyacmlgniauqgtpusdqdlysexqzhabrbjsuddevyldkdcrrcuh"));
	this->yteptcinmuunm(string("zyadljugntsimhmtq"), string("cgzmthorspbueyjhxwbtoukftxdszxkakvciroetncnpxqjdgpl"), string("uqoiintryoqfcmqthvzpb"), 24375);
	this->efxdkqivnbhoadpqmzxlxow(46377, string("riiqpsclbpsphzvwpumggziyssnjmzympamfqyylaa"));
	this->rgjnsaacxtqdopplllyaopz(1449);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class qzqglyx {
public:
	int jnyzvfpft;
	qzqglyx();
	string yygqlaydodpepq(string nedzxyvbne, string ltqpzbuaanmpjub, string sgtxfdilyqxcp, double zczmvevg, bool ygfdkopd, bool ffvpcmr, double brejaaoaymge, string efbypyhhlagiksu);
	double dhwpupuaguuwszqv(bool rfrsgtfzckcjfpf, bool babkwczhlcrjaqn, bool dfbjtbrhxtycndu);
	int xjyioqiawblpuotwdyp(double xhozx, bool ycezejavvmiyy, bool tjjrosvosbeiv, bool kunqecfiowz, double yvifj, bool cfnyulloerkvka, string bwooqhedkaeih, bool wlohzjzgrlona);
	int dysgfsxciq(string sdvlwlfvhwp, int srxsh, double kfasyswsm, int gickcttaowed, int jmnwgn, string vkbolqgoamsqon, string adcajr, double tetgffsxzgkgp, bool ucqgvm, int xvlwuxmwytm);
	int snldtwupkvkd(bool tvuovctord, bool urcggh, double hswcjlu, double jbvvrjzjevkx, string bblrwtoji, string rdprft, double lnzqrbvmc, string naessgcmymndcs, bool ytdcgnronmalvy, bool ldwqdvgknfsd);
	string lctjdklewuyohhwocxvyjhwoe(int ytbmuaejxno);

protected:
	double pnmejt;
	string jgocphxtvy;
	int xhpvvfoufazvwmu;
	bool nqqoujpqmai;
	double iumrrmf;

	void rjjsewxuhuz(bool glundnahuqxx);
	double eqculoivngpusu(double uxwrabnxveq, string toddlcqehebfs, bool toidozrca, int opgbamjrzss, string bnudenzyurkh, double uisen);

private:
	bool cxshrxckblob;
	bool kfrigdl;
	int ruizfyougada;
	string yqhehiouihfjwo;
	bool epqdsrxjftswer;

	double qfcmvxfdxajaid(string mnmulnp, int kgtcd, string twusjtxlutbsray, int xlexabolgnfkuek, string ahhxsighyf);
	string ubirrmmislocjgkq(double baqwuwxvpnavz, string dkerdogajcposmd, double obqzav, double ksyzeztcouobxh, bool pkitkvorqc, int xpgdka, string tsomlf, string dqrykp, double mskznnu);
	string hwvnngmwqaaapn(double ishcxjjl, int nziho, string lrtvpvpexbvmx, double eqrnfnxld, string icbnaje);
	int zkxdkfuijx(string cwysgsrhphomqm, string euejft, bool eiimeqxj, int tpyzkjtk, double tthor, double ilbtiakujof, double gcyyu, bool jkemsdulsfp, double ktsmsydtzt);

};


double qzqglyx::qfcmvxfdxajaid(string mnmulnp, int kgtcd, string twusjtxlutbsray, int xlexabolgnfkuek, string ahhxsighyf) {
	double maxsszysawgq = 3051;
	bool jzfkrrhwbccy = false;
	int yjydwwyvlj = 788;
	bool rezrimuqvwist = true;
	int sztwypavcbuwq = 3843;
	int anaffcs = 351;
	if (788 != 788) {
		int fx;
		for (fx = 69; fx > 0; fx--) {
			continue;
		}
	}
	if (351 != 351) {
		int joaocjtjx;
		for (joaocjtjx = 64; joaocjtjx > 0; joaocjtjx--) {
			continue;
		}
	}
	if (3843 != 3843) {
		int srtwv;
		for (srtwv = 30; srtwv > 0; srtwv--) {
			continue;
		}
	}
	if (3051 != 3051) {
		int rhwrrusezi;
		for (rhwrrusezi = 63; rhwrrusezi > 0; rhwrrusezi--) {
			continue;
		}
	}
	return 35991;
}

string qzqglyx::ubirrmmislocjgkq(double baqwuwxvpnavz, string dkerdogajcposmd, double obqzav, double ksyzeztcouobxh, bool pkitkvorqc, int xpgdka, string tsomlf, string dqrykp, double mskznnu) {
	double xdpnyllzznw = 23567;
	int wggnhmxbmv = 3206;
	bool wtspzrfgrh = true;
	string rysxkrcl = "ptdkvegetsqvhqpcletgcnbeygezsbcflubwykjlycaraosumito";
	bool tpwbpqzgn = false;
	double bvklxgfpvr = 51535;
	int tsmrbj = 2177;
	if (string("ptdkvegetsqvhqpcletgcnbeygezsbcflubwykjlycaraosumito") == string("ptdkvegetsqvhqpcletgcnbeygezsbcflubwykjlycaraosumito")) {
		int cqaak;
		for (cqaak = 2; cqaak > 0; cqaak--) {
			continue;
		}
	}
	if (2177 == 2177) {
		int lvqe;
		for (lvqe = 28; lvqe > 0; lvqe--) {
			continue;
		}
	}
	if (true != true) {
		int zisfcpi;
		for (zisfcpi = 67; zisfcpi > 0; zisfcpi--) {
			continue;
		}
	}
	if (false != false) {
		int cotge;
		for (cotge = 95; cotge > 0; cotge--) {
			continue;
		}
	}
	return string("");
}

string qzqglyx::hwvnngmwqaaapn(double ishcxjjl, int nziho, string lrtvpvpexbvmx, double eqrnfnxld, string icbnaje) {
	int jcmksdjio = 3411;
	string rtsmrnqtat = "mcnajmksvohczssd";
	int lrdltioqgfccthz = 1981;
	bool cqzwbxrtkwvadge = true;
	double oxwekesf = 8018;
	int vfogrmarrp = 1708;
	string qcfshdl = "zgqdvrizfuvsmhqmtazufvddjqqwzyagarhwujfsjjfalnfzlgggjpusjqawkkylgcjflolayrytoavramcztgaimmz";
	string adynnbnfegb = "bozwibanktowkxkfkrmipic";
	int rbsslkwsxq = 6700;
	double vwgizzedowvnum = 66453;
	return string("filgtjgqtdrpcblwtgp");
}

int qzqglyx::zkxdkfuijx(string cwysgsrhphomqm, string euejft, bool eiimeqxj, int tpyzkjtk, double tthor, double ilbtiakujof, double gcyyu, bool jkemsdulsfp, double ktsmsydtzt) {
	int puzhhjhfcpdtu = 1576;
	int mozzphfbosomy = 2436;
	bool yzgkfzciuda = false;
	if (false != false) {
		int rdp;
		for (rdp = 66; rdp > 0; rdp--) {
			continue;
		}
	}
	if (2436 == 2436) {
		int yvecyb;
		for (yvecyb = 16; yvecyb > 0; yvecyb--) {
			continue;
		}
	}
	if (2436 == 2436) {
		int udcdxjg;
		for (udcdxjg = 10; udcdxjg > 0; udcdxjg--) {
			continue;
		}
	}
	if (1576 != 1576) {
		int bg;
		for (bg = 89; bg > 0; bg--) {
			continue;
		}
	}
	return 39038;
}

void qzqglyx::rjjsewxuhuz(bool glundnahuqxx) {
	bool dtaumtq = true;
	string fevjm = "ztvbjqtojejenqwwt";
	int nyhcuujzem = 5061;

}

double qzqglyx::eqculoivngpusu(double uxwrabnxveq, string toddlcqehebfs, bool toidozrca, int opgbamjrzss, string bnudenzyurkh, double uisen) {
	int dahwo = 7289;
	if (7289 == 7289) {
		int wfbsxmgw;
		for (wfbsxmgw = 5; wfbsxmgw > 0; wfbsxmgw--) {
			continue;
		}
	}
	if (7289 != 7289) {
		int nrpwmfchp;
		for (nrpwmfchp = 45; nrpwmfchp > 0; nrpwmfchp--) {
			continue;
		}
	}
	return 38339;
}

string qzqglyx::yygqlaydodpepq(string nedzxyvbne, string ltqpzbuaanmpjub, string sgtxfdilyqxcp, double zczmvevg, bool ygfdkopd, bool ffvpcmr, double brejaaoaymge, string efbypyhhlagiksu) {
	bool mfuvimveiq = false;
	string ekaamgftnzl = "xiyrtnoumkrsvdsdfiwukutxpwefrhaivgiyahhwoaezczfixcjhbchfmzyjgpf";
	string zjafh = "udnkvcqooetjnprmnudrvfkbbovgqlwblinjvyyjgvaapmewpwqkapqdkksczsjavjjleaglmvfflob";
	string vvbakurtuxeww = "nfpbpiihzwgi";
	if (string("udnkvcqooetjnprmnudrvfkbbovgqlwblinjvyyjgvaapmewpwqkapqdkksczsjavjjleaglmvfflob") == string("udnkvcqooetjnprmnudrvfkbbovgqlwblinjvyyjgvaapmewpwqkapqdkksczsjavjjleaglmvfflob")) {
		int yam;
		for (yam = 90; yam > 0; yam--) {
			continue;
		}
	}
	return string("mjaczmysbtvhhc");
}

double qzqglyx::dhwpupuaguuwszqv(bool rfrsgtfzckcjfpf, bool babkwczhlcrjaqn, bool dfbjtbrhxtycndu) {
	bool pqshsrt = false;
	string pgbbhaoin = "qsqyswuyzncqzdvungrry";
	bool jeemohgdyg = false;
	if (string("qsqyswuyzncqzdvungrry") == string("qsqyswuyzncqzdvungrry")) {
		int heuojfn;
		for (heuojfn = 65; heuojfn > 0; heuojfn--) {
			continue;
		}
	}
	return 13958;
}

int qzqglyx::xjyioqiawblpuotwdyp(double xhozx, bool ycezejavvmiyy, bool tjjrosvosbeiv, bool kunqecfiowz, double yvifj, bool cfnyulloerkvka, string bwooqhedkaeih, bool wlohzjzgrlona) {
	double fxewxz = 14808;
	double ghskfmsmxprmxr = 50797;
	bool wjfiuzn = true;
	bool zxyrzxerqf = false;
	bool cpbuirujq = false;
	string fxvkmy = "xxsxeddenawmq";
	double fnexkey = 76046;
	if (false != false) {
		int avfjr;
		for (avfjr = 36; avfjr > 0; avfjr--) {
			continue;
		}
	}
	return 73385;
}

int qzqglyx::dysgfsxciq(string sdvlwlfvhwp, int srxsh, double kfasyswsm, int gickcttaowed, int jmnwgn, string vkbolqgoamsqon, string adcajr, double tetgffsxzgkgp, bool ucqgvm, int xvlwuxmwytm) {
	int qsxrqozajkppck = 926;
	string tbvklxitofzvjlb = "unrycroukqmeqqtdnawvihnjnckjnamoscmlrarpqxdgbltkjzvvxryxxaepfmbheshnowh";
	bool rzysv = true;
	bool cijdyl = true;
	int jtfmmkewqrjiwm = 4668;
	string hizrgun = "celnhjcsxlzlmcyfzclwyzubnpdadtpc";
	bool qigapcozj = true;
	bool hpivutvubskkua = false;
	if (4668 == 4668) {
		int atbhes;
		for (atbhes = 31; atbhes > 0; atbhes--) {
			continue;
		}
	}
	return 58149;
}

int qzqglyx::snldtwupkvkd(bool tvuovctord, bool urcggh, double hswcjlu, double jbvvrjzjevkx, string bblrwtoji, string rdprft, double lnzqrbvmc, string naessgcmymndcs, bool ytdcgnronmalvy, bool ldwqdvgknfsd) {
	double ipyrkgsvu = 15266;
	bool uyptxpmkifywool = false;
	bool psdrm = false;
	string lgwnhvjfk = "kbtwrgrehwidlfwydurarhlwofiggrmtonneoncuhgbnaztpbojsqftjurawprlhgcwvbueeydvoo";
	string sdomyxpojaed = "aorltyvnvfudiqihfuxiisubydsnyreuklnufyzaglyzmgfhsxwqqsphyjhvdscvfqzrikfekreylmwgsobe";
	int rmcil = 3870;
	double ycfoogw = 19261;
	string spnisiw = "dbxcxxvjdqeyauqrtxynjjzbwfajdvwfwrhkxyqluo";
	bool wclbfxidoevld = true;
	if (false == false) {
		int gw;
		for (gw = 36; gw > 0; gw--) {
			continue;
		}
	}
	if (3870 == 3870) {
		int qh;
		for (qh = 42; qh > 0; qh--) {
			continue;
		}
	}
	if (false != false) {
		int mqilthf;
		for (mqilthf = 67; mqilthf > 0; mqilthf--) {
			continue;
		}
	}
	return 73464;
}

string qzqglyx::lctjdklewuyohhwocxvyjhwoe(int ytbmuaejxno) {
	return string("blaewtjde");
}

qzqglyx::qzqglyx() {
	this->yygqlaydodpepq(string("ksyguquuqhthkdwsreadnmiv"), string("ldpszdunjqbhcfxyjvkduxglabghujaomtcvgrxkkaczhrufnufmojphernzwjaqhprmgvanxaunhd"), string("mjwthbwqpsptzkqvciglxtgxzevrurwceyfuuzjtengas"), 39338, true, true, 36055, string("trmawyjczwalbt"));
	this->dhwpupuaguuwszqv(true, true, true);
	this->xjyioqiawblpuotwdyp(59450, true, true, true, 1984, true, string("vceamsaijinbsnmqjeeqdrjnawszlwgowwbmxbwhtthozuppkbxnpndpbcrnukzpjlyxzsjyurlaewzuxronaw"), false);
	this->dysgfsxciq(string("aamfktpuymbnxo"), 4616, 11255, 895, 720, string("qyeezllxxbvfzkbrzdlgoqexpfqibbnvxwwijvqedlqynowmjmbfgfivlsrsrvhdpkizgqiaueeurswjeenaindrta"), string("yxvaejnlljjydgqkwoibzpbdevjnqpbgdyddqhukdeghhnhyihqlbakkaloayrzer"), 21244, false, 5305);
	this->snldtwupkvkd(true, true, 73972, 41756, string("mlfhpfohpnohosdrwe"), string("emizmzlwphdncakuvvufxqmwikjcpvdgjdiqikvy"), 1911, string("lqfbfdkkxpqfujfypronqcrbspaccskxspjzrwfastwxsgjyvadadkarmxkaxviov"), false, true);
	this->lctjdklewuyohhwocxvyjhwoe(7013);
	this->rjjsewxuhuz(false);
	this->eqculoivngpusu(21373, string("tuzlqnzmylxecddpcrcyejjccxdidfncautocifbofxzxdghxbbystchtzcahqlozyjmfombavucyqwev"), false, 1941, string("wcbvrbcjvcbofwkunwadhexxttyfkbybrtrwjlfulapvoehkpp"), 29167);
	this->qfcmvxfdxajaid(string("tnnkyitznfcayffdnfyuwmuzamaccnpmcbktmsmdszaqjreeqprnexmeggpqndvmsqkdplemfzizlsfohzmccapsobuo"), 8230, string("kuhijozdirjpqbouiyigkouohghfdoeoyymkvxrbrapdh"), 2527, string("bcvgibwwiuqukbdpivfdghcozyazchkesfzyxatuchuvrdmsabvvuz"));
	this->ubirrmmislocjgkq(11465, string("pzwpgizlrmrneblcszahbsffghptsxbgaewcqtciyumynywmcwcmkw"), 64989, 2697, false, 3736, string("vgxpbctajvstreehfwpsxpalzrinkrdanbvcsjbyywduynmvoaqcmeeeghozyfexfcmn"), string("uavdyluettrppiovudlpqsmqzgsomtttwcyqhahmjdp"), 74813);
	this->hwvnngmwqaaapn(18756, 3322, string("tklpljyrickzn"), 12361, string("tkntkmjodlntpuwkbklthponyzpkjtimsawgjhdtvfsekhlqpqgsphdzqevrxcguzellubshvgxcywvfyic"));
	this->zkxdkfuijx(string("ks"), string("ozgsuwukckcofbyuehndbfjcexntimnojiifztthblpmrhkkjagtyhxsfz"), false, 1946, 23977, 51142, 31502, true, 36303);
}
