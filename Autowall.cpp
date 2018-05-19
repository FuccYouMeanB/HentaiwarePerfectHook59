#include "AutoWall.h"
//#include "R.h"

#define    HITGROUP_GENERIC    0
#define    HITGROUP_HEAD        1
#define    HITGROUP_CHEST        2
#define    HITGROUP_STOMACH    3
#define HITGROUP_LEFTARM    4    
#define HITGROUP_RIGHTARM    5
#define HITGROUP_LEFTLEG    6
#define HITGROUP_RIGHTLEG    7
#define HITGROUP_GEAR        10

inline bool CGameTrace::DidHitWorld() const
{
	return m_pEnt->GetIndex() == 0;
}

inline bool CGameTrace::DidHitNonWorldEntity() const
{
	return m_pEnt != NULL && !DidHitWorld();
}

bool HandleBulletPenetration(CSWeaponInfo *wpn_data, FireBulletData &data);
float GetHitgroupDamageMult(int iHitGroup)
{
	switch (iHitGroup)
	{
	case HITGROUP_GENERIC:
		return 1.f;
	case HITGROUP_HEAD:
		return 4.f;
	case HITGROUP_CHEST:
		return 1.f;
	case HITGROUP_STOMACH:
		return 1.25f;
	case HITGROUP_LEFTARM:
		return 1.f;
	case HITGROUP_RIGHTARM:
		return 1.f;
	case HITGROUP_LEFTLEG:
		return 0.75f;
	case HITGROUP_RIGHTLEG:
		return 0.75f;
	case HITGROUP_GEAR:
		return 1.f;
	default:
		break;
	}

	return 1.f;
}
bool IsArmored(C_BaseEntity* Entity, int ArmorValue, int Hitgroup)
{
    bool result = false;

    if (ArmorValue > 0)
    {
        switch (Hitgroup)
        {
        case HITGROUP_GENERIC:
        case HITGROUP_CHEST:
        case HITGROUP_STOMACH:
        case HITGROUP_LEFTARM:
        case HITGROUP_RIGHTARM:
            result = true;
            break;
        case HITGROUP_HEAD:
            result = Entity->HasHelmet(); // DT_CSPlayer -> m_bHasHelmet
            break;
        }
    }

    return result;
}

void ScaleDamage(int Hitgroup,  C_BaseEntity* Entity, float WeaponArmorRatio, float &Damage)
{
    // NOTE: the Guardian/Coop Missions/Gamemode have bots with heavy armor which has a less damage modifier
    auto HeavyArmor = Entity->m_bHasHeavyArmor(); // DT_CSPlayer -> m_bHasHeavyArmor
    auto ArmorValue = Entity->ArmorValue(); // DT_CSPlayer -> m_ArmorValue

    switch (Hitgroup)
    {
    case HITGROUP_HEAD:
        if (HeavyArmor)
            Damage = (Damage * 4.f) * 0.5f;
        else
            Damage *= 4.f;
        break;
    case HITGROUP_STOMACH:
        Damage *= 1.25f;
        break;
    case HITGROUP_LEFTLEG:
    case HITGROUP_RIGHTLEG:
        Damage *= 0.75f;
        break;
    }

    if (IsArmored(Entity, ArmorValue, Hitgroup))
    {
        float v47 = 1.f, ArmorBonusRatio = 0.5f, ArmorRatio = WeaponArmorRatio * 0.5f;

        if (HeavyArmor)
        {
            ArmorBonusRatio = 0.33f;
            ArmorRatio = (WeaponArmorRatio * 0.5f) * 0.5f;
            v47 = 0.33f;
        }

        auto NewDamage = Damage * ArmorRatio;

        if (HeavyArmor)
            NewDamage *= 0.85f;

        if (((Damage - (Damage * ArmorRatio)) * (v47 * ArmorBonusRatio)) > ArmorValue)
            NewDamage = Damage - (ArmorValue / ArmorBonusRatio);

        Damage = NewDamage;
    }
}

/*void ScaleDamage(int hitgroup, C_BaseEntity *enemy, float weapon_armor_ratio, float &current_damage)
{
	current_damage *= GetHitgroupDamageMult(hitgroup);

	if (enemy->ArmorValue() > 0)
	{
		if (hitgroup == HITGROUP_HEAD)
		{
			if (enemy->HasHelmet())
			{
				current_damage *= (weapon_armor_ratio * 0.25f);
			}
		}
		else
		{
			current_damage *= (weapon_armor_ratio * 0.5f);
		}
	}

}*/

bool SimulateFireBullet(C_BaseEntity* entity, C_BaseEntity *local, CBaseCombatWeapon *weapon, FireBulletData &data)
{
	//Utils::ToLog("SimulateFireBullet");
	data.penetrate_count = 4;
	data.trace_length = 0.0f;
	auto *wpn_data = weapon->GetCSWpnData();

	data.current_damage = static_cast<float>(wpn_data->m_iDamage);

	while ((data.penetrate_count > 0) && (data.current_damage >= 1.0f))
	{
		data.trace_length_remaining = wpn_data->m_fRange - data.trace_length;

		Vector end = data.src + data.direction * data.trace_length_remaining;

		UTIL_TraceLine(data.src, end, 0x4600400B, local, 0, &data.enter_trace);
		UTIL_ClipTraceToPlayers(entity, data.src, end + data.direction * 40.f, 0x4600400B, &data.filter, &data.enter_trace);

		if (data.enter_trace.fraction == 1.0f)
			break;

		if ((data.enter_trace.hitgroup <= 7)
			&& (data.enter_trace.hitgroup > 0)
			&& (local->GetTeamNum() != data.enter_trace.m_pEnt->GetTeamNum()
				|| g_Options.Ragebot.FriendlyFire))
		{
			data.trace_length += (float)(data.enter_trace.fraction * data.trace_length_remaining);
			data.current_damage *= (float)(pow(wpn_data->m_fRangeModifier, data.trace_length * 0.002));
			ScaleDamage(data.enter_trace.hitgroup, data.enter_trace.m_pEnt, wpn_data->m_fArmorRatio, data.current_damage);

			return true;
		}

		if (!HandleBulletPenetration(wpn_data, data))
			break;
	}

	return false;
}

bool HandleBulletPenetration(CSWeaponInfo *wpn_data, FireBulletData &data)
{
	surfacedata_t *enter_surface_data = g_PhysProps->GetSurfaceData(data.enter_trace.surface.surfaceProps);
	int enter_material = enter_surface_data->game.material;
	float enter_surf_penetration_mod = enter_surface_data->game.flPenetrationModifier;


	data.trace_length += data.enter_trace.fraction * data.trace_length_remaining;
	data.current_damage *= (float)(pow(wpn_data->m_fRangeModifier, (data.trace_length * 0.002)));

	if ((data.trace_length > 3000.f) || (enter_surf_penetration_mod < 0.1f))
		data.penetrate_count = 0;

	if (data.penetrate_count <= 0)
		return false;

	Vector dummy;
	trace_t trace_exit;
	if (!TraceToExit(dummy, data.enter_trace, data.enter_trace.endpos, data.direction, &trace_exit))
		return false;

	surfacedata_t *exit_surface_data = g_PhysProps->GetSurfaceData(trace_exit.surface.surfaceProps);
	int exit_material = exit_surface_data->game.material;

	float exit_surf_penetration_mod = exit_surface_data->game.flPenetrationModifier;
	float final_damage_modifier = 0.16f;
	float combined_penetration_modifier = 0.0f;

	if (((data.enter_trace.contents & CONTENTS_GRATE) != 0) || (enter_material == 89) || (enter_material == 71))
	{
		combined_penetration_modifier = 3.0f;
		final_damage_modifier = 0.05f;
	}
	else
	{
		combined_penetration_modifier = (enter_surf_penetration_mod + exit_surf_penetration_mod) * 0.5f;
	}

	if (enter_material == exit_material)
	{
		if (exit_material == 87 || exit_material == 85)
			combined_penetration_modifier = 3.0f;
		else if (exit_material == 76)
			combined_penetration_modifier = 2.0f;
	}

	float v34 = fmaxf(0.f, 1.0f / combined_penetration_modifier);
	float v35 = (data.current_damage * final_damage_modifier) + v34 * 3.0f * fmaxf(0.0f, (3.0f / wpn_data->m_fPenetration) * 1.25f);
	float thickness = VectorLength(trace_exit.endpos - data.enter_trace.endpos);

	thickness *= thickness;
	thickness *= v34;
	thickness /= 24.0f;


	float lost_damage = fmaxf(0.0f, v35 + thickness);

	if (lost_damage > data.current_damage)
		return false;

	if (lost_damage >= 0.0f)
		data.current_damage -= lost_damage;

	if (data.current_damage < 1.0f)
		return false;

	data.src = trace_exit.endpos;
	data.penetrate_count--;

	return true;
}


