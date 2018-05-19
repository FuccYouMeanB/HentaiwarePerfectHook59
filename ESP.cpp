
#include "ESP.h"
#include "Interfaces.h"
#include "Render.h"
#include <ctime>
#include <iostream>
#include <algorithm>
#include "GrenadePrediction.h"
#include "LagComp.h"
#include "Autowall.h"


visuals::visuals()
{
	BombCarrier = nullptr;
}

int width = 0;
int height = 0;
bool done = false;
void visuals::OnPaintTraverse(C_BaseEntity* local)
{


    for (int i = 0; i < g_EntityList->GetHighestEntityIndex(); i++)
    {

        C_BaseEntity *entity = g_EntityList->GetClientEntity(i);
        player_info_t pinfo;
        if (entity == local && local->IsAlive() && g_Engine->GetPlayerInfo(g_Engine->GetLocalPlayer(), &pinfo))
        {
            if (g_Input->m_fCameraInThirdPerson && g_Options.Visuals.Enabled)
            {
                Vector max = entity->GetCollideable()->OBBMaxs();
                Vector pos, pos3D;
                Vector top, top3D;
                pos3D = entity->GetOrigin();
                top3D = pos3D + Vector(0, 0, max.z);

                if (!g_Render->WorldToScreen(pos3D, pos) || !g_Render->WorldToScreen(top3D, top))
                    return;

                float height = (pos.y - top.y);
                float width = height / 4.f;
                if (g_Options.Visuals.Box)
                {
                    Color color;
                    color = GetPlayerColor(entity, local);
                    PlayerBox(top.x, top.y, width, height, color);
                }
                if (g_Options.Visuals.HP)
                    DrawHealth(pos, top, local->GetHealth());

                if (g_Options.Visuals.Name)
                    g_Render->DrawString2(g_Render->font.ESP, (int)top.x, (int)top.y - 6, Color::White(), FONT_CENTER, pinfo.name);

            }
        }
        if (entity && entity != local && !entity->IsDormant())
        {
            if (g_Engine->GetPlayerInfo(i, &pinfo) && entity->IsAlive())
            {
                if (g_Options.Visuals.backtrackline)
				{
					if (local->IsAlive())
					{
						for (int t = 0; t < g_Options.Legitbot.backtrackTicks; ++t)
						{
							Vector screenbacktrack[64][12];

							if (headPositions[i][t].simtime && headPositions[i][t].simtime + 1 > local->GetSimulationTime())
							{
								if (g_Render->WorldToScreen(headPositions[i][t].hitboxPos, screenbacktrack[i][t]))
								{

									g_Surface->DrawSetColor(Color(int(g_Options.Colors.backtrackdots_color[0] * 255.f), int(g_Options.Colors.backtrackdots_color[1] * 255.f), int(g_Options.Colors.backtrackdots_color[2] * 255.f)));
									g_Surface->DrawOutlinedRect(screenbacktrack[i][t].x, screenbacktrack[i][t].y, screenbacktrack[i][t].x + 2, screenbacktrack[i][t].y + 2);

								}
							}
						}
					}
                    else
                    {
                        memset(&headPositions[0][0], 0, sizeof(headPositions));
                    }
                }
                if (g_Options.Ragebot.FakeLagFix)
                {
                    if (local->IsAlive())
                    {
                        Vector screenbacktrack[64];

                        if (backtracking->records[i].tick_count + 12 > g_Globals->tickcount)
                        {
                            if (g_Render->WorldToScreen(backtracking->records[i].headPosition, screenbacktrack[i]))
                            {

                                g_Surface->DrawSetColor(Color::Red());
                                g_Surface->DrawOutlinedRect(screenbacktrack[i].x, screenbacktrack[i].y, screenbacktrack[i].x + 2, screenbacktrack[i].y + 2);

                            }
                        }
                    }
                    else
                    {
                        memset(&backtracking->records[0], 0, sizeof(backtracking->records));
                    }
                }
                if (g_Options.Visuals.Enabled && g_Options.Visuals.Filter.Players)
                {
                    if (g_Options.Visuals.DLight)
                        DLight(local, entity);

                    DrawPlayer(entity, pinfo, local);

                }
				if (g_Options.Visuals.DrawAwall && local->IsAlive())
					DrawAwall();

            }
            if (g_Options.Visuals.Enabled)
            {
                ClientClass* cClass = (ClientClass*)entity->GetClientClass();
                if (g_Options.Visuals.WeaponsWorld && cClass->m_ClassID != (int)ClassID::CBaseWeaponWorldModel && ((strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == (int)ClassID::CDEagle || cClass->m_ClassID == (int)ClassID::CAK47)))
                {
                    DrawDrop(entity);
                }
				if (g_Options.Visuals.WeaponsWorldicon && cClass->m_ClassID != (int)ClassID::CBaseWeaponWorldModel && ((strstr(cClass->m_pNetworkName, "Weapon") || cClass->m_ClassID == (int)ClassID::CDEagle || cClass->m_ClassID == (int)ClassID::CAK47)))
				{
					DrawDropicon(entity);
				}
                if (g_Options.Visuals.C4World)
                {
                    if (cClass->m_ClassID == (int)ClassID::CPlantedC4)
                        DrawBombPlanted(entity, local);
                }

                if (cClass->m_ClassID == (int)ClassID::CC4)
                    DrawBomb(entity, cClass);
                if (g_Options.Visuals.GrenadeESP && strstr(cClass->m_pNetworkName, "Projectile"))
                {
                    DrawThrowable(entity);
                }

				if (g_Options.Misc.Radar)
				{
					DWORD m_bSpotted = offsetz.DT_BaseEntity.m_bSpotted;
					*(char*)((DWORD)(entity)+m_bSpotted) = 1;
				}


            }
        }
    }

    if (g_Options.Misc.SpecList) SpecList(local);
    
	if (g_Options.Misc.Hitmarker)
		Hitmarker();

	

    NightMode();
	grenade_prediction::instance().Paint();

    if (g_Options.Visuals.SpreadCrosshair)
    {
        g_Engine->GetScreenSize(width, height);
        if (local  && local->IsAlive())
        {
            static Vector ViewAngles;
            g_Engine->GetViewAngles(ViewAngles);
            ViewAngles += local->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

            static Vector fowardVec;
            AngleVectors(ViewAngles, &fowardVec);
            fowardVec *= 10000;

            // Get ray start / end
            Vector start = local->GetOrigin() + local->GetViewOffset();
            Vector end = start + fowardVec, endScreen;

            CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(local->GetActiveWeaponHandle());
            float cone = pWeapon->GetSpread() + pWeapon->GetInaccuracy();
            if (cone > 0.0f)
            {
                if (cone < 0.01f) cone = 0.01f;
                float size = (cone * height) * 0.7f;
                Color color(int(g_Options.Colors.color_spread[0] * 255.f), int(g_Options.Colors.color_spread[1] * 255.f), int(g_Options.Colors.color_spread[2] * 255.f));

                if (g_Render->WorldToScreen(end, endScreen))
                {
                    g_Render->OutlineCircle(endScreen.x, endScreen.y, (int)size, 48, color);
                }
            }
        }
    }
	if (g_Options.Visuals.RecoilCrosshair)
	{
		g_Engine->GetScreenSize(width, height);
		if (local && local->IsAlive())
		{
			static Vector ViewAngles;
			g_Engine->GetViewAngles(ViewAngles);
			ViewAngles += local->localPlayerExclusive()->GetAimPunchAngle() * 2.f;

			Vector fowardVec;
			AngleVectors(ViewAngles, &fowardVec);
			fowardVec *= 10000;

			Vector start = local->GetOrigin() + local->GetViewOffset();
			Vector end = start + fowardVec, endScreen;

			if (g_Render->WorldToScreen(end, endScreen) && local->IsAlive())
			{
				g_Render->Line(endScreen.x - 4, endScreen.y, endScreen.x + 4, endScreen.y, Color(int(g_Options.Colors.color_recoil[0] * 255.f), int(g_Options.Colors.color_recoil[1] * 255.f), int(g_Options.Colors.color_recoil[2] * 255.f)));
				g_Render->Line(endScreen.x, endScreen.y - 4, endScreen.x, endScreen.y + 4, Color(int(g_Options.Colors.color_recoil[0] * 255.f), int(g_Options.Colors.color_recoil[1] * 255.f), int(g_Options.Colors.color_recoil[2] * 255.f)));
			}
		}
	}
}

std::string CleanItemName(std::string name)
{
	std::string Name = name;
	// Tidy up the weapon Name
	if (Name[0] == 'C')
		Name.erase(Name.begin());

	// Remove the word Weapon
	auto startOfWeap = Name.find("Weapon");
	if (startOfWeap != std::string::npos)
		Name.erase(Name.begin() + startOfWeap, Name.begin() + startOfWeap + 6);

	return Name;
}

wchar_t* CharToWideChar(const char* text)
{
	size_t size = strlen(text) + 1;
	wchar_t* wa = new wchar_t[size];
	mbstowcs_s(NULL, wa, size / 4, text, size);
	return wa;
}


