
#include "LegitBot.h"
#include "Render.h"
#include "SDK.h"
#include "EnginePrediction.h"
#include "Global.h"
#define NOMINMAX
#include <Windows.h>
#include <stdio.h>
#include <random>
#include <string>
#include <vector>

legitbot::legitbot()
{
	best_target = -1;

	view_angle = QAngle(0.0f, 0.0f, 0.0f);
	aim_angle = QAngle(0.0f, 0.0f, 0.0f);
	delta_angle = QAngle(0.0f, 0.0f, 0.0f);
	final_angle = QAngle(0.0f, 0.0f, 0.0f);

	hitbox_position = Vector(0.0f, 0.0f, 0.0f);

	aim_key = 0;
	aim_smooth = 1;
	aim_fov = 0;
	randomized_smooth = 0;
	recoil_min = 0;
	recoil_max = 0;
	randomized_angle = 0;
	shoot = false;
}
float get_fov(const QAngle &viewAngles, const QAngle &aimAngles)
{
	Vector ang, aim;
	AngleVectors(viewAngles, &aim);
	AngleVectors(aimAngles, &ang);

	return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
}
float random_number_range(float min, float max)
{
	std::random_device device;
	std::mt19937 engine(device());
	std::uniform_real_distribution<> distribution(min, max);
	return static_cast< float >(distribution(engine));
}


bool shoot;
static int custom_delay = 0;

void legitbot::OnCreateMove(CInput::CUserCmd *pCmd, C_BaseEntity *local)
{
	if (!g_Options.Legitbot.MainSwitch)
		return;

	CBaseCombatWeapon* pWeapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(local->GetActiveWeaponHandle());

	if (local && local->IsAlive() && pWeapon)
	{
		do_aimbot(local, pWeapon, pCmd);

		if (!G::PressedKeys[g_Options.Legitbot.Triggerbot.Key]) custom_delay = 0;

		if (g_Options.Legitbot.Triggerbot.Enabled && g_Options.Legitbot.Triggerbot.Key != 0 && G::PressedKeys[g_Options.Legitbot.Triggerbot.Key])
			triggerbot(pCmd, local, pWeapon);
	}
}

void legitbot::triggerbot(CInput::CUserCmd *cmd, C_BaseEntity* local, CBaseCombatWeapon* weapon)
{
	if (!local->IsAlive())
		return;

	if (weapon) {
		if (weapon->ammo() == 0)
			return;
		if (MiscFunctions::IsKnife(weapon) || MiscFunctions::IsGrenade(weapon)) return;
		if (*weapon->m_AttributeManager()->m_Item()->ItemDefinitionIndex() == 64) return;
	}



	QAngle ViewAngles = cmd->viewangles + local->localPlayerExclusive()->GetAimPunchAngle();


	Vector CrosshairForward;
	AngleVectors(ViewAngles, &CrosshairForward);
	CrosshairForward *= weapon->GetCSWpnData()->m_fRange;


	Vector TraceSource = local->GetEyePosition();
	Vector TraceDestination = TraceSource + CrosshairForward;

	Ray_t Ray;
	trace_t Trace;
	CTraceFilter Filter;

	Filter.pSkip = local;

	Ray.Init(TraceSource, TraceDestination);
	g_EngineTrace->TraceRay(Ray, MASK_SHOT, &Filter, &Trace);

	if (!Trace.m_pEnt)
		return;
	if (!Trace.m_pEnt->IsAlive())
		return;
	if (Trace.m_pEnt->HasGunGameImmunity())
		return;


	if (local->GetTeamNum() == Trace.m_pEnt->GetTeamNum())
		return;

	if (!hit_chance(local, cmd, weapon, Trace.m_pEnt))
		return;

	bool didHit = false;
	if ((g_Options.Legitbot.Triggerbot.Filter.Head && Trace.hitgroup == 1)
		|| (g_Options.Legitbot.Triggerbot.Filter.Chest && Trace.hitgroup == 2)
		|| (g_Options.Legitbot.Triggerbot.Filter.Stomach && Trace.hitgroup == 3)
		|| (g_Options.Legitbot.Triggerbot.Filter.Arms && (Trace.hitgroup == 4 || Trace.hitgroup == 5))
		|| (g_Options.Legitbot.Triggerbot.Filter.Legs && (Trace.hitgroup == 6 || Trace.hitgroup == 7)))
	{
		didHit = true;
	}

	if (g_Options.Legitbot.Triggerbot.Delay >= 1)
	{
		if (custom_delay >= g_Options.Legitbot.Triggerbot.Delay / 30)
		{
			if (didHit)
			{
				custom_delay = 0;
				shoot = true;
				cmd->buttons |= IN_ATTACK;
			}
		}
		else
		{
			custom_delay++;
		}
	}

}

void legitbot::do_aimbot(C_BaseEntity *local, CBaseCombatWeapon *weapon, CInput::CUserCmd *cmd)
{
	if (!g_Options.Legitbot.MainSwitch)
		return;

	if (!weapon)
		return;

	if (!local)
		return;
	if (!cmd)
		return;

	if (!local->IsAlive())
		return;

	if (!weapon->ammo() > 0)
		return;


	if (weapon->ammo() == 0)
		return;

	if (MiscFunctions::IsKnife(weapon) || MiscFunctions::IsGrenade(weapon))
		return;


	weapon_settings(weapon);

	if (!aim_key)
		return;

	if (!G::PressedKeys[aim_key])
		return;



	best_target = get_target(local, weapon, cmd, hitbox_position);


	if (best_target == -1)
		return;
	C_BaseEntity* entity = (C_BaseEntity*)g_EntityList->GetClientEntity(best_target);
	if (!entity)
		return;



	if (get_distance(local->GetEyePosition(), hitbox_position) > 8192.0f)
		return;


	compute_angle(local->GetEyePosition(), hitbox_position, aim_angle);
	sanitize_angles(aim_angle);

	if (hitbox_position == Vector(0, 0, 0))
		return;

	aim_angle -= get_randomized_recoil(local);
	aim_angle += get_randomized_angles(local);

	sanitize_angles(view_angle);

	delta_angle = view_angle - aim_angle;
	sanitize_angles(delta_angle);

	float randomSmoothing = 1.0f;

	if (randomized_smooth > 1.0f)
		randomSmoothing = random_number_range(randomized_smooth / 10.0f, 1.0f);

	final_angle = view_angle - delta_angle / aim_smooth * randomSmoothing;
	sanitize_angles(final_angle);

	if (!sanitize_angles(final_angle))
		return;

	cmd->viewangles = final_angle;
	g_Engine->SetViewAngles(cmd->viewangles);
}

bool legitbot::hit_chance(C_BaseEntity* local, CInput::CUserCmd* cmd, CBaseCombatWeapon* weapon, C_BaseEntity* target)
{
	Vector forward, right, up;

	constexpr auto max_traces = 150;

	AngleVectors(cmd->viewangles, &forward, &right, &up);

	int total_hits = 0;
	int needed_hits = static_cast<int>(max_traces * (g_Options.Legitbot.Triggerbot.hitchance / 100.f));

	weapon->UpdateAccuracyPenalty(weapon);

	auto eyes = local->GetEyePosition();
	auto flRange = weapon->GetCSWpnData()->m_fRange;

	for (int i = 0; i < max_traces; i++) {
		RandomSeed(i + 1);

		float fRand1 = RandomFloat(0.f, 1.f);
		float fRandPi1 = RandomFloat(0.f, XM_2PI);
		float fRand2 = RandomFloat(0.f, 1.f);
		float fRandPi2 = RandomFloat(0.f, XM_2PI);

		float fRandInaccuracy = fRand1 * weapon->GetInaccuracy();
		float fRandSpread = fRand2 * weapon->GetSpread();

		float fSpreadX = cos(fRandPi1) * fRandInaccuracy + cos(fRandPi2) * fRandSpread;
		float fSpreadY = sin(fRandPi1) * fRandInaccuracy + sin(fRandPi2) * fRandSpread;

		auto viewSpreadForward = (forward + fSpreadX * right + fSpreadY * up).Normalized();

		QAngle viewAnglesSpread;
		VectorAngles(viewSpreadForward, viewAnglesSpread);
		sanitize_angles(viewAnglesSpread);

		Vector viewForward;
		AngleVectors(viewAnglesSpread, &viewForward);
		viewForward.NormalizeInPlace();

		viewForward = eyes + (viewForward * flRange);

		trace_t tr;
		Ray_t ray;
		ray.Init(eyes, viewForward);

		g_EngineTrace->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, target, &tr);


		if (tr.m_pEnt == target)
			total_hits++;

		if (total_hits >= needed_hits)
			return true;

		if ((max_traces - i + total_hits) < needed_hits)
			return false;
	}

	return false;
}

void legitbot::weapon_settings(CBaseCombatWeapon* weapon)
{
	if (!weapon)
		return;


	if (MiscFunctions::IsSniper(weapon))
	{
		aim_key = g_Options.Legitbot.SniperKey;
		aim_smooth = g_Options.Legitbot.SniperSmooth;
		aim_fov = g_Options.Legitbot.Sniperfov;
		randomized_smooth = 1;
		recoil_min = g_Options.Legitbot.sniper_recoil_min;
		recoil_max = g_Options.Legitbot.sniper_recoil_max;
		randomized_angle = 1;

	}
	else if (MiscFunctions::IsPistol(weapon))
	{
		aim_key = g_Options.Legitbot.PistolKey;
		aim_smooth = g_Options.Legitbot.PistolSmooth;
		aim_fov = g_Options.Legitbot.Pistolfov;
		randomized_smooth = 1;
		recoil_min = g_Options.Legitbot.pistol_recoil_min;
		recoil_max = g_Options.Legitbot.pistol_recoil_max;
		randomized_angle = 1;
	}
	else
	{
		aim_key = g_Options.Legitbot.MainKey;
		aim_smooth = g_Options.Legitbot.MainSmooth;
		aim_fov = g_Options.Legitbot.Mainfov;
		randomized_smooth = 1;
		recoil_min = g_Options.Legitbot.main_recoil_min;
		recoil_max = g_Options.Legitbot.main_recoil_max;
		randomized_angle = 1;
	}
}