/*
*    CanHit() - example of how to use the code
*     @in  point: target hitbox vector
*     @out damage_given: amount of damage the shot would do
*/
bool CanHit(C_BaseEntity* entity,const Vector &point, float *damage_given)
{
	//Utils::ToLog("CANHIT");
	auto *local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	auto data = FireBulletData(local->GetOrigin() + local->GetViewOffset());
	data.filter = CTraceFilter();
	data.filter.pSkip = local;

	Vector angles;
//	CalcAngle(data.src, point, angles);
	VectorAngles(point - data.src, angles);
	AngleVectors(angles, &data.direction);
	VectorNormalize(data.direction);

	if (SimulateFireBullet(entity, local, reinterpret_cast<CBaseCombatWeapon*>(g_EntityList->GetClientEntityFromHandle(static_cast<HANDLE>(local->GetActiveWeaponHandle()))), data))
	{
		*damage_given = data.current_damage;
		//Utils::ToLog("CANHIT END");
		return true;
	}

	return false;
}

bool CanWallbang(int &dmg)
{

	auto *local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle((HANDLE)local->GetActiveWeaponHandle());
	if (!local)
		return false;
	FireBulletData data = FireBulletData(local->GetEyePosition());
	data.filter = CTraceFilter();
	data.filter.pSkip = local;
	auto flRange = weapon->GetCSWpnData()->m_fRange;
	Vector EyeAng;
	g_Engine->GetViewAngles(EyeAng);

	Vector dst, forward;

	AngleVectors(EyeAng, &forward);
	dst = data.src + (forward * 8000.f);

	Vector angles;
	CalcAngle(data.src, dst, angles);
	AngleVectors(angles, &data.direction);
	VectorNormalize(data.direction);



	if (!weapon)
		return false;

	data.penetrate_count = 1;

	CSWeaponInfo *weaponData = weapon->GetCSWpnData();

	if (!weaponData)
		return false;

	data.current_damage = (int)weaponData->m_iDamage;

	data.trace_length_remaining = weaponData->m_fRange;

	Vector end = data.src + data.direction * data.trace_length_remaining;

	UTIL_TraceLine(data.src, end, MASK_SHOT | CONTENTS_GRATE, local, 0, &data.enter_trace);

	if (data.enter_trace.fraction == 1.f)
		return false;

	if (HandleBulletPenetration(weaponData, data))
	{
		dmg = data.current_damage;
		return true;
	}

	return false;
}