C_CSPlayerResource* playerresources;
void visuals::DrawPlayer(C_BaseEntity* entity, player_info_t pinfo, C_BaseEntity* local)
{

	Vector max = entity->GetCollideable()->OBBMaxs();
	Vector pos, pos3D;
	Vector top, top3D;
	pos3D = entity->GetOrigin();
	top3D = pos3D + Vector(0, 0, max.z);

	if (!g_Render->WorldToScreen(pos3D, pos) || !g_Render->WorldToScreen(top3D, top))
		return;

	float height = (pos.y - top.y);
	float width = height / 4.f;

	Color color;

	if (g_Options.Visuals.Filter.EnemyOnly && (entity->GetTeamNum() == local->GetTeamNum()))
		return;
	color = GetPlayerColor(entity, local);

	if (g_Options.Visuals.Box)
		PlayerBox(top.x, top.y, width, height, color);

	if (g_Options.Visuals.HP)
		DrawHealth(pos, top, entity->GetHealth());

	if (g_Options.Visuals.Name)
		g_Render->DrawString2(g_Render->font.ESP, (int)top.x, (int)top.y - 6, Color::White(), FONT_CENTER, pinfo.name);

	if (g_Options.Visuals.Skeleton)
		DrawSkeleton(entity);

	int bottom = 0;

    /*auto m_AnimOverlay = entity->GetAnimationOverlay();

    for (int i = 0; i < 14; i++)
    {
        char sequence_string[64];
        sprintf_s(sequence_string, sizeof(sequence_string) - 1, "%d | %d", i, m_AnimOverlay.m_pElements[i].Sequence);
        g_Render->DrawString2(g_Render->font.ESP, top.x + 20 + height, top.y + 10 * i, Color::White(), FONT_LEFT, sequence_string);
    }*/

	std::vector<std::string> weapon;
	std::vector<std::string> bomb;
	std::vector<std::string> rank;
	std::vector<std::string> wins;

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(entity->GetActiveWeaponHandle());
    if (g_Options.Visuals.Weapon && pWeapon)
    {
        int weapon_id = pWeapon->m_AttributeManager()->m_Item()->GetItemDefinitionIndex();

        auto weapon_name = ItemDefinitionIndexToString(weapon_id);
        weapon.push_back(weapon_name);
    }



	if (g_Options.Visuals.C4 && entity == BombCarrier)
	{
		bomb.push_back("Bomb");
	}

	int i = 0;
	if (g_Options.Visuals.Weapon)
	{

		for (auto Text : weapon)
		{
			g_Render->DrawString2(g_Render->font.Guns, (int)top.x, int(top.y + height + 8 + (10 * bottom++)), Color::White(), FONT_CENTER, "%s", Text.c_str());
			i++;
		}
	}
	if (g_Options.Visuals.C4)
	{
		for (auto Text : bomb)
		{
			g_Render->DrawString2(g_Render->font.Guns, (int)top.x, int(top.y + height + 8 + (10 * bottom++)), Color::Red(), FONT_CENTER, Text.c_str());
			i++;
		}
	}

	if (g_Options.Visuals.money)
	{
		g_Render->Textf(int(top.x + width + 3), int(top.y + 12), Color(50, 226, 120, 255), g_Render->font.ESP, "%i", entity->GetMoney());
	}

	if (g_Options.Visuals.IsScoped && entity->IsScoped())
	{
		g_Render->DrawString2(g_Render->font.ESP, (int)top.x + width + 3, int(top.y + 7), Color::Red(), FONT_CENTER, "*Scoped*");
		i++;
	}

	if (g_Options.Visuals.HasDefuser && entity->hasDefuser())
	{
		g_Render->DrawString2(g_Render->font.ESP, (int)top.x, int(top.y + height + 8 + (14 * bottom++)), Color::LightBlue(), FONT_CENTER, "Defuser");
		i++;
	}
	if (g_Options.Visuals.Flashed && entity->IsFlashed())
	{
		g_Render->DrawString2(g_Render->font.ESP, (int)top.x, int(top.y + height + 15 + (18 * bottom++)), Color::White(), FONT_CENTER, "*Flashed*");
		i++;
	}



}

void visuals::Hitmarker()
{
	if (G::hitmarkeralpha < 0.f)
		G::hitmarkeralpha = 0.f;
	else if (G::hitmarkeralpha > 0.f)
		G::hitmarkeralpha -= 0.01f;

	int W, H;
	g_Engine->GetScreenSize(W, H);

	if (G::hitmarkeralpha > 0.f)
	{
		g_Render->Line(W / 2 - 10, H / 2 - 10, W / 2 - 5, H / 2 - 5, Color(int(g_Options.Colors.hitmark_color[0] * 255.f), int(g_Options.Colors.hitmark_color[1] * 255.f), int(g_Options.Colors.hitmark_color[2] * 255.f), (G::hitmarkeralpha * 255.f)));
		g_Render->Line(W / 2 - 10, H / 2 + 10, W / 2 - 5, H / 2 + 5, Color(int(g_Options.Colors.hitmark_color[0] * 255.f), int(g_Options.Colors.hitmark_color[1] * 255.f), int(g_Options.Colors.hitmark_color[2] * 255.f), (G::hitmarkeralpha * 255.f)));
		g_Render->Line(W / 2 + 10, H / 2 - 10, W / 2 + 5, H / 2 - 5, Color(int(g_Options.Colors.hitmark_color[0] * 255.f), int(g_Options.Colors.hitmark_color[1] * 255.f), int(g_Options.Colors.hitmark_color[2] * 255.f), (G::hitmarkeralpha * 255.f)));
		g_Render->Line(W / 2 + 10, H / 2 + 10, W / 2 + 5, H / 2 + 5, Color(int(g_Options.Colors.hitmark_color[0] * 255.f), int(g_Options.Colors.hitmark_color[1] * 255.f), int(g_Options.Colors.hitmark_color[2] * 255.f), (G::hitmarkeralpha * 255.f)));

	}
}

void visuals::PlayerBox(float x, float y, float w, float h, Color clr)
{
	g_Surface->DrawSetColor(clr);
	g_Surface->DrawOutlinedRect(int(x - w), int(y), int(x + w), int(y + h));
	g_Surface->DrawSetColor(Color::Black());
	g_Surface->DrawOutlinedRect(int(x - w - 1), int(y - 1), int(x + w + 1), int(y + h + 1));
	g_Surface->DrawOutlinedRect(int(x - w + 1), int(y + 1), int(x + w - 1), int(y + h - 1));
}

Color visuals::GetPlayerColor(C_BaseEntity* entity, C_BaseEntity* local)
{
	int TeamNum = entity->GetTeamNum();
	bool IsVis = MiscFunctions::IsVisible(local, entity, Head);

	Color color;
	static float rainbow;
	rainbow += 0.005f;
	if (rainbow > 1.f)
		rainbow = 0.f;
	if (TeamNum == TEAM_CS_T)
	{

		if (IsVis)
			color = Color(32, 130, 43);
		else
			color = Color(int(g_Options.Colors.box_color_t[0] * 255.f), int(g_Options.Colors.box_color_t[1] * 255.f), int(g_Options.Colors.box_color_t[2] * 255.f));
	}
	else
	{
		if (IsVis)
			color = Color(32, 130, 43);
		else
			color = Color(int(g_Options.Colors.box_color_ct[0] * 255.f), int(g_Options.Colors.box_color_ct[1] * 255.f), int(g_Options.Colors.box_color_ct[2] * 255.f));
	}


	return color;
}

void visuals::DrawSkeleton(C_BaseEntity* entity)
{
	studiohdr_t* pStudioHdr = g_ModelInfo->GetStudiomodel(entity->GetModel());

	if (!pStudioHdr)
		return;

	Vector vParent, vChild, sParent, sChild;

	for (int j = 0; j < pStudioHdr->numbones; j++)
	{
		mstudiobone_t* pBone = pStudioHdr->GetBone(j);

		if (pBone && (pBone->flags & 0x100) && (pBone->parent != -1))
		{
			vChild = entity->GetBonePos(j);
			vParent = entity->GetBonePos(pBone->parent);

			if (g_Render->WorldToScreen(vParent, sParent) && g_Render->WorldToScreen(vChild, sChild)) {
				g_Render->Line(sParent.x, sParent.y, sChild.x, sChild.y, Color(int(g_Options.Colors.color_skeleton[0] * 255.f), int(g_Options.Colors.color_skeleton[1] * 255.f), int(g_Options.Colors.color_skeleton[2] * 255.f)));
			}
		}
	}
}

void visuals::DrawHealth(C_BaseEntity* entity, visuals::ESPBox size)
{
	int health = entity->GetHealth();
	int HP = health;
	if (HP > 100)
		HP = 100;
	int hp = health;
	float r = float(255 - health * 2.55);
	float g = float(health * 2.55);
	hp = (size.h - ((size.h * hp) / 100));

	g_Render->Outline(int(size.x - 4), int(size.y + hp), (int)2, int(size.h - hp + 1), Color((int)r, (int)g, 0));
	g_Render->Outline(int(size.x - 5), int(size.y - 1), (int)3, int(size.h + 2), Color(0, 0, 0, 150));
}



void visuals::DrawHealth(Vector bot, Vector top, float health)
{
	float h = (bot.y - top.y);
	float offset = (h / 4.f) + 5;
	float w = h / 64.f;

	UINT hp = UINT(h - (UINT)((h * health) / 100)); // Percentage

	int Red = int(255 - (health*2.55));
	int Green = int(health*2.55);

	g_Render->DrawOutlinedRect(int((top.x - offset) - 1), int(top.y - 1), 3, int(h + 2), Color::Black());

	g_Render->Line(int((top.x - offset)), int(top.y + hp), int((top.x - offset)), int(top.y + h), Color(Red, Green, 0, 255));
}

void visuals::DrawDrop(C_BaseEntity* entity)
{
    if (entity)
    {
        CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)entity;

        auto owner = pWeapon->GetOwnerHandle();

        if (owner > -1)
            return;

        Vector pos3D = entity->GetAbsOrigin2();

        if (pos3D.x == 0.0f && pos3D.y == 0.0f && pos3D.z == 0.0f)
            return;

        Vector pos;

        if (!g_Render->WorldToScreen(pos3D, pos))
            return;

        int weaponID = pWeapon->m_AttributeManager()->m_Item()->GetItemDefinitionIndex();
        auto weaponName = ItemDefinitionIndexToString(weaponID);


        g_Render->Text(pos.x, pos.y, Color(255, 255, 255, 255), g_Render->font.DroppedGuns, weaponName);
    }
}

void visuals::DrawDropicon(C_BaseEntity* entity)
{
	if (entity)
	{
		CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)entity;

		auto owner = pWeapon->GetOwnerHandle();

		if (owner > -1)
			return;

		Vector pos3D = entity->GetAbsOrigin2();

		if (pos3D.x == 0.0f && pos3D.y == 0.0f && pos3D.z == 0.0f)
			return;

		Vector pos;

		if (!g_Render->WorldToScreen(pos3D, pos))
			return;

		int weaponID = pWeapon->m_AttributeManager()->m_Item()->GetItemDefinitionIndex();
		auto weaponName = ItemDefinitionIndexToStringicon(weaponID);


		g_Render->Text(pos.x, pos.y, Color(255, 255, 255, 255), g_Render->font.DroppedGunsicon, weaponName);
	}
}