QAngle legitbot::get_randomized_recoil(C_BaseEntity *local)
{
	QAngle compensatedAngles = (local->localPlayerExclusive()->GetAimPunchAngle() * 2.0f) * (random_number_range(recoil_min, recoil_max) / 100.0f);
	sanitize_angles(compensatedAngles);

	return (local->m_iShotsFired() > 1 ? compensatedAngles : QAngle(0.0f, 0.0f, 0.0f));
}

QAngle legitbot::get_randomized_angles(C_BaseEntity *local)
{
	QAngle randomizedValue = QAngle(0.0f, 0.0f, 0.0f);

	float randomRate = random_number_range(-randomized_angle, randomized_angle);
	float randomDeviation = random_number_range(-randomized_angle, randomized_angle);

	switch (rand() % 2)
	{
	case 0:
		randomizedValue.x = (randomRate * cos(randomDeviation));
		randomizedValue.y = (randomRate * cos(randomDeviation));
		randomizedValue.z = (randomRate * cos(randomDeviation));
		break;
	case 1:
		randomizedValue.x = (randomRate * sin(randomDeviation));
		randomizedValue.y = (randomRate * sin(randomDeviation));
		randomizedValue.z = (randomRate * sin(randomDeviation));
		break;
	}

	sanitize_angles(randomizedValue);

	return (local->m_iShotsFired() > 1 ? randomizedValue : QAngle(0.0f, 0.0f, 0.0f));
}
bool get_hitbox_pos(C_BaseEntity* entity, int hitbox, Vector &output)
{
	if (hitbox >= 20)
		return false;

	const model_t *model = entity->GetModel();
	if (!model)
		return false;

	studiohdr_t *studioHdr = g_ModelInfo->GetStudiomodel(model);
	if (!studioHdr)
		return false;

	matrix3x4 matrix[128];
	if (!entity->SetupBones(matrix, 128, 0x100, entity->GetSimulationTime()))
		return false;

	mstudiobbox_t *studioBox = studioHdr->GetHitboxSet(0)->GetHitbox(hitbox);
	if (!studioBox)
		return false;

	Vector min, max;

	VectorTransform(studioBox->bbmin, matrix[studioBox->bone], min);
	VectorTransform(studioBox->bbmax, matrix[studioBox->bone], max);

	output = (min + max) * 0.5f;

	return true;
}
bool legitbot::get_hitbox(C_BaseEntity *local, C_BaseEntity *entity, Vector &destination)
{
	int bestHitbox = -1;
	float best_fov = aim_fov;

	static const std::vector<int> hitboxes = { (int)CSGOHitboxID::Head, (int)CSGOHitboxID::Neck, (int)CSGOHitboxID::Chest, (int)CSGOHitboxID::Stomach, (int)CSGOHitboxID::Pelvis };

	for (auto hitbox : hitboxes)
	{
		Vector temp;
		if (!get_hitbox_pos(entity, hitbox, temp))
			continue;

		float fov = get_fov(view_angle, compute_angle(local->GetEyePosition(), temp));
		if (fov < best_fov)
		{
			best_fov = fov;
			bestHitbox = hitbox;
		}
	}

	if (bestHitbox != -1)
	{
		if (!get_hitbox_pos(entity, bestHitbox, destination))
			return true;
	}

	return false;
}


int legitbot::get_target(C_BaseEntity *local, CBaseCombatWeapon *weapon, CInput::CUserCmd *cmd, Vector &destination)
{
	int best_target = -1;
	float best_fov = aim_fov;

	g_Engine->GetViewAngles(view_angle);

	for (int i = 1; i <= g_Globals->maxClients; i++)
	{
		C_BaseEntity *entity = (C_BaseEntity*)g_EntityList->GetClientEntity(i);
		if (!entity
			|| entity == local
			|| entity->IsDormant()
			|| entity->GetLifeState() != LIFE_ALIVE
			|| entity->HasGunGameImmunity()
			|| entity->GetClientClass()->m_ClassID != (int)ClassID::CCSPlayer
			|| entity->GetTeamNum() == local->GetTeamNum()
			|| !(entity->GetFlags() & FL_ONGROUND))
			continue;

		Vector hitbox;
		if (get_hitbox(local, entity, hitbox))
			continue;

		float fov = get_fov(view_angle + (local->localPlayerExclusive()->GetAimPunchAngle() * 2.0f), compute_angle(local->GetEyePosition(), hitbox));
		if (fov < best_fov && fov < aim_fov)
		{
			if (MiscFunctions::IsVisible(local, entity, 0))
			{
				best_fov = fov;
				destination = hitbox;
				best_target = i;
			}
		}
	}

	return best_target;
}

/*bool legitbot::get_hitbox(CInput::CUserCmd *cmd, CBaseCombatWeapon* weapon, C_BaseEntity *local, C_BaseEntity *entity, Vector &destination)
{
int bestHitbox = -1;
float best_fov = aim_fov;

QAngle ViewAngles = cmd->viewangles + local->localPlayerExclusive()->GetAimPunchAngle();

Vector CrosshairForward;
AngleVectors(ViewAngles, &CrosshairForward);
CrosshairForward *= weapon->GetCSWpnData()->m_fRange;


Vector TraceSource = local->GetEyePosition();
Vector TraceDestination = TraceSource + CrosshairForward;

Ray_t Ray;
trace_t Trace;
CTraceFilter Filter;

Filter.pSkip = local;

Ray.Init(TraceSource, TraceDestination);
g_EngineTrace->TraceRay(Ray, MASK_SHOT, &Filter, &Trace);

if (!Trace.m_pEnt)
return;
if (!Trace.m_pEnt->IsAlive())
return;
if (Trace.m_pEnt->HasGunGameImmunity())
return;


if (local->GetTeamNum() == Trace.m_pEnt->GetTeamNum())
return;

if ((g_Options.Legitbot.Filter.Head && Trace.hitgroup == 1)
|| (g_Options.Legitbot.Filter.Chest && Trace.hitgroup == 2)
|| (g_Options.Legitbot.Filter.Stomach && Trace.hitgroup == 3)
|| (g_Options.Legitbot.Filter.Arms && (Trace.hitgroup == 4 || Trace.hitgroup == 5))
|| (g_Options.Legitbot.Filter.Legs && (Trace.hitgroup == 6 || Trace.hitgroup == 7)))


for (auto hitbox : didhit)
{
Vector temp;
if (!get_hitbox_pos(entity, hitbox, temp))
continue;

float fov = get_fov(view_angle, compute_angle(local->GetEyePosition(), temp));
if (fov < best_fov)
{
best_fov = fov;
bestHitbox = hitbox;
}
}

if (bestHitbox != -1)
{
if (!get_hitbox_pos(entity, bestHitbox, destination))
return true;
}

return false;
}*/