namespace junk3605309 {
	void junk469931() {
		return;
		float ek12e5;
		float off34e;
		float pgnwkr;
		float frhkmc;
		float gssova;
		float q2o4hk;
		float x3h4tl;
		float wqel;
		float jvnttu;
		float ft8hw5;
		for (int wmlqd = 0; wmlqd > 100; wmlqd++)
		{
			off34e = 3617462.6003;
		}
		pgnwkr = 3379581.5087;
		ek12e5 = 1599322.8619;
		q2o4hk = 6234485.2445;
		for (int draoki = 0; draoki > 100; draoki++)
		{
			off34e = 8637810.2752;
		}
		frhkmc = 6116999.5519;
		q2o4hk = 998347.8225;
		off34e = 2211888.7249;
		frhkmc = 9590885.4586;
		if (off34e == 7347236.4783)
			off34e = 5885196.9932;
		q2o4hk = 2291155.4815;
		if (wqel == 9008204.0925)
			wqel = 6769977.1975;
		gssova = 2233252.9019;
		for (int d1onol = 0; d1onol > 100; d1onol++)
		{
			x3h4tl = 3103981.6822;
		}
		pgnwkr = 6436812.7927;
	}
	void junk4508386() {
		return;
		float j6ckss;
		float llqsx8;
		float cdtdoo;
		float v3s9wr;
		float ltznbh;
		float ecqmls;
		float uv49a9;
		float z7b68;
		float cwpzrk;
		float i690ko;
		while (i690ko == 8941788.3698)
		{
			i690ko = 10446885.8030;
		}
		ltznbh = 4910691.5863;
		while (ltznbh == 4145860.1170)
		{
			ltznbh = 6149033.1204;
		}
		i690ko = 1505823.7085;
		for (int bnd7i = 0; bnd7i > 100; bnd7i++)
		{
			cwpzrk = 858262.1891;
		}
		ltznbh = 2692386.0746;
		for (int teqail = 0; teqail > 100; teqail++)
		{
			j6ckss = 9140773.3278;
		}
		llqsx8 = 3509050.2495;
		while (z7b68 == 7401200.9318)
		{
			z7b68 = 8318837.3155;
		}
		cwpzrk = 5164633.2908;
		if (cwpzrk == 5746214.0370)
			cwpzrk = 4586616.8652;
		llqsx8 = 9289383.9827;
		if (cwpzrk == 8289640.6370)
			cwpzrk = 4135147.3090;
		ecqmls = 8779460.6880;
		ltznbh = 2093653.5116;
		cwpzrk = 2893053.8950;
		while (i690ko == 4162772.7912)
		{
			i690ko = 2286145.6963;
		}
		uv49a9 = 9810677.7016;
	}
	void junk5505519() {
		return;
		float rwo2e3;
		float j7lyurc;
		float uym9bg;
		float xkdd3x;
		float b57swl;
		float sbhug8;
		float eg8w9;
		float pjdldm;
		float k2m8hb;
		float b21hqg;
		for (int a03ick = 0; a03ick > 100; a03ick++)
		{
			b57swl = 2182622.9251;
		}
		uym9bg = 9241634.1951;
		if (rwo2e3 == 3086504.5830)
			rwo2e3 = 2972655.9295;
		xkdd3x = 4295445.5956;
		uym9bg = 5441923.7161;
		while (pjdldm == 7314767.5392)
		{
			pjdldm = 9450493.1257;
		}
		b57swl = 3385166.1008;
		if (uym9bg == 7475705.2698)
			uym9bg = 5453243.4431;
		uym9bg = 5604374.8993;
		pjdldm = 3020879.5515;
		while (b57swl == 10012162.5996)
		{
			b57swl = 8791988.6464;
		}
		b57swl = 7249816.9141;
		while (b57swl == 4321700.6636)
		{
			b57swl = 1977924.4049;
		}
		uym9bg = 5598205.8510;
		while (b57swl == 8030487.7446)
		{
			b57swl = 820434.7235;
		}
		b57swl = 4489910.7901;
		k2m8hb = 5769305.7549;
	}
	void junk8881878() {
		return;
		float svt0ezr;
		float noh65e;
		float sxtnt;
		float oyirro;
		float hq481y;
		float rjkyx;
		float hq7pbj;
		float j6gbde;
		float cyqsgj;
		float tiaz4;
		if (noh65e == 9759680.3303)
			noh65e = 7015781.8764;
		hq7pbj = 7195552.3020;
		while (tiaz4 == 9705780.8359)
		{
			tiaz4 = 10151032.8527;
		}
		oyirro = 2511890.6302;
		j6gbde = 573189.6531;
		for (int b48e2m = 0; b48e2m > 100; b48e2m++)
		{
			j6gbde = 3464188.9623;
		}
		hq481y = 5259401.1533;
		if (oyirro == 8158852.3413)
			oyirro = 192490.5540;
		j6gbde = 525295.0713;
		sxtnt = 4669728.1743;
		oyirro = 8278598.8179;
		while (svt0ezr == 1858908.2031)
		{
			svt0ezr = 8161754.8736;
		}
		noh65e = 1339191.6046;
		rjkyx = 7842004.3753;
		while (svt0ezr == 4033871.9602)
		{
			svt0ezr = 4459598.4741;
		}
		tiaz4 = 6636830.1559;
	}
	void junk7081057() {
		return;
		float l588e;
		float u6h43v;
		float de2h2s;
		float xi1xcn;
		float q8zmos;
		float n2uft;
		float zaw4we;
		float i1o9q;
		float m3irq;
		float b2j6h;
		for (int cky4z = 0; cky4z > 100; cky4z++)
		{
			m3irq = 8279656.2943;
		}
		l588e = 5828598.7658;
		if (l588e == 7797152.2017)
			l588e = 7939437.9282;
		de2h2s = 6506039.8088;
		while (de2h2s == 7285317.5862)
		{
			de2h2s = 8734311.4750;
		}
		zaw4we = 7343662.6606;
		for (int f6p3a6 = 0; f6p3a6 > 100; f6p3a6++)
		{
			xi1xcn = 5581126.8288;
		}
		de2h2s = 4417398.9673;
		xi1xcn = 9879169.1689;
		l588e = 700766.1808;
		for (int udn0nq = 0; udn0nq > 100; udn0nq++)
		{
			i1o9q = 1480078.9861;
		}
		b2j6h = 8425852.3322;
		for (int mipvhl = 0; mipvhl > 100; mipvhl++)
		{
			zaw4we = 1360090.0205;
		}
		q8zmos = 9403862.7655;
		while (n2uft == 7543042.5525)
		{
			n2uft = 8787418.3668;
		}
		q8zmos = 2643453.5307;
		while (xi1xcn == 2555893.2304)
		{
			xi1xcn = 8603707.6837;
		}
		de2h2s = 5412601.5514;
	}
	void junk8806478() {
		return;
		float grvxua;
		float od53bm;
		float utyzyp;
		float ln2n3w;
		float urbpna;
		float d5vanm;
		float d7uj8;
		float peuk2;
		float njlju;
		float ex7ei;
		if (ex7ei == 8525865.6651)
			ex7ei = 172912.8001;
		peuk2 = 1749064.0935;
		peuk2 = 600679.3244;
		urbpna = 9095385.0560;
		while (njlju == 8315821.3390)
		{
			njlju = 1672679.4255;
		}
		ex7ei = 6334709.6797;
		for (int avljs5 = 0; avljs5 > 100; avljs5++)
		{
			ex7ei = 4788376.8712;
		}
		od53bm = 5219913.0716;
		for (int q1g0up = 0; q1g0up > 100; q1g0up++)
		{
			ex7ei = 4502640.2152;
		}
		peuk2 = 4129338.3280;
		urbpna = 4953208.4782;
		while (d5vanm == 4490484.3868)
		{
			d5vanm = 7798821.1300;
		}
		utyzyp = 4287607.5787;
		if (njlju == 2435805.3305)
			njlju = 8015209.9651;
		ln2n3w = 1957480.2391;
		if (grvxua == 6482995.1674)
			grvxua = 3754618.9771;
		d7uj8 = 6386816.5075;
	}
	void junk5587814() {
		return;
		float fj4eoe;
		float k3c21l;
		float bshkih;
		float mxp7k;
		float rl2k8w;
		float tlovn;
		float gp6itd;
		float ap1ccd;
		float em5rkn;
		float ojse6;
		while (mxp7k == 8684735.0950)
		{
			mxp7k = 3389566.8357;
		}
		k3c21l = 3203421.4726;
		for (int jvbtf7 = 0; jvbtf7 > 100; jvbtf7++)
		{
			gp6itd = 1636244.0737;
		}
		fj4eoe = 4374038.4433;
		if (mxp7k == 2914265.0745)
			mxp7k = 5207408.3909;
		bshkih = 5173938.0875;
		for (int zg2df = 0; zg2df > 100; zg2df++)
		{
			k3c21l = 3321868.9559;
		}
		em5rkn = 9970239.2817;
		if (mxp7k == 1071680.4280)
			mxp7k = 1733904.2987;
		ojse6 = 857245.0456;
		while (ap1ccd == 275908.0837)
		{
			ap1ccd = 2655328.9578;
		}
		bshkih = 8765391.0364;
		while (rl2k8w == 1824766.8775)
		{
			rl2k8w = 8966844.8330;
		}
		tlovn = 3743293.5548;
		if (ap1ccd == 10064772.5736)
			ap1ccd = 6960171.9030;
		rl2k8w = 6153162.6139;
		for (int pn85g9 = 0; pn85g9 > 100; pn85g9++)
		{
			fj4eoe = 4051359.8961;
		}
		rl2k8w = 91985.3932;
		while (tlovn == 1181633.6994)
		{
			tlovn = 7299612.5775;
		}
		ap1ccd = 7597796.2877;
	}
	void junk3580477() {
		return;
		float hiw2dq;
		float x7qjrc;
		float v5ycca;
		float vtkwqp;
		float h8n8me;
		float mem30w;
		float zen0b4;
		float uotk9j;
		float a7gb0k;
		float zt41hc;
		while (hiw2dq == 632721.6482)
		{
			hiw2dq = 10530448.6931;
		}
		vtkwqp = 8676485.4073;
		if (a7gb0k == 1483553.3788)
			a7gb0k = 5851297.9422;
		vtkwqp = 5444601.0196;
		zen0b4 = 3971916.6647;
		for (int c9mcz = 0; c9mcz > 100; c9mcz++)
		{
			mem30w = 1581656.9312;
		}
		x7qjrc = 6742154.8037;
		x7qjrc = 2695661.2402;
		if (a7gb0k == 238350.6087)
			a7gb0k = 6836203.5347;
		mem30w = 9617470.0176;
		if (a7gb0k == 4066158.6371)
			a7gb0k = 8762438.6155;
		vtkwqp = 372320.8703;
		if (uotk9j == 7500908.5554)
			uotk9j = 5469051.5385;
		uotk9j = 6348659.4400;
		zt41hc = 8835593.4279;
		for (int o954e = 0; o954e > 100; o954e++)
		{
			a7gb0k = 7657626.2879;
		}
		v5ycca = 4566632.8346;
	}
	void junk10132239() {
		return;
		float np39wu;
		float hzya2;
		float uhavjp;
		float f5wwip;
		float lp7ve;
		float pekqw;
		float vw4p6;
		float u7p686;
		float yt8ir;
		float awjydc;
		if (hzya2 == 2167820.5356)
			hzya2 = 2379213.0402;
		uhavjp = 6872863.9574;
		if (pekqw == 7998537.2784)
			pekqw = 2740055.9207;
		f5wwip = 4045818.1017;
		for (int kajty = 0; kajty > 100; kajty++)
		{
			uhavjp = 9239815.1375;
		}
		uhavjp = 4679433.2179;
		while (np39wu == 1576022.1719)
		{
			np39wu = 367760.3943;
		}
		u7p686 = 3917740.8355;
		while (f5wwip == 9080038.0868)
		{
			f5wwip = 5719211.6583;
		}
		lp7ve = 4441923.1257;
		for (int ya7jl8 = 0; ya7jl8 > 100; ya7jl8++)
		{
			uhavjp = 9189769.5298;
		}
		u7p686 = 8978874.7394;
		while (hzya2 == 3220720.1013)
		{
			hzya2 = 4424054.0475;
		}
		u7p686 = 8608040.8785;
		while (pekqw == 1825247.7597)
		{
			pekqw = 5331427.1122;
		}
		yt8ir = 5496107.4064;
		u7p686 = 4357954.7167;
		while (f5wwip == 9312240.1591)
		{
			f5wwip = 2643529.7218;
		}
		pekqw = 3648617.7208;
	}
	void junk10170155() {
		return;
		float x4yw6i;
		float ufuln9;
		float x29vge;
		float qlkntl;
		float fuuj6k;
		float i20m6;
		float si2lif;
		float llhpz4;
		float djsmpq;
		float tiu59q;
		for (int i4bx6t = 0; i4bx6t > 100; i4bx6t++)
		{
			tiu59q = 4697147.9147;
		}
		ufuln9 = 1788624.7325;
		if (x29vge == 2411872.4565)
			x29vge = 6697038.5143;
		qlkntl = 4525564.4119;
		while (ufuln9 == 3799887.5363)
		{
			ufuln9 = 4842358.1065;
		}
		x29vge = 4549385.8270;
		if (qlkntl == 1494036.5057)
			qlkntl = 2997602.2082;
		x4yw6i = 951683.7070;
		if (djsmpq == 9249160.1379)
			djsmpq = 7960004.3145;
		x29vge = 10268667.8346;
		if (x4yw6i == 4946982.8723)
			x4yw6i = 6569881.3960;
		si2lif = 4521432.6279;
		x29vge = 6365472.6109;
		if (qlkntl == 5899742.6252)
			qlkntl = 10083691.7908;
		ufuln9 = 56247.4246;
		if (llhpz4 == 466309.0405)
			llhpz4 = 807968.1922;
		llhpz4 = 2249908.4608;
		while (x4yw6i == 1267476.5294)
		{
			x4yw6i = 3541967.2618;
		}
		qlkntl = 1872650.2365;
	}
	void doJunk() {
		junk3605309::junk469931();
		junk3605309::junk4508386();
		junk3605309::junk5505519();
		junk3605309::junk8881878();
		junk3605309::junk7081057();
		junk3605309::junk8806478();
		junk3605309::junk5587814();
		junk3605309::junk3580477();
		junk3605309::junk10132239();
		junk3605309::junk10170155();
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class zrxeakd {
public:
	bool egyxjhgk;
	int uolqwhtmvcbbnyw;
	zrxeakd();
	void wmuxwfvjoanhvfrbvpsvyn(string xrjazbhxnbwti, double pbevdn, bool vcdznljtmf);
	double wnttwcptnmybszgiuegz(bool nljxrlpotnnvcko, int tpyayx, int biafjckqjejfbre);
	bool xhimqxxzqokulj(bool clcim, double ddawiqq, bool egqmmqsomdd, string izvjfhutrmu, int ivtghjgveduh, double vupxkummttpod, double ixxauqzq, int dnlcqwl);

protected:
	double wjhbzgcxgoza;
	string mzfecnoevi;
	double kbujce;

	bool hgyzkttznbiwovasiqikacsjp(int gkabsutnjnc, bool wzfkrideawilt, int ckljqy, int mkkmmgjyfewg, string kdpckhdngzbfmoc, string wmtedukdxseqh, int ufqdz, int jzhmqdwyb, int frbbcmljconl);
	int sarceehqdvclxnj(int ifwyhc, int yubjpgl, bool eegfnhpbshf, int niqbhcfkkc, int zvhqhguymrd, bool poqzdyrjnqbhz, bool xqpkwshrigpvr, double vlvfyrkyhax, bool yftirepamvais, bool jypahxoiywmsji);

private:
	bool tfogzufz;
	bool hxoukyap;

	string dqnwullrxopbheerriyvtgm(double iosuushzsikomu, int zbypjiqtyspphlt, bool baqihl, int rkvliidnsepeha, bool vwhynpazoiez, int rhawzyhzgpu, bool pdvqkswoqubzlps, double auhhbwfpwx, int oaugxnjbkt);
	double sonugqmzyplz(bool cexzzutkxvqxxcp, string waypassvx, bool admsnlu);
	int buppjitegkomhfy(double wdtpz, int nryhrukaaazad, bool xobac, int wcqakc, bool gwusuin, int ibuexmlqrvn);
	double cilapvhpjdzgbzkniwxfuvdk(string pzhvnujmqjwmfkr);
	void srzhkyyfmolw(string lsujfylj, string czoufglkchedph);
	bool woerbmnpxckznfy(string bvpcvym, int oqplihmbf, string nhhnjohnszchf, int rmpqzfdfafk, bool opkyzwhsxk, int whvevkjde, bool mcftlrfydpvanfd, string wqzcka, string kcssqipteqt);
	string akhqzlcplm();
	bool qhhaukpehiecycceypspshui(bool qxoebja, bool mztkm, int vjhge, double svsicdob, int mmqsfawoywqpobp, double jwecvujjqkqb, int cfxqzla);

};


string zrxeakd::dqnwullrxopbheerriyvtgm(double iosuushzsikomu, int zbypjiqtyspphlt, bool baqihl, int rkvliidnsepeha, bool vwhynpazoiez, int rhawzyhzgpu, bool pdvqkswoqubzlps, double auhhbwfpwx, int oaugxnjbkt) {
	double koknjdihof = 7211;
	string rwvxt = "pizqocmzegigthtroajkkivlzzvpajebxhrkoxmdxjzcjfsfalbxntwzfjazheqgijppfkifcerczbxmcdggc";
	string lekdrq = "guagwxlziuniswvrmvvpheydcmyvmmjlofmaazaxw";
	return string("womhjgkgzvwop");
}

double zrxeakd::sonugqmzyplz(bool cexzzutkxvqxxcp, string waypassvx, bool admsnlu) {
	string brlnibvog = "hiyygrdsgezmsler";
	bool qksjlccdbdwsyqe = true;
	string ywkbrb = "wihyrnmqimcwiybcxiacttmdyrgykawwnwwbffwkbgnibgbulwdyufaiynzhyczdjuvxm";
	int rsirufkqvdlzm = 2016;
	if (2016 == 2016) {
		int occwhlnyly;
		for (occwhlnyly = 48; occwhlnyly > 0; occwhlnyly--) {
			continue;
		}
	}
	if (true == true) {
		int oeihmuhemg;
		for (oeihmuhemg = 4; oeihmuhemg > 0; oeihmuhemg--) {
			continue;
		}
	}
	if (string("hiyygrdsgezmsler") == string("hiyygrdsgezmsler")) {
		int amiqjpzt;
		for (amiqjpzt = 77; amiqjpzt > 0; amiqjpzt--) {
			continue;
		}
	}
	if (true == true) {
		int xbzpdncjrl;
		for (xbzpdncjrl = 85; xbzpdncjrl > 0; xbzpdncjrl--) {
			continue;
		}
	}
	if (2016 == 2016) {
		int jjlxqmpa;
		for (jjlxqmpa = 24; jjlxqmpa > 0; jjlxqmpa--) {
			continue;
		}
	}
	return 78480;
}

int zrxeakd::buppjitegkomhfy(double wdtpz, int nryhrukaaazad, bool xobac, int wcqakc, bool gwusuin, int ibuexmlqrvn) {
	string egrkiodvxrbfil = "qjtssqndzmvygnkxkkg";
	int colefdcbxlr = 3704;
	double guvpzxgzez = 31312;
	int cgdotd = 4462;
	string wvldeoohca = "wswswyxsleyjlanzbgpqzitymawksnookwjoghyncwuuurolfgjcbxxkwmusjtkp";
	bool qemufcp = true;
	int wmgujaifj = 6336;
	string ytsay = "qbngfyjawocnqx";
	string mgtxsngruf = "limhevgoowqgbgbfkjkmzpfqkypjsntrjqdbjikuxeafvijuuyqcyzvwlclbjxgyyvagtwkfgqweojdgeqkntqqgdjgoqxgpdoel";
	bool xiyvqw = true;
	if (6336 == 6336) {
		int tco;
		for (tco = 28; tco > 0; tco--) {
			continue;
		}
	}
	if (4462 != 4462) {
		int uem;
		for (uem = 72; uem > 0; uem--) {
			continue;
		}
	}
	if (6336 != 6336) {
		int npnwmdo;
		for (npnwmdo = 78; npnwmdo > 0; npnwmdo--) {
			continue;
		}
	}
	return 55906;
}

double zrxeakd::cilapvhpjdzgbzkniwxfuvdk(string pzhvnujmqjwmfkr) {
	int fjfcsmbpn = 4453;
	string mdbvdpa = "aynmfqipsqahsxkfdtlgdrhnfghuqgrqsmqaeefyftxkdiiefhj";
	double ymmnjhlrdi = 5701;
	bool tencbxkvclldixf = false;
	double mkoxfmqwvectx = 3634;
	int inthwozinlbn = 7457;
	string ibfkwztvxp = "xvmrcbiw";
	bool edfufukpkpremm = false;
	string nhdulluqh = "l";
	string kztuing = "xignxnnkmdtpmpnfhrvyxruvibq";
	if (string("aynmfqipsqahsxkfdtlgdrhnfghuqgrqsmqaeefyftxkdiiefhj") != string("aynmfqipsqahsxkfdtlgdrhnfghuqgrqsmqaeefyftxkdiiefhj")) {
		int iyfs;
		for (iyfs = 77; iyfs > 0; iyfs--) {
			continue;
		}
	}
	if (5701 == 5701) {
		int xtp;
		for (xtp = 96; xtp > 0; xtp--) {
			continue;
		}
	}
	if (4453 == 4453) {
		int laydouzoj;
		for (laydouzoj = 96; laydouzoj > 0; laydouzoj--) {
			continue;
		}
	}
	return 28022;
}

void zrxeakd::srzhkyyfmolw(string lsujfylj, string czoufglkchedph) {
	int kserrfxfnti = 4424;
	string lywvtryrcon = "ikdziestjwvyupxicjlxekargwdthbythxsjndmorcxvovomj";
	bool coalmfycoiqyj = true;
	if (string("ikdziestjwvyupxicjlxekargwdthbythxsjndmorcxvovomj") == string("ikdziestjwvyupxicjlxekargwdthbythxsjndmorcxvovomj")) {
		int knx;
		for (knx = 61; knx > 0; knx--) {
			continue;
		}
	}
	if (string("ikdziestjwvyupxicjlxekargwdthbythxsjndmorcxvovomj") == string("ikdziestjwvyupxicjlxekargwdthbythxsjndmorcxvovomj")) {
		int fqecfyrbbk;
		for (fqecfyrbbk = 38; fqecfyrbbk > 0; fqecfyrbbk--) {
			continue;
		}
	}

}

bool zrxeakd::woerbmnpxckznfy(string bvpcvym, int oqplihmbf, string nhhnjohnszchf, int rmpqzfdfafk, bool opkyzwhsxk, int whvevkjde, bool mcftlrfydpvanfd, string wqzcka, string kcssqipteqt) {
	int yoxuoo = 469;
	double lrdbk = 4297;
	int rngbfz = 2099;
	double nycws = 14084;
	double mtlnutmi = 34035;
	double ehtbmrcq = 20115;
	bool wuwibrskxx = true;
	bool epmxgbhitaehvl = false;
	return true;
}

string zrxeakd::akhqzlcplm() {
	int qekwjynydbz = 5045;
	bool ezoxyfq = false;
	int eheeq = 6798;
	bool ibawghiqvyjyrrw = true;
	if (6798 != 6798) {
		int gmtz;
		for (gmtz = 17; gmtz > 0; gmtz--) {
			continue;
		}
	}
	if (6798 == 6798) {
		int eo;
		for (eo = 35; eo > 0; eo--) {
			continue;
		}
	}
	if (6798 != 6798) {
		int xxvbfvkv;
		for (xxvbfvkv = 33; xxvbfvkv > 0; xxvbfvkv--) {
			continue;
		}
	}
	if (true == true) {
		int ihoz;
		for (ihoz = 100; ihoz > 0; ihoz--) {
			continue;
		}
	}
	if (5045 == 5045) {
		int ckymxd;
		for (ckymxd = 79; ckymxd > 0; ckymxd--) {
			continue;
		}
	}
	return string("abhxutrovcgkoitw");
}

bool zrxeakd::qhhaukpehiecycceypspshui(bool qxoebja, bool mztkm, int vjhge, double svsicdob, int mmqsfawoywqpobp, double jwecvujjqkqb, int cfxqzla) {
	int kdusswdtmr = 3580;
	return true;
}

bool zrxeakd::hgyzkttznbiwovasiqikacsjp(int gkabsutnjnc, bool wzfkrideawilt, int ckljqy, int mkkmmgjyfewg, string kdpckhdngzbfmoc, string wmtedukdxseqh, int ufqdz, int jzhmqdwyb, int frbbcmljconl) {
	double ygqfqmjbkvfog = 91964;
	double fxybmchkobpetu = 21555;
	string emmjwjt = "wbrkmrqmugf";
	string mjjhe = "jexesnvjcwuwdsuxpuptwvgxqqiyducjdbrktdifxfviefdkqwimlmxfjueb";
	string semzwa = "xaixzkkjnfyihdqzbxvkcnqgxeyjfrxndsitpcghrecylspwpviczvsmlmwplfnkjaqxonnlmquhdcczyfkttg";
	double kshcvmqcpw = 61418;
	bool qxflrim = false;
	int admyphlbrioiq = 995;
	bool yqcuovmt = false;
	if (string("xaixzkkjnfyihdqzbxvkcnqgxeyjfrxndsitpcghrecylspwpviczvsmlmwplfnkjaqxonnlmquhdcczyfkttg") == string("xaixzkkjnfyihdqzbxvkcnqgxeyjfrxndsitpcghrecylspwpviczvsmlmwplfnkjaqxonnlmquhdcczyfkttg")) {
		int lwzop;
		for (lwzop = 87; lwzop > 0; lwzop--) {
			continue;
		}
	}
	return false;
}

int zrxeakd::sarceehqdvclxnj(int ifwyhc, int yubjpgl, bool eegfnhpbshf, int niqbhcfkkc, int zvhqhguymrd, bool poqzdyrjnqbhz, bool xqpkwshrigpvr, double vlvfyrkyhax, bool yftirepamvais, bool jypahxoiywmsji) {
	bool dgkczh = false;
	if (false == false) {
		int fvtqbr;
		for (fvtqbr = 8; fvtqbr > 0; fvtqbr--) {
			continue;
		}
	}
	return 46428;
}

void zrxeakd::wmuxwfvjoanhvfrbvpsvyn(string xrjazbhxnbwti, double pbevdn, bool vcdznljtmf) {
	int ylzmrr = 2199;
	if (2199 == 2199) {
		int rhfgk;
		for (rhfgk = 60; rhfgk > 0; rhfgk--) {
			continue;
		}
	}
	if (2199 != 2199) {
		int gzkfuwolrg;
		for (gzkfuwolrg = 91; gzkfuwolrg > 0; gzkfuwolrg--) {
			continue;
		}
	}
	if (2199 != 2199) {
		int drnqtjowdl;
		for (drnqtjowdl = 60; drnqtjowdl > 0; drnqtjowdl--) {
			continue;
		}
	}

}

double zrxeakd::wnttwcptnmybszgiuegz(bool nljxrlpotnnvcko, int tpyayx, int biafjckqjejfbre) {
	int rwbkcjqa = 1576;
	bool brlarpzv = false;
	bool ghvgqq = false;
	bool cohegfowavcy = true;
	int tvemjyq = 2704;
	if (2704 != 2704) {
		int belbkc;
		for (belbkc = 68; belbkc > 0; belbkc--) {
			continue;
		}
	}
	if (false == false) {
		int otjgxr;
		for (otjgxr = 75; otjgxr > 0; otjgxr--) {
			continue;
		}
	}
	return 67348;
}

bool zrxeakd::xhimqxxzqokulj(bool clcim, double ddawiqq, bool egqmmqsomdd, string izvjfhutrmu, int ivtghjgveduh, double vupxkummttpod, double ixxauqzq, int dnlcqwl) {
	double zxzelo = 13116;
	double eignowtd = 11560;
	string qyypp = "yoyyuusksgcvapltxvsgflwslebybcrftmrkbqycwysxabgzypvbbrtizrvrlnsukdamscj";
	string beblyggxddfefra = "fgygrsfsgwpxdqbtjuzxlunbcnchpkbdkihvykuaieggcektlha";
	string taeyjfsxmh = "pzqyxrucefpkjnlkwjjpxybtoxdyzxejwsbxyhdlttuwzfbmrmfqfgcdprpmzodtmzrqoidb";
	double dsambblkxdskh = 31115;
	bool xgspzftmddba = true;
	if (11560 == 11560) {
		int yojmyuxx;
		for (yojmyuxx = 41; yojmyuxx > 0; yojmyuxx--) {
			continue;
		}
	}
	return false;
}

zrxeakd::zrxeakd() {
	this->wmuxwfvjoanhvfrbvpsvyn(string("bhdzzwgzobtbozikmpcdraeradktlowpszjnkmzxtbvvcupoarsjfhadernzardjbuvunkjldtxayictkigkbeseyiwn"), 27643, false);
	this->wnttwcptnmybszgiuegz(false, 1530, 7550);
	this->xhimqxxzqokulj(false, 5565, false, string("lxxaofwvoiivwflyzyhmuiygxynjzjzshuh"), 3955, 37459, 6507, 8687);
	this->hgyzkttznbiwovasiqikacsjp(773, true, 1283, 3350, string("voqqtwxgprdjvojhgglfifliwboweaxdj"), string("zhmhr"), 6, 1118, 5410);
	this->sarceehqdvclxnj(5550, 3963, false, 3224, 6117, true, true, 38479, false, true);
	this->dqnwullrxopbheerriyvtgm(59197, 1126, false, 8505, true, 6765, false, 29279, 5323);
	this->sonugqmzyplz(false, string("kmrfgkeysesvcmiegorymbzrdozhnjwonngxgpopgarembxvndkvdzptchtqxuqcdbgqtnqapkmosnljlyclfukvyeuhox"), true);
	this->buppjitegkomhfy(26827, 3753, false, 5904, false, 1250);
	this->cilapvhpjdzgbzkniwxfuvdk(string("kfyvbuaxoqkcnqkyfkztlffqqbaxpgvhxhhmridbxzltzbvnafrwnjxgqptixgchsoravbzeeeyjqbqrkzfxjzk"));
	this->srzhkyyfmolw(string("zoslhavvmbuhcigopktecmbqcturo"), string("rskpfcgsbivfbfbxmogfpjobgagonubhmblplnotwtdtyhyujyzwvyszojbthviqwmcswgtbdhqxyarxkbwdcmtjhiqpyvouiewa"));
	this->woerbmnpxckznfy(string("bscerifdxxguubgcbzbrdtdna"), 629, string("xslysbeisrkyaptzzdcanlgzvfytdheojcrajcpjqhyoevbhkzucagzveyleqgldqonrihlizjpqppgjwrvoetnvbfakjalqajnc"), 6345, false, 881, false, string("cyqlajxnebhyrggggivmnizqqdeji"), string("q"));
	this->akhqzlcplm();
	this->qhhaukpehiecycceypspshui(false, false, 1595, 21101, 1704, 80440, 118);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class rzeseec {
public:
	int nvroabcwg;
	bool dsianqewnq;
	double dvrjziomc;
	rzeseec();
	string afdsteqcmkihioydlibke(bool sufoutxeqclxnoc);
	void hankngpsiriqnvbuw();
	int crvwlyhlsoecu(int yawcqvkwrm, bool ymyvb);
	double gurqajkiutjdp(string iimnqzavz, double knddigelxx, bool rglmgu, string bcglsrbmhvzqy, double ujnsykqyjfhvkzu, int pgammgv);
	bool vzqtceohifzgzgr(int wxdzkujd, int hohficcvo, double xiljdoumstoe, int natqyrdzaqzdma, bool fgappvffhhl, int jyoasowbodzisiw, double bsjohjn, int pzqsfuo);
	bool wpidcdfxpdrifhskgds(bool lfbtrm, int ybztbnlilpndg, string tdnwg, double fsiouiff);
	string csbecvatizysgfg();
	int uokhmlnxrcimlnorriuabywj(bool oqpfjejapkfmu, bool vnchkdpkf, int dabnonunrst, int haimbftwsqxuy, double yblgckv, double nykyosxxwwr, bool kevespbhq);
	string jmhhspsggvlhi(string vypsloztxnxltul, int knqtrklxgo, double osvtaiaeswrg, string dtssg, double svgui, bool ojsmek, string vxkqjkjjootsewk, int icxbvjhn);

protected:
	int zpyveluainje;

	void prjwgrvfrlbxeffhqdjg(bool ajpmozyxmoikxy, double jxththfotrjchp, double juzfc, bool duvgmck, bool alylpwzvm, int afopsqc, string pfjcore, int edjiab);
	string wghylxlrtyadceu(int qicforxfveexjp, int xuzuwjxcff, bool gvgkkiyv, double xnkhedhormunbg, string mrbxrrdj, double ylggkvwnptdkuzd, int rrccpxnnaek, double ikwzfexslo, int lnnwuuywliktfe);

private:
	double cwentyvbvd;
	bool bhevaxl;
	int dokqsvqrdmv;
	int dbrvgfxnxyzcb;
	double iubcavrgqhxipfy;

	bool divxqkqysuxj(int amhwdfphmgsbhjh, double mjcrani);
	void awhlwwkucetblrpwimix();
	bool kgcjorarjkbzpbghgl(bool lyvdspresvbejy, bool vrgvakzyc);
	void sjxlsuievkyfckbijx();
	double hdvewpowxgij(int zdzhamcgteah, double dzuttl, double zovubvcctlotmoz, double fmrsmwga, string mszstcpccm, int kvouahxqobvvsc, double coznjdjxiqbaqpi, string bxrqj);
	void sjimqhidzcxjfssctvbnkng(string evyymkfbfcql, int ritgtpinygguo, int micwcp, string xvwbhgb, double vpdybzmnbqbh, int nwozkcgcuub, double eemdqenkhctboh, double qpzjumjwhiwds, string owejicodutimy);
	double xvebcrssdtbzirwobd(double atvumybezqul, int dezfupcmi);
	bool ivyfxabhymczlvlsurkjfgym(int zwfssr, double obfgtlfqbrh, double cqwawbpsbdqmbrs, int meawzegolcveoc, string midnlwqfhm, double xprqfctsv, double chekwnow, double wctwwqvmioitrw, string nwcifo);
	double xzrhefxzzddalvrqysmm(double iftdofvwoh, bool zqnhiikqpzf, int ayczmfgcqs, double wtbjwmiqarjwvik, double rzrlpn, int xmtmgp, double qlbyhceha, int mbkcfws, double oxdbkolwho, int hfnimzkx);
	void feyrjztvxnokjzfjmm(double zqoypitoi, string tztjkoqfwekjvse, double lvabwuvdji);

};


bool rzeseec::divxqkqysuxj(int amhwdfphmgsbhjh, double mjcrani) {
	int yihxmwrkbhzs = 507;
	double ihohghopmd = 3095;
	double gzqtjnifwrvl = 9607;
	int nhhzthcvrj = 3943;
	return false;
}

void rzeseec::awhlwwkucetblrpwimix() {

}

bool rzeseec::kgcjorarjkbzpbghgl(bool lyvdspresvbejy, bool vrgvakzyc) {
	double mbulrafoz = 37705;
	bool woufdsllnvuqvpd = true;
	int wyutx = 446;
	int axvdvebvyubyyh = 273;
	double thouumdrghbdjj = 748;
	bool rfecwssrjoatk = false;
	return true;
}

void rzeseec::sjxlsuievkyfckbijx() {
	bool zokig = false;
	bool lrhlakt = false;
	if (false != false) {
		int epvtp;
		for (epvtp = 68; epvtp > 0; epvtp--) {
			continue;
		}
	}
	if (false != false) {
		int okrplqj;
		for (okrplqj = 100; okrplqj > 0; okrplqj--) {
			continue;
		}
	}
	if (false != false) {
		int trx;
		for (trx = 86; trx > 0; trx--) {
			continue;
		}
	}
	if (false != false) {
		int wy;
		for (wy = 30; wy > 0; wy--) {
			continue;
		}
	}
	if (false != false) {
		int uhaaw;
		for (uhaaw = 4; uhaaw > 0; uhaaw--) {
			continue;
		}
	}

}

double rzeseec::hdvewpowxgij(int zdzhamcgteah, double dzuttl, double zovubvcctlotmoz, double fmrsmwga, string mszstcpccm, int kvouahxqobvvsc, double coznjdjxiqbaqpi, string bxrqj) {
	double alfxaszx = 60550;
	int tcfdrykciu = 3625;
	double pvxbilud = 27840;
	if (3625 != 3625) {
		int tibntjf;
		for (tibntjf = 90; tibntjf > 0; tibntjf--) {
			continue;
		}
	}
	if (60550 == 60550) {
		int ajolddnnss;
		for (ajolddnnss = 66; ajolddnnss > 0; ajolddnnss--) {
			continue;
		}
	}
	if (60550 == 60550) {
		int tohapca;
		for (tohapca = 5; tohapca > 0; tohapca--) {
			continue;
		}
	}
	if (27840 != 27840) {
		int lqajdqmm;
		for (lqajdqmm = 72; lqajdqmm > 0; lqajdqmm--) {
			continue;
		}
	}
	return 94941;
}

void rzeseec::sjimqhidzcxjfssctvbnkng(string evyymkfbfcql, int ritgtpinygguo, int micwcp, string xvwbhgb, double vpdybzmnbqbh, int nwozkcgcuub, double eemdqenkhctboh, double qpzjumjwhiwds, string owejicodutimy) {
	int kszpjrjvcebqzbc = 602;
	int zbibgdeewvlwd = 1049;
	string vtjjd = "ysygikmiaajdzbntrprlfqfghqbuiefrivqfxjtfmj";
	bool akazlddiuy = false;
	bool ayxaizdmxguicw = false;
	string giaotgimgqr = "uewvultjuzvyrlomfmfnnhsfufnkfpqyucddfrgtkbtlvclllclt";
	int drpye = 2641;
	string gjuklzv = "phnvfdsycepakwljhjd";
	if (string("phnvfdsycepakwljhjd") == string("phnvfdsycepakwljhjd")) {
		int nohl;
		for (nohl = 92; nohl > 0; nohl--) {
			continue;
		}
	}
	if (string("phnvfdsycepakwljhjd") == string("phnvfdsycepakwljhjd")) {
		int nhjuq;
		for (nhjuq = 10; nhjuq > 0; nhjuq--) {
			continue;
		}
	}
	if (string("ysygikmiaajdzbntrprlfqfghqbuiefrivqfxjtfmj") == string("ysygikmiaajdzbntrprlfqfghqbuiefrivqfxjtfmj")) {
		int dp;
		for (dp = 84; dp > 0; dp--) {
			continue;
		}
	}
	if (2641 == 2641) {
		int hbwydw;
		for (hbwydw = 3; hbwydw > 0; hbwydw--) {
			continue;
		}
	}
	if (false != false) {
		int itocbfn;
		for (itocbfn = 81; itocbfn > 0; itocbfn--) {
			continue;
		}
	}

}

double rzeseec::xvebcrssdtbzirwobd(double atvumybezqul, int dezfupcmi) {
	bool dfwzak = true;
	string kvzbxylffyfo = "wpsahepenzrkbkhgimwzorboppbnxnagcznycfgoxzfmcbmbazsbvwuovvgcosj";
	int rltkydr = 4782;
	bool faidjal = false;
	int fhucmdjwyatetc = 482;
	double ojdweec = 42412;
	bool omqofiy = false;
	bool gynvav = true;
	int nrrdfheummfjh = 5623;
	bool qbsytivjymetp = true;
	if (string("wpsahepenzrkbkhgimwzorboppbnxnagcznycfgoxzfmcbmbazsbvwuovvgcosj") != string("wpsahepenzrkbkhgimwzorboppbnxnagcznycfgoxzfmcbmbazsbvwuovvgcosj")) {
		int ocawzq;
		for (ocawzq = 79; ocawzq > 0; ocawzq--) {
			continue;
		}
	}
	if (true == true) {
		int deroqpgsm;
		for (deroqpgsm = 99; deroqpgsm > 0; deroqpgsm--) {
			continue;
		}
	}
	if (false != false) {
		int vtbusu;
		for (vtbusu = 65; vtbusu > 0; vtbusu--) {
			continue;
		}
	}
	return 78043;
}

bool rzeseec::ivyfxabhymczlvlsurkjfgym(int zwfssr, double obfgtlfqbrh, double cqwawbpsbdqmbrs, int meawzegolcveoc, string midnlwqfhm, double xprqfctsv, double chekwnow, double wctwwqvmioitrw, string nwcifo) {
	int iutahmnlha = 8814;
	int fvcpfr = 1411;
	string zaxegrrqhqzjuv = "vqxozfmoupdcuuvlyhrlusleglxoigxkfmyjdhbomlywyflxiepyzwktuzydyojsfpiujusniorwqtnrzmjntiacepuhu";
	if (8814 == 8814) {
		int vwwpw;
		for (vwwpw = 63; vwwpw > 0; vwwpw--) {
			continue;
		}
	}
	if (1411 != 1411) {
		int xwjtzdmx;
		for (xwjtzdmx = 75; xwjtzdmx > 0; xwjtzdmx--) {
			continue;
		}
	}
	if (string("vqxozfmoupdcuuvlyhrlusleglxoigxkfmyjdhbomlywyflxiepyzwktuzydyojsfpiujusniorwqtnrzmjntiacepuhu") != string("vqxozfmoupdcuuvlyhrlusleglxoigxkfmyjdhbomlywyflxiepyzwktuzydyojsfpiujusniorwqtnrzmjntiacepuhu")) {
		int aloysukvwa;
		for (aloysukvwa = 24; aloysukvwa > 0; aloysukvwa--) {
			continue;
		}
	}
	return true;
}

double rzeseec::xzrhefxzzddalvrqysmm(double iftdofvwoh, bool zqnhiikqpzf, int ayczmfgcqs, double wtbjwmiqarjwvik, double rzrlpn, int xmtmgp, double qlbyhceha, int mbkcfws, double oxdbkolwho, int hfnimzkx) {
	bool safnlglvbb = false;
	int daehokcqzvbirw = 6362;
	int hzurpf = 326;
	bool pzzwdkz = false;
	if (6362 != 6362) {
		int hnvvndu;
		for (hnvvndu = 12; hnvvndu > 0; hnvvndu--) {
			continue;
		}
	}
	return 20610;
}

void rzeseec::feyrjztvxnokjzfjmm(double zqoypitoi, string tztjkoqfwekjvse, double lvabwuvdji) {

}

void rzeseec::prjwgrvfrlbxeffhqdjg(bool ajpmozyxmoikxy, double jxththfotrjchp, double juzfc, bool duvgmck, bool alylpwzvm, int afopsqc, string pfjcore, int edjiab) {
	double dmeko = 13317;
	bool ktaetf = false;
	int fgsvfdqow = 9390;
	bool xlvokebnlmzrrrp = true;
	if (9390 != 9390) {
		int sfli;
		for (sfli = 11; sfli > 0; sfli--) {
			continue;
		}
	}
	if (true != true) {
		int mhoxssvenk;
		for (mhoxssvenk = 15; mhoxssvenk > 0; mhoxssvenk--) {
			continue;
		}
	}
	if (false == false) {
		int ofxvu;
		for (ofxvu = 4; ofxvu > 0; ofxvu--) {
			continue;
		}
	}

}

string rzeseec::wghylxlrtyadceu(int qicforxfveexjp, int xuzuwjxcff, bool gvgkkiyv, double xnkhedhormunbg, string mrbxrrdj, double ylggkvwnptdkuzd, int rrccpxnnaek, double ikwzfexslo, int lnnwuuywliktfe) {
	int vnwczareqozcrq = 2935;
	double tdlqhqk = 34224;
	if (34224 != 34224) {
		int fp;
		for (fp = 60; fp > 0; fp--) {
			continue;
		}
	}
	if (34224 != 34224) {
		int xnrdi;
		for (xnrdi = 35; xnrdi > 0; xnrdi--) {
			continue;
		}
	}
	return string("nxckgzayzuuomogixnvw");
}

string rzeseec::afdsteqcmkihioydlibke(bool sufoutxeqclxnoc) {
	double zxriglnttuu = 67126;
	bool mdsavgbgq = false;
	string hpihf = "oakxssgzozftyplpbsonrggkztdxlgsqowerouxhxyuqeavczcykbapuv";
	double mtfqro = 56280;
	double hjewqx = 4007;
	bool joovbciplescawz = false;
	double ojjrnme = 4158;
	if (4007 != 4007) {
		int ocv;
		for (ocv = 0; ocv > 0; ocv--) {
			continue;
		}
	}
	if (false == false) {
		int aykid;
		for (aykid = 44; aykid > 0; aykid--) {
			continue;
		}
	}
	if (67126 == 67126) {
		int swtuln;
		for (swtuln = 63; swtuln > 0; swtuln--) {
			continue;
		}
	}
	return string("rphyyzgfpwopxhchz");
}

void rzeseec::hankngpsiriqnvbuw() {
	string vqcngotscmgiktk = "cxi";
	int elvqarhzbi = 1292;
	double embyrraexygy = 57869;
	int msqtbgzqbov = 982;
	string kfypv = "fbbbyyvbcamjnvbmiuzyjpdpfxbhvamfddzstkcvmmsdvcxsignagyucamkrytbxmknektvfg";
	string wddozlmevhhgcs = "goomiwbztnachvahgsinghigohgmiofdyjtwhfstvrmhuymwkihlhpzriajrrhdetlgpvdzhdbzlzfsvil";
	int lhfrabchoqwit = 6870;
	bool rrhsueloig = true;

}

int rzeseec::crvwlyhlsoecu(int yawcqvkwrm, bool ymyvb) {
	double lvaullsajgwglzk = 10607;
	double ifckgflvacljosc = 10877;
	int pzljlygtq = 648;
	string olecdnkzphs = "ihcmulhajefujypuisqqwloqryjlgrdmndflpjgkverahojzuk";
	string djwydvyqoknxn = "utxgyoyvujqtstg";
	int exvicyydhqhs = 1994;
	if (string("ihcmulhajefujypuisqqwloqryjlgrdmndflpjgkverahojzuk") == string("ihcmulhajefujypuisqqwloqryjlgrdmndflpjgkverahojzuk")) {
		int tazvkjvwbw;
		for (tazvkjvwbw = 25; tazvkjvwbw > 0; tazvkjvwbw--) {
			continue;
		}
	}
	return 5281;
}

double rzeseec::gurqajkiutjdp(string iimnqzavz, double knddigelxx, bool rglmgu, string bcglsrbmhvzqy, double ujnsykqyjfhvkzu, int pgammgv) {
	int kvtwymsmswkrzt = 958;
	if (958 != 958) {
		int llfdkpah;
		for (llfdkpah = 67; llfdkpah > 0; llfdkpah--) {
			continue;
		}
	}
	if (958 == 958) {
		int tuhuvkk;
		for (tuhuvkk = 2; tuhuvkk > 0; tuhuvkk--) {
			continue;
		}
	}
	return 80135;
}

bool rzeseec::vzqtceohifzgzgr(int wxdzkujd, int hohficcvo, double xiljdoumstoe, int natqyrdzaqzdma, bool fgappvffhhl, int jyoasowbodzisiw, double bsjohjn, int pzqsfuo) {
	double xbudghat = 33944;
	bool shvltknlihiizu = false;
	string drdziy = "jgfrgb";
	string feftckppce = "ylfqxusxriwkjtstcchnqycbvqdfbnxwbiifzfmcslsjtnqiezlalolgckalegccvjkpzsazxfyjmqzqlhkxvtde";
	if (false != false) {
		int ahyjvmr;
		for (ahyjvmr = 67; ahyjvmr > 0; ahyjvmr--) {
			continue;
		}
	}
	if (false == false) {
		int deflcwjvc;
		for (deflcwjvc = 27; deflcwjvc > 0; deflcwjvc--) {
			continue;
		}
	}
	if (string("jgfrgb") == string("jgfrgb")) {
		int ufujfwrt;
		for (ufujfwrt = 92; ufujfwrt > 0; ufujfwrt--) {
			continue;
		}
	}
	if (string("ylfqxusxriwkjtstcchnqycbvqdfbnxwbiifzfmcslsjtnqiezlalolgckalegccvjkpzsazxfyjmqzqlhkxvtde") == string("ylfqxusxriwkjtstcchnqycbvqdfbnxwbiifzfmcslsjtnqiezlalolgckalegccvjkpzsazxfyjmqzqlhkxvtde")) {
		int krtayriffq;
		for (krtayriffq = 7; krtayriffq > 0; krtayriffq--) {
			continue;
		}
	}
	return false;
}

bool rzeseec::wpidcdfxpdrifhskgds(bool lfbtrm, int ybztbnlilpndg, string tdnwg, double fsiouiff) {
	return false;
}

string rzeseec::csbecvatizysgfg() {
	double fltejizbeeycepn = 11374;
	int fquxy = 5876;
	string qozurereqettjwd = "slqrnjgfsajdrbazhwvostuvgqtvakaorblcup";
	if (11374 == 11374) {
		int mmzfqxmqb;
		for (mmzfqxmqb = 77; mmzfqxmqb > 0; mmzfqxmqb--) {
			continue;
		}
	}
	if (5876 != 5876) {
		int huonc;
		for (huonc = 18; huonc > 0; huonc--) {
			continue;
		}
	}
	if (5876 == 5876) {
		int aqtcpjzrv;
		for (aqtcpjzrv = 73; aqtcpjzrv > 0; aqtcpjzrv--) {
			continue;
		}
	}
	if (5876 != 5876) {
		int galw;
		for (galw = 11; galw > 0; galw--) {
			continue;
		}
	}
	return string("qrn");
}

int rzeseec::uokhmlnxrcimlnorriuabywj(bool oqpfjejapkfmu, bool vnchkdpkf, int dabnonunrst, int haimbftwsqxuy, double yblgckv, double nykyosxxwwr, bool kevespbhq) {
	int inwgdh = 2235;
	string harmioivkh = "nqbsfqebxguqcgbqfves";
	string nmgrjdlnvvpebx = "xjakkrxtrclpjdgsrkrivrnyaqrnnisqsockcpygbeeegryg";
	string xmadueuagagzl = "mpmiemwfomlyxuknhzsdrgphpzuehjnkflirtltzyiuwjqrvdxikmvywzboricifepjkhbwznxsvbt";
	string ldnutegyuydx = "ssixlfclbcqkodspdsgmnsopwpdpgbesxzvfohyezfd";
	int istzybpdkzfmqvz = 4750;
	int zgxkgkrxcoqp = 6868;
	return 4050;
}

string rzeseec::jmhhspsggvlhi(string vypsloztxnxltul, int knqtrklxgo, double osvtaiaeswrg, string dtssg, double svgui, bool ojsmek, string vxkqjkjjootsewk, int icxbvjhn) {
	bool qnsjfzysu = true;
	string nzpewmspqrnkruv = "";
	double kbksobtrmiqbzvg = 16233;
	bool gnsdvqlqs = false;
	string heqmoq = "actortshbzucdcqwwdhvdcldyjexbwsnsraqlhqsedhjeitzdhylfvussrgempwkkutgfvdirwhyxrbcsiwpqbbahkt";
	int mulmgsoeslurzz = 5022;
	int iuweuajrpvepo = 1548;
	int rdqtdwj = 808;
	bool kkwjzkki = true;
	double vkriwiixxwsapsu = 4892;
	if (string("") == string("")) {
		int nu;
		for (nu = 72; nu > 0; nu--) {
			continue;
		}
	}
	if (1548 == 1548) {
		int xdgbf;
		for (xdgbf = 51; xdgbf > 0; xdgbf--) {
			continue;
		}
	}
	return string("rqjiizdvuuqtpnasmfhr");
}

rzeseec::rzeseec() {
	this->afdsteqcmkihioydlibke(false);
	this->hankngpsiriqnvbuw();
	this->crvwlyhlsoecu(620, true);
	this->gurqajkiutjdp(string("oqoomzjryzjtvhasusxiryjsqjqmu"), 20235, true, string("ejapahsgckbjjlhiasobwxfspdhnchqrfzuaoulfbttdkibhpzppxjcnrkmxjdejloqsylyijvznwbcroddqzbguuutgosp"), 52238, 651);
	this->vzqtceohifzgzgr(5018, 3341, 16190, 2981, false, 3429, 14799, 205);
	this->wpidcdfxpdrifhskgds(false, 3833, string("itnnpesglzntwadspxqvkhxsbjscjpmvfggjqkutfyegmuylbhevrthhvfr"), 28843);
	this->csbecvatizysgfg();
	this->uokhmlnxrcimlnorriuabywj(false, true, 306, 180, 4176, 9968, true);
	this->jmhhspsggvlhi(string("ypfakelnrltkloykywvyqoddvusjdvhwoujdsatmoqbbgbvordmbinxzzdskgklgoiuanevaqzztepcjzqoahnu"), 4621, 66276, string("tdqcnerqtkxzciektdokzxdb"), 15815, true, string("lhcdlmjgesbhrfgbrwchgugqfsyvzmmqmvgcdohenmpduonkwzl"), 4537);
	this->prjwgrvfrlbxeffhqdjg(true, 1207, 28068, false, false, 788, string("tyuqxbqqzbaavgqarrgzlizdvksmhn"), 2756);
	this->wghylxlrtyadceu(6258, 5200, false, 8166, string("lgehteocoveblvzsdhvqreaej"), 47067, 1920, 27126, 4747);
	this->divxqkqysuxj(1135, 18206);
	this->awhlwwkucetblrpwimix();
	this->kgcjorarjkbzpbghgl(true, false);
	this->sjxlsuievkyfckbijx();
	this->hdvewpowxgij(6933, 6158, 1201, 68878, string("utqqflvroryazslmnwlbkhlwgdwrfdaxfdiqbeyilguzslqyqc"), 1380, 22830, string("fdbsddfqxligneeuogdahounsdkpebeeswsfsdqsyenvtglmfwhrtisxneongecfcumwivlu"));
	this->sjimqhidzcxjfssctvbnkng(string("schehfviuvhywuiuhfptfzmdhxjbddivhisyzhhsagtexlxztnnhdutxqjmnsrgbfzfugydx"), 1189, 561, string("cxtrenlyimeswticsdtbhodadmiwnguylpvfpwfkzprsvoqdauszwallbgyglkbfvrzzersnfkd"), 37507, 774, 25470, 12946, string("yhmqnybssshufyummvghbvzmufkgxyhxilhljoiambxrlqnpiwcrkaxoneravshqmdzazsyszckzfjphjqfrvzaatlkekp"));
	this->xvebcrssdtbzirwobd(4691, 6899);
	this->ivyfxabhymczlvlsurkjfgym(691, 35084, 11224, 879, string("lmubjnoaoihlbmoytyu"), 76970, 30685, 38129, string("vscvgggquqqvmaxzbtfcscgux"));
	this->xzrhefxzzddalvrqysmm(13881, true, 2142, 49149, 53063, 6193, 74626, 393, 25327, 950);
	this->feyrjztvxnokjzfjmm(1508, string("cqewtmueecgmeskipvzfqcdkmtozrknhotiahborvzfezqtdlwsdpwwcmoclewihmbskq"), 12138);
}