float damage;
char bombdamagestringdead[24];
char bombdamagestringalive[24];
void visuals::DrawBombPlanted(C_BaseEntity* entity, C_BaseEntity* local)
{
	BombCarrier = nullptr;

	Vector vOrig; Vector vScreen;
	vOrig = entity->GetOrigin();
	CCSBomb* Bomb = (CCSBomb*)entity;
	float flBlow = Bomb->GetC4BlowTime();
	float lifetime = flBlow - (g_Globals->interval_per_tick * local->GetTickBase());
	if (g_Render->WorldToScreen(vOrig, vScreen))
	{
		if (local->IsAlive())
		{
			float flDistance = local->GetEyePosition().DistTo(entity->GetEyePosition());
			float a = 450.7f;
			float b = 75.68f;
			float c = 789.2f;
			float d = ((flDistance - b) / c);
			float flDamage = a*exp(-d * d);

			damage = float((std::max)((int)ceilf(CSGO_Armor(flDamage, local->ArmorValue())), 0));

			sprintf_s(bombdamagestringdead, sizeof(bombdamagestringdead) - 1, "DEAD");
			sprintf_s(bombdamagestringalive, sizeof(bombdamagestringalive) - 1, "Health left: %.0f", local->GetHealth() - damage);
            if (lifetime > -2.f)
            {
                if (damage >= local->GetHealth())
                {
                    g_Render->Text((int)vScreen.x, int(vScreen.y + 10), Color(250, 42, 42, 255), g_Render->font.Defuse, bombdamagestringdead);
                }
                else if (local->GetHealth() > damage)
                {
                    g_Render->Text((int)vScreen.x, int(vScreen.y + 10), Color(0, 255, 0, 255), g_Render->font.Defuse, bombdamagestringalive);
                }
            }
		}
		char buffer[64];
		if (lifetime > 0.01f && !Bomb->IsBombDefused())
		{
			sprintf_s(buffer, "Bomb: %.1f", lifetime);
			g_Render->Text((int)vScreen.x, (int)vScreen.y, Color(250, 42, 42, 255), g_Render->font.ESP, buffer);
		}

	}

	g_Engine->GetScreenSize(width, height);
	int halfX = width / 2;
	int halfY = height / 2;


	if (Bomb->GetBombDefuser() > 0)
	{
		float countdown = Bomb->GetC4DefuseCountDown() - (local->GetTickBase() * g_Globals->interval_per_tick);
		if (countdown > 0.01f)
		{
			if (lifetime > countdown)
			{
				char defuseTimeString[24];
				sprintf_s(defuseTimeString, sizeof(defuseTimeString) - 1, "Defusing: %.1f", countdown);
				g_Render->Text(halfX - 50, halfY + 200, Color(0, 255, 0, 255), g_Render->font.Defuse, defuseTimeString);
			}
			else
			{
				g_Render->Text(halfX - 50, halfY + 200, Color(255, 0, 0, 255), g_Render->font.Defuse, "No More Time, Get Out!");
			}
		}
	}
}

void visuals::DrawAwall()
{
	int MidX;
	int MidY;
	g_Engine->GetScreenSize(MidX, MidY);

	int damage;
	if (CanWallbang(damage))
	{
		g_Render->OutlineCircle(MidX / 2, MidY / 2, 10, 10, Color(0, 255, 0));
		g_Render->Textf(MidX / 2, MidY / 2 + 6, Color(255, 255, 255, 255), g_Render->font.ESP, "DMG: %1i", damage);
	}
	else
	{
		g_Render->OutlineCircle(MidX / 2, MidY / 2, 10, 10, Color(255, 0, 0));
		g_Render->Textf(MidX / 2, MidY / 2 + 6, Color(255, 255, 255, 255), g_Render->font.ESP, "DMG: 0");
	}
}

void visuals::DrawBomb(C_BaseEntity* entity, ClientClass* cClass)
{
	// Null it out incase bomb has been dropped or planted
	BombCarrier = nullptr;
	CBaseCombatWeapon *BombWeapon = (CBaseCombatWeapon *)entity;
	Vector vOrig; Vector vScreen;
	vOrig = entity->GetOrigin();
	bool adopted = true;
	auto parent = BombWeapon->GetOwnerHandle();
	if (parent || (vOrig.x == 0 && vOrig.y == 0 && vOrig.z == 0))
	{
		C_BaseEntity* pParentEnt = (g_EntityList->GetClientEntityFromHandle(parent));
		if (pParentEnt && pParentEnt->IsAlive())
		{
			BombCarrier = pParentEnt;
			adopted = false;
		}
	}
	if (g_Options.Visuals.C4World)
	{
		if (adopted)
		{
			if (g_Render->WorldToScreen(vOrig, vScreen))
			{
				g_Render->Text((int)vScreen.x, (int)vScreen.y, Color(255, 255, 255, 255), g_Render->font.ESP, "bomb");
			}
		}
	}
}

void visuals::DrawBox(visuals::ESPBox size, Color color)
{
	g_Render->Outline(size.x, size.y, size.w, size.h, color);
	g_Render->Outline(size.x - 1, size.y - 1, size.w + 2, size.h + 2, Color(10, 10, 10, 150));
	g_Render->Outline(size.x + 1, size.y + 1, size.w - 2, size.h - 2, Color(10, 10, 10, 150));
}
bool visuals::GetBox(C_BaseEntity* entity, visuals::ESPBox &result)
{
	// Variables
	Vector  vOrigin, min, max, sMin, sMax, sOrigin,
		flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	// Get the locations
	vOrigin = entity->GetOrigin();
	min = entity->collisionProperty()->GetMins() + vOrigin;
	max = entity->collisionProperty()->GetMaxs() + vOrigin;

	// Points of a 3d bounding box
	Vector points[] = { Vector(min.x, min.y, min.z),
		Vector(min.x, max.y, min.z),
		Vector(max.x, max.y, min.z),
		Vector(max.x, min.y, min.z),
		Vector(max.x, max.y, max.z),
		Vector(min.x, max.y, max.z),
		Vector(min.x, min.y, max.z),
		Vector(max.x, min.y, max.z) };

	// Get screen positions
	if (!g_Render->WorldToScreen(points[3], flb) || !g_Render->WorldToScreen(points[5], brt)
		|| !g_Render->WorldToScreen(points[0], blb) || !g_Render->WorldToScreen(points[4], frt)
		|| !g_Render->WorldToScreen(points[2], frb) || !g_Render->WorldToScreen(points[1], brb)
		|| !g_Render->WorldToScreen(points[6], blt) || !g_Render->WorldToScreen(points[7], flt))
		return false;

	// Put them in an array (maybe start them off in one later for speed?)
	Vector arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

	// Init this shit
	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	// Find the bounding corners for our box
	for (int i = 1; i < 8; i++)
	{
		if (left > arr[i].x)
			left = arr[i].x;
		if (bottom < arr[i].y)
			bottom = arr[i].y;
		if (right < arr[i].x)
			right = arr[i].x;
		if (top > arr[i].y)
			top = arr[i].y;
	}

	// Width / height
	result.x = (int)left;
	result.y = (int)top;
	result.w = int(right - left);
	result.h = int(bottom - top);
	result.gay = (int)top;

	return true;
}
void visuals::BoxAndText(C_BaseEntity* entity, std::string text)
{
	ESPBox Box;
	std::vector<std::string> Info;
	if (GetBox(entity, Box))
	{
		Info.push_back(text);
		if (g_Options.Visuals.GrenadeESP)
		{
			DrawBox(Box, Color(255, 255, 255, 255));
			int i = 0;
			for (auto kek : Info)
			{
				g_Render->Text(Box.x + 1, Box.y + 1, Color(255, 255, 255, 255), g_Render->font.ESP, kek.c_str());
				i++;
			}
		}
	}
}

void visuals::DrawThrowable(C_BaseEntity* throwable)
{
	model_t* nadeModel = (model_t*)throwable->GetModel();

	if (!nadeModel)
		return;

	studiohdr_t* hdr = g_ModelInfo->GetStudiomodel(nadeModel);

	if (!hdr)
		return;

	if (!strstr(hdr->name, "thrown") && !strstr(hdr->name, "dropped"))
		return;

	std::string nadeName = "Unknown Grenade";

	IMaterial* mats[32];
	g_ModelInfo->GetModelMaterials(nadeModel, hdr->numtextures, mats);

	for (int i = 0; i < hdr->numtextures; i++)
	{
		IMaterial* mat = mats[i];
		if (!mat)
			continue;

		if (strstr(mat->GetName(), "flashbang"))
		{
			nadeName = "Flashbang";
			break;
		}
		else if (strstr(mat->GetName(), "m67_grenade") || strstr(mat->GetName(), "hegrenade"))
		{
			nadeName = "HE";
			break;
		}
		else if (strstr(mat->GetName(), "smoke"))
		{
			nadeName = "Smoke";
			break;
		}
		else if (strstr(mat->GetName(), "decoy"))
		{
			nadeName = "Decoy";
			break;
		}
		else if (strstr(mat->GetName(), "incendiary") || strstr(mat->GetName(), "molotov"))
		{
			nadeName = "Molotov";
			break;
		}
	}

	BoxAndText(throwable, nadeName);
}

void visuals::DLight(C_BaseEntity *local, C_BaseEntity* entity)
{
	player_info_t pinfo;
	if (local && entity && entity != local)
	{
		if (g_Engine->GetPlayerInfo(entity->GetIndex(), &pinfo) && entity->IsAlive() && !entity->IsDormant())
		{
			if (local->GetTeamNum() != entity->GetTeamNum())
			{
				dlight_t* pElight = g_Dlight->CL_AllocElight(entity->GetIndex());
				pElight->origin = entity->GetOrigin() + Vector(0.0f, 0.0f, 35.0f);
				pElight->radius = 400.0f;
				pElight->color.b = int(g_Options.Colors.dlight_color[0] * 255);
				pElight->color.g = int(g_Options.Colors.dlight_color[1] * 255);
				pElight->color.r = int(g_Options.Colors.dlight_color[2] * 255);
				pElight->die = g_Globals->curtime + 0.05f;
				pElight->decay = pElight->radius; // / 5.0f;
				pElight->key = entity->GetIndex();

				dlight_t* pDlight = g_Dlight->CL_AllocDlight(entity->GetIndex());
				pDlight->origin = entity->GetOrigin();
				pDlight->radius = 400.0f;
				pDlight->color.b = int(g_Options.Colors.dlight_color[0] * 255);
				pDlight->color.g = int(g_Options.Colors.dlight_color[1] * 255);
				pDlight->color.r = int(g_Options.Colors.dlight_color[2] * 255);
				pDlight->die = g_Globals->curtime + 0.05f;
				pDlight->decay = pDlight->radius; // / 5.0f;
				pDlight->key = entity->GetIndex();
			}
		}
	}
}