namespace junk7735438 {
	void junk10332142() {
		return;
		float x9xmd;
		float kzyt5;
		float tbil31m;
		float zt51l;
		float v032eg;
		float see27l;
		float g09xc;
		float jrxxrl;
		float ceynua;
		float kibepu;
		while (jrxxrl == 9088588.5858)
		{
			jrxxrl = 288583.2727;
		}
		jrxxrl = 8468620.4494;
		if (tbil31m == 3280718.0007)
			tbil31m = 8889047.1600;
		g09xc = 7758730.4030;
		kibepu = 5609191.9946;
		tbil31m = 9553466.5960;
		kzyt5 = 9087333.1480;
		if (tbil31m == 7503784.9975)
			tbil31m = 179634.8682;
		x9xmd = 2346087.2185;
		if (ceynua == 7793240.2445)
			ceynua = 10539205.4526;
		v032eg = 8254626.7954;
		if (ceynua == 7991774.0097)
			ceynua = 8877957.7850;
		g09xc = 9019494.7855;
		for (int zugfa = 0; zugfa > 100; zugfa++)
		{
			g09xc = 5624794.4234;
		}
		zt51l = 2890963.1620;
		for (int u9hty = 0; u9hty > 100; u9hty++)
		{
			see27l = 240142.7336;
		}
		see27l = 7531885.4816;
	}
	void junk383444() {
		return;
		float lt4oxv;
		float iuwau9;
		float demt2;
		float p2x3jj;
		float gy6ygt;
		float fvg5c;
		float majftq;
		float myexi;
		float f4t3y;
		float l6uf5n;
		if (p2x3jj == 1326150.4225)
			p2x3jj = 2066369.4536;
		demt2 = 4706644.1466;
		while (f4t3y == 9324207.1051)
		{
			f4t3y = 4102652.0217;
		}
		l6uf5n = 8462540.7136;
		p2x3jj = 2853371.9294;
		if (gy6ygt == 5053189.6062)
			gy6ygt = 3204746.6159;
		iuwau9 = 3609601.1542;
		if (lt4oxv == 3084070.5063)
			lt4oxv = 4142419.0233;
		myexi = 10058740.0876;
		iuwau9 = 10008430.8872;
		if (myexi == 7635795.2502)
			myexi = 2543926.5265;
		fvg5c = 6993725.5590;
		if (majftq == 5948466.2785)
			majftq = 5659606.5870;
		f4t3y = 534161.4036;
		if (majftq == 3344176.5991)
			majftq = 8518648.6528;
		myexi = 8269688.2633;
		while (majftq == 7485266.8250)
		{
			majftq = 2538561.5903;
		}
		myexi = 8421733.8377;
	}
	void junk5353524() {
		return;
		float j4i7yb;
		float cadx5l;
		float q9gwzb;
		float thftvn;
		float y5ijmpb;
		float ws8ias;
		float zqadrr;
		float zpol;
		float y339dr;
		float qogdd7;
		y5ijmpb = 6247932.1297;
		for (int u3uzet = 0; u3uzet > 100; u3uzet++)
		{
			q9gwzb = 5537563.4808;
		}
		ws8ias = 3462671.7364;
		for (int uhxzoor = 0; uhxzoor > 100; uhxzoor++)
		{
			j4i7yb = 10507037.2782;
		}
		y339dr = 5374680.8902;
		for (int uxlq43 = 0; uxlq43 > 100; uxlq43++)
		{
			j4i7yb = 865715.5520;
		}
		thftvn = 2491117.4029;
		for (int q9xqm = 0; q9xqm > 100; q9xqm++)
		{
			q9gwzb = 557012.5612;
		}
		ws8ias = 567146.0288;
		if (zpol == 2887760.4461)
			zpol = 3634396.0361;
		q9gwzb = 524808.3888;
		while (y5ijmpb == 6092452.8580)
		{
			y5ijmpb = 91324.7426;
		}
		ws8ias = 1592362.9477;
		y5ijmpb = 9778034.7919;
		y339dr = 10037969.6761;
		if (j4i7yb == 2354252.3801)
			j4i7yb = 2126579.8776;
		y339dr = 8208995.1167;
	}
	void junk4131746() {
		return;
		float ewnksf;
		float n9esz;
		float vgeyr;
		float rrpu89;
		float blq3zv;
		float v0swth;
		float flgdkc;
		float hy8efc;
		float mkity;
		float k77zud;
		for (int bkk6z4 = 0; bkk6z4 > 100; bkk6z4++)
		{
			hy8efc = 9737784.0851;
		}
		mkity = 7100291.6573;
		ewnksf = 6044602.7462;
		if (rrpu89 == 7186377.3983)
			rrpu89 = 8127418.2580;
		n9esz = 5488144.9121;
		while (hy8efc == 7796567.5372)
		{
			hy8efc = 6422785.6299;
		}
		blq3zv = 7827826.9669;
		while (ewnksf == 6224964.7172)
		{
			ewnksf = 2067846.1036;
		}
		k77zud = 3565640.6211;
		vgeyr = 708587.2571;
		if (k77zud == 6507749.9138)
			k77zud = 9320702.7947;
		ewnksf = 1149123.1054;
		for (int oepcpt = 0; oepcpt > 100; oepcpt++)
		{
			ewnksf = 8971812.7144;
		}
		flgdkc = 10044964.5515;
		while (n9esz == 1133534.4573)
		{
			n9esz = 5902086.3095;
		}
		rrpu89 = 3343073.1965;
		while (hy8efc == 9066057.8193)
		{
			hy8efc = 7119091.9471;
		}
		ewnksf = 5684726.4746;
	}
	void junk1781031() {
		return;
		float slaqdg;
		float btg95t;
		float wd072;
		float e0qe2;
		float ug1o7m;
		float e4onml;
		float mrzydo;
		float nnc3sj;
		float kkr1ck;
		float dsv2g4;
		for (int ko1uv = 0; ko1uv > 100; ko1uv++)
		{
			slaqdg = 5695999.0879;
		}
		slaqdg = 2068150.6201;
		for (int s4kyh = 0; s4kyh > 100; s4kyh++)
		{
			btg95t = 130861.6649;
		}
		slaqdg = 7477403.7262;
		btg95t = 3739264.9771;
		while (kkr1ck == 5154585.6139)
		{
			kkr1ck = 5357963.3442;
		}
		mrzydo = 1983630.2560;
		e4onml = 7084144.5697;
		while (mrzydo == 6010557.2807)
		{
			mrzydo = 2941002.4706;
		}
		dsv2g4 = 7755912.6483;
		while (e0qe2 == 3409336.8260)
		{
			e0qe2 = 2909308.2264;
		}
		nnc3sj = 2321737.5684;
		if (dsv2g4 == 1436975.5257)
			dsv2g4 = 4511078.6890;
		mrzydo = 2056547.2021;
		if (kkr1ck == 1430318.2409)
			kkr1ck = 9622160.0570;
		dsv2g4 = 1731350.8106;
		for (int m9b54 = 0; m9b54 > 100; m9b54++)
		{
			btg95t = 7540030.0350;
		}
		wd072 = 1591284.0859;
	}
	void junk4538318() {
		return;
		float a1ftpr;
		float ybj3fi;
		float mve72p;
		float fahnoi;
		float ymief;
		float rvi0ed;
		float to00ihn;
		float a1jy5;
		float ldlp1i;
		float k5ed4p;
		to00ihn = 5279378.5717;
		while (fahnoi == 5034691.4605)
		{
			fahnoi = 9354585.5240;
		}
		ybj3fi = 4627503.5349;
		if (mve72p == 977286.6938)
			mve72p = 5714037.8325;
		a1jy5 = 6445035.7170;
		ybj3fi = 2398686.3651;
		ldlp1i = 8921902.4570;
		k5ed4p = 4142310.9444;
		if (ymief == 1944201.4577)
			ymief = 8221441.6176;
		ldlp1i = 9475615.8797;
		for (int l3egja = 0; l3egja > 100; l3egja++)
		{
			to00ihn = 4593480.3775;
		}
		ldlp1i = 556036.2292;
		for (int kyjt2d = 0; kyjt2d > 100; kyjt2d++)
		{
			k5ed4p = 1285439.9231;
		}
		fahnoi = 228276.9215;
		for (int zngwsk = 0; zngwsk > 100; zngwsk++)
		{
			ldlp1i = 545682.3599;
		}
		ldlp1i = 2304080.2516;
	}
	void junk9120928() {
		return;
		float ux8hqv;
		float b3sfl;
		float j3mz4;
		float x5sddc;
		float cglp9f;
		float yhg92h;
		float i6opbc;
		float fkq2a;
		float cni7pr;
		float qgmnc7;
		i6opbc = 10015379.7038;
		cglp9f = 6354625.8401;
		while (ux8hqv == 4214898.6472)
		{
			ux8hqv = 4909518.3162;
		}
		b3sfl = 8827537.0740;
		while (ux8hqv == 8016495.1412)
		{
			ux8hqv = 9128762.8432;
		}
		cni7pr = 2407322.0814;
		for (int abmr3d = 0; abmr3d > 100; abmr3d++)
		{
			cglp9f = 5851958.3209;
		}
		yhg92h = 405068.7399;
		yhg92h = 3273129.7459;
		while (cni7pr == 6408064.6380)
		{
			cni7pr = 10366750.3692;
		}
		yhg92h = 3828510.0139;
		for (int enx2a = 0; enx2a > 100; enx2a++)
		{
			qgmnc7 = 1429483.1593;
		}
		b3sfl = 7199931.0189;
		while (fkq2a == 2344775.6795)
		{
			fkq2a = 2888111.5996;
		}
		i6opbc = 6801697.4673;
		for (int hf2ytn = 0; hf2ytn > 100; hf2ytn++)
		{
			b3sfl = 1611914.8023;
		}
		ux8hqv = 6223217.5102;
	}
	void junk77692() {
		return;
		float rwt6ng;
		float js0wkk;
		float vwsrl4;
		float pwz3fc;
		float xdie0o;
		float t8adq;
		float qpkrk;
		float ceuk3t;
		float sy9qtyj;
		float vzo0p;
		for (int zxuls6 = 0; zxuls6 > 100; zxuls6++)
		{
			vwsrl4 = 638232.3313;
		}
		pwz3fc = 7748501.1645;
		if (sy9qtyj == 10072757.2435)
			sy9qtyj = 9266641.8345;
		vzo0p = 2882865.5460;
		if (js0wkk == 7739403.8869)
			js0wkk = 74413.2119;
		js0wkk = 8657827.8549;
		if (pwz3fc == 4569180.5622)
			pwz3fc = 6166179.8169;
		sy9qtyj = 4853393.0979;
		for (int vj6znm = 0; vj6znm > 100; vj6znm++)
		{
			vwsrl4 = 3120146.3389;
		}
		rwt6ng = 2628842.2285;
		for (int mx7z0f = 0; mx7z0f > 100; mx7z0f++)
		{
			t8adq = 4448575.7527;
		}
		ceuk3t = 8959930.3618;
		if (rwt6ng == 9584840.1671)
			rwt6ng = 3586784.0932;
		sy9qtyj = 4465039.0514;
		for (int hs5gr8 = 0; hs5gr8 > 100; hs5gr8++)
		{
			sy9qtyj = 9715623.0747;
		}
		qpkrk = 7921855.1751;
		for (int v4vi2e = 0; v4vi2e > 100; v4vi2e++)
		{
			sy9qtyj = 7242254.4480;
		}
		ceuk3t = 5839795.4409;
		for (int ky9m6 = 0; ky9m6 > 100; ky9m6++)
		{
			pwz3fc = 9157762.7535;
		}
		vwsrl4 = 4668388.1245;
	}
	void junk4835432() {
		return;
		float gh1bki;
		float s887gn;
		float n9o984;
		float bggdnp;
		float qk3ibs;
		float fje8k;
		float zhogjo;
		float hh1zi;
		float qcpa7b;
		float veh8a;
		veh8a = 2696612.5743;
		for (int n3ymr = 0; n3ymr > 100; n3ymr++)
		{
			qcpa7b = 584441.4633;
		}
		s887gn = 7615492.6649;
		for (int bw35fg = 0; bw35fg > 100; bw35fg++)
		{
			qcpa7b = 2378666.9346;
		}
		qcpa7b = 816677.4401;
		bggdnp = 8523997.7901;
		hh1zi = 6210217.7370;
		for (int ybasdd = 0; ybasdd > 100; ybasdd++)
		{
			bggdnp = 3161231.1261;
		}
		gh1bki = 7183800.7375;
		for (int cdbnol = 0; cdbnol > 100; cdbnol++)
		{
			veh8a = 59017.4411;
		}
		gh1bki = 3606227.9915;
		if (qcpa7b == 6012928.9814)
			qcpa7b = 6640121.0358;
		zhogjo = 1834481.5847;
		while (qk3ibs == 9664038.5772)
		{
			qk3ibs = 7510676.3909;
		}
		n9o984 = 1504885.5615;
		if (fje8k == 4970055.0663)
			fje8k = 7529144.6854;
		zhogjo = 3388017.3063;
	}
	void junk5737131() {
		return;
		float reddz;
		float pda229;
		float q8k3d;
		float tcz0pu;
		float vlde1f;
		float gy8kli;
		float um1rji;
		float hj08lc;
		float zgr4s;
		float cstbw;
		if (gy8kli == 4413686.0369)
			gy8kli = 5933636.1286;
		vlde1f = 1606050.4259;
		hj08lc = 2587345.5295;
		for (int yo16g = 0; yo16g > 100; yo16g++)
		{
			gy8kli = 9220790.5243;
		}
		um1rji = 5653662.3671;
		while (hj08lc == 9385728.5314)
		{
			hj08lc = 8554921.0123;
		}
		q8k3d = 2475788.8530;
		for (int m9n0f = 0; m9n0f > 100; m9n0f++)
		{
			hj08lc = 8707195.3707;
		}
		gy8kli = 8434508.8823;
		if (um1rji == 8844440.9026)
			um1rji = 5570225.4473;
		q8k3d = 2643056.3706;
		if (hj08lc == 6184241.5976)
			hj08lc = 4881852.7560;
		um1rji = 140112.3434;
		for (int wu7yh = 0; wu7yh > 100; wu7yh++)
		{
			zgr4s = 6480231.5109;
		}
		cstbw = 550928.0232;
		if (tcz0pu == 2269266.6617)
			tcz0pu = 7277323.1987;
		q8k3d = 3357108.1209;
		vlde1f = 897356.2634;
	}
	void doJunk() {
		junk7735438::junk10332142();
		junk7735438::junk383444();
		junk7735438::junk5353524();
		junk7735438::junk4131746();
		junk7735438::junk1781031();
		junk7735438::junk4538318();
		junk7735438::junk9120928();
		junk7735438::junk77692();
		junk7735438::junk4835432();
		junk7735438::junk5737131();
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class fipjexp {
public:
	int ltymalbpne;
	string tpjyfiqlmpd;
	int ziosyckaxvxzbw;
	double afojfckthwozirt;
	fipjexp();
	bool ttvvbxegtfsryykpvlneh();
	bool frmekchugeevnjvzla(string akxpglngejgjcx, double zdkwnk, double ktozjy, bool aicqavvprpvfgno);
	void afeejsknoimwy(string ftjjdehillt, bool ffuzveqvkrk, bool wwujycyauo, bool rzfrhistzana, bool iskuw, bool smdjbxeczmfyx, string qrfrxyjtg);

protected:
	int xotrvurvuhqgia;
	int hlnkos;
	string flmgyhjjflyd;
	bool aedsdlczl;

	double epqscgdmezafeisnvr(int sdisdaccrjufusx, bool scmqe, int ydjym, bool yybytrlrqiyifqm);
	bool xnayoetieelqzihbc(string hlgewwoneetooh, string kmihmzwlug, string rhntdzioxxgstrp, string ebvlrcrxooc, string jbxgmloujlpon, string tybkuedey, double xzigduxbdrxuvo, string uqakourqn, double gbkiopu, int efajmpakmsi);
	double gitthpyrvjeybjkcwdsb(double bkftkycop);
	void plqdpdzizpxrugswsjq(string kejgzmfejfa, int dhsyvolhzuyue);
	double jweavaubjdxmdujj(bool xokjej, int kwwwfdqnmslhjyk, bool elnzrvhvgpslbx);
	int uwqfbdgtqjbcbrrrjabnxdrm(double vvijoaqjkxmukkq, string fiugqzayho, string wnhksujdvcmn, string wzrmjwei, string hgemlhtieecqpcr, string tpmivwtwadsxctu, double isduh, bool bxhausmjmxgs, string nsdtktejxtmoq);
	double ajoetrfmnzahubkblqy(double ywzwtrdilz, int eynvvxeupptgysp, bool nmboqvrtdmusffj, string msvadnyzlltzp, double crzqr, bool shxwpz);
	int ojhguwrsctahbfaftjgcc(int etnim, int krokewuxvghb, bool mrbzbimp, string mukem, double isuetkulcguri, double yxhglbjfd);
	double djkgnipzxncuybkrfglnuaiu(int ctsxvuggomqrvdm, string ikxvrknr, string ryxopzd, int wxwguzfsf, int fqdhiclnztcu, double jsqssnbkqxtzpx, double ikwgfkex, int pktucm, double oqncxqpbqxddoan);

private:
	int odyoenjre;
	int aluogi;
	bool mtelu;
	double lbgyisefozjln;

	string jmnxiekqhh(string ftrdpzguwzvrdd, bool dkgdd, string noeolwwf, bool wgkevhmqgwkmeu, string tnbotyary, bool ouchhvwqhm, double jxftkontvfizuse, bool qfxdxhote, bool fwbscxtk, string ttxpguioi);
	void mfffqymzcwdorsyeuhmbiyqqg(string mfqgeolrzvqlng, bool qummzxretdpudsz, string kxndjpnyyahh, bool yfuuxuvsqmkaz, string vdshhpiuiode, bool lbjmw, double vjlfqqgud, double xzgaclwnqwqcps, string cnehceb);
	void sbybwyzfqyvnlwewelhl(string srstbijysuyajqh, double syvvjr, string plpzzlftcljv, int punnrlmmy, bool vvqravdczj, bool ztxxdpuq, double rvrfaytfkhbcml);
	double yjifqywgasosscid(double xbdkc, bool zpppxsyixvtil, string wtvrtg, string dcmms, double zxirjvppnhf, double csjugkhnczvmv, string qkydbhhoc, double nsdbnbbjrsjt, string zylefy, bool ehgpsrmnksf);
	double uvxsrgnlda(int lvcij, double wjrfw, double jtlrryxxerjep, double rihsqp, bool bnnyqqcgrgeydcv, double jzacjzhlsqn);
	double ivudixzyoeoxbbanhomjhpkhx(string dzfgyavzs, int iwquy, int syiinistm, string nrvqowtst, double agghrzytmawoddr);
	string gylmsyaftrmzcaewkwivymqer(bool npjwhqmvggse, double fcdyvq, bool abhlhpfjdhzy, double hqsqxf, double dfngsetjwuro, string vxhxia);
	string sdbywctgljru(int jddslrnxerwvya, int qwwcotx, double cylqjxmochghtzt, int dukiocxxbkthz, bool fjobygcdaaha, int hraampbvuz, int pdftn, int awgmoi, string ifhihwbgv, string imddptd);
	double bcydlkxgxygzgf(double rxibt, string czfvbyawghdyfv, double lhnkjgeen, bool wzmqypbywi, string ojkcq, int vozjla);
	int sidxnubhuemjnjmbpohabiq(bool msoofnuqzezp, int ubyareft, string oosiha, bool olmejjvqaryobys, bool zkihtz, bool woimeysongcafvq, string szahpx, string izjfrwwi);

};


string fipjexp::jmnxiekqhh(string ftrdpzguwzvrdd, bool dkgdd, string noeolwwf, bool wgkevhmqgwkmeu, string tnbotyary, bool ouchhvwqhm, double jxftkontvfizuse, bool qfxdxhote, bool fwbscxtk, string ttxpguioi) {
	return string("lxtilsdxrwzypu");
}

void fipjexp::mfffqymzcwdorsyeuhmbiyqqg(string mfqgeolrzvqlng, bool qummzxretdpudsz, string kxndjpnyyahh, bool yfuuxuvsqmkaz, string vdshhpiuiode, bool lbjmw, double vjlfqqgud, double xzgaclwnqwqcps, string cnehceb) {
	string qwkowlyveyhooa = "mkuwystzqbntwingyvavwmvzzwwnlbkkvzjsokatwwnctkxdbdklzlpvsptjzf";
	int punntptaydbwkc = 3119;
	bool ctkpv = true;
	int zmsuknqlm = 5519;
	string fusutnsjp = "hocuunxvvssiywkikyvnkeyjhdgynprx";
	bool uyfpkos = true;
	int fpthrqfcdcao = 4956;
	if (string("hocuunxvvssiywkikyvnkeyjhdgynprx") != string("hocuunxvvssiywkikyvnkeyjhdgynprx")) {
		int psyos;
		for (psyos = 37; psyos > 0; psyos--) {
			continue;
		}
	}
	if (true == true) {
		int sgdktg;
		for (sgdktg = 93; sgdktg > 0; sgdktg--) {
			continue;
		}
	}
	if (true != true) {
		int md;
		for (md = 39; md > 0; md--) {
			continue;
		}
	}
	if (true == true) {
		int gpmn;
		for (gpmn = 80; gpmn > 0; gpmn--) {
			continue;
		}
	}

}

void fipjexp::sbybwyzfqyvnlwewelhl(string srstbijysuyajqh, double syvvjr, string plpzzlftcljv, int punnrlmmy, bool vvqravdczj, bool ztxxdpuq, double rvrfaytfkhbcml) {
	string vxibwa = "kqsvhvzvaqwjlmobzzdzviwwlzmttlfgeybfcndqiiglmrszsulktaepsuvg";
	int mvkglf = 717;
	if (string("kqsvhvzvaqwjlmobzzdzviwwlzmttlfgeybfcndqiiglmrszsulktaepsuvg") == string("kqsvhvzvaqwjlmobzzdzviwwlzmttlfgeybfcndqiiglmrszsulktaepsuvg")) {
		int qygyk;
		for (qygyk = 81; qygyk > 0; qygyk--) {
			continue;
		}
	}
	if (717 != 717) {
		int ya;
		for (ya = 47; ya > 0; ya--) {
			continue;
		}
	}
	if (string("kqsvhvzvaqwjlmobzzdzviwwlzmttlfgeybfcndqiiglmrszsulktaepsuvg") == string("kqsvhvzvaqwjlmobzzdzviwwlzmttlfgeybfcndqiiglmrszsulktaepsuvg")) {
		int uwj;
		for (uwj = 51; uwj > 0; uwj--) {
			continue;
		}
	}
	if (string("kqsvhvzvaqwjlmobzzdzviwwlzmttlfgeybfcndqiiglmrszsulktaepsuvg") != string("kqsvhvzvaqwjlmobzzdzviwwlzmttlfgeybfcndqiiglmrszsulktaepsuvg")) {
		int aplu;
		for (aplu = 57; aplu > 0; aplu--) {
			continue;
		}
	}

}

double fipjexp::yjifqywgasosscid(double xbdkc, bool zpppxsyixvtil, string wtvrtg, string dcmms, double zxirjvppnhf, double csjugkhnczvmv, string qkydbhhoc, double nsdbnbbjrsjt, string zylefy, bool ehgpsrmnksf) {
	bool gxaxzqyj = false;
	if (false == false) {
		int xtvozatyk;
		for (xtvozatyk = 75; xtvozatyk > 0; xtvozatyk--) {
			continue;
		}
	}
	if (false != false) {
		int rvqrnse;
		for (rvqrnse = 1; rvqrnse > 0; rvqrnse--) {
			continue;
		}
	}
	return 91009;
}

double fipjexp::uvxsrgnlda(int lvcij, double wjrfw, double jtlrryxxerjep, double rihsqp, bool bnnyqqcgrgeydcv, double jzacjzhlsqn) {
	bool ibhpzqsbstf = false;
	if (false != false) {
		int kpwdaxtbha;
		for (kpwdaxtbha = 40; kpwdaxtbha > 0; kpwdaxtbha--) {
			continue;
		}
	}
	if (false != false) {
		int mflp;
		for (mflp = 77; mflp > 0; mflp--) {
			continue;
		}
	}
	if (false == false) {
		int yxqvly;
		for (yxqvly = 31; yxqvly > 0; yxqvly--) {
			continue;
		}
	}
	return 72949;
}

double fipjexp::ivudixzyoeoxbbanhomjhpkhx(string dzfgyavzs, int iwquy, int syiinistm, string nrvqowtst, double agghrzytmawoddr) {
	bool wkcruzgsdirsi = true;
	if (true == true) {
		int bcqqa;
		for (bcqqa = 67; bcqqa > 0; bcqqa--) {
			continue;
		}
	}
	if (true == true) {
		int tsxobzncc;
		for (tsxobzncc = 2; tsxobzncc > 0; tsxobzncc--) {
			continue;
		}
	}
	if (true == true) {
		int bqjj;
		for (bqjj = 91; bqjj > 0; bqjj--) {
			continue;
		}
	}
	if (true == true) {
		int shhltttuhb;
		for (shhltttuhb = 22; shhltttuhb > 0; shhltttuhb--) {
			continue;
		}
	}
	if (true != true) {
		int huslhlq;
		for (huslhlq = 24; huslhlq > 0; huslhlq--) {
			continue;
		}
	}
	return 77941;
}

string fipjexp::gylmsyaftrmzcaewkwivymqer(bool npjwhqmvggse, double fcdyvq, bool abhlhpfjdhzy, double hqsqxf, double dfngsetjwuro, string vxhxia) {
	bool cbqahhcb = false;
	bool kwwzk = false;
	string lvpdwafzppk = "tbmsgapbhizckfaypteirfwlmrsukivzcw";
	double eliwvddwoep = 1080;
	if (false == false) {
		int kvurdl;
		for (kvurdl = 41; kvurdl > 0; kvurdl--) {
			continue;
		}
	}
	return string("omthkmougckygsngu");
}

string fipjexp::sdbywctgljru(int jddslrnxerwvya, int qwwcotx, double cylqjxmochghtzt, int dukiocxxbkthz, bool fjobygcdaaha, int hraampbvuz, int pdftn, int awgmoi, string ifhihwbgv, string imddptd) {
	bool rtwiqvhuotpc = true;
	double exvdfzpbkkhwtea = 29796;
	bool afgyfpegpbxnwu = true;
	int blykyfjj = 2721;
	string bvybxvbplfdeb = "adxblkduyukhzcsffpblhmucgukrlgahrsgeevcqfvzqwjzzdlvlzewetnbkq";
	string ahwftfoaz = "zpekkdwemqmefyqdhotmajhyrykuxcyfeghtatcobskugxvwifgujccnxlsgcobsriicpkaunxmuzept";
	string xzgvf = "mdwmpondhtwycxjnkpc";
	if (true != true) {
		int dbufnlcfq;
		for (dbufnlcfq = 18; dbufnlcfq > 0; dbufnlcfq--) {
			continue;
		}
	}
	if (string("adxblkduyukhzcsffpblhmucgukrlgahrsgeevcqfvzqwjzzdlvlzewetnbkq") == string("adxblkduyukhzcsffpblhmucgukrlgahrsgeevcqfvzqwjzzdlvlzewetnbkq")) {
		int cafeaw;
		for (cafeaw = 53; cafeaw > 0; cafeaw--) {
			continue;
		}
	}
	if (29796 == 29796) {
		int hmfzg;
		for (hmfzg = 18; hmfzg > 0; hmfzg--) {
			continue;
		}
	}
	if (2721 != 2721) {
		int ewz;
		for (ewz = 18; ewz > 0; ewz--) {
			continue;
		}
	}
	return string("tfactvbyowt");
}

double fipjexp::bcydlkxgxygzgf(double rxibt, string czfvbyawghdyfv, double lhnkjgeen, bool wzmqypbywi, string ojkcq, int vozjla) {
	int yhxwxwy = 1344;
	if (1344 == 1344) {
		int vozjlgddre;
		for (vozjlgddre = 87; vozjlgddre > 0; vozjlgddre--) {
			continue;
		}
	}
	if (1344 != 1344) {
		int nnhhuyd;
		for (nnhhuyd = 88; nnhhuyd > 0; nnhhuyd--) {
			continue;
		}
	}
	if (1344 != 1344) {
		int grezmin;
		for (grezmin = 16; grezmin > 0; grezmin--) {
			continue;
		}
	}
	if (1344 == 1344) {
		int kf;
		for (kf = 3; kf > 0; kf--) {
			continue;
		}
	}
	if (1344 != 1344) {
		int jpaip;
		for (jpaip = 64; jpaip > 0; jpaip--) {
			continue;
		}
	}
	return 69128;
}

int fipjexp::sidxnubhuemjnjmbpohabiq(bool msoofnuqzezp, int ubyareft, string oosiha, bool olmejjvqaryobys, bool zkihtz, bool woimeysongcafvq, string szahpx, string izjfrwwi) {
	string rnyxmqgopzfi = "esgsuwzillhmmn";
	string lmqts = "gujeeuirzdxzwbyzrzjgwqltryenkbpspifnlqiircoihlgmvvqanijwetmfgxklyfljcpdbuoh";
	string xtylkbcr = "sgltjcjsyccbdglplcvqepnkxzedohwkrrcslotmuuemybbiou";
	if (string("sgltjcjsyccbdglplcvqepnkxzedohwkrrcslotmuuemybbiou") == string("sgltjcjsyccbdglplcvqepnkxzedohwkrrcslotmuuemybbiou")) {
		int ophgrxfzg;
		for (ophgrxfzg = 84; ophgrxfzg > 0; ophgrxfzg--) {
			continue;
		}
	}
	if (string("sgltjcjsyccbdglplcvqepnkxzedohwkrrcslotmuuemybbiou") == string("sgltjcjsyccbdglplcvqepnkxzedohwkrrcslotmuuemybbiou")) {
		int crdexvd;
		for (crdexvd = 0; crdexvd > 0; crdexvd--) {
			continue;
		}
	}
	if (string("esgsuwzillhmmn") == string("esgsuwzillhmmn")) {
		int srcf;
		for (srcf = 84; srcf > 0; srcf--) {
			continue;
		}
	}
	if (string("gujeeuirzdxzwbyzrzjgwqltryenkbpspifnlqiircoihlgmvvqanijwetmfgxklyfljcpdbuoh") == string("gujeeuirzdxzwbyzrzjgwqltryenkbpspifnlqiircoihlgmvvqanijwetmfgxklyfljcpdbuoh")) {
		int qkfm;
		for (qkfm = 0; qkfm > 0; qkfm--) {
			continue;
		}
	}
	return 79484;
}

double fipjexp::epqscgdmezafeisnvr(int sdisdaccrjufusx, bool scmqe, int ydjym, bool yybytrlrqiyifqm) {
	double rwewniyanbxbph = 20810;
	string tqnvzbhkdkmm = "kufwobeya";
	string jvsrfxyq = "ejbfmdinwkvdr";
	string gjmqnbvonbhqm = "mblgblsvczfgpmuouaobrpwschzyfuoawmxcbocsvruumhucxcbiwdwbnzvpvvzfkjyomdctdgmfawyypxpqiuxezwo";
	int lhetc = 3551;
	int udvkxxiu = 1471;
	string tdkht = "mqispmctxmghspqhhwybyusqtzkirdpprrgeyklunmaautzqhsqskatakhptyiwcevychnnageik";
	bool dhvvue = false;
	if (string("kufwobeya") == string("kufwobeya")) {
		int qtorxuuled;
		for (qtorxuuled = 81; qtorxuuled > 0; qtorxuuled--) {
			continue;
		}
	}
	if (3551 == 3551) {
		int rjynize;
		for (rjynize = 39; rjynize > 0; rjynize--) {
			continue;
		}
	}
	if (string("kufwobeya") == string("kufwobeya")) {
		int ztkyalc;
		for (ztkyalc = 69; ztkyalc > 0; ztkyalc--) {
			continue;
		}
	}
	if (3551 != 3551) {
		int yhbi;
		for (yhbi = 99; yhbi > 0; yhbi--) {
			continue;
		}
	}
	if (3551 != 3551) {
		int ppk;
		for (ppk = 65; ppk > 0; ppk--) {
			continue;
		}
	}
	return 80697;
}

bool fipjexp::xnayoetieelqzihbc(string hlgewwoneetooh, string kmihmzwlug, string rhntdzioxxgstrp, string ebvlrcrxooc, string jbxgmloujlpon, string tybkuedey, double xzigduxbdrxuvo, string uqakourqn, double gbkiopu, int efajmpakmsi) {
	int imxuxssqxsy = 3125;
	int jkyyppqetoocsa = 3334;
	double mdnks = 42951;
	string doebfk = "mfmdlsxmrxkfxegfyylhwnqyrhdynvpwccuqmzlajjgretaymynkitgglwudbhoglanzjlnmzgplqrmskpnoplmhdhkcv";
	int gespnixvcmkefgf = 2813;
	string dxrpcm = "hgvqtgyypmxdqrrehohbreewdqwbhcfsxnjuikbyxqlvaoejtsqw";
	bool wgoxk = false;
	int hirmzac = 4299;
	bool qrhkiiogwgop = true;
	if (false != false) {
		int smuxmrfjg;
		for (smuxmrfjg = 45; smuxmrfjg > 0; smuxmrfjg--) {
			continue;
		}
	}
	if (42951 != 42951) {
		int qlfsfkmaio;
		for (qlfsfkmaio = 86; qlfsfkmaio > 0; qlfsfkmaio--) {
			continue;
		}
	}
	return false;
}

double fipjexp::gitthpyrvjeybjkcwdsb(double bkftkycop) {
	string ipjwwvzmrraenot = "antbxynmdqjczxuyifjvwnqhjlbygevxsfdkvcpkdjdednlujjnwwnyqgy";
	bool svmmpvggjppqdzr = true;
	int qglzp = 6406;
	bool gtchq = false;
	int kicviv = 4295;
	return 15124;
}

void fipjexp::plqdpdzizpxrugswsjq(string kejgzmfejfa, int dhsyvolhzuyue) {
	string ftfdpmynhd = "ydcd";
	double fvarqpzk = 22403;
	bool dyaxuckkwmq = false;
	string ajkhkxklb = "yqigslqqfkcmfncsqsowved";
	bool ktbnlaztqgyp = false;
	double ptblweprpytvkze = 53090;
	string csxaayaxqsoidms = "dzudqoxeczfymuaii";
	if (53090 == 53090) {
		int vhiejp;
		for (vhiejp = 89; vhiejp > 0; vhiejp--) {
			continue;
		}
	}
	if (string("yqigslqqfkcmfncsqsowved") != string("yqigslqqfkcmfncsqsowved")) {
		int pbovtozvfe;
		for (pbovtozvfe = 6; pbovtozvfe > 0; pbovtozvfe--) {
			continue;
		}
	}
	if (string("dzudqoxeczfymuaii") != string("dzudqoxeczfymuaii")) {
		int osfbwrtxul;
		for (osfbwrtxul = 24; osfbwrtxul > 0; osfbwrtxul--) {
			continue;
		}
	}
	if (53090 != 53090) {
		int uedbw;
		for (uedbw = 48; uedbw > 0; uedbw--) {
			continue;
		}
	}
	if (false != false) {
		int ojy;
		for (ojy = 5; ojy > 0; ojy--) {
			continue;
		}
	}

}

double fipjexp::jweavaubjdxmdujj(bool xokjej, int kwwwfdqnmslhjyk, bool elnzrvhvgpslbx) {
	bool szzyhh = false;
	bool gxonjatrwo = true;
	int ojhuy = 413;
	double mmeydrexxhzm = 50617;
	double fsoykabodhpppo = 26194;
	bool ljzymroujodggoj = false;
	int hiknhrdyckr = 271;
	string uksopodi = "glzrpbhqhayydwwzxgcgqkabazryfonbnmri";
	string tzqvqfamxeng = "nvjlptibvuiuusaqdpqhzftnsbzmkcrfpcktdqsnkxzwoylhppovqrdeihebufeqmomfiictaa";
	if (string("nvjlptibvuiuusaqdpqhzftnsbzmkcrfpcktdqsnkxzwoylhppovqrdeihebufeqmomfiictaa") != string("nvjlptibvuiuusaqdpqhzftnsbzmkcrfpcktdqsnkxzwoylhppovqrdeihebufeqmomfiictaa")) {
		int fvwjlegh;
		for (fvwjlegh = 51; fvwjlegh > 0; fvwjlegh--) {
			continue;
		}
	}
	return 48860;
}

int fipjexp::uwqfbdgtqjbcbrrrjabnxdrm(double vvijoaqjkxmukkq, string fiugqzayho, string wnhksujdvcmn, string wzrmjwei, string hgemlhtieecqpcr, string tpmivwtwadsxctu, double isduh, bool bxhausmjmxgs, string nsdtktejxtmoq) {
	string upodosaskxvjcp = "iyeiqjhxtbqgguywsqqjjmlyiovedlxozcgwveuzalytktxrdrynmwbwpzlzmlvhfbtuudyfogueljr";
	bool ufrxqhxrtsgq = false;
	string mmceesmwvybjowu = "igdbqisfbtvrzctcblpactwizqrteroqcwelkhdkdm";
	double aitckbujkgekxn = 3051;
	string vyfyqzigknh = "uahgdzpqphnkwdoggxbyomkwxgfsrineqbxlnmvzjnhbyvdimyxeqzifpqbburndtvilhsevufmgvufusoipstqd";
	bool yosovkhnqo = true;
	if (string("igdbqisfbtvrzctcblpactwizqrteroqcwelkhdkdm") != string("igdbqisfbtvrzctcblpactwizqrteroqcwelkhdkdm")) {
		int pijjm;
		for (pijjm = 95; pijjm > 0; pijjm--) {
			continue;
		}
	}
	if (true == true) {
		int tvxvwjpnyw;
		for (tvxvwjpnyw = 15; tvxvwjpnyw > 0; tvxvwjpnyw--) {
			continue;
		}
	}
	if (3051 == 3051) {
		int eumszholg;
		for (eumszholg = 76; eumszholg > 0; eumszholg--) {
			continue;
		}
	}
	return 43010;
}

double fipjexp::ajoetrfmnzahubkblqy(double ywzwtrdilz, int eynvvxeupptgysp, bool nmboqvrtdmusffj, string msvadnyzlltzp, double crzqr, bool shxwpz) {
	string ojynfbl = "axtwrjhnlyujihjsbcqtsvfakpqhcznzscrzxlmknhyjshqhbkuvosxlgboexmmitidqnpxxoohohnccrtqzz";
	double xehotpmtaatdmk = 60;
	int qjxqspxkwzl = 455;
	bool uccgkqde = true;
	double zizcjtiomtzhok = 30063;
	if (60 != 60) {
		int ibqqzp;
		for (ibqqzp = 53; ibqqzp > 0; ibqqzp--) {
			continue;
		}
	}
	if (30063 != 30063) {
		int lrhezo;
		for (lrhezo = 20; lrhezo > 0; lrhezo--) {
			continue;
		}
	}
	return 84891;
}

int fipjexp::ojhguwrsctahbfaftjgcc(int etnim, int krokewuxvghb, bool mrbzbimp, string mukem, double isuetkulcguri, double yxhglbjfd) {
	return 13375;
}

double fipjexp::djkgnipzxncuybkrfglnuaiu(int ctsxvuggomqrvdm, string ikxvrknr, string ryxopzd, int wxwguzfsf, int fqdhiclnztcu, double jsqssnbkqxtzpx, double ikwgfkex, int pktucm, double oqncxqpbqxddoan) {
	bool liwhqmkujrojd = true;
	bool xeollohnyprzts = true;
	string jmfpw = "rolublefkqanomgcpjrjhcibckjekyrgyuitrylrocivtkzfhpzbkzwvnonukbolijcqxuur";
	int ldryodlpqiqlegh = 68;
	int ijvpxhivexjxn = 6485;
	if (string("rolublefkqanomgcpjrjhcibckjekyrgyuitrylrocivtkzfhpzbkzwvnonukbolijcqxuur") == string("rolublefkqanomgcpjrjhcibckjekyrgyuitrylrocivtkzfhpzbkzwvnonukbolijcqxuur")) {
		int raqg;
		for (raqg = 64; raqg > 0; raqg--) {
			continue;
		}
	}
	if (68 != 68) {
		int kkrfmohga;
		for (kkrfmohga = 2; kkrfmohga > 0; kkrfmohga--) {
			continue;
		}
	}
	return 54889;
}

bool fipjexp::ttvvbxegtfsryykpvlneh() {
	double gcxoa = 12755;
	string xyqxuruhol = "akahesntjadinnbgnysnpqk";
	int mfvaddnxi = 6717;
	double dofhsdivff = 14510;
	if (6717 != 6717) {
		int bw;
		for (bw = 87; bw > 0; bw--) {
			continue;
		}
	}
	if (6717 != 6717) {
		int nvak;
		for (nvak = 66; nvak > 0; nvak--) {
			continue;
		}
	}
	if (string("akahesntjadinnbgnysnpqk") == string("akahesntjadinnbgnysnpqk")) {
		int crpyksmad;
		for (crpyksmad = 54; crpyksmad > 0; crpyksmad--) {
			continue;
		}
	}
	return false;
}

bool fipjexp::frmekchugeevnjvzla(string akxpglngejgjcx, double zdkwnk, double ktozjy, bool aicqavvprpvfgno) {
	string pmklmjxbzsn = "adukgdxmssfqwnqezfnssceflzttbjulcoiyuioxncuiazgbwgcsfxaqftmtgxqmgmpyripvfrxbfijeiqzcxciplnje";
	bool vvxkkludopjlc = false;
	int nnklk = 7667;
	if (false != false) {
		int yovh;
		for (yovh = 33; yovh > 0; yovh--) {
			continue;
		}
	}
	return true;
}

void fipjexp::afeejsknoimwy(string ftjjdehillt, bool ffuzveqvkrk, bool wwujycyauo, bool rzfrhistzana, bool iskuw, bool smdjbxeczmfyx, string qrfrxyjtg) {
	int dhisrfibk = 1501;
	double uipuhl = 7365;
	if (7365 == 7365) {
		int hssvtppsu;
		for (hssvtppsu = 84; hssvtppsu > 0; hssvtppsu--) {
			continue;
		}
	}
	if (7365 != 7365) {
		int ilentolj;
		for (ilentolj = 89; ilentolj > 0; ilentolj--) {
			continue;
		}
	}
	if (7365 == 7365) {
		int ubjyixk;
		for (ubjyixk = 16; ubjyixk > 0; ubjyixk--) {
			continue;
		}
	}
	if (1501 != 1501) {
		int lgxys;
		for (lgxys = 72; lgxys > 0; lgxys--) {
			continue;
		}
	}
	if (7365 == 7365) {
		int gwckms;
		for (gwckms = 33; gwckms > 0; gwckms--) {
			continue;
		}
	}

}

fipjexp::fipjexp() {
	this->ttvvbxegtfsryykpvlneh();
	this->frmekchugeevnjvzla(string("mjmknlhtdehptdkntqcsupxahgfbndqwzcjvbzktjaaaue"), 26817, 4648, true);
	this->afeejsknoimwy(string("pfxvbhafcyzapuqcpcnwdifgwfebculcuvbztsvdo"), false, true, false, true, false, string("khvdphgdlydlnkifmykmludmsikonixkzdfjfcdgetclnuhckmugwkjybyzqctze"));
	this->epqscgdmezafeisnvr(558, false, 1909, false);
	this->xnayoetieelqzihbc(string("iiwmkrrjjtebeqgzwkodmha"), string("kmmmywgcftofxypifjpmlvroeuavkqan"), string("krrmibqgdbovvtdrkebixbpghqqraazxdlhkpuvsuxpipyybwqpikwjjmhjccjr"), string("pcpjdcdzrscnhbticxhtpwazdefzoitsrjazbuxyponnykmpbhxvdedmeaokmdxuxvipfnx"), string("mxokostpooykjcmffzzdpgmbyljvikglxsdjoesyswgkj"), string("ngkxhtoebhtzaieykmgtkymzvwiesaypgpkoqokswduanknidciinpilpcksylxiyaccfpvcwjhvpi"), 21870, string("gjaqltkzurusotizbhccrrybwwtcownguxujtubtaognhxmcvtormrfe"), 34076, 3394);
	this->gitthpyrvjeybjkcwdsb(1856);
	this->plqdpdzizpxrugswsjq(string("gaonmmqsqtblrbtxbpoykmsxvkhatvyucdcdbpwrbhucftomhocpuambuo"), 3769);
	this->jweavaubjdxmdujj(true, 2512, false);
	this->uwqfbdgtqjbcbrrrjabnxdrm(11416, string("qityozqucqnkkideogc"), string("bwsmwauwfkikvfjrqrwcmeljgwaabrspvbjdmuycjijhczwgerihnxtkjfabnuhozzeicyezpzxpjnbdfyqhuaskulwtemetm"), string("bgdfxnnmtxtautjpeooipzxafhstoxefbtjyzxacocqtlwodpugyonzpicqlvnqobzdxbnjwjcuyxaotzywlfslmeolt"), string("ipule"), string("kqqtseqxbljrysqfs"), 2039, true, string("awwpswzkkokbexmbnfxtuhsiwgvjbuvziioveozxvsflxtpcfalfjaaobbdrsxlv"));
	this->ajoetrfmnzahubkblqy(21592, 6673, false, string("qotspdhoibwxqnsjbymxtzcvhaatquckgip"), 29664, false);
	this->ojhguwrsctahbfaftjgcc(1669, 1891, true, string("kugtbnmwjvnktnhyvhrctpfvzmtc"), 19369, 59935);
	this->djkgnipzxncuybkrfglnuaiu(2677, string("aqlbfkfobjmfqnwypjmvjiaengazfdqdfpwpxydxhqzbzpkoozofoorvnwzwdqsw"), string("ukwcictiscekmssikuvhnnkkixkitvrkfjdejcmfhcyimsznzzyvwinpfrugfcgcckqhgdcyakrnexvi"), 3415, 3523, 7304, 6905, 1524, 22007);
	this->jmnxiekqhh(string("dnpsdbkaczncjccbkwnyxreilhqocqkf"), true, string("spmmnfoctqvur"), true, string("gbuineedluloctdyrdqxcrgayjbecbykxnypkbinjnehhdvijo"), true, 17486, true, true, string("slsmurahtyp"));
	this->mfffqymzcwdorsyeuhmbiyqqg(string("wdpsssafjundmkkbcdiwfcwxragauvpggagzdxaslzdfxigtohzsejxapo"), false, string("maroxwbcjugswyddiaybaomyztvcuuafrlswefritcvahrobubvhrhpratbwfw"), true, string("phvhwelarprtvaqrobd"), false, 21606, 50944, string("ockptoevybzdelmllxzpqwfhyvn"));
	this->sbybwyzfqyvnlwewelhl(string("fqtmnjsrgrfwfccdrukwlqxdtzlfmgsaftdkfymjflgpfiimggfbcednqvglruieaglbwikwepqoptjypdtzgvgfzcaxxebf"), 74817, string("vzuegghzkqtpzpd"), 2830, false, false, 40895);
	this->yjifqywgasosscid(32421, true, string("orgrdbeaeobpshucnkvkesigkiajhmprqztqzhybkciqqvwrnvvmfxmpxvozqgsps"), string("ibfnrrpeclvxfr"), 48147, 71811, string("q"), 66194, string("jdmlxnngwagwlycjirrxglwjmzrxjopggqlancfhgozfkreguivqzgqhrvdaawedaccamgnrgrgngfbnifd"), true);
	this->uvxsrgnlda(7376, 23244, 4895, 8432, true, 8661);
	this->ivudixzyoeoxbbanhomjhpkhx(string("nbvdfginlguchsnvkhuhqwwqflyjwisnqbhintejckwcvfb"), 1355, 627, string("hmbi"), 20077);
	this->gylmsyaftrmzcaewkwivymqer(true, 33124, false, 16449, 5241, string("vahxvcpeehnvofpkeswpwghqcqfejwsyzqmqvitbnikrqarpehjbzaommyzwrfrarnedwnfteun"));
	this->sdbywctgljru(2338, 23, 10540, 323, true, 5405, 3524, 3873, string("fhoxntanjoawlfdnnlnjdr"), string("jfgbywwmfpneuoyqxs"));
	this->bcydlkxgxygzgf(76548, string("oirrsrjhhnijtqnbrfwspaqlcnztrvjgndxhkghh"), 9175, true, string("hpfcedmcdyrnpgrypromkakimfatphyctlgqtaprlce"), 635);
	this->sidxnubhuemjnjmbpohabiq(true, 7189, string("cstgcggimvivgcyjewlpif"), true, false, true, string("klnfovrnxnwkzkjaqzmpktpduvfazekbveuwcqhugxcktkfjixrtqzrfwthwmoalaijcjixpzbvrnucpxxoabagyfemjcjoonn"), string("uouvsrwnhzpejqfmyrdppwuuhwdwlconmfaajjpnjvfnhxgs"));
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class zcbkbgm {
public:
	double iivtnxebrkpiut;
	bool adkgsvfchcnvkl;
	double hgedpste;
	double volfzwrdmbce;
	zcbkbgm();
	bool ppyjlipaagyaiemdxkvazvp(double loluz, double pinfcvxsl, int tfdhz);
	string ankpqipfij(int zbsgdrmgdwkmdgh);
	void rlqarjfdvkjyrpfufevympue(int evxotjgvpbnk, double iwjuydjejvpem);
	int dslwewthvg(double unqwmsilyoub, int vyahzx, string sauwrxrlbje);
	bool kvftfcqqnzkm(bool zhdirxwuwdwmk, int koiixk, int xegejrjkpve, int lwbgoyzwa, bool vqjyycfmjotcrjy, string ubppthvkv, int qwarihrvx);

protected:
	int liimtpquolc;

	void emqmhmymwotq(double qrkfrkfiquolh);
	bool btgzjvoscnmuzynaawygxjkpg(bool cvfowgvznetoen, double clbjyvxhnaqwbfq, string dnmhnmqnzuc, string rrsfzabcxhc, double fyctdveib, int twxxtujbakxzy);
	string ppyquzcytwafndcgbkcibqsis();
	int buznvfixafxriohynetmdhyru(string bdtvsfbrne);

private:
	bool mkzkkr;
	string wodpfhcwtaenjjf;
	string azmlkkopiayinol;
	bool wdslfhujjc;
	double fovpqmggn;

	double fiihmomsrwvb();
	double jzwxyjdfvvxs(bool ofhvntisct, double rxflebu);

};


double zcbkbgm::fiihmomsrwvb() {
	string tfyyci = "mhflzogjbcwdkpatugjqublasjfiphtrubylhzoitoarhpmscyeumrakrunqfcbysmeihjmwkcpq";
	bool wkoyqz = true;
	double mrlrexvflovixw = 6762;
	double ixqnpzwgvgf = 18286;
	if (true == true) {
		int vtn;
		for (vtn = 24; vtn > 0; vtn--) {
			continue;
		}
	}
	if (18286 == 18286) {
		int pvrem;
		for (pvrem = 100; pvrem > 0; pvrem--) {
			continue;
		}
	}
	if (6762 != 6762) {
		int njxmit;
		for (njxmit = 78; njxmit > 0; njxmit--) {
			continue;
		}
	}
	return 85110;
}

double zcbkbgm::jzwxyjdfvvxs(bool ofhvntisct, double rxflebu) {
	double sgzslarjjiqwso = 51743;
	string uxrnprj = "lmvu";
	if (string("lmvu") == string("lmvu")) {
		int gpduv;
		for (gpduv = 88; gpduv > 0; gpduv--) {
			continue;
		}
	}
	if (51743 == 51743) {
		int xlbmc;
		for (xlbmc = 100; xlbmc > 0; xlbmc--) {
			continue;
		}
	}
	return 26137;
}

void zcbkbgm::emqmhmymwotq(double qrkfrkfiquolh) {
	string euxqujtqcvanb = "ojmzsdedctmovohktkas";
	double dsyyyqdxeptux = 61133;
	int kzasdb = 1702;
	int pyxvuild = 1038;
	double gxwsqpjkrcdrkal = 34260;
	bool uofta = false;
	double ueunxvqxsisrt = 14588;
	double sfrltrvsqsvkrf = 10333;
	if (61133 == 61133) {
		int khcsvjs;
		for (khcsvjs = 91; khcsvjs > 0; khcsvjs--) {
			continue;
		}
	}
	if (61133 == 61133) {
		int kf;
		for (kf = 14; kf > 0; kf--) {
			continue;
		}
	}
	if (1038 != 1038) {
		int eqlhj;
		for (eqlhj = 2; eqlhj > 0; eqlhj--) {
			continue;
		}
	}
	if (1702 == 1702) {
		int xacczalf;
		for (xacczalf = 84; xacczalf > 0; xacczalf--) {
			continue;
		}
	}
	if (string("ojmzsdedctmovohktkas") != string("ojmzsdedctmovohktkas")) {
		int reizazcn;
		for (reizazcn = 54; reizazcn > 0; reizazcn--) {
			continue;
		}
	}

}

bool zcbkbgm::btgzjvoscnmuzynaawygxjkpg(bool cvfowgvznetoen, double clbjyvxhnaqwbfq, string dnmhnmqnzuc, string rrsfzabcxhc, double fyctdveib, int twxxtujbakxzy) {
	bool shwcpxsb = false;
	string uokhmhafgp = "pbplnftzsalsfxnwwyccfdaepnkraccmrllxtcsakpkgzyirzqlnmox";
	string ulkhqckedkcqe = "cnazyslrmclofkjgizfpoek";
	string nodcdh = "drzdpgggkqhgeiwxwpuvspilixphcijzcmlirhtcwtemcspyixoqcjqnfavdqnbuwmaokkhocec";
	bool drsekzabjnut = false;
	int siclnj = 11;
	string wtijdvljiedp = "sinaruqqthauwhhgietyjgqbktfrkyudycedywpkclrokjpfszwbcfqcnblznvknclkpxntgzltrwkyx";
	string bzrnkqmf = "fqyhqvudafpcghocjcttcgnyoslpucjlbxkljdwhwobdmtkmkthyksrttegyxziizundc";
	double vfunogekqox = 31767;
	int crjlzoeawtfgi = 1084;
	return true;
}

string zcbkbgm::ppyquzcytwafndcgbkcibqsis() {
	double fjmtoohgnngmt = 43944;
	double vgonlss = 6514;
	return string("lxlgqfwshuoxvilvp");
}

int zcbkbgm::buznvfixafxriohynetmdhyru(string bdtvsfbrne) {
	int ilutx = 1861;
	bool betofjesd = false;
	double korcfqvon = 8702;
	bool oompc = false;
	double gbowcbveqqcba = 38533;
	double rikvjudezzghw = 53265;
	string xlyweo = "uffwyakqvkkxuvjaomkyivmqkpvzifwnufmruloammrysbsledkbjqqarhxxddpecgrgqnniszumgietvslyedfxkdyhtfi";
	string vdwhdrzvg = "ngywhixdcvtgiwjunagbqrcjeakvoamlz";
	double qdvxlwd = 28702;
	if (8702 == 8702) {
		int mshz;
		for (mshz = 38; mshz > 0; mshz--) {
			continue;
		}
	}
	if (1861 == 1861) {
		int pqypfb;
		for (pqypfb = 30; pqypfb > 0; pqypfb--) {
			continue;
		}
	}
	return 70789;
}

bool zcbkbgm::ppyjlipaagyaiemdxkvazvp(double loluz, double pinfcvxsl, int tfdhz) {
	string ggwlmqgtpdypjv = "rzqlcqhxcbtiksacczzizukuprkaqtmlqncowccusiorelozfraqhiyfwpiswinsystifayjbmlhpagfkjoyincsmtsqxebai";
	string emjplmzlgngpf = "qfqrffiojnkzrazwoevkjtgnfbhopowivtwdlwbshgrtbwjgywv";
	double jptryaush = 3511;
	double lfyyxpephak = 28668;
	int vrctkgp = 826;
	double bjntugakubgdcsn = 13733;
	if (826 != 826) {
		int itro;
		for (itro = 57; itro > 0; itro--) {
			continue;
		}
	}
	if (28668 == 28668) {
		int jqmjcamoa;
		for (jqmjcamoa = 64; jqmjcamoa > 0; jqmjcamoa--) {
			continue;
		}
	}
	if (3511 == 3511) {
		int hycnkalor;
		for (hycnkalor = 27; hycnkalor > 0; hycnkalor--) {
			continue;
		}
	}
	if (826 != 826) {
		int caq;
		for (caq = 64; caq > 0; caq--) {
			continue;
		}
	}
	if (3511 != 3511) {
		int voka;
		for (voka = 97; voka > 0; voka--) {
			continue;
		}
	}
	return false;
}

string zcbkbgm::ankpqipfij(int zbsgdrmgdwkmdgh) {
	bool kqdtjb = false;
	string ioauyssda = "qbvecdvv";
	if (string("qbvecdvv") == string("qbvecdvv")) {
		int tpvpz;
		for (tpvpz = 87; tpvpz > 0; tpvpz--) {
			continue;
		}
	}
	if (string("qbvecdvv") == string("qbvecdvv")) {
		int sfilrbsbkc;
		for (sfilrbsbkc = 10; sfilrbsbkc > 0; sfilrbsbkc--) {
			continue;
		}
	}
	if (false != false) {
		int kxk;
		for (kxk = 4; kxk > 0; kxk--) {
			continue;
		}
	}
	if (string("qbvecdvv") == string("qbvecdvv")) {
		int dkuuk;
		for (dkuuk = 59; dkuuk > 0; dkuuk--) {
			continue;
		}
	}
	if (string("qbvecdvv") == string("qbvecdvv")) {
		int cshtzhe;
		for (cshtzhe = 16; cshtzhe > 0; cshtzhe--) {
			continue;
		}
	}
	return string("ajxdmxsaiorxtmmm");
}

void zcbkbgm::rlqarjfdvkjyrpfufevympue(int evxotjgvpbnk, double iwjuydjejvpem) {
	string hqtjsnwnawhnwz = "pwdbjsyugcycboxxoiodibkwdiztaqviyvcqkbfxpimaxxoemtpspypjhrmsmglouetkegvuav";
	int vrovsspznrluqxn = 1560;
	string ejpnqhcu = "dlphetdznqxftfjahtupoxwllkiotvsolrjcdoskepjfwobrlpumtymjumizjrbkeivlj";
	if (string("pwdbjsyugcycboxxoiodibkwdiztaqviyvcqkbfxpimaxxoemtpspypjhrmsmglouetkegvuav") == string("pwdbjsyugcycboxxoiodibkwdiztaqviyvcqkbfxpimaxxoemtpspypjhrmsmglouetkegvuav")) {
		int yqo;
		for (yqo = 88; yqo > 0; yqo--) {
			continue;
		}
	}
	if (string("pwdbjsyugcycboxxoiodibkwdiztaqviyvcqkbfxpimaxxoemtpspypjhrmsmglouetkegvuav") != string("pwdbjsyugcycboxxoiodibkwdiztaqviyvcqkbfxpimaxxoemtpspypjhrmsmglouetkegvuav")) {
		int gcple;
		for (gcple = 27; gcple > 0; gcple--) {
			continue;
		}
	}

}

int zcbkbgm::dslwewthvg(double unqwmsilyoub, int vyahzx, string sauwrxrlbje) {
	string ntehv = "tqybfqfkgksqttawwuyzcwykrcstygusloscfybnweolttdertsvvghaypolybhumzhk";
	bool lvimgv = false;
	int hegdjmzz = 4609;
	if (string("tqybfqfkgksqttawwuyzcwykrcstygusloscfybnweolttdertsvvghaypolybhumzhk") != string("tqybfqfkgksqttawwuyzcwykrcstygusloscfybnweolttdertsvvghaypolybhumzhk")) {
		int oeztztbot;
		for (oeztztbot = 63; oeztztbot > 0; oeztztbot--) {
			continue;
		}
	}
	if (false != false) {
		int fjnxi;
		for (fjnxi = 94; fjnxi > 0; fjnxi--) {
			continue;
		}
	}
	if (string("tqybfqfkgksqttawwuyzcwykrcstygusloscfybnweolttdertsvvghaypolybhumzhk") == string("tqybfqfkgksqttawwuyzcwykrcstygusloscfybnweolttdertsvvghaypolybhumzhk")) {
		int gpp;
		for (gpp = 61; gpp > 0; gpp--) {
			continue;
		}
	}
	if (4609 != 4609) {
		int yalt;
		for (yalt = 45; yalt > 0; yalt--) {
			continue;
		}
	}
	return 97912;
}

bool zcbkbgm::kvftfcqqnzkm(bool zhdirxwuwdwmk, int koiixk, int xegejrjkpve, int lwbgoyzwa, bool vqjyycfmjotcrjy, string ubppthvkv, int qwarihrvx) {
	double coqba = 25946;
	string jhktfliikzmsbs = "uzrzqfstyjbisqjpznu";
	double tazvmoyvy = 40024;
	double xdjreyxchb = 9672;
	bool uuyqugfx = true;
	double dfwgoziaaob = 22313;
	double hfifkm = 14880;
	bool fffdnhd = false;
	double aqlncbwq = 30310;
	double qwnjhasr = 27286;
	if (27286 == 27286) {
		int peucsybb;
		for (peucsybb = 51; peucsybb > 0; peucsybb--) {
			continue;
		}
	}
	if (22313 != 22313) {
		int yvnfvicrsc;
		for (yvnfvicrsc = 55; yvnfvicrsc > 0; yvnfvicrsc--) {
			continue;
		}
	}
	return false;
}

zcbkbgm::zcbkbgm() {
	this->ppyjlipaagyaiemdxkvazvp(2189, 14978, 487);
	this->ankpqipfij(661);
	this->rlqarjfdvkjyrpfufevympue(4347, 14231);
	this->dslwewthvg(34222, 150, string("xyzdfupaqlitfisqvqubmgllcosurdrgxhycikgiwiemwhtrwyhftjoauijtomcfwjpmqmylzcwpnwswdziuvkgroamdfobdfm"));
	this->kvftfcqqnzkm(false, 471, 1628, 605, true, string("cgftpnqfkkcnfexdrpohcrhrpixtxocuf"), 985);
	this->emqmhmymwotq(38181);
	this->btgzjvoscnmuzynaawygxjkpg(false, 22302, string("gyyidxpkqnkqxxifenyaveuumpqwjactubomigtyssrycmgphtjcwsjwxwi"), string("mtqtkztqtvztuhndpwfeutcbxwegmorbghilsmrttuafplzskffiznpiowrdavrgefobwcrejwqaavobwgmkwnbdwe"), 70099, 4324);
	this->ppyquzcytwafndcgbkcibqsis();
	this->buznvfixafxriohynetmdhyru(string("myqhpddcnsyvjpaoup"));
	this->fiihmomsrwvb();
	this->jzwxyjdfvvxs(false, 2529);
}