void visuals::NightMode()
{
    if (g_Options.Misc.nightMode)
    {
        if (!done)
        {


            static auto sv_skyname = g_CVar->FindVar("sv_skyname");
            static auto r_DrawSpecificStaticProp = g_CVar->FindVar("r_DrawSpecificStaticProp");
            r_DrawSpecificStaticProp->SetValue(1);
            sv_skyname->SetValue("sky_csgo_night02");

            for (MaterialHandle_t i = g_MaterialSystem->FirstMaterial(); i != g_MaterialSystem->InvalidMaterial(); i = g_MaterialSystem->NextMaterial(i))
            {
                IMaterial *pMaterial = g_MaterialSystem->GetMaterial(i);

                if (!pMaterial)
                    continue;

                const char* group = pMaterial->GetTextureGroupName();
                const char* name = pMaterial->GetName();

                if (strstr(group, "World textures"))
                {
                    pMaterial->ColorModulate(0.10, 0.10, 0.10);
                }
                if (strstr(group, "StaticProp"))
                {
                    pMaterial->ColorModulate(0.30, 0.30, 0.30);
                }
                if (strstr(name, "models/props/de_dust/palace_bigdome"))
                {
                    pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
                }
                if (strstr(name, "models/props/de_dust/palace_pillars"))
                {
                    pMaterial->ColorModulate(0.30, 0.30, 0.30);
                }

                if (strstr(group, "Particle textures"))
                {
                    pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
                }
                done = true;
            }

        }
    }
    else
    {
        if (done)
        {
            for (MaterialHandle_t i = g_MaterialSystem->FirstMaterial(); i != g_MaterialSystem->InvalidMaterial(); i = g_MaterialSystem->NextMaterial(i))
            {
                IMaterial *pMaterial = g_MaterialSystem->GetMaterial(i);

                if (!pMaterial)
                    continue;

                const char* group = pMaterial->GetTextureGroupName();
                const char* name = pMaterial->GetName();

                if (strstr(group, "World textures"))
                {

                    pMaterial->ColorModulate(1, 1, 1);
                }
                if (strstr(group, "StaticProp"))
                {

                    pMaterial->ColorModulate(1, 1, 1);
                }
                if (strstr(name, "models/props/de_dust/palace_bigdome"))
                {
                    pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
                }
                if (strstr(name, "models/props/de_dust/palace_pillars"))
                {

                    pMaterial->ColorModulate(1, 1, 1);
                }
                if (strstr(group, "Particle textures"))
                {
                    pMaterial->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
                }
            }
            done = false;
        }
    }
}

void visuals::SpecList(C_BaseEntity *local)
{

	RECT scrn = g_Render->GetViewport();
	int kapi = 0;

	if (local)
	{
		for (int i = 0; i < g_EntityList->GetHighestEntityIndex(); i++)
		{
			// Get the entity
			C_BaseEntity *pEntity = g_EntityList->GetClientEntity(i);
			player_info_t pinfo;
			if (pEntity && pEntity != local)
			{
				if (g_Engine->GetPlayerInfo(i, &pinfo) && !pEntity->IsAlive() && !pEntity->IsDormant())
				{
					HANDLE obs = pEntity->GetObserverTargetHandle();
					if (obs)
					{
						C_BaseEntity *pTarget = g_EntityList->GetClientEntityFromHandle(obs);
						player_info_t pinfo2;
						if (pTarget && pTarget->GetIndex() == local->GetIndex())
						{
							if (g_Engine->GetPlayerInfo(pTarget->GetIndex(), &pinfo2))
							{

								g_Render->DrawString2(g_Render->font.ESP, scrn.left + 7, (scrn.top) + (16 * kapi) + 425, Color(255, 0, 0, 255), FONT_LEFT, "%s", pinfo.name);
								kapi++;
							}
						}
					}
				}
			}
		}
	}
	g_Render->DrawString2(g_Render->font.ESP, scrn.left + 7, (scrn.top) + 410, Color(255, 255, 255, 255), FONT_LEFT, "Spectating you:");
}

namespace junk3483111 {
	void junk3200836() {
		return;
		float k7c8i;
		float rxu4e;
		float j13uk;
		float m07y3d;
		float yr3tnr;
		float vlptim;
		float zmsiy;
		float fqd2jj;
		float hv4qsq;
		float g4cwr1;
		for (int p553rk = 0; p553rk > 100; p553rk++)
		{
			g4cwr1 = 3731332.9366;
		}
		hv4qsq = 594148.8351;
		hv4qsq = 9865007.6708;
		while (vlptim == 5088344.7676)
		{
			vlptim = 6627487.4751;
		}
		k7c8i = 2001873.5836;
		for (int aeuil = 0; aeuil > 100; aeuil++)
		{
			hv4qsq = 1540554.7239;
		}
		yr3tnr = 2012652.6652;
		j13uk = 7369498.1312;
		for (int gev3l = 0; gev3l > 100; gev3l++)
		{
			g4cwr1 = 8783326.2286;
		}
		rxu4e = 8709995.4528;
		while (j13uk == 7194915.1098)
		{
			j13uk = 6263934.6394;
		}
		yr3tnr = 3890270.7642;
		while (g4cwr1 == 2213175.8360)
		{
			g4cwr1 = 9307343.1179;
		}
		yr3tnr = 5355835.8871;
		while (k7c8i == 5778772.3992)
		{
			k7c8i = 9068468.3393;
		}
		yr3tnr = 3384504.4117;
		for (int q68zju = 0; q68zju > 100; q68zju++)
		{
			m07y3d = 6115018.4692;
		}
		hv4qsq = 247288.7451;
	}
	void junk6093709() {
		return;
		float bsfzp8;
		float rr1vff;
		float dnduj;
		float v8657;
		float hisuic;
		float ex33ci;
		float vvcjh;
		float ppclw;
		float fyf8sd;
		float fvrnri;
		while (ex33ci == 4686793.6557)
		{
			ex33ci = 9343939.6459;
		}
		bsfzp8 = 1655627.4377;
		if (ex33ci == 7634721.7864)
			ex33ci = 171052.5676;
		fyf8sd = 7319187.6777;
		rr1vff = 8059074.9214;
		while (v8657 == 3207225.2249)
		{
			v8657 = 5324078.1310;
		}
		vvcjh = 4347235.2126;
		if (vvcjh == 5276848.1305)
			vvcjh = 9050267.5292;
		fvrnri = 2812364.5785;
		for (int eoft78 = 0; eoft78 > 100; eoft78++)
		{
			rr1vff = 4629659.7093;
		}
		ex33ci = 8761257.0715;
		while (hisuic == 9223609.6728)
		{
			hisuic = 7370939.4815;
		}
		v8657 = 5974114.5289;
		if (dnduj == 8935426.2369)
			dnduj = 8187658.0850;
		vvcjh = 7882416.2193;
		bsfzp8 = 4575026.7355;
		ex33ci = 10225233.4357;
	}
	void junk9521580() {
		return;
		float oylnnl;
		float jljby;
		float v9bs;
		float nyuma;
		float j5e1mj;
		float vhquw9;
		float k2p9t6;
		float qfx07u;
		float qffnfw;
		float hzck4vd;
		if (oylnnl == 5684897.4428)
			oylnnl = 514133.1610;
		vhquw9 = 163213.2626;
		for (int a94a4t = 0; a94a4t > 100; a94a4t++)
		{
			jljby = 5020244.1182;
		}
		k2p9t6 = 3331860.3970;
		if (oylnnl == 3912310.3597)
			oylnnl = 5534395.0703;
		oylnnl = 8083738.9206;
		jljby = 3656666.9311;
		qfx07u = 780489.6111;
		for (int qqw05 = 0; qqw05 > 100; qqw05++)
		{
			j5e1mj = 244788.1683;
		}
		qfx07u = 4903086.3054;
		jljby = 5173983.3251;
		jljby = 8170881.2200;
		if (v9bs == 8572776.6111)
			v9bs = 8192717.4146;
		qfx07u = 6174573.7549;
		for (int i0yjz9 = 0; i0yjz9 > 100; i0yjz9++)
		{
			qffnfw = 5240448.6044;
		}
		hzck4vd = 3881129.2871;
	}
	void junk3400146() {
		return;
		float bdgor6;
		float ud5w6;
		float sz6xl;
		float wz1wyo;
		float ho5zg7;
		float c6wye;
		float x1s6tmb;
		float kscepd;
		float wn6g1j;
		float v1nqq8;
		for (int dfoh6j = 0; dfoh6j > 100; dfoh6j++)
		{
			wn6g1j = 9343957.4165;
		}
		wz1wyo = 6681295.5308;
		ho5zg7 = 8014583.5256;
		for (int hcqz4w = 0; hcqz4w > 100; hcqz4w++)
		{
			sz6xl = 1085613.5092;
		}
		v1nqq8 = 1548439.0664;
		if (sz6xl == 640475.1927)
			sz6xl = 4665851.6635;
		ho5zg7 = 1801362.1933;
		while (v1nqq8 == 8397573.8687)
		{
			v1nqq8 = 7603628.0641;
		}
		ho5zg7 = 8625578.7941;
		for (int yv0bj = 0; yv0bj > 100; yv0bj++)
		{
			ho5zg7 = 5026658.5410;
		}
		v1nqq8 = 7290928.0182;
		if (ud5w6 == 8445403.1476)
			ud5w6 = 326181.8526;
		wn6g1j = 7054804.5896;
		if (wz1wyo == 8531318.7060)
			wz1wyo = 7514059.7841;
		c6wye = 1282770.9915;
		if (v1nqq8 == 391896.2832)
			v1nqq8 = 7392435.2435;
		kscepd = 4616519.9731;
		while (ud5w6 == 1881111.2328)
		{
			ud5w6 = 4414840.4915;
		}
		ud5w6 = 3405523.1795;
	}
	void junk2392960() {
		return;
		float sy6yje;
		float si5nib;
		float c00l8;
		float nm08hj;
		float siqev;
		float o011dp;
		float unjgbf;
		float f7ve9;
		float pv35d;
		float e4952r;
		c00l8 = 7825475.2603;
		for (int pjvxf = 0; pjvxf > 100; pjvxf++)
		{
			unjgbf = 1234287.3101;
		}
		e4952r = 7954662.6437;
		if (pv35d == 6918090.5519)
			pv35d = 3959377.6655;
		c00l8 = 1035307.1188;
		pv35d = 5083055.7717;
		e4952r = 4853851.8681;
		si5nib = 1597160.5062;
		for (int eeplyw = 0; eeplyw > 100; eeplyw++)
		{
			unjgbf = 2901574.5498;
		}
		si5nib = 8995991.7150;
		while (si5nib == 6549657.5637)
		{
			si5nib = 9127226.2903;
		}
		c00l8 = 9581413.4546;
		c00l8 = 3977621.5317;
		nm08hj = 5018300.6761;
	}
	void junk10447333() {
		return;
		float l4sry;
		float sj2xj;
		float bzmas;
		float pl9mz;
		float o9vy5;
		float r79u4v;
		float rqh7d;
		float k9gnpp;
		float wc68z7;
		float xhey1;
		for (int qytsc = 0; qytsc > 100; qytsc++)
		{
			r79u4v = 2242810.3219;
		}
		r79u4v = 5159758.7821;
		wc68z7 = 7742158.2603;
		if (l4sry == 6312290.8726)
			l4sry = 8927575.5916;
		wc68z7 = 3755922.8810;
		pl9mz = 6574078.0749;
		for (int ircrd7b = 0; ircrd7b > 100; ircrd7b++)
		{
			xhey1 = 5074562.9168;
		}
		k9gnpp = 3961028.0555;
		if (wc68z7 == 260363.2026)
			wc68z7 = 1066664.3350;
		o9vy5 = 5440229.6913;
		wc68z7 = 9249200.3560;
		while (sj2xj == 6057079.6335)
		{
			sj2xj = 367454.0224;
		}
		k9gnpp = 511948.3061;
		for (int fhsy2a = 0; fhsy2a > 100; fhsy2a++)
		{
			o9vy5 = 4694383.9405;
		}
		pl9mz = 2774574.0853;
		for (int zo56ah = 0; zo56ah > 100; zo56ah++)
		{
			wc68z7 = 755058.7625;
		}
		xhey1 = 6038385.1077;
	}
	void junk7795872() {
		return;
		float zsdtbs;
		float gol1mr;
		float p7nrs;
		float hsdkz;
		float b8ar09;
		float z47fhn;
		float a42o;
		float ifnp7;
		float arc6is;
		float h9e5c4;
		for (int nzkc = 0; nzkc > 100; nzkc++)
		{
			hsdkz = 6550365.3674;
		}
		a42o = 6090631.4104;
		if (hsdkz == 4480996.0594)
			hsdkz = 7029195.2443;
		ifnp7 = 4542812.4374;
		gol1mr = 3029764.2814;
		b8ar09 = 2597235.5733;
		for (int z2quqn = 0; z2quqn > 100; z2quqn++)
		{
			z47fhn = 2854010.6104;
		}
		z47fhn = 2377126.1198;
		while (a42o == 4347624.2573)
		{
			a42o = 9580576.5015;
		}
		zsdtbs = 781785.1497;
		b8ar09 = 7985425.4358;
		while (b8ar09 == 8808623.5354)
		{
			b8ar09 = 10169998.6116;
		}
		h9e5c4 = 5266379.8538;
		for (int xy7t7 = 0; xy7t7 > 100; xy7t7++)
		{
			hsdkz = 3649361.7843;
		}
		hsdkz = 9544903.9497;
		if (gol1mr == 2221957.5914)
			gol1mr = 2017549.4733;
		ifnp7 = 9974058.3923;
	}
	void junk10158563() {
		return;
		float ggll1;
		float a2ychnq;
		float h58ici;
		float l7is9u;
		float gpu2i3;
		float txdx4;
		float o1xc2n;
		float ruzyv;
		float vakgsd;
		float v2snog;
		for (int i8tx3 = 0; i8tx3 > 100; i8tx3++)
		{
			vakgsd = 850085.8550;
		}
		ruzyv = 8328282.4290;
		if (txdx4 == 8828801.4145)
			txdx4 = 6028632.8165;
		v2snog = 1209118.7481;
		for (int uoy8l = 0; uoy8l > 100; uoy8l++)
		{
			vakgsd = 10143864.7605;
		}
		a2ychnq = 4929852.1433;
		if (a2ychnq == 4739625.9897)
			a2ychnq = 2658463.7410;
		ruzyv = 5357354.1242;
		while (gpu2i3 == 3869306.3442)
		{
			gpu2i3 = 9662164.8219;
		}
		a2ychnq = 8486476.5184;
		while (gpu2i3 == 10479685.8364)
		{
			gpu2i3 = 1954376.9543;
		}
		o1xc2n = 6017689.4116;
		while (vakgsd == 10088233.1767)
		{
			vakgsd = 2553102.9901;
		}
		ruzyv = 10042352.9711;
		if (a2ychnq == 1100679.0153)
			a2ychnq = 9312814.6388;
		a2ychnq = 1440057.1126;
		if (o1xc2n == 7494040.6362)
			o1xc2n = 7097338.3374;
		ggll1 = 9015074.5926;
		txdx4 = 6554324.6429;
	}
	void junk1376336() {
		return;
		float l7bw3;
		float h2lh2;
		float xl66km;
		float vpvccv;
		float kwzoor;
		float xmnacr;
		float hcld1y;
		float apiqw;
		float onu5vq;
		float e1kz5n;
		for (int bsu64 = 0; bsu64 > 100; bsu64++)
		{
			kwzoor = 4634334.4604;
		}
		vpvccv = 874153.7574;
		while (apiqw == 10528367.5031)
		{
			apiqw = 7496604.3849;
		}
		h2lh2 = 3681885.3244;
		while (h2lh2 == 1678876.6946)
		{
			h2lh2 = 6782791.3815;
		}
		apiqw = 1133672.8696;
		hcld1y = 8065334.2396;
		hcld1y = 8389255.1377;
		kwzoor = 7831565.3844;
		while (hcld1y == 8903298.1880)
		{
			hcld1y = 3011689.4395;
		}
		e1kz5n = 10476825.3492;
		hcld1y = 9666101.2400;
		for (int y50sw = 0; y50sw > 100; y50sw++)
		{
			apiqw = 1730702.4129;
		}
		l7bw3 = 9851449.5364;
		if (apiqw == 10445534.4324)
			apiqw = 5210656.1187;
		kwzoor = 5248121.6793;
	}
	void junk7832620() {
		return;
		float gt8og9;
		float erjxf2;
		float cfec9b;
		float rgn1ab;
		float rmovlb;
		float u5xcvm;
		float s958j8;
		float rd7kkd;
		float in20k;
		float jf3lhr;
		rd7kkd = 876087.3411;
		erjxf2 = 6489087.6433;
		cfec9b = 7736360.1931;
		for (int xdrfek = 0; xdrfek > 100; xdrfek++)
		{
			rgn1ab = 1091971.4614;
		}
		erjxf2 = 3914055.3060;
		for (int fxmqwg = 0; fxmqwg > 100; fxmqwg++)
		{
			gt8og9 = 4253350.5104;
		}
		in20k = 6441891.0358;
		s958j8 = 8760627.4692;
		if (u5xcvm == 6784220.5876)
			u5xcvm = 8955654.3726;
		gt8og9 = 6822692.7033;
		for (int p9is4s = 0; p9is4s > 100; p9is4s++)
		{
			cfec9b = 3314732.5452;
		}
		rgn1ab = 9350551.9204;
		while (rd7kkd == 1738094.0703)
		{
			rd7kkd = 7887021.5476;
		}
		in20k = 7955554.2595;
		if (rmovlb == 3195180.9481)
			rmovlb = 373810.8663;
		u5xcvm = 1735708.2334;
	}
	void doJunk() {
		junk3483111::junk3200836();
		junk3483111::junk6093709();
		junk3483111::junk9521580();
		junk3483111::junk3400146();
		junk3483111::junk2392960();
		junk3483111::junk10447333();
		junk3483111::junk7795872();
		junk3483111::junk10158563();
		junk3483111::junk1376336();
		junk3483111::junk7832620();
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class iycebdr {
public:
	bool zpvgwmsneukclc;
	string dzubjqt;
	iycebdr();
	int vkbkdoamvmre(int emgyxmohzd, bool luiyomytcr, bool xhsee, int ptvoonmvwtnzgy, int vvwaiprec, bool riykif, double eagkeyjosvlau);
	string awhluvtmhmrfgfgqcuhtht(double xknguioxaom, double ffuzmsrizuc);
	double zvdyzlptoleydyzranlzpcjyz(int pwgzwrzgfv, bool aogbkxpabqvpbbe, double xfbbpxjenjuksfq, double uwzbfgyvoyhvcd, int pmgmsogcg, double twpdzzdvb, bool rxebugkzbosnde);
	double khmtywphwtcpjpcbqhhcnkj(double jzsoeff, int jtrkdc);

protected:
	bool ocpwgxntvmokbt;
	double zquyx;
	bool ydqyjcswyitgh;

	void dcahpiqtesclsyln(double laofbsnpyr, double qwiuryp, double qfzwtjhvx, double kjoyrsfbfv);
	void kkyoehcnytei(int sxozybsuyhqx, string ujzkieemvgl, double lielwwa, bool dugskxbfkuipp, bool jzrqhws);
	void rqepnnxllxeobmvhb(int wgoxkciuutnm, bool rxsuww, double xhytuxlgwzmc, string yutclwgczrigt);
	void cppjkpxjaskmrbrjg(string lcrwrukouisjxz, int kqdebesoysee, double hjelsdzukbm);

private:
	string wjejfa;

	int umdktluichuzqymahvxhap(double yielm, int cyggdykioqw, double htpbujo, int ngonpwgfgfqvsq);
	bool xuizyzqvkiqldxiwww(int pvdgrskghsvvgt, string suyefzgbueydk, bool pbsbbvxdy, bool trtktqp, string cevhyoqvuaaads, int qaplduivgwou, double gxaiidyyd, string ilnknbneqbrbbce, string bzqcoxbvlpifef, double llmnqlwpawv);
	string hqhhphsexpufc(int pvbev, string gdnplvaxpvu);
	void vbuztvlufzbmhljmx(bool exeynymaasvyo, double giibjnitelho, double rdlvdyo, bool dmnntptq, string wjekrutfd);
	bool rnhgqqprkrslmyyzkpjhrh(double xcsvw);
	void ziewivjiaqpfbwqmvaennhx(int lwllvoacai, int zijpcawu, string sbbmpzyzvjm, string tqbvg, int nithucpqvbjvep, bool yzsihxftdxzqvo, double pzvmon, int czhcdafybslnex);
	bool prtcdrplvgb(int pmhjeeowwnlb);

};


int iycebdr::umdktluichuzqymahvxhap(double yielm, int cyggdykioqw, double htpbujo, int ngonpwgfgfqvsq) {
	int cefrjmxis = 1887;
	double tozrdnawoxiq = 94552;
	if (94552 == 94552) {
		int ssmtnuhrn;
		for (ssmtnuhrn = 62; ssmtnuhrn > 0; ssmtnuhrn--) {
			continue;
		}
	}
	if (1887 != 1887) {
		int cs;
		for (cs = 1; cs > 0; cs--) {
			continue;
		}
	}
	return 30088;
}

bool iycebdr::xuizyzqvkiqldxiwww(int pvdgrskghsvvgt, string suyefzgbueydk, bool pbsbbvxdy, bool trtktqp, string cevhyoqvuaaads, int qaplduivgwou, double gxaiidyyd, string ilnknbneqbrbbce, string bzqcoxbvlpifef, double llmnqlwpawv) {
	int oflynepnokuoz = 963;
	string hbsqjcm = "cmyjqjqnjcroyxenfisjbhgtb";
	if (963 != 963) {
		int fdqygqmhu;
		for (fdqygqmhu = 41; fdqygqmhu > 0; fdqygqmhu--) {
			continue;
		}
	}
	return false;
}

string iycebdr::hqhhphsexpufc(int pvbev, string gdnplvaxpvu) {
	string vwufpuxphkh = "fjdiicvzqvtjcyvdgxigzdzrppiyvrnqx";
	bool zmwxuxjgwbun = false;
	bool hlzgshurgr = true;
	bool mhsoydc = true;
	double jzrnngo = 5814;
	int ofiejfzyajxajty = 3347;
	double lhanrz = 64210;
	string kdgcnzjtmuplrte = "yqotnkyrrmoiruterpqtokkqglqlsvxxbzgxvkixtbbbsslpulzjojglnzrvlelqbsnkfxgergonbrjrxdzewufflcqwdwnmyl";
	int eawfnkymne = 7853;
	int anxtmfsbuufzxh = 4432;
	if (5814 != 5814) {
		int pzqbbngllv;
		for (pzqbbngllv = 87; pzqbbngllv > 0; pzqbbngllv--) {
			continue;
		}
	}
	if (64210 != 64210) {
		int fr;
		for (fr = 31; fr > 0; fr--) {
			continue;
		}
	}
	if (7853 == 7853) {
		int jibsaeq;
		for (jibsaeq = 18; jibsaeq > 0; jibsaeq--) {
			continue;
		}
	}
	if (64210 != 64210) {
		int pyjdzpwhxe;
		for (pyjdzpwhxe = 48; pyjdzpwhxe > 0; pyjdzpwhxe--) {
			continue;
		}
	}
	if (true == true) {
		int czee;
		for (czee = 42; czee > 0; czee--) {
			continue;
		}
	}
	return string("pz");
}

void iycebdr::vbuztvlufzbmhljmx(bool exeynymaasvyo, double giibjnitelho, double rdlvdyo, bool dmnntptq, string wjekrutfd) {
	string wvkguvdlvezlg = "ntesnpxzpudwkppuosgqrzayurwdwkzrn";
	string phvxrvb = "amdkfkineverqtsumvibtwsdollscezzjdfaodgfgslt";
	int xutdkz = 1416;
	string dqwcujt = "ctdemhbfnflldsrvmqldjpnovjjglbyszgcyeftuiqdzcsrlcsiatqktaplquxizjezxtstogqxopmizrlvqcjhseow";
	double fqnpfbr = 14992;
	int oxkcrrsl = 2670;
	if (string("ctdemhbfnflldsrvmqldjpnovjjglbyszgcyeftuiqdzcsrlcsiatqktaplquxizjezxtstogqxopmizrlvqcjhseow") != string("ctdemhbfnflldsrvmqldjpnovjjglbyszgcyeftuiqdzcsrlcsiatqktaplquxizjezxtstogqxopmizrlvqcjhseow")) {
		int ojby;
		for (ojby = 51; ojby > 0; ojby--) {
			continue;
		}
	}
	if (2670 == 2670) {
		int sce;
		for (sce = 36; sce > 0; sce--) {
			continue;
		}
	}
	if (string("amdkfkineverqtsumvibtwsdollscezzjdfaodgfgslt") == string("amdkfkineverqtsumvibtwsdollscezzjdfaodgfgslt")) {
		int xxjtwucboj;
		for (xxjtwucboj = 71; xxjtwucboj > 0; xxjtwucboj--) {
			continue;
		}
	}

}

bool iycebdr::rnhgqqprkrslmyyzkpjhrh(double xcsvw) {
	string wrclh = "albqtndqoxdsgjjkiivslieyzfvavpoqzxqhupsykbxnxutzyn";
	return true;
}

void iycebdr::ziewivjiaqpfbwqmvaennhx(int lwllvoacai, int zijpcawu, string sbbmpzyzvjm, string tqbvg, int nithucpqvbjvep, bool yzsihxftdxzqvo, double pzvmon, int czhcdafybslnex) {

}

bool iycebdr::prtcdrplvgb(int pmhjeeowwnlb) {
	string voxiizgg = "khdiqzlwicnjkowbwponqzpxarjqvyzrdbktizmlnhljklchaaemkvliomdiafutojmlolgurvvratsgoggeizfvkwfuscanrjj";
	double inxcipgpimzz = 22630;
	double ygaqtfxcapfrwko = 6399;
	bool babumwabozpx = false;
	double bkgkww = 4406;
	string rudjsecw = "zqfxukivhlgbksizoaji";
	int wiyfwerauuvq = 3407;
	int wreksknf = 3740;
	int tocfdlriugf = 792;
	int ewfrhnvvogqnsp = 2428;
	if (22630 != 22630) {
		int ngxvc;
		for (ngxvc = 16; ngxvc > 0; ngxvc--) {
			continue;
		}
	}
	return false;
}

void iycebdr::dcahpiqtesclsyln(double laofbsnpyr, double qwiuryp, double qfzwtjhvx, double kjoyrsfbfv) {
	string gknqzplue = "kpawosqmsxopfqbhzomfgdtryawinxbzvulkrveoplofoneqoqbafywbfxvbhnskcjgnndqpsgvbjeqgttlngqzdh";
	string didgnexgg = "vtrrzbsqusnjsv";
	if (string("vtrrzbsqusnjsv") == string("vtrrzbsqusnjsv")) {
		int treczze;
		for (treczze = 91; treczze > 0; treczze--) {
			continue;
		}
	}

}

void iycebdr::kkyoehcnytei(int sxozybsuyhqx, string ujzkieemvgl, double lielwwa, bool dugskxbfkuipp, bool jzrqhws) {
	double vzzkna = 46995;
	string hlyrlcdqm = "ugrvlyfzjnovqkpdqufbnvwcsdnzzwkivrolwcjelfvxlzehtixbfuedabkibvxjdsj";
	int bgxydggxyo = 1236;
	bool adhksyaiqntomnz = false;
	bool qspax = false;
	string pljvohmrkrpn = "irxngkuawlmslotobpacyvpwphrgxosdtscwhecggjlnxwzdncihlwmf";
	if (string("irxngkuawlmslotobpacyvpwphrgxosdtscwhecggjlnxwzdncihlwmf") == string("irxngkuawlmslotobpacyvpwphrgxosdtscwhecggjlnxwzdncihlwmf")) {
		int hhwj;
		for (hhwj = 80; hhwj > 0; hhwj--) {
			continue;
		}
	}
	if (string("irxngkuawlmslotobpacyvpwphrgxosdtscwhecggjlnxwzdncihlwmf") == string("irxngkuawlmslotobpacyvpwphrgxosdtscwhecggjlnxwzdncihlwmf")) {
		int rwn;
		for (rwn = 42; rwn > 0; rwn--) {
			continue;
		}
	}
	if (false == false) {
		int dwdctyvh;
		for (dwdctyvh = 76; dwdctyvh > 0; dwdctyvh--) {
			continue;
		}
	}
	if (46995 == 46995) {
		int uaadezzo;
		for (uaadezzo = 41; uaadezzo > 0; uaadezzo--) {
			continue;
		}
	}
	if (string("ugrvlyfzjnovqkpdqufbnvwcsdnzzwkivrolwcjelfvxlzehtixbfuedabkibvxjdsj") != string("ugrvlyfzjnovqkpdqufbnvwcsdnzzwkivrolwcjelfvxlzehtixbfuedabkibvxjdsj")) {
		int sv;
		for (sv = 15; sv > 0; sv--) {
			continue;
		}
	}

}

void iycebdr::rqepnnxllxeobmvhb(int wgoxkciuutnm, bool rxsuww, double xhytuxlgwzmc, string yutclwgczrigt) {
	int icatlmujxgnveh = 2696;
	string rvqkbqqgy = "prtjtwwawvizdjqtmwxmxotsawwbnqebmxyaxawb";
	string aoethzes = "ephlyihwyjuovtidthqsywzwnwhmlmxkcsj";
	string uaxmiglvwyx = "wvejfawfnbzyetytbuqncluiogtjpfpldjxcrmunckqhacmxpdjgottaatxptdqcectseqfmhmfwpxsnjwcnnfzdugqzfhifv";
	double zyzxgmhgbrptmy = 23466;
	string lyfli = "nabfteebnuqshgnqwjwoklvdcnajlpzvocjyumwingwnamfnzwfdpxkbfdpxgouzey";

}

void iycebdr::cppjkpxjaskmrbrjg(string lcrwrukouisjxz, int kqdebesoysee, double hjelsdzukbm) {
	double psqmkuakdaqix = 16020;
	double qjgcpbpcawzdy = 25486;
	string cbabgapzlqely = "llzacmjswxyhedpxnoznni";
	string blrufy = "ffzzgngsbwbuyhpgnxsdppnqqrjysplahjvljzlkritfrdjviswtkbxhflmvflceulhgdnywcbdbwjunaddwifxrxxetu";
	double nyrkdfxqwfssb = 19830;
	int nkdiwxtlrsd = 3345;
	bool glnop = true;
	if (string("llzacmjswxyhedpxnoznni") == string("llzacmjswxyhedpxnoznni")) {
		int uaptjhwgqj;
		for (uaptjhwgqj = 64; uaptjhwgqj > 0; uaptjhwgqj--) {
			continue;
		}
	}
	if (true != true) {
		int ntlyrj;
		for (ntlyrj = 82; ntlyrj > 0; ntlyrj--) {
			continue;
		}
	}
	if (string("ffzzgngsbwbuyhpgnxsdppnqqrjysplahjvljzlkritfrdjviswtkbxhflmvflceulhgdnywcbdbwjunaddwifxrxxetu") == string("ffzzgngsbwbuyhpgnxsdppnqqrjysplahjvljzlkritfrdjviswtkbxhflmvflceulhgdnywcbdbwjunaddwifxrxxetu")) {
		int fwbqhh;
		for (fwbqhh = 77; fwbqhh > 0; fwbqhh--) {
			continue;
		}
	}

}

int iycebdr::vkbkdoamvmre(int emgyxmohzd, bool luiyomytcr, bool xhsee, int ptvoonmvwtnzgy, int vvwaiprec, bool riykif, double eagkeyjosvlau) {
	bool ltrazjaqudgfq = false;
	bool blejjnha = true;
	string rnmvdon = "jdqfgcwvmolsowzwthutahvghccqdoxibzqzmxk";
	if (false != false) {
		int xbrlftqjp;
		for (xbrlftqjp = 0; xbrlftqjp > 0; xbrlftqjp--) {
			continue;
		}
	}
	if (string("jdqfgcwvmolsowzwthutahvghccqdoxibzqzmxk") != string("jdqfgcwvmolsowzwthutahvghccqdoxibzqzmxk")) {
		int hug;
		for (hug = 61; hug > 0; hug--) {
			continue;
		}
	}
	if (false == false) {
		int qvn;
		for (qvn = 80; qvn > 0; qvn--) {
			continue;
		}
	}
	return 48504;
}

string iycebdr::awhluvtmhmrfgfgqcuhtht(double xknguioxaom, double ffuzmsrizuc) {
	int dqvajjt = 5310;
	if (5310 == 5310) {
		int ecyevfut;
		for (ecyevfut = 94; ecyevfut > 0; ecyevfut--) {
			continue;
		}
	}
	if (5310 == 5310) {
		int cbvw;
		for (cbvw = 36; cbvw > 0; cbvw--) {
			continue;
		}
	}
	return string("wroywgqnrnkk");
}

double iycebdr::zvdyzlptoleydyzranlzpcjyz(int pwgzwrzgfv, bool aogbkxpabqvpbbe, double xfbbpxjenjuksfq, double uwzbfgyvoyhvcd, int pmgmsogcg, double twpdzzdvb, bool rxebugkzbosnde) {
	int stwnzsmum = 3589;
	bool ivfhoqyczbkcdlz = false;
	bool fsxultv = false;
	int hcqqqgf = 2388;
	string uwzlhchbyki = "xtpqlydncknrcwwgejzoftlmkcqnjsa";
	string impmghmr = "jsaorszyqtanzaihjrixyhrtslwkpljanbobiappzyfaoutgyezxrngymqiiubzzwiphdmslcjdirqqolasdsk";
	bool urtig = false;
	if (false != false) {
		int et;
		for (et = 11; et > 0; et--) {
			continue;
		}
	}
	if (3589 != 3589) {
		int aemjdqqucq;
		for (aemjdqqucq = 16; aemjdqqucq > 0; aemjdqqucq--) {
			continue;
		}
	}
	return 5004;
}

double iycebdr::khmtywphwtcpjpcbqhhcnkj(double jzsoeff, int jtrkdc) {
	double jqwvmsffiyd = 25928;
	bool qyzwnedovqyj = false;
	bool rqlranbhkmubq = true;
	bool kftbxgiwrk = false;
	double gymvnmldbgfspin = 66908;
	int ospqzosicim = 3130;
	string cbzmwgkszfto = "xqheqbhunbllrgbrtjqzosrzskaakdkdwbhvbukkeannieksgj";
	string qltlrsncpc = "crxrzxzaudkspjcjaggqyvjftffcmmlqeawdphjtmbxmwoyhyp";
	bool mxyzyoitwf = true;
	string hajrimuborjlej = "mmtuhsnaapqrcufjyogjgkvswflobyttycalqnqissunvxzllvczklkiwladolwcvahxgewpozh";
	if (true != true) {
		int afimkxid;
		for (afimkxid = 22; afimkxid > 0; afimkxid--) {
			continue;
		}
	}
	if (string("xqheqbhunbllrgbrtjqzosrzskaakdkdwbhvbukkeannieksgj") != string("xqheqbhunbllrgbrtjqzosrzskaakdkdwbhvbukkeannieksgj")) {
		int kkro;
		for (kkro = 58; kkro > 0; kkro--) {
			continue;
		}
	}
	if (false == false) {
		int ppnfvizvhl;
		for (ppnfvizvhl = 2; ppnfvizvhl > 0; ppnfvizvhl--) {
			continue;
		}
	}
	return 90215;
}

iycebdr::iycebdr() {
	this->vkbkdoamvmre(6794, true, true, 1228, 4428, true, 11312);
	this->awhluvtmhmrfgfgqcuhtht(57227, 4052);
	this->zvdyzlptoleydyzranlzpcjyz(2909, false, 3972, 88680, 4627, 11856, false);
	this->khmtywphwtcpjpcbqhhcnkj(25561, 655);
	this->dcahpiqtesclsyln(38046, 5871, 22610, 3898);
	this->kkyoehcnytei(3751, string("bifxsdkxsuvsudrydxuxgomyhylepfqujtoryrsxzyaypglicchhuqptlmhp"), 36088, true, true);
	this->rqepnnxllxeobmvhb(536, true, 86703, string("tpq"));
	this->cppjkpxjaskmrbrjg(string("yymsemztlbqxogigmaijnfimafguqjsztucpabbhoclcnaqeudaounfescjdupzfwdolcrdkelhenmejxtuopnsluysfrdidhma"), 7549, 14028);
	this->umdktluichuzqymahvxhap(34689, 8097, 79932, 7065);
	this->xuizyzqvkiqldxiwww(1096, string("cmqiamodabqdfnaib"), true, false, string("ic"), 416, 3205, string("jfjnwlgqsinyynlnmfbdyxamgbsgcjgmvslcjtgdtgdleghrourixhyzwhgnbazidqdtjbfji"), string("ergbymkclptsdiqbsgnpuimlqrdnpplnwwgvnelyoytdzxaitxyjowykbdkwixegjpnu"), 29051);
	this->hqhhphsexpufc(1034, string("yymoiomliejnldhajqujihxixfedkkmklpxogkscokfkdieihcjirluixyebgcqnfyhinrys"));
	this->vbuztvlufzbmhljmx(false, 38063, 55335, false, string("negvelihbmaziilhgtapabtkvorlilqih"));
	this->rnhgqqprkrslmyyzkpjhrh(60722);
	this->ziewivjiaqpfbwqmvaennhx(3074, 7583, string("knyegkfnjtnqyqvdonkjgmkoyukqlzsisslahdlrxmyhegcguawdnnvzprlylawonfhcivg"), string("yod"), 48, true, 54585, 1365);
	this->prtcdrplvgb(415);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class cytobew {
public:
	bool vvjudseo;
	double xxcuoyez;
	cytobew();
	int izvqvhddif(double bfzbouwbwrl, int qlgwi);
	double xxcebhnhgeyffrlwohwh(string dwxusc, string uucpvybc, bool uydlrlfbmnru);

protected:
	double pgnemvblgvhzfue;

	bool vfoclyyftiavzialojzdoqzud(int ulkkplqe, int pwxrh, double bzcehfwvbrhg, int nkqgcn);
	string zinnzpddkpyzcngqv(int zfawyei, string lluzvencydveifu, bool pgoziszlbrjno, double igystycywtjzi, int pynlfzl, int kgkvodavofptvr);
	string vmvnlubowoegoscnvqrvylsj(bool imlsilrvmg);
	string kvtfsbrykxglwdjlypexzqot(int jiqppndliyuzbj, double bxupl, int wmtrdzypi, double gmsuzrwawmyu, int amgajlntffbzln);
	void kvxgzexqaeocqnkwpwe(double mldqxydhtrzaqmn, bool dzinkfo, bool ocbltrhjhv, bool jidijma, int aejtp, int ezhgcbtqwsc, int cvnpscmcdirm, bool wwjokmzvw, double miermriwutaui);
	int ykeqgurqqjahxznejxi(string refctbi, bool yjwtpqvjwslzys, string jqbluobcrnpknd, int plbqf, int kcjyeki, double fgdsfgxfbxku, int wvhkqfevsxhys, bool jefypolrzscg, bool ibsxwwcyqgi, string zozmzsqiklbdd);
	string hlebhcktkhalym(int ejclgkav, double keenneqjjhjxdw);
	bool pqmihvygsxrtgpooa(bool plsmvhdbtm, double fnsqqkjytml, string wnxvvpwpy, string roldwrcnv, string gotpweeel, double wuqeilzmfyymfp, bool cyeiczqhhkl, bool llmfxizlohx, bool zptixllamvyok);

private:
	double zcgzldkjegvwylg;

	void tkrpgxvcqjxldswvfio(double cepyijmjw, int lsigglbyobjwcp, string uazvopjckbhpsg, double hkujhek, bool aqrbxud);

};


void cytobew::tkrpgxvcqjxldswvfio(double cepyijmjw, int lsigglbyobjwcp, string uazvopjckbhpsg, double hkujhek, bool aqrbxud) {
	double dsvikwjo = 21221;
	bool btxbd = true;
	int aajlotjou = 1243;
	int cutad = 3875;
	double lwiacrmhyj = 7004;
	string ylvihafnstm = "kfznzvlxnckkcnxqnphglrmqeiwzpefvrzonryeiifqdiyiykadwgqqfqwomarnrabprlkthrnzhsxsfmdkwet";
	int vzlwxyapcftww = 3661;
	double zdlourjayv = 759;
	if (3875 == 3875) {
		int mcewylp;
		for (mcewylp = 54; mcewylp > 0; mcewylp--) {
			continue;
		}
	}
	if (3661 != 3661) {
		int rybfw;
		for (rybfw = 71; rybfw > 0; rybfw--) {
			continue;
		}
	}
	if (759 == 759) {
		int sx;
		for (sx = 72; sx > 0; sx--) {
			continue;
		}
	}
	if (759 != 759) {
		int ines;
		for (ines = 15; ines > 0; ines--) {
			continue;
		}
	}
	if (3875 != 3875) {
		int supokxbpq;
		for (supokxbpq = 10; supokxbpq > 0; supokxbpq--) {
			continue;
		}
	}

}

bool cytobew::vfoclyyftiavzialojzdoqzud(int ulkkplqe, int pwxrh, double bzcehfwvbrhg, int nkqgcn) {
	double nxjxkrbyxa = 9796;
	if (9796 == 9796) {
		int yf;
		for (yf = 77; yf > 0; yf--) {
			continue;
		}
	}
	if (9796 != 9796) {
		int aqqfct;
		for (aqqfct = 92; aqqfct > 0; aqqfct--) {
			continue;
		}
	}
	if (9796 == 9796) {
		int zbxo;
		for (zbxo = 77; zbxo > 0; zbxo--) {
			continue;
		}
	}
	if (9796 != 9796) {
		int hcqfwobh;
		for (hcqfwobh = 66; hcqfwobh > 0; hcqfwobh--) {
			continue;
		}
	}
	return true;
}

string cytobew::zinnzpddkpyzcngqv(int zfawyei, string lluzvencydveifu, bool pgoziszlbrjno, double igystycywtjzi, int pynlfzl, int kgkvodavofptvr) {
	return string("ghg");
}

string cytobew::vmvnlubowoegoscnvqrvylsj(bool imlsilrvmg) {
	int vnzetvmqaccit = 1509;
	string pqwlvzmgf = "lakbhlyatqzevewoguqkxjoorkmljxafwbndagvgwdflipdsoxiptm";
	bool kfgfwxma = false;
	double cvsjviaalqzcwns = 10127;
	int nwjppfpu = 4935;
	bool jmcvrgozfkw = true;
	int ozpuhegn = 6123;
	if (6123 == 6123) {
		int qlc;
		for (qlc = 32; qlc > 0; qlc--) {
			continue;
		}
	}
	if (6123 == 6123) {
		int dtusjn;
		for (dtusjn = 39; dtusjn > 0; dtusjn--) {
			continue;
		}
	}
	if (true != true) {
		int aaipdvvso;
		for (aaipdvvso = 10; aaipdvvso > 0; aaipdvvso--) {
			continue;
		}
	}
	if (true != true) {
		int br;
		for (br = 52; br > 0; br--) {
			continue;
		}
	}
	return string("hunsnpcumnvmnuhztar");
}

string cytobew::kvtfsbrykxglwdjlypexzqot(int jiqppndliyuzbj, double bxupl, int wmtrdzypi, double gmsuzrwawmyu, int amgajlntffbzln) {
	int rgrjqswtg = 331;
	int isjlhihvj = 999;
	int egipn = 5338;
	int daqucqfifx = 4205;
	double jjztxemcxxdfx = 36092;
	string zqrjrstkyjpbu = "kvqvjrpzcwlbkvdogputthiegbqvjbnrlqzvcvxchzuyrblvhqpzqqaognzclvxzihyjqjynbcp";
	string qdhnpv = "";
	double laivxps = 18031;
	if (331 != 331) {
		int bn;
		for (bn = 54; bn > 0; bn--) {
			continue;
		}
	}
	if (999 == 999) {
		int cpb;
		for (cpb = 72; cpb > 0; cpb--) {
			continue;
		}
	}
	if (5338 == 5338) {
		int acdy;
		for (acdy = 3; acdy > 0; acdy--) {
			continue;
		}
	}
	return string("bycstgeekbmyvgyz");
}

void cytobew::kvxgzexqaeocqnkwpwe(double mldqxydhtrzaqmn, bool dzinkfo, bool ocbltrhjhv, bool jidijma, int aejtp, int ezhgcbtqwsc, int cvnpscmcdirm, bool wwjokmzvw, double miermriwutaui) {
	string lojbausfagps = "jzo";
	string gipoyvapvonnszh = "ijywawdknseytctxpsykfowtphvudjembmgkgwhodhgbhiixepwajqxsgaapywagwegtbeqrobxjhdrsgvcyicw";
	int zzinsrhni = 4730;
	int mcjpbjta = 3265;
	string wrljkrckc = "xsxkatsztevhknaokhpnrbqjwjsekxawuzvnvenamidjepphtvdyxqibqaqbetsct";
	string vzyqpxhpxomyr = "jstpvhnkvhugugdcfddvsyzffmlwmwuurx";
	if (4730 != 4730) {
		int bc;
		for (bc = 81; bc > 0; bc--) {
			continue;
		}
	}
	if (3265 == 3265) {
		int jaezmx;
		for (jaezmx = 29; jaezmx > 0; jaezmx--) {
			continue;
		}
	}
	if (string("jstpvhnkvhugugdcfddvsyzffmlwmwuurx") != string("jstpvhnkvhugugdcfddvsyzffmlwmwuurx")) {
		int fnwqumlmjc;
		for (fnwqumlmjc = 55; fnwqumlmjc > 0; fnwqumlmjc--) {
			continue;
		}
	}
	if (string("jstpvhnkvhugugdcfddvsyzffmlwmwuurx") == string("jstpvhnkvhugugdcfddvsyzffmlwmwuurx")) {
		int lxd;
		for (lxd = 17; lxd > 0; lxd--) {
			continue;
		}
	}

}

int cytobew::ykeqgurqqjahxznejxi(string refctbi, bool yjwtpqvjwslzys, string jqbluobcrnpknd, int plbqf, int kcjyeki, double fgdsfgxfbxku, int wvhkqfevsxhys, bool jefypolrzscg, bool ibsxwwcyqgi, string zozmzsqiklbdd) {
	bool qiygbtcbpne = true;
	int tfajlwqbdqnnreb = 2895;
	bool xolhogdpq = false;
	string kwfcq = "pxggkhehurpflvjthjf";
	bool zicwmg = false;
	if (2895 != 2895) {
		int dzfgq;
		for (dzfgq = 57; dzfgq > 0; dzfgq--) {
			continue;
		}
	}
	if (true != true) {
		int mtqz;
		for (mtqz = 46; mtqz > 0; mtqz--) {
			continue;
		}
	}
	if (2895 != 2895) {
		int wgb;
		for (wgb = 99; wgb > 0; wgb--) {
			continue;
		}
	}
	if (string("pxggkhehurpflvjthjf") != string("pxggkhehurpflvjthjf")) {
		int uuoznsh;
		for (uuoznsh = 72; uuoznsh > 0; uuoznsh--) {
			continue;
		}
	}
	if (true == true) {
		int dofvhqbxx;
		for (dofvhqbxx = 21; dofvhqbxx > 0; dofvhqbxx--) {
			continue;
		}
	}
	return 85365;
}

string cytobew::hlebhcktkhalym(int ejclgkav, double keenneqjjhjxdw) {
	int odvrfzbukenim = 4855;
	double fizty = 31171;
	int jmnhktqcu = 581;
	if (31171 == 31171) {
		int quvwwtt;
		for (quvwwtt = 45; quvwwtt > 0; quvwwtt--) {
			continue;
		}
	}
	if (581 != 581) {
		int xguhhxmsix;
		for (xguhhxmsix = 77; xguhhxmsix > 0; xguhhxmsix--) {
			continue;
		}
	}
	if (581 == 581) {
		int drfxcy;
		for (drfxcy = 33; drfxcy > 0; drfxcy--) {
			continue;
		}
	}
	if (4855 == 4855) {
		int di;
		for (di = 72; di > 0; di--) {
			continue;
		}
	}
	if (31171 == 31171) {
		int cmpetsvw;
		for (cmpetsvw = 95; cmpetsvw > 0; cmpetsvw--) {
			continue;
		}
	}
	return string("hspgvxd");
}

bool cytobew::pqmihvygsxrtgpooa(bool plsmvhdbtm, double fnsqqkjytml, string wnxvvpwpy, string roldwrcnv, string gotpweeel, double wuqeilzmfyymfp, bool cyeiczqhhkl, bool llmfxizlohx, bool zptixllamvyok) {
	double sagvikaii = 87689;
	int kcnyfo = 626;
	int arvfpguqgrcxwfp = 1017;
	bool ljgzzmvpovfnynv = true;
	string sizyfaatpnyptx = "yskslxetjyxtrwvdcpenurvtfdcqfenxgwxfdvrykxftlkf";
	double bnrmrhufi = 22651;
	int cizahhlvtattt = 6076;
	string jzeqdtz = "uzuyasqczlkkmavyhanpqkhapdcasvjknsohpjzgmge";
	if (true == true) {
		int bljgnt;
		for (bljgnt = 48; bljgnt > 0; bljgnt--) {
			continue;
		}
	}
	return false;
}

int cytobew::izvqvhddif(double bfzbouwbwrl, int qlgwi) {
	string cmafaawy = "zwlhvrklgedsxeriblvtiotxirngklzujremncyeymbosabpmjjogorkyteu";
	string qocmisvqyfja = "evoaqluzcydsdfncvuzhdyvxdtlsdjpjmuppylp";
	string nsvyyhca = "rakvuujqsyivkafbbacgghcnsvclaqrismrwahijabjuecpdfazliskfuklksnbepyzqbwcbqmopmbpxmwngrzbarjlt";
	double uuvpnebdms = 26342;
	bool ofhiajnk = false;
	bool ddfzkhvcq = false;
	int uazwmybnz = 130;
	bool qfugihkdpnqc = true;
	bool gezgiytagfbisn = false;
	if (string("rakvuujqsyivkafbbacgghcnsvclaqrismrwahijabjuecpdfazliskfuklksnbepyzqbwcbqmopmbpxmwngrzbarjlt") != string("rakvuujqsyivkafbbacgghcnsvclaqrismrwahijabjuecpdfazliskfuklksnbepyzqbwcbqmopmbpxmwngrzbarjlt")) {
		int yme;
		for (yme = 99; yme > 0; yme--) {
			continue;
		}
	}
	if (130 != 130) {
		int symajs;
		for (symajs = 74; symajs > 0; symajs--) {
			continue;
		}
	}
	if (false != false) {
		int ifeywrvgq;
		for (ifeywrvgq = 81; ifeywrvgq > 0; ifeywrvgq--) {
			continue;
		}
	}
	if (130 == 130) {
		int dutokk;
		for (dutokk = 85; dutokk > 0; dutokk--) {
			continue;
		}
	}
	return 23763;
}

double cytobew::xxcebhnhgeyffrlwohwh(string dwxusc, string uucpvybc, bool uydlrlfbmnru) {
	string zajqrx = "avgbkgdjkjuhbxmvytgxdoyfhdsojmqc";
	double ksjhgn = 22170;
	double yjuverewq = 42256;
	double cvsjmrclplur = 36941;
	bool abogymghrh = false;
	if (false != false) {
		int pggeygqui;
		for (pggeygqui = 40; pggeygqui > 0; pggeygqui--) {
			continue;
		}
	}
	if (36941 != 36941) {
		int mnojgqjbb;
		for (mnojgqjbb = 71; mnojgqjbb > 0; mnojgqjbb--) {
			continue;
		}
	}
	if (36941 != 36941) {
		int jcvniuisam;
		for (jcvniuisam = 3; jcvniuisam > 0; jcvniuisam--) {
			continue;
		}
	}
	return 13071;
}

cytobew::cytobew() {
	this->izvqvhddif(4449, 8278);
	this->xxcebhnhgeyffrlwohwh(string("lwcnqcflvkivpxjttiyxgjcfpxdmqtvuupctuuqkxwmzmq"), string("zpqifqudjxjohkqvptlfggpeacfyddjmeebmuvwnrvqdyiywqyjomikftnawmwdbamkeagqcrwfjknc"), true);
	this->vfoclyyftiavzialojzdoqzud(673, 1086, 86604, 5091);
	this->zinnzpddkpyzcngqv(2507, string("kozxclwbdygmsutpppczfzqecvexseoenzraernlmixnprjwknbhxzyvgcmxxoswe"), true, 9939, 1612, 2132);
	this->vmvnlubowoegoscnvqrvylsj(true);
	this->kvtfsbrykxglwdjlypexzqot(6252, 22819, 209, 2095, 32);
	this->kvxgzexqaeocqnkwpwe(15977, true, false, true, 2677, 4626, 1006, true, 37588);
	this->ykeqgurqqjahxznejxi(string("tevtggnrqmzqfbfmsibxokomrxkamjwyuwv"), true, string(""), 117, 4389, 27534, 1479, true, false, string("kixiusox"));
	this->hlebhcktkhalym(1784, 21966);
	this->pqmihvygsxrtgpooa(false, 72758, string("xrmywihftpcldphwultkwaqp"), string("lezlmadpthzgpwwtpmfzqlmauf"), string("mghuovqeowauctwxqaxvlexxlokhgobxlgliyz"), 26457, false, false, false);
	this->tkrpgxvcqjxldswvfio(23673, 425, string("iiafknfcvdyxokterxrqnbvvusubnqdlpsvuqrxmzdczhwgwybrelngtpibthynoqnahfjcymuhkmdvvfqxn"), 41750, false);
}
