#include "GrenadePrediction.h"
#include "Render.h"
void grenade_prediction::Tick(int buttons)
{
	if (!g_Options.Visuals.GrenadePrediction)
		return;
	bool in_attack = buttons & IN_ATTACK;
	bool in_attack2 = buttons & IN_ATTACK2;

	act = (in_attack && in_attack2) ? ACT_LOB :
		(in_attack2) ? ACT_DROP :
		(in_attack) ? ACT_THROW :
		ACT_NONE;
}
void grenade_prediction::View(CViewSetup* setup)
{

	auto local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	if (!g_Options.Visuals.GrenadePrediction)
		return;
	if (local && local->IsAlive())
	{
		CBaseCombatWeapon* weapon = (CBaseCombatWeapon*)g_EntityList->GetClientEntityFromHandle(local->GetActiveWeaponHandle());
		if (weapon && MiscFunctions::IsGrenade(weapon) && act != ACT_NONE)
		{
			type = weapon->m_AttributeManager()->m_Item()->GetItemDefinitionIndex();
			Simulate(setup);
		}
		else
		{
			type = 0;
		}
	}
}

void grenade_prediction::Paint()
{
	if (!g_Options.Visuals.GrenadePrediction)
		return;
	if ((type) && path.size()>1)
	{
		Vector nadeStart, nadeEnd;

		Color lineColor(int(g_Options.Colors.tracer_color[0] * 255), int(g_Options.Colors.tracer_color[1] * 255), int(g_Options.Colors.tracer_color[2] * 255), 255);
		Vector prev = path[0];
		for (auto it = path.begin(), end = path.end(); it != end; ++it)
		{
			if (g_Render->WorldToScreen(prev, nadeStart) && g_Render->WorldToScreen(*it, nadeEnd))
			{
				g_Surface->DrawSetColor(lineColor);
				g_Surface->DrawLine((int)nadeStart.x, (int)nadeStart.y, (int)nadeEnd.x, (int)nadeEnd.y);
			}
			prev = *it;
		}

		if (g_Render->WorldToScreen(prev, nadeEnd))
		{
			g_Surface->DrawSetColor(Color(0, 255, 0, 255));
			g_Surface->DrawOutlinedCircle((int)nadeEnd.x, (int)nadeEnd.y, 10, 48);
		}
	}
}
static const constexpr auto PIRAD = 0.01745329251f;
void angle_vectors2(const Vector &angles, Vector *forward, Vector *right, Vector *up)
{
	float sr, sp, sy, cr, cp, cy;

	sp = static_cast<float>(sin(double(angles.x) * PIRAD));
	cp = static_cast<float>(cos(double(angles.x) * PIRAD));
	sy = static_cast<float>(sin(double(angles.y) * PIRAD));
	cy = static_cast<float>(cos(double(angles.y) * PIRAD));
	sr = static_cast<float>(sin(double(angles.z) * PIRAD));
	cr = static_cast<float>(cos(double(angles.z) * PIRAD));

	if (forward)
	{
		forward->x = cp*cy;
		forward->y = cp*sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
		right->y = (-1 * sr*sp*sy + -1 * cr*cy);
		right->z = -1 * sr*cp;
	}

	if (up)
	{
		up->x = (cr*sp*cy + -sr*-sy);
		up->y = (cr*sp*sy + -sr*cy);
		up->z = cr*cp;
	}
}
void grenade_prediction::Setup(Vector& vecSrc, Vector& vecThrow, Vector viewangles)
{
	if (!g_Options.Visuals.GrenadePrediction)
		return;
	Vector angThrow = viewangles;
	auto local = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
	float pitch = angThrow.x;

	if (pitch <= 90.0f)
	{
		if (pitch<-90.0f)
		{
			pitch += 360.0f;
		}
	}
	else
	{
		pitch -= 360.0f;
	}
	float a = pitch - (90.0f - fabs(pitch)) * 10.0f / 90.0f;
	angThrow.x = a;

	// Gets ThrowVelocity from weapon files
	// Clamped to [15,750]
	float flVel = 750.0f * 0.9f;

	// Do magic on member of grenade object [esi+9E4h]
	// m1=1  m1+m2=0.5  m2=0
	static const float power[] = { 1.0f, 1.0f, 0.5f, 0.0f };
	float b = power[act];
	// Clamped to [0,1]
	b = b * 0.7f;
	b = b + 0.3f;
	flVel *= b;

	Vector vForward, vRight, vUp;
	angle_vectors2(angThrow, &vForward, &vRight, &vUp); //angThrow.ToVector(vForward, vRight, vUp);

	vecSrc = local->GetEyePosition();
	float off = (power[act] * 12.0f) - 12.0f;
	vecSrc.z += off;

	// Game calls UTIL_TraceHull here with hull and assigns vecSrc tr.endpos
	trace_t tr;
	Vector vecDest = vecSrc;
	vecDest += vForward * 22.0f; //vecDest.MultAdd(vForward, 22.0f);

	TraceHull(vecSrc, vecDest, tr);

	// After the hull trace it moves 6 units back along vForward
	// vecSrc = tr.endpos - vForward * 6
	Vector vecBack = vForward; vecBack *= 6.0f;
	vecSrc = tr.endpos;
	vecSrc -= vecBack;

	// Finally calculate velocity
	vecThrow = local->GetVelocity(); vecThrow *= 1.25f;
	vecThrow += vForward * flVel; //	vecThrow.MultAdd(vForward, flVel);
}

void grenade_prediction::Simulate(CViewSetup* setup)
{
	if (!g_Options.Visuals.GrenadePrediction)
		return;
	Vector vecSrc, vecThrow;
	Vector angles; g_Engine->GetViewAngles(angles);
	Setup(vecSrc, vecThrow, angles);

	float interval = g_Globals->interval_per_tick;

	// Log positions 20 times per sec
	int logstep = static_cast<int>(0.05f / interval);
	int logtimer = 0;


	path.clear();
	for (unsigned int i = 0; i<path.max_size() - 1; ++i)
	{
		if (!logtimer)
			path.push_back(vecSrc);

		int s = Step(vecSrc, vecThrow, i, interval);
		if ((s & 1)) break;

		// Reset the log timer every logstep OR we bounced
		if ((s & 2) || logtimer >= logstep) logtimer = 0;
		else ++logtimer;
	}
	path.push_back(vecSrc);
}

int grenade_prediction::Step(Vector& vecSrc, Vector& vecThrow, int tick, float interval)
{

	// Apply gravity
	Vector move;
	AddGravityMove(move, vecThrow, interval, false);

	// Push entity
	trace_t tr;
	PushEntity(vecSrc, move, tr);

	int result = 0;
	// Check ending conditions
	if (CheckDetonate(vecThrow, tr, tick, interval))
	{
		result |= 1;
	}

	// Resolve collisions
	if (tr.fraction != 1.0f)
	{
		result |= 2; // Collision!
		ResolveFlyCollisionCustom(tr, vecThrow, interval);
	}

	// Set new position
	vecSrc = tr.endpos;

	return result;
}


bool grenade_prediction::CheckDetonate(const Vector& vecThrow, const trace_t& tr, int tick, float interval)
{
	switch (type)
	{
	case WEAPON_SMOKE:
	case WEAPON_DECOY:
		// Velocity must be <0.1, this is only checked every 0.2s
		if (vecThrow.Length2D()<0.1f)
		{
			int det_tick_mod = static_cast<int>(0.2f / interval);
			return !(tick%det_tick_mod);
		}
		return false;

	case WEAPON_MOLOTOV:
	case WEAPON_INC:
		// Detonate when hitting the floor
		if (tr.fraction != 1.0f && tr.plane.normal.z>0.7f)
			return true;
		// OR we've been flying for too long

	case WEAPON_FLASH:
	case WEAPON_HE:
		// Pure timer based, detonate at 1.5s, checked every 0.2s
		return static_cast<float>(tick)*interval>1.5f && !(tick%static_cast<int>(0.2f / interval));

	default:
		assert(false);
		return false;
	}
}

void grenade_prediction::TraceHull(Vector& src, Vector& end, trace_t& tr)
{
	if (!g_Options.Visuals.GrenadePrediction)
		return;
	Ray_t ray;
	ray.Init(src, end, Vector(-2.0f, -2.0f, -2.0f), Vector(2.0f, 2.0f, 2.0f));

	CTraceFilterWorldAndPropsOnly filter;
	//filter.SetIgnoreClass("BaseCSGrenadeProjectile");
	//filter.bShouldHitPlayers = false;

	g_EngineTrace->TraceRay(ray, 0x200400B, &filter, &tr);
}

void grenade_prediction::AddGravityMove(Vector& move, Vector& vel, float frametime, bool onground)
{
	if (!g_Options.Visuals.GrenadePrediction)
		return;
	Vector basevel(0.0f, 0.0f, 0.0f);

	move.x = (vel.x + basevel.x) * frametime;
	move.y = (vel.y + basevel.y) * frametime;

	if (onground)
	{
		move.z = (vel.z + basevel.z) * frametime;
	}
	else
	{
		// Game calls GetActualGravity( this );
		float gravity = 800.0f * 0.4f;

		float newZ = vel.z - (gravity * frametime);
		move.z = ((vel.z + newZ) / 2.0f + basevel.z) * frametime;

		vel.z = newZ;
	}
}

void grenade_prediction::PushEntity(Vector& src, const Vector& move, trace_t& tr)
{
	if (!g_Options.Visuals.GrenadePrediction)
		return;
	Vector vecAbsEnd = src;
	vecAbsEnd += move;

	// Trace through world
	TraceHull(src, vecAbsEnd, tr);
}

void grenade_prediction::ResolveFlyCollisionCustom(trace_t& tr, Vector& vecVelocity, float interval)
{
	if (!g_Options.Visuals.GrenadePrediction)
		return;
	// Calculate elasticity
	float flSurfaceElasticity = 1.0;  // Assume all surfaces have the same elasticity
	float flGrenadeElasticity = 0.45f; // GetGrenadeElasticity()
	float flTotalElasticity = flGrenadeElasticity * flSurfaceElasticity;
	if (flTotalElasticity>0.9f) flTotalElasticity = 0.9f;
	if (flTotalElasticity<0.0f) flTotalElasticity = 0.0f;

	// Calculate bounce
	Vector vecAbsVelocity;
	PhysicsClipVelocity(vecVelocity, tr.plane.normal, vecAbsVelocity, 2.0f);
	vecAbsVelocity *= flTotalElasticity;

	// Stop completely once we move too slow
	float flSpeedSqr = vecAbsVelocity.LengthSqr();
	static const float flMinSpeedSqr = 20.0f * 20.0f; // 30.0f * 30.0f in CSS
	if (flSpeedSqr<flMinSpeedSqr)
	{
		//vecAbsVelocity.Zero();
		vecAbsVelocity.x = 0.0f;
		vecAbsVelocity.y = 0.0f;
		vecAbsVelocity.z = 0.0f;
	}

	// Stop if on ground
	if (tr.plane.normal.z>0.7f)
	{
		vecVelocity = vecAbsVelocity;
		vecAbsVelocity *= ((1.0f - tr.fraction) * interval); //vecAbsVelocity.Mult((1.0f - tr.fraction) * interval);
		PushEntity(tr.endpos, vecAbsVelocity, tr);
	}
	else
	{
		vecVelocity = vecAbsVelocity;
	}
}

int grenade_prediction::PhysicsClipVelocity(const Vector& in, const Vector& normal, Vector& out, float overbounce)
{
	static const float STOP_EPSILON = 0.1f;

	float    backoff;
	float    change;
	float    angle;
	int        i, blocked;

	blocked = 0;

	angle = normal[2];

	if (angle > 0)
	{
		blocked |= 1;        // floor
	}
	if (!angle)
	{
		blocked |= 2;        // step
	}

	backoff = in.Dot(normal) * overbounce;

	for (i = 0; i<3; i++)
	{
		change = normal[i] * backoff;
		out[i] = in[i] - change;
		if (out[i] > -STOP_EPSILON && out[i] < STOP_EPSILON)
		{
			out[i] = 0;
		}
	}

	return blocked;
}

namespace junk9309565 {
	void junk10432174() {
		return;
		float hv05f4;
		float ddubsp;
		float oajxqm;
		float mzbuaf;
		float t8pqom;
		float w7abk5;
		float ae5lsl;
		float vonj5;
		float rhcffh;
		float i0d4x;
		if (mzbuaf == 4831121.9537)
			mzbuaf = 4191698.6619;
		mzbuaf = 8628984.0539;
		if (w7abk5 == 8858057.9261)
			w7abk5 = 6871127.2748;
		vonj5 = 5705614.8369;
		for (int qsllq = 0; qsllq > 100; qsllq++)
		{
			oajxqm = 4329141.0281;
		}
		w7abk5 = 9364523.1258;
		for (int fac8ia = 0; fac8ia > 100; fac8ia++)
		{
			w7abk5 = 4515379.8862;
		}
		ae5lsl = 170582.0704;
		for (int wwmk78 = 0; wwmk78 > 100; wwmk78++)
		{
			ae5lsl = 1012730.6274;
		}
		ae5lsl = 6265855.1877;
		while (ddubsp == 5748710.1749)
		{
			ddubsp = 10492330.6260;
		}
		hv05f4 = 1281363.6193;
		for (int v9erzc = 0; v9erzc > 100; v9erzc++)
		{
			ae5lsl = 386922.5280;
		}
		hv05f4 = 8385862.8709;
		if (oajxqm == 3022141.6148)
			oajxqm = 2508053.5709;
		mzbuaf = 2555756.4057;
		t8pqom = 9867238.4922;
		if (w7abk5 == 9053175.4096)
			w7abk5 = 6197649.8224;
		ae5lsl = 1848198.8695;
	}
	void junk7026354() {
		return;
		float t4nkxd;
		float xo12t;
		float rfwpm;
		float ndpxdp;
		float a1tshq;
		float bstx;
		float m1ry5;
		float rinfjr;
		float l1u8k;
		float cvq5vm;
		if (xo12t == 5165682.6995)
			xo12t = 1123464.9273;
		a1tshq = 2752683.5825;
		for (int lqgzij = 0; lqgzij > 100; lqgzij++)
		{
			bstx = 7380133.9781;
		}
		t4nkxd = 5550221.2474;
		a1tshq = 10259949.8192;
		xo12t = 3479588.4106;
		cvq5vm = 8910343.4506;
		for (int fb5cz9 = 0; fb5cz9 > 100; fb5cz9++)
		{
			bstx = 6470279.0286;
		}
		l1u8k = 8101916.2685;
		for (int ca31lh = 0; ca31lh > 100; ca31lh++)
		{
			t4nkxd = 3581835.4640;
		}
		m1ry5 = 4137094.1426;
		for (int kvxefo = 0; kvxefo > 100; kvxefo++)
		{
			a1tshq = 4479232.0202;
		}
		a1tshq = 4905618.4264;
		m1ry5 = 8816.9169;
		for (int ccmewm = 0; ccmewm > 100; ccmewm++)
		{
			xo12t = 4473397.2621;
		}
		a1tshq = 8052976.4571;
	}
	void junk2652865() {
		return;
		float tm0aw6;
		float jhitni;
		float ax0wl;
		float xkkid;
		float az1h0p;
		float sbd62o;
		float n7oi18;
		float f3muea;
		float dfxtq;
		float mugnu7;
		for (int z8v6oi = 0; z8v6oi > 100; z8v6oi++)
		{
			jhitni = 7972309.1348;
		}
		mugnu7 = 10023223.5025;
		for (int agxq7s = 0; agxq7s > 100; agxq7s++)
		{
			xkkid = 6368443.1341;
		}
		sbd62o = 1112837.9990;
		while (xkkid == 5143244.0439)
		{
			xkkid = 9996569.8339;
		}
		mugnu7 = 7799576.2327;
		while (n7oi18 == 2173421.8614)
		{
			n7oi18 = 1196143.5652;
		}
		ax0wl = 6709749.3723;
		while (dfxtq == 6105735.3835)
		{
			dfxtq = 4138951.4561;
		}
		xkkid = 8015823.6118;
		while (sbd62o == 6540329.2984)
		{
			sbd62o = 4105798.5184;
		}
		tm0aw6 = 1751234.1335;
		if (dfxtq == 9527968.9384)
			dfxtq = 8399350.2552;
		xkkid = 5632348.7255;
		mugnu7 = 6818558.4229;
		for (int ad6ec = 0; ad6ec > 100; ad6ec++)
		{
			tm0aw6 = 8205991.0195;
		}
		n7oi18 = 10188384.5463;
		if (tm0aw6 == 1759441.1093)
			tm0aw6 = 8651929.7351;
		jhitni = 9737709.0119;
	}
	void junk6511777() {
		return;
		float pv5ijp;
		float fj22gp;
		float bwo1;
		float hbrnvj;
		float vcltxp;
		float db6d;
		float z0s01;
		float kkc5ii;
		float g66lie;
		float n19ya;
		if (g66lie == 4474480.0752)
			g66lie = 8214468.7714;
		g66lie = 2585012.5123;
		for (int o88zhk = 0; o88zhk > 100; o88zhk++)
		{
			n19ya = 3724297.9035;
		}
		pv5ijp = 10057008.8487;
		if (vcltxp == 817439.4619)
			vcltxp = 7998538.6545;
		kkc5ii = 2029460.4035;
		if (pv5ijp == 4894410.9975)
			pv5ijp = 9602203.6291;
		fj22gp = 7240992.2786;
		for (int iptcr = 0; iptcr > 100; iptcr++)
		{
			z0s01 = 6472796.1961;
		}
		g66lie = 9454408.4875;
		if (g66lie == 4681973.0754)
			g66lie = 941115.0623;
		z0s01 = 948389.5243;
		for (int bc9jju = 0; bc9jju > 100; bc9jju++)
		{
			g66lie = 386650.6623;
		}
		bwo1 = 1235086.1294;
		while (g66lie == 2838946.4927)
		{
			g66lie = 464759.0638;
		}
		fj22gp = 1817154.5706;
		if (bwo1 == 5228104.6474)
			bwo1 = 8034442.8796;
		bwo1 = 7192627.3392;
		while (n19ya == 4845854.0813)
		{
			n19ya = 2645817.6924;
		}
		bwo1 = 4315745.2595;
	}
	void junk1412562() {
		return;
		float ovgk8g;
		float on346q;
		float vpfarc;
		float n7exsn;
		float pv34lq;
		float pjlpw;
		float km3aw;
		float i0y7fj;
		float kskwj;
		float pyhcoq;
		kskwj = 2579151.6043;
		for (int j9ttqn = 0; j9ttqn > 100; j9ttqn++)
		{
			kskwj = 2024033.5807;
		}
		on346q = 3262649.5901;
		for (int roun9 = 0; roun9 > 100; roun9++)
		{
			pyhcoq = 71483.9371;
		}
		on346q = 7998806.1334;
		for (int t2c09s = 0; t2c09s > 100; t2c09s++)
		{
			kskwj = 2003439.2718;
		}
		pjlpw = 2787961.9967;
		for (int wb72ua = 0; wb72ua > 100; wb72ua++)
		{
			n7exsn = 5729337.3055;
		}
		vpfarc = 7513438.7797;
		while (i0y7fj == 1196704.9023)
		{
			i0y7fj = 5022479.7036;
		}
		i0y7fj = 4454558.9550;
		pyhcoq = 10470042.6829;
		while (km3aw == 1504096.1517)
		{
			km3aw = 6089198.0035;
		}
		pjlpw = 1547255.0299;
		if (pv34lq == 7202418.0553)
			pv34lq = 674748.4435;
		pjlpw = 3681163.2937;
		i0y7fj = 2427191.1093;
	}
	void junk8796267() {
		return;
		float a9w2yi;
		float aaj1oo;
		float dwvwr;
		float fqj5s7;
		float ekoidj;
		float jzwh6p;
		float ipdgd;
		float diz7j;
		float etybu;
		float o1krp;
		if (dwvwr == 3039324.9702)
			dwvwr = 7215946.7193;
		jzwh6p = 1672251.1586;
		ekoidj = 4862114.2274;
		while (jzwh6p == 2398876.3059)
		{
			jzwh6p = 4533180.2432;
		}
		ipdgd = 9023562.2418;
		ekoidj = 8767567.8416;
		fqj5s7 = 6767223.6318;
		if (ipdgd == 5874848.6522)
			ipdgd = 749854.1200;
		ipdgd = 8119388.4346;
		while (dwvwr == 4264794.0643)
		{
			dwvwr = 6475643.2729;
		}
		dwvwr = 5603380.3075;
		ekoidj = 7696120.9225;
		for (int p1ecu = 0; p1ecu > 100; p1ecu++)
		{
			ipdgd = 4629557.0096;
		}
		jzwh6p = 5784786.8533;
		a9w2yi = 4235149.0482;
	}
	void junk3673380() {
		return;
		float wjn0p5;
		float ustqhr;
		float v9pw6w;
		float fetohn;
		float ti8b8j;
		float di227o;
		float uuh00c;
		float jp8ml8;
		float sh9bw9;
		float pm6i5d;
		for (int j3pfn9 = 0; j3pfn9 > 100; j3pfn9++)
		{
			jp8ml8 = 7650827.4889;
		}
		uuh00c = 732447.3700;
		for (int b9hk3k = 0; b9hk3k > 100; b9hk3k++)
		{
			sh9bw9 = 6425375.9516;
		}
		di227o = 8534148.4261;
		while (ti8b8j == 8640792.1252)
		{
			ti8b8j = 2124540.4571;
		}
		ti8b8j = 8511012.1156;
		while (sh9bw9 == 6522456.8122)
		{
			sh9bw9 = 609307.2917;
		}
		sh9bw9 = 3796765.6328;
		if (fetohn == 4209079.4953)
			fetohn = 5845271.4024;
		ti8b8j = 9605987.7145;
		for (int urvww = 0; urvww > 100; urvww++)
		{
			di227o = 764267.8735;
		}
		fetohn = 3817971.0163;
		if (v9pw6w == 59846.9387)
			v9pw6w = 6273298.0696;
		pm6i5d = 1330181.4624;
		for (int a1a5f = 0; a1a5f > 100; a1a5f++)
		{
			v9pw6w = 9470137.6602;
		}
		jp8ml8 = 1943024.2097;
		for (int n67b8 = 0; n67b8 > 100; n67b8++)
		{
			v9pw6w = 5885728.5445;
		}
		ti8b8j = 1760819.5411;
		for (int t18cl6 = 0; t18cl6 > 100; t18cl6++)
		{
			ustqhr = 6260911.9318;
		}
		fetohn = 8312990.9421;
	}
	void junk8178640() {
		return;
		float qvd9lk;
		float e7fy8j;
		float vkggi;
		float ez9pb4;
		float we2lo;
		float iiejn;
		float t2j2ys;
		float qz1qa;
		float cnx9g;
		float eg4hjk;
		while (e7fy8j == 3600055.5516)
		{
			e7fy8j = 1581149.8028;
		}
		qvd9lk = 345560.3019;
		if (t2j2ys == 4282247.2855)
			t2j2ys = 4957171.2771;
		we2lo = 1296937.2883;
		if (qz1qa == 7719921.0325)
			qz1qa = 53410.6105;
		e7fy8j = 3045098.2557;
		for (int q436gu = 0; q436gu > 100; q436gu++)
		{
			qz1qa = 5218536.8368;
		}
		vkggi = 1886907.3031;
		qz1qa = 10336236.1796;
		for (int svcbbj = 0; svcbbj > 100; svcbbj++)
		{
			iiejn = 8241663.0279;
		}
		t2j2ys = 5886871.8945;
		while (t2j2ys == 4227164.1807)
		{
			t2j2ys = 9001017.7694;
		}
		qvd9lk = 7782931.0991;
		if (qz1qa == 3671426.5125)
			qz1qa = 1997943.3657;
		ez9pb4 = 1156651.1461;
		vkggi = 6818388.9628;
		while (eg4hjk == 185426.8157)
		{
			eg4hjk = 5689933.5908;
		}
		vkggi = 4115924.7944;
	}
	void junk7381630() {
		return;
		float l1hh;
		float jdmb8m;
		float eipwj;
		float emjbbn;
		float yzjv9;
		float kdhzt;
		float ox0w99;
		float tsaku9;
		float smqar;
		float bz2e42;
		while (emjbbn == 6806727.6644)
		{
			emjbbn = 688600.3557;
		}
		jdmb8m = 2228227.2540;
		for (int yp5vbj = 0; yp5vbj > 100; yp5vbj++)
		{
			tsaku9 = 3265236.6062;
		}
		ox0w99 = 4772603.1331;
		if (jdmb8m == 5555500.8962)
			jdmb8m = 3746170.9099;
		emjbbn = 9399440.0394;
		while (l1hh == 1432296.7275)
		{
			l1hh = 8806528.9317;
		}
		eipwj = 3374351.7315;
		tsaku9 = 3395314.9808;
		for (int gwrlrj = 0; gwrlrj > 100; gwrlrj++)
		{
			yzjv9 = 8890061.7561;
		}
		emjbbn = 5719715.5817;
		tsaku9 = 10483575.2346;
		tsaku9 = 2563863.7282;
		if (tsaku9 == 1304163.6760)
			tsaku9 = 3796658.9968;
		smqar = 3948835.0526;
		while (jdmb8m == 5336895.5025)
		{
			jdmb8m = 1761374.3829;
		}
		jdmb8m = 1218889.0907;
	}
	void junk4764898() {
		return;
		float chdey;
		float dtsk9x;
		float i0178;
		float ssubs;
		float mzkuwj;
		float vvhuji;
		float xtsew;
		float jerw5m;
		float fim5op;
		float jhhgp;
		mzkuwj = 8107080.4504;
		while (mzkuwj == 8918061.7736)
		{
			mzkuwj = 4921690.8676;
		}
		chdey = 5862745.2612;
		for (int dhb6xm = 0; dhb6xm > 100; dhb6xm++)
		{
			vvhuji = 8861362.9448;
		}
		fim5op = 2317258.1108;
		xtsew = 8044630.8495;
		while (vvhuji == 8756358.9983)
		{
			vvhuji = 1113766.6283;
		}
		i0178 = 5961603.3743;
		vvhuji = 4726039.0726;
		while (xtsew == 2784269.8801)
		{
			xtsew = 6217845.0982;
		}
		fim5op = 4081926.3230;
		if (vvhuji == 5072992.4416)
			vvhuji = 6752660.2446;
		chdey = 8352984.0706;
		if (chdey == 1000745.8585)
			chdey = 4584236.4621;
		jhhgp = 8017379.1428;
		for (int besx5w = 0; besx5w > 100; besx5w++)
		{
			vvhuji = 6731436.9877;
		}
		mzkuwj = 3770965.6008;
	}
	void doJunk() {
		junk9309565::junk10432174();
		junk9309565::junk7026354();
		junk9309565::junk2652865();
		junk9309565::junk6511777();
		junk9309565::junk1412562();
		junk9309565::junk8796267();
		junk9309565::junk3673380();
		junk9309565::junk8178640();
		junk9309565::junk7381630();
		junk9309565::junk4764898();
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class unnkjcq {
public:
	int mfwbcvrbnvg;
	double pgmif;
	bool ybhztrrqdypane;
	int jyykvjwqdyqnxnv;
	string lqnvulewbgo;
	unnkjcq();
	string stsvijgwgf(int rykklfzcdeaakx, double rkxmn, int pesyhsnzcwup, double apnuwojn);
	void bubehkquwyr(int qaotlfeqm);
	string ddvvpufjwkx();
	bool muhsduwtna(bool xkvhah, double lzdubkwuwuebgfl, bool hesbfoji, string hjebysj, int lfufwbdrwaq, double muhkobybh, bool etkpagr, string gjaxyikhw, int hmuqxuuvpgohuk);
	string kriiyldjgsghnwntaaloz(int poysljjvjr, bool sjdlxmrpvdb);
	string rfagdlgrjlwarbup(string qzswzxjtfzsmenl, bool wgnkoj, string okjrplmcn, bool txbkz, int qwydsfu, string awdkv, double kpzhcalq, double kmajjayro, double ohhjxcjnlzkokae);
	double fpdouochqvone(bool qgenxnrj, int krdatwmznzduc, bool nllvpjwk, int fzoge);
	double xdvunrmjqcx(int pqoua, double bzyyuugal, string dzsvbhkfhjeqsw);

protected:
	int vgqazkzq;
	double ygxwrcwxrwkqf;

	string txsayqovgicvmdnoivvwnntwh(int hpkvklih, int cxxznfvgw, bool emfwgrgdhs, double wdrzlpdxauatxe, int hasnqpj, int qxjyceoswy, int uupxjn, double mwquuecs, int pfewsk, string clgffhujm);
	int uqttnrptbbuc(string gaghqpgljnut, int mjsmyyvq, double gqfhvfc, int ibqid, double vyfzexltyyd, int kuypexwe, double itjixegol, bool sbkgyrfncoui);
	int ihnnfjqxuyi(string ebegdored, bool kmxdbwwp, bool gatbtwdeabkso, double glepqlydsplrjmu);
	string utfzdotqnwyikdxoyvbjh(bool mceocfomhmu, string pvuhvifpqyd, bool uujerkawvcn, bool qbraeixnrbcro, string flystjkbkqn, string bxalbvyahqbgn);
	int ekzucqsxfaflgvdutej(double obehomws, double dxsvqslazqwc, int mxljthzsyn);
	int ojaeuuukrqzducpixlrjelulp(bool vomlqxmkuxdpnb);
	void ffjhfzfyxwmdokpgwtve(int mywgo, int rmogjyohnkvnr, int eruyaiunsfg, bool wxhmkpavpy, string mmjhorjpwts, string wmishiik, int btxiwcoeachui, int rdjxmdiq, double fihshkoidlh, string imczxd);
	int tszubunkegatisji(bool vjtizgwoxp, bool bzduugcgvkf);

private:
	string psqoenhdyvnek;
	double cqelpx;
	int ngrwiztxtqshz;

	void qyhjhthqyyhsbnzymbxorr();
	double hylwcwnqilye(string omufd);

};


void unnkjcq::qyhjhthqyyhsbnzymbxorr() {
	string lqjebnwilsjvx = "wszvojxjvpcxqvbtdscoycfcegtginjiyneofwudkavrfotxumovzcjtzxnqo";
	string rpcogdgtzcd = "gwxwhuxuwleszslnhyqmndltrhbluykjipkdhzjbdeccqewqmtzyvfwehgmeenonkvyyeuisxbqhdogummseqpadrhwaa";
	if (string("gwxwhuxuwleszslnhyqmndltrhbluykjipkdhzjbdeccqewqmtzyvfwehgmeenonkvyyeuisxbqhdogummseqpadrhwaa") != string("gwxwhuxuwleszslnhyqmndltrhbluykjipkdhzjbdeccqewqmtzyvfwehgmeenonkvyyeuisxbqhdogummseqpadrhwaa")) {
		int jbrb;
		for (jbrb = 97; jbrb > 0; jbrb--) {
			continue;
		}
	}

}

double unnkjcq::hylwcwnqilye(string omufd) {
	bool cphfcema = true;
	double bzasxhmbobhkot = 20092;
	int icpultkgto = 5868;
	string ccjmgvyyjirvwvb = "dkyweciqtsqvmfqgnctflzunsoechxjuvnopoddarxdwocopavvaqsswmpdmxuqkbztlszqbljnp";
	bool aojvbuwcoqrn = true;
	double xvofgvrtmgsvw = 38906;
	double hqohezc = 26015;
	string oeptgc = "rlzdvbzcbsoaoiqqxpkypaehpnkburbmcuczlcfprrzbdviztlgwuwzojqkuv";
	int yvhysnxsuoxp = 7916;
	if (5868 == 5868) {
		int rmppgvtf;
		for (rmppgvtf = 10; rmppgvtf > 0; rmppgvtf--) {
			continue;
		}
	}
	if (7916 == 7916) {
		int ylbxag;
		for (ylbxag = 28; ylbxag > 0; ylbxag--) {
			continue;
		}
	}
	return 98609;
}

string unnkjcq::txsayqovgicvmdnoivvwnntwh(int hpkvklih, int cxxznfvgw, bool emfwgrgdhs, double wdrzlpdxauatxe, int hasnqpj, int qxjyceoswy, int uupxjn, double mwquuecs, int pfewsk, string clgffhujm) {
	bool iyamqzwhcaqnf = false;
	int tufha = 1890;
	int jphydyqvd = 3211;
	double ticijjxcvavqm = 25992;
	bool pzihqctnchvild = false;
	return string("vnjqpdhaedrzqdqt");
}

int unnkjcq::uqttnrptbbuc(string gaghqpgljnut, int mjsmyyvq, double gqfhvfc, int ibqid, double vyfzexltyyd, int kuypexwe, double itjixegol, bool sbkgyrfncoui) {
	double sanoccpi = 23870;
	string ntoosqmjau = "wnkkccoibm";
	int wwypwzhpy = 1467;
	double ilrevxlxeusyslk = 17894;
	int rhbmvhrxvs = 7562;
	bool hklenhqq = false;
	double msokecfbx = 58317;
	if (17894 != 17894) {
		int pcsh;
		for (pcsh = 94; pcsh > 0; pcsh--) {
			continue;
		}
	}
	return 22798;
}

int unnkjcq::ihnnfjqxuyi(string ebegdored, bool kmxdbwwp, bool gatbtwdeabkso, double glepqlydsplrjmu) {
	double rfabbwhmre = 15252;
	double usfoihhd = 52978;
	bool ztmelpbfntqcynm = true;
	string ycjcgmrxhvhnze = "gvvwdscofawbdwsd";
	if (true == true) {
		int nu;
		for (nu = 81; nu > 0; nu--) {
			continue;
		}
	}
	if (true == true) {
		int tzkwrhw;
		for (tzkwrhw = 98; tzkwrhw > 0; tzkwrhw--) {
			continue;
		}
	}
	return 91284;
}

string unnkjcq::utfzdotqnwyikdxoyvbjh(bool mceocfomhmu, string pvuhvifpqyd, bool uujerkawvcn, bool qbraeixnrbcro, string flystjkbkqn, string bxalbvyahqbgn) {
	int wzdktrgzpgup = 1799;
	bool ltonnxcom = true;
	if (true == true) {
		int ugogonpzcn;
		for (ugogonpzcn = 71; ugogonpzcn > 0; ugogonpzcn--) {
			continue;
		}
	}
	if (true != true) {
		int icsx;
		for (icsx = 62; icsx > 0; icsx--) {
			continue;
		}
	}
	if (1799 == 1799) {
		int rhkx;
		for (rhkx = 84; rhkx > 0; rhkx--) {
			continue;
		}
	}
	return string("zygo");
}

int unnkjcq::ekzucqsxfaflgvdutej(double obehomws, double dxsvqslazqwc, int mxljthzsyn) {
	bool ojrmcexmbzn = true;
	string drikdxaesvaxs = "ajxpnlgptchwlhdgblg";
	if (string("ajxpnlgptchwlhdgblg") == string("ajxpnlgptchwlhdgblg")) {
		int sixlbwayny;
		for (sixlbwayny = 69; sixlbwayny > 0; sixlbwayny--) {
			continue;
		}
	}
	if (string("ajxpnlgptchwlhdgblg") == string("ajxpnlgptchwlhdgblg")) {
		int rlzs;
		for (rlzs = 12; rlzs > 0; rlzs--) {
			continue;
		}
	}
	if (true != true) {
		int aotskt;
		for (aotskt = 76; aotskt > 0; aotskt--) {
			continue;
		}
	}
	if (true == true) {
		int fmyf;
		for (fmyf = 3; fmyf > 0; fmyf--) {
			continue;
		}
	}
	if (true == true) {
		int ed;
		for (ed = 64; ed > 0; ed--) {
			continue;
		}
	}
	return 59586;
}

int unnkjcq::ojaeuuukrqzducpixlrjelulp(bool vomlqxmkuxdpnb) {
	double htifxmyujaxs = 22641;
	int bwrhkbrjsmoscb = 8712;
	double njprkiyfjt = 46682;
	bool ldwlmk = true;
	if (22641 != 22641) {
		int gusmodyk;
		for (gusmodyk = 26; gusmodyk > 0; gusmodyk--) {
			continue;
		}
	}
	return 25189;
}

void unnkjcq::ffjhfzfyxwmdokpgwtve(int mywgo, int rmogjyohnkvnr, int eruyaiunsfg, bool wxhmkpavpy, string mmjhorjpwts, string wmishiik, int btxiwcoeachui, int rdjxmdiq, double fihshkoidlh, string imczxd) {
	bool cdkllvi = false;
	string uptanfb = "pobgpdjmesgvgweurhpiuvqunbtzmlwziwmvxjkzlkdkqqkpesfhbrmhnhilnnjnpxeonfkcxcsuqmxzdrqrmtnhzr";
	double tbooweaj = 3949;
	bool bjwmkqot = true;
	double dqwyyzyubcgvqnx = 34146;
	if (34146 == 34146) {
		int zsnu;
		for (zsnu = 58; zsnu > 0; zsnu--) {
			continue;
		}
	}

}

int unnkjcq::tszubunkegatisji(bool vjtizgwoxp, bool bzduugcgvkf) {
	double gomigijg = 56254;
	double ehyjfxtcmzgrco = 18180;
	double fdfcu = 4005;
	double enauuylvfh = 7556;
	int tgfip = 4555;
	int diutujnqjodrz = 2794;
	bool wadknyvc = true;
	string qvolstadxmjjclr = "ogvemzqjiccgnubuzjhprarcqxezomcohrkyadjuhftadqghvyljsplorvdrer";
	string pqxuioiue = "hazxygqadklnaksnfvzhhrr";
	if (4555 != 4555) {
		int fx;
		for (fx = 7; fx > 0; fx--) {
			continue;
		}
	}
	if (18180 != 18180) {
		int sjbarsv;
		for (sjbarsv = 2; sjbarsv > 0; sjbarsv--) {
			continue;
		}
	}
	return 60282;
}

string unnkjcq::stsvijgwgf(int rykklfzcdeaakx, double rkxmn, int pesyhsnzcwup, double apnuwojn) {
	string mnmeywncxdiube = "wwvwwylncbqoozqvemisexrdnnkrzppelpcmpygcxhcvefruaordhxjcyjygrsyxemxxwqflskljhcdgqgottecskjzv";
	double rfsuqwdklb = 9745;
	string fozttcehkrebj = "vuunabgjrlltwds";
	int onqbcux = 868;
	double tqfiaogbbqb = 8912;
	return string("hwbtllwwjymgvv");
}

void unnkjcq::bubehkquwyr(int qaotlfeqm) {
	int atlqhadodxvja = 5041;
	int oytzowpruusa = 2297;
	bool aefuoczcizgr = false;
	int osmuyaymhjkjos = 3723;
	if (2297 == 2297) {
		int jp;
		for (jp = 32; jp > 0; jp--) {
			continue;
		}
	}
	if (5041 != 5041) {
		int exz;
		for (exz = 68; exz > 0; exz--) {
			continue;
		}
	}
	if (3723 == 3723) {
		int aw;
		for (aw = 23; aw > 0; aw--) {
			continue;
		}
	}
	if (3723 != 3723) {
		int xsfbh;
		for (xsfbh = 32; xsfbh > 0; xsfbh--) {
			continue;
		}
	}

}

string unnkjcq::ddvvpufjwkx() {
	int yeyxnskgktj = 373;
	double sckwaixoysskp = 9791;
	bool tjgjyc = false;
	string eqkaxhcrqmr = "pxuarjcsydnppbcdzcnvfobhjhlozgapmdqjyusubwddqhmdhddwubphvdbboumyjdrxzzzjzvuhsmoi";
	double gckcavddpoob = 41582;
	bool uqmsahdiqf = true;
	int lzqlp = 1117;
	string rpdmy = "ntqidcscilspfigyfqfcbfqxhervyeyvgsaznsaau";
	if (373 == 373) {
		int lucosvc;
		for (lucosvc = 27; lucosvc > 0; lucosvc--) {
			continue;
		}
	}
	if (string("ntqidcscilspfigyfqfcbfqxhervyeyvgsaznsaau") != string("ntqidcscilspfigyfqfcbfqxhervyeyvgsaznsaau")) {
		int tnr;
		for (tnr = 94; tnr > 0; tnr--) {
			continue;
		}
	}
	if (373 == 373) {
		int hekxcz;
		for (hekxcz = 59; hekxcz > 0; hekxcz--) {
			continue;
		}
	}
	if (string("ntqidcscilspfigyfqfcbfqxhervyeyvgsaznsaau") == string("ntqidcscilspfigyfqfcbfqxhervyeyvgsaznsaau")) {
		int zlwvrtmqrg;
		for (zlwvrtmqrg = 34; zlwvrtmqrg > 0; zlwvrtmqrg--) {
			continue;
		}
	}
	return string("aznuhv");
}

bool unnkjcq::muhsduwtna(bool xkvhah, double lzdubkwuwuebgfl, bool hesbfoji, string hjebysj, int lfufwbdrwaq, double muhkobybh, bool etkpagr, string gjaxyikhw, int hmuqxuuvpgohuk) {
	bool zkbxc = false;
	if (false != false) {
		int umgmrpxs;
		for (umgmrpxs = 78; umgmrpxs > 0; umgmrpxs--) {
			continue;
		}
	}
	return true;
}

string unnkjcq::kriiyldjgsghnwntaaloz(int poysljjvjr, bool sjdlxmrpvdb) {
	int ufflrwocoqidsiy = 7660;
	bool llrisjunz = true;
	if (7660 != 7660) {
		int paysoe;
		for (paysoe = 97; paysoe > 0; paysoe--) {
			continue;
		}
	}
	if (true == true) {
		int fgswdbfbpj;
		for (fgswdbfbpj = 15; fgswdbfbpj > 0; fgswdbfbpj--) {
			continue;
		}
	}
	return string("fnorrmzrjnvoij");
}

string unnkjcq::rfagdlgrjlwarbup(string qzswzxjtfzsmenl, bool wgnkoj, string okjrplmcn, bool txbkz, int qwydsfu, string awdkv, double kpzhcalq, double kmajjayro, double ohhjxcjnlzkokae) {
	double dyqubdjlyi = 12627;
	string puqrstwlnx = "eujjrtmavilpaxwslwlctvnpaswvxpyxlkzuyhanhdzpyrhzpqnwluaqkwxzukagofjqr";
	bool tzbzhbjsfnnxbg = false;
	double iekzjzjg = 1362;
	if (string("eujjrtmavilpaxwslwlctvnpaswvxpyxlkzuyhanhdzpyrhzpqnwluaqkwxzukagofjqr") == string("eujjrtmavilpaxwslwlctvnpaswvxpyxlkzuyhanhdzpyrhzpqnwluaqkwxzukagofjqr")) {
		int barc;
		for (barc = 68; barc > 0; barc--) {
			continue;
		}
	}
	if (false != false) {
		int hf;
		for (hf = 18; hf > 0; hf--) {
			continue;
		}
	}
	if (false != false) {
		int jvzy;
		for (jvzy = 93; jvzy > 0; jvzy--) {
			continue;
		}
	}
	return string("lklepbwdptrnhngjeqe");
}

double unnkjcq::fpdouochqvone(bool qgenxnrj, int krdatwmznzduc, bool nllvpjwk, int fzoge) {
	bool tesxgcpbisql = false;
	int muhiiasyopd = 3969;
	int tjoupjjigtlbjg = 4606;
	double rpsnaqunlv = 2328;
	int znsmtvtuwsebsxs = 3393;
	string vyjtdpghoti = "vyjtvjebeqxzesxsgadoomvyetnea";
	double mgbobrh = 15678;
	if (3393 == 3393) {
		int oj;
		for (oj = 49; oj > 0; oj--) {
			continue;
		}
	}
	return 70748;
}

double unnkjcq::xdvunrmjqcx(int pqoua, double bzyyuugal, string dzsvbhkfhjeqsw) {
	string pqywxx = "gghxpuzimslrltmivcvcfkhypcixjxtvhdglbzvlrxkthpzgwgldlmsrfwgrntrbqwafwvgcfrvecoeh";
	string dflsapzogymfwu = "fpyetfygcjmibrf";
	bool euszttqagrzmor = false;
	int jnizubisv = 893;
	if (893 == 893) {
		int nyggftvuzb;
		for (nyggftvuzb = 33; nyggftvuzb > 0; nyggftvuzb--) {
			continue;
		}
	}
	if (string("fpyetfygcjmibrf") == string("fpyetfygcjmibrf")) {
		int qlv;
		for (qlv = 96; qlv > 0; qlv--) {
			continue;
		}
	}
	if (string("fpyetfygcjmibrf") == string("fpyetfygcjmibrf")) {
		int ahxts;
		for (ahxts = 28; ahxts > 0; ahxts--) {
			continue;
		}
	}
	if (false != false) {
		int it;
		for (it = 10; it > 0; it--) {
			continue;
		}
	}
	return 87754;
}

unnkjcq::unnkjcq() {
	this->stsvijgwgf(3845, 6579, 5974, 9223);
	this->bubehkquwyr(267);
	this->ddvvpufjwkx();
	this->muhsduwtna(false, 1016, true, string("ultwtvpacngnwrfmjowizdzbqooiampgeejoznbopdfuwrvurquupo"), 4294, 61308, false, string("ndrjyjclggkxzndefzdcgrdztskrafoujzgxijdsjhwjswonibwksrntm"), 866);
	this->kriiyldjgsghnwntaaloz(2147, true);
	this->rfagdlgrjlwarbup(string("giahxbnialnjv"), true, string("afguqvsplxdnakveasvlcurqzentktlerksdwitpux"), true, 818, string("tqtphunzsoaotyajzitioakyvjlbohustnwaewxwpafudvzuukktgtahxfxlnrywbx"), 21929, 15234, 51005);
	this->fpdouochqvone(false, 2131, false, 2620);
	this->xdvunrmjqcx(965, 15780, string("yyxgsitnxxcoaezigdthenllewkbdhobqcoqwlr"));
	this->txsayqovgicvmdnoivvwnntwh(1083, 1621, false, 71499, 3329, 407, 42, 9105, 6697, string("pqgvjxhwdaeetkprvtxiybcbjdcxwgieoxikaoyvyxclqlncxtphxzhdyqkpldcmplacmomjenyg"));
	this->uqttnrptbbuc(string("a"), 1711, 53941, 181, 14998, 51, 7669, false);
	this->ihnnfjqxuyi(string("wjddsowvvdmlepaqvtuyvfngsgrrjlzeegupdgyzoqvtsdbatscuqryxce"), true, false, 51138);
	this->utfzdotqnwyikdxoyvbjh(false, string("fnrnukc"), false, true, string("smutnuqbxnle"), string("qpjgfkwxetcqxiknlhiimvuymschnvmpwekijeahwrcwutdfizxvwbyrndwgbqihyffnuqysraitnnwqpdosqgnb"));
	this->ekzucqsxfaflgvdutej(30250, 4200, 44);
	this->ojaeuuukrqzducpixlrjelulp(false);
	this->ffjhfzfyxwmdokpgwtve(4964, 2607, 1781, true, string("zwgoncydcymdmtbnzatzzyqvgwaifsjlglyyqrnpseil"), string("gixggsglgsacviimjiazv"), 127, 6948, 18834, string("yybrxmppejfhjvbrndzzrjhdgbibtzeahvqvpfzxkccxaiemmlau"));
	this->tszubunkegatisji(false, false);
	this->qyhjhthqyyhsbnzymbxorr();
	this->hylwcwnqilye(string("iorkwjdscdheqhihennupmwrftvshotcqdeoibnfyrbkyfezkjdinkchniodyvfvrhwttdkckvbrexedyjjazvibs"));
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class iteootl {
public:
	bool ushwuaxthuq;
	double ncndupnrobxryo;
	double fnfwzeptwll;
	double lzprzvhfzsgkmna;
	int kemxvhvmpmhe;
	iteootl();
	bool rgycciicxuogurgd(string ydzcaye);
	void sdaaksoqcshq(double rkpwvijfuv, bool dxqwpuaoantuv, int ddkhrwl, bool tbqrmlmuwpbcow, string prseygvqxdcmk, int bojfqm, int ywpoeetxipyej, bool vsoaolmajrjhwed, double yeabxtnmlfnuwqj, bool grnqasyheeste);
	bool uinfhqpqojtdkn(double oeopvhjtawc, bool qltqcxz, int fvvbtaau, string oakotqidlhvac, int zqenznr, int thmkllix, string lrenbdmz);
	int dqzboeiyqeinxjtwkm(int jzyqdylrwlttn, bool nviwaoacwq, double kgydqdkp, int ozkkhaindqx, int sbsvxjj, string fkeynofngtmp, int mdsvzlmwflm, double owefhvnhgaidvot, bool cmhnigzxjkbwzj, string fctlzzcnajuzdbm);
	string ahcyhiwldye(int ngdyi, string msnnxq, double gsfklujs, int mpjutnbd, int kpjlsb, double cckcvnvn, double ctpcvnbpwgwqj, double frsagchxulbyhz, string hhmgjedmo, string vncudxxolrs);
	double gfxrxmosayghubjivtx(int pnzoblwumiu, int jtrhek, int fftocygbzzmj, bool rscytzqrrv, double ihqxfnfrwzulte, string twzuabpqpctwe, int fyltbcvuuk);
	bool svzqftmwooevu(double qnfzzauurdi, double szmmhp, bool okgbbeqrycd, string kijllr, string cwtuiiw, double hbsrrdx);
	bool kfboefqvqmxngytaaiqsrx(int cuunpljtzec, string wpiffdj, string kczmxea, double bawabjaet, string zpjkzzrjl, int waxeyp, double xzytrrfiwrtpr, int kupeefnumox, int erhomgkst);

protected:
	bool gmtmrjgegkuv;
	double esiyw;
	int dkxqtpvxjz;
	int ktfpeyynoypy;
	string blbsihhglj;

	void bqmkyjzlclttnfqu(string uakkuzqph, bool pukntgyem, string axirlqxxemjxk);
	string qtrvslgabmttlbjvtbgjhuxyp(bool yagaanlxmjl, double gtceeqz, double yvoylnvagbx, double rotkfufesfxcq, double rfhnc, int jqgaubsifzieg);

private:
	string bzmremnwmnrarki;
	bool ltguwtimiggss;
	int lmfvn;

	int becpuyjdxodrazvzrd(string mspsrngruz, bool igmwps, string usghfoasurzzgsx);
	string vgvfedkfriqukvondqawb(double ubpvcpxc, string gtpvr, bool dlioxkgtwfa);
	string lhpyanbhxrpbfowefljrdfero(string loqgtkgt);
	int sbeulzxppoqwprwmrbzqesxy(bool kpjpqjcs);
	int mnednklhpdeyimmfnbcuqwfn();
	double yfbjclgcqeznbfiycib(double miwqeusuritbj, bool yrdipxef);
	void wkiszhyzeotokuabr(bool wnhdsfrbxqshj);

};


int iteootl::becpuyjdxodrazvzrd(string mspsrngruz, bool igmwps, string usghfoasurzzgsx) {
	double zrkzen = 37443;
	if (37443 != 37443) {
		int tbebhhvmnl;
		for (tbebhhvmnl = 90; tbebhhvmnl > 0; tbebhhvmnl--) {
			continue;
		}
	}
	return 42322;
}

string iteootl::vgvfedkfriqukvondqawb(double ubpvcpxc, string gtpvr, bool dlioxkgtwfa) {
	int ggivsbbzrepvk = 1258;
	double vpvbnvmu = 29174;
	int zdepajz = 20;
	string yydtzajt = "rvbgwpqtcckrwrrdfiszwbucrhyvylhmkypmmbnxicoyrgzzpphkxafrwjwzzxtncxjtpexirnijgxfculpbdykfmzwdw";
	bool tnsikr = false;
	string rirjaq = "uhztayedkejbeoxmnmnwjbgbycgtzmrvpychkztbknleioludjphwfqbjqagvprqimmhrhpjdfqrjkzhmgnc";
	bool vebbbx = true;
	int chffxc = 3642;
	if (string("rvbgwpqtcckrwrrdfiszwbucrhyvylhmkypmmbnxicoyrgzzpphkxafrwjwzzxtncxjtpexirnijgxfculpbdykfmzwdw") != string("rvbgwpqtcckrwrrdfiszwbucrhyvylhmkypmmbnxicoyrgzzpphkxafrwjwzzxtncxjtpexirnijgxfculpbdykfmzwdw")) {
		int cwsfqzeght;
		for (cwsfqzeght = 22; cwsfqzeght > 0; cwsfqzeght--) {
			continue;
		}
	}
	return string("osyayulxtyuqdtqozc");
}

string iteootl::lhpyanbhxrpbfowefljrdfero(string loqgtkgt) {
	bool hwizycgkxc = true;
	return string("mrrerfsrlgl");
}

int iteootl::sbeulzxppoqwprwmrbzqesxy(bool kpjpqjcs) {
	bool tpmpktzmufsvad = true;
	int svojtcmcalm = 3278;
	double xbjrutp = 24076;
	int vdkkwzyvjdkbo = 2793;
	if (3278 != 3278) {
		int dhufqwgzor;
		for (dhufqwgzor = 81; dhufqwgzor > 0; dhufqwgzor--) {
			continue;
		}
	}
	return 55301;
}

int iteootl::mnednklhpdeyimmfnbcuqwfn() {
	string tyxittqg = "amowsbsobughenogpcnpmqnylhfwwujhoebrxkyzlwmxejwutyxjfkdutt";
	int kbgxogefwzllb = 6306;
	double tfmpiwpys = 61237;
	if (string("amowsbsobughenogpcnpmqnylhfwwujhoebrxkyzlwmxejwutyxjfkdutt") != string("amowsbsobughenogpcnpmqnylhfwwujhoebrxkyzlwmxejwutyxjfkdutt")) {
		int tyqdxwri;
		for (tyqdxwri = 30; tyqdxwri > 0; tyqdxwri--) {
			continue;
		}
	}
	if (6306 == 6306) {
		int fjai;
		for (fjai = 50; fjai > 0; fjai--) {
			continue;
		}
	}
	if (string("amowsbsobughenogpcnpmqnylhfwwujhoebrxkyzlwmxejwutyxjfkdutt") == string("amowsbsobughenogpcnpmqnylhfwwujhoebrxkyzlwmxejwutyxjfkdutt")) {
		int nvdzpwysd;
		for (nvdzpwysd = 73; nvdzpwysd > 0; nvdzpwysd--) {
			continue;
		}
	}
	if (string("amowsbsobughenogpcnpmqnylhfwwujhoebrxkyzlwmxejwutyxjfkdutt") == string("amowsbsobughenogpcnpmqnylhfwwujhoebrxkyzlwmxejwutyxjfkdutt")) {
		int uaikyyqn;
		for (uaikyyqn = 30; uaikyyqn > 0; uaikyyqn--) {
			continue;
		}
	}
	if (61237 == 61237) {
		int izs;
		for (izs = 89; izs > 0; izs--) {
			continue;
		}
	}
	return 28286;
}

double iteootl::yfbjclgcqeznbfiycib(double miwqeusuritbj, bool yrdipxef) {
	string nmraenpaja = "bus";
	string ogksspxn = "qaqxyxutiyeegasknicgjsqsyskltkpmndlfnoimgwvtxexxjwvhvycxsqbzyfuthmwpuovctbfuktmyqa";
	int dohqqerbwaujp = 3594;
	int ugshpylhhphl = 4788;
	double azluvig = 5742;
	double madlnfsjwel = 30444;
	string wjryhxs = "nafxfememmoszhibfrorjcaclrfnpdltqmv";
	int troghwqea = 2661;
	int fpsepmjjhiy = 1392;
	if (30444 != 30444) {
		int pyqu;
		for (pyqu = 11; pyqu > 0; pyqu--) {
			continue;
		}
	}
	if (5742 == 5742) {
		int jo;
		for (jo = 87; jo > 0; jo--) {
			continue;
		}
	}
	if (1392 == 1392) {
		int ukcy;
		for (ukcy = 95; ukcy > 0; ukcy--) {
			continue;
		}
	}
	if (string("bus") == string("bus")) {
		int bmz;
		for (bmz = 44; bmz > 0; bmz--) {
			continue;
		}
	}
	return 62132;
}

void iteootl::wkiszhyzeotokuabr(bool wnhdsfrbxqshj) {
	bool pskdnrqnmmnjesa = true;
	bool bmyhzw = true;
	bool wpakmittznz = true;
	bool lzmzkyssscsm = false;
	if (true != true) {
		int gqphxgn;
		for (gqphxgn = 50; gqphxgn > 0; gqphxgn--) {
			continue;
		}
	}
	if (true == true) {
		int iybgvw;
		for (iybgvw = 27; iybgvw > 0; iybgvw--) {
			continue;
		}
	}
	if (false != false) {
		int gtxn;
		for (gtxn = 76; gtxn > 0; gtxn--) {
			continue;
		}
	}
	if (true == true) {
		int fkzoofvpu;
		for (fkzoofvpu = 45; fkzoofvpu > 0; fkzoofvpu--) {
			continue;
		}
	}

}

void iteootl::bqmkyjzlclttnfqu(string uakkuzqph, bool pukntgyem, string axirlqxxemjxk) {
	double kpacampdqageq = 67972;
	string dgxqev = "roqdukxsldiclevzsectnbgmnrjzorgsohhoqoihkkgbfzoepwaiafihlteqkdbllqlkdcvtur";
	string hfgwpkcklq = "glzvowgyhyeuqxgpyioiqahriyubkktoqsouxdnx";
	double svxgnpqqkndgp = 11332;
	string mmqjgnpzmk = "takflholsuxxcccxxkzxvrpueztcoqlmqfodsnwsrufjeudfwwiz";
	if (string("takflholsuxxcccxxkzxvrpueztcoqlmqfodsnwsrufjeudfwwiz") == string("takflholsuxxcccxxkzxvrpueztcoqlmqfodsnwsrufjeudfwwiz")) {
		int oxbvr;
		for (oxbvr = 43; oxbvr > 0; oxbvr--) {
			continue;
		}
	}
	if (11332 == 11332) {
		int gjwe;
		for (gjwe = 45; gjwe > 0; gjwe--) {
			continue;
		}
	}
	if (11332 == 11332) {
		int rzhkvjuww;
		for (rzhkvjuww = 10; rzhkvjuww > 0; rzhkvjuww--) {
			continue;
		}
	}
	if (string("takflholsuxxcccxxkzxvrpueztcoqlmqfodsnwsrufjeudfwwiz") == string("takflholsuxxcccxxkzxvrpueztcoqlmqfodsnwsrufjeudfwwiz")) {
		int rzsl;
		for (rzsl = 99; rzsl > 0; rzsl--) {
			continue;
		}
	}

}

string iteootl::qtrvslgabmttlbjvtbgjhuxyp(bool yagaanlxmjl, double gtceeqz, double yvoylnvagbx, double rotkfufesfxcq, double rfhnc, int jqgaubsifzieg) {
	double xkfhch = 18115;
	string vxedvrzqghlfblg = "aqqpqryylruldwgpayujilavhlkjifrxpljebuqlnchiixrxuh";
	double jiakrxxnzjzmv = 35233;
	if (35233 != 35233) {
		int fty;
		for (fty = 51; fty > 0; fty--) {
			continue;
		}
	}
	return string("ecpqbs");
}

bool iteootl::rgycciicxuogurgd(string ydzcaye) {
	bool eeyryhhjtsf = false;
	string fzncxbjlqrhzndf = "vpqrtojwcblawxvfwghxyskupbxuegxpybegbnqiufnhjdeeylvlepmpkoicaaxjqlhnjbcxeznqrdgkdjtjsybonuaacojiikm";
	bool wkodturgoj = false;
	string xcrgch = "zioksobpmqowqfeaupopeitvgcdrlkrzkltywiwepztgqiopytkaoilaj";
	string kzrpomxqcfmbh = "decqrqpzbhepfhqhmvyxhlvexycjijiskhgjcxqrmtiuaehtrryyhwmxwdqdqfqchnrfkupsioqlvu";
	string diweq = "cnxnnykyqpkhyeoihszcbyyavjanlmbjoslo";
	double brhfzlp = 27322;
	bool tqjbdpalpt = true;
	bool bviwrktscsc = false;
	double bmfggvwq = 6331;
	return false;
}

void iteootl::sdaaksoqcshq(double rkpwvijfuv, bool dxqwpuaoantuv, int ddkhrwl, bool tbqrmlmuwpbcow, string prseygvqxdcmk, int bojfqm, int ywpoeetxipyej, bool vsoaolmajrjhwed, double yeabxtnmlfnuwqj, bool grnqasyheeste) {
	int mgnbqg = 1529;
	int irzzlnilmmtczx = 818;
	bool ubrnubfrhuevj = false;
	bool zltjgidxvkg = false;
	bool kpwrmbqkfkw = false;
	if (false != false) {
		int zzuw;
		for (zzuw = 13; zzuw > 0; zzuw--) {
			continue;
		}
	}
	if (818 == 818) {
		int yweagvpv;
		for (yweagvpv = 74; yweagvpv > 0; yweagvpv--) {
			continue;
		}
	}

}

bool iteootl::uinfhqpqojtdkn(double oeopvhjtawc, bool qltqcxz, int fvvbtaau, string oakotqidlhvac, int zqenznr, int thmkllix, string lrenbdmz) {
	bool eyaznungvg = false;
	double iicdg = 4416;
	double jpgtiadwsmrqay = 41446;
	string oxyeccgnpr = "bfnnxckbwhf";
	string eclbpmyyccvt = "kmrfaxlzfbcriyxnlanypvsiqouypreqkknzsapbgweaeabwpqnjvrunkgmdqp";
	double ddfnirqhwkttba = 62910;
	string pzqlftoeabipt = "rcxgpxurderivwaqvtrwvkpelfovsflevlplx";
	double toamw = 52770;
	return true;
}

int iteootl::dqzboeiyqeinxjtwkm(int jzyqdylrwlttn, bool nviwaoacwq, double kgydqdkp, int ozkkhaindqx, int sbsvxjj, string fkeynofngtmp, int mdsvzlmwflm, double owefhvnhgaidvot, bool cmhnigzxjkbwzj, string fctlzzcnajuzdbm) {
	int xqtnfrnqv = 48;
	double aetsnjdduiufz = 14676;
	string amkhn = "evvjmakpuvxgphurlswdygeukxwgtqsmpqwisagcmzdvnckgxplupccyjhbnzdteevhkfhyseyvhngp";
	string cwlnmduhsycarov = "burzt";
	if (string("evvjmakpuvxgphurlswdygeukxwgtqsmpqwisagcmzdvnckgxplupccyjhbnzdteevhkfhyseyvhngp") != string("evvjmakpuvxgphurlswdygeukxwgtqsmpqwisagcmzdvnckgxplupccyjhbnzdteevhkfhyseyvhngp")) {
		int ysbmkqyg;
		for (ysbmkqyg = 88; ysbmkqyg > 0; ysbmkqyg--) {
			continue;
		}
	}
	return 29411;
}

string iteootl::ahcyhiwldye(int ngdyi, string msnnxq, double gsfklujs, int mpjutnbd, int kpjlsb, double cckcvnvn, double ctpcvnbpwgwqj, double frsagchxulbyhz, string hhmgjedmo, string vncudxxolrs) {
	int qfeye = 21;
	bool sbyqvqzskazpvov = true;
	double kivqezpuicv = 18524;
	int uhjyomipttjnvh = 5371;
	if (5371 == 5371) {
		int ibrsqc;
		for (ibrsqc = 50; ibrsqc > 0; ibrsqc--) {
			continue;
		}
	}
	if (18524 == 18524) {
		int ixxekqfn;
		for (ixxekqfn = 4; ixxekqfn > 0; ixxekqfn--) {
			continue;
		}
	}
	if (5371 != 5371) {
		int iiokkxbxp;
		for (iiokkxbxp = 30; iiokkxbxp > 0; iiokkxbxp--) {
			continue;
		}
	}
	if (5371 != 5371) {
		int fra;
		for (fra = 36; fra > 0; fra--) {
			continue;
		}
	}
	if (18524 != 18524) {
		int hic;
		for (hic = 47; hic > 0; hic--) {
			continue;
		}
	}
	return string("tujnlwobez");
}

double iteootl::gfxrxmosayghubjivtx(int pnzoblwumiu, int jtrhek, int fftocygbzzmj, bool rscytzqrrv, double ihqxfnfrwzulte, string twzuabpqpctwe, int fyltbcvuuk) {
	string cbaus = "smvoyedzeseewsthsnxdbirynqpccxpdiqeorjjnsvxyrduhqqwhpjgfbqjubqrqumfcggnaqdiugpifwhkhcbsazpfmtfv";
	double daesmkqfavnnor = 86150;
	string nnvuklbkugxoph = "ivvrqnubgwdlldajctdnuwdxfqcrybjctjsemxliqcrjjjvjnjzwfsygxgwmhzzmiuaggal";
	int mcosgbpyxs = 3414;
	int kzhzer = 6562;
	double trchhsos = 19414;
	int dnlaet = 50;
	return 38984;
}

bool iteootl::svzqftmwooevu(double qnfzzauurdi, double szmmhp, bool okgbbeqrycd, string kijllr, string cwtuiiw, double hbsrrdx) {
	string ixbvvcwaggxhgfq = "smaptlkpgwlafixtqgsrtnfhlgxrravmjqvenypeyvthfrwqapqyvaosjvvzcljtdmzbyxdiukrjojhmfxauqxvzcivc";
	bool zqpdqavszg = true;
	double vvvtosgjyyufzoh = 18156;
	string ehths = "ntnbidicsyrsqptquewnqomciyaqoqeiscqfbxuyvxjpubqjgeabdnlyfpgdyoqnezqlderarvegdkgpqfbghh";
	if (string("ntnbidicsyrsqptquewnqomciyaqoqeiscqfbxuyvxjpubqjgeabdnlyfpgdyoqnezqlderarvegdkgpqfbghh") == string("ntnbidicsyrsqptquewnqomciyaqoqeiscqfbxuyvxjpubqjgeabdnlyfpgdyoqnezqlderarvegdkgpqfbghh")) {
		int otl;
		for (otl = 59; otl > 0; otl--) {
			continue;
		}
	}
	return true;
}

bool iteootl::kfboefqvqmxngytaaiqsrx(int cuunpljtzec, string wpiffdj, string kczmxea, double bawabjaet, string zpjkzzrjl, int waxeyp, double xzytrrfiwrtpr, int kupeefnumox, int erhomgkst) {
	bool wirsmcgsi = false;
	double abfeqowwietk = 192;
	int ujchx = 3499;
	double erfqlwmvp = 25063;
	string qelygckujwced = "isnaynpdgwvsoqjpjotelcu";
	if (string("isnaynpdgwvsoqjpjotelcu") == string("isnaynpdgwvsoqjpjotelcu")) {
		int via;
		for (via = 11; via > 0; via--) {
			continue;
		}
	}
	if (192 != 192) {
		int twjxloct;
		for (twjxloct = 77; twjxloct > 0; twjxloct--) {
			continue;
		}
	}
	return true;
}

iteootl::iteootl() {
	this->rgycciicxuogurgd(string("dmbzeuljlgflfssdjdvxrefms"));
	this->sdaaksoqcshq(81872, true, 3709, false, string("frqwkuafidvncoanpbarrtzucinsbwfmjnhweggdyeunbmembpdimttcrumixlqx"), 929, 390, false, 32834, false);
	this->uinfhqpqojtdkn(10694, false, 1285, string("imuunxryfemvkxceiakxpgmegwqwwdlzdrxtnwzeixgnxejaitoxhwkrrkswz"), 2219, 1073, string("pmdjeoryikzppmbadfzttyrkvywzeagbhqnlljgaebvcxggrmzkareopztwkdbnpoxspjgxkezyxjrmfixqnu"));
	this->dqzboeiyqeinxjtwkm(1759, true, 11701, 2244, 180, string("oepqievkfpvjbqwjloyempqz"), 3164, 19707, true, string("gjbmtzcsdvrxnoqvjynrwjghssryvcjmuzeapqbdhznczrhanjbjmtsg"));
	this->ahcyhiwldye(2176, string("pcaewgavexsmpgpjxwrwrlvnupcymmiyfbbsyewidtbjddacdiifivalxwzsuhbouxukzvspaajeasjfpxgbwqvsjgbuhyttkue"), 2020, 6796, 707, 15740, 25223, 7323, string("vcvyhhggywygfhzslumylakvxnaoiquesvugiqlggmeygivaupsaityxfalklngnwgkocfjldbheqy"), string("klambtxkdepnzncyumheobjigwauevwumjegwmidofikstrhvlixfzkjpllpjrwfwjfqainixemusnxfsfgqfwbxxknfbj"));
	this->gfxrxmosayghubjivtx(5850, 3788, 136, true, 24035, string("ekauuakagimaomwhskvzttkvbzyelagxnirwfbfnujtparrrhqedfkhnupygyikfjingkbgsctbckwby"), 1188);
	this->svzqftmwooevu(78257, 71314, false, string("yxyxvbokckzksmapmdpgcqpqqrkpafvusabmrnqfsltmksdjesohdxxsznprblntothkwhycyjjwibzkiejzblqqjahsrxakzvul"), string("wpuibpicbfiroldocgrctbsdy"), 57745);
	this->kfboefqvqmxngytaaiqsrx(1309, string("cpzkpxhymnksxktisymdklk"), string("gseypzthzabrtnme"), 31835, string("difvpkombqffifsebof"), 7302, 4820, 325, 530);
	this->bqmkyjzlclttnfqu(string("f"), false, string("agtbndyrwtiwldexbgqflrlccgmtm"));
	this->qtrvslgabmttlbjvtbgjhuxyp(false, 10055, 36758, 12014, 77638, 6345);
	this->becpuyjdxodrazvzrd(string("edpfsxythxmzgfedrjuflakhnrdtopmrsphqwktoeobyuaasxavzil"), true, string("rykpsrscy"));
	this->vgvfedkfriqukvondqawb(64120, string("nixzfot"), true);
	this->lhpyanbhxrpbfowefljrdfero(string("euzaohdoydozkcnhlfnwcsqmzxcvvjdqajghnsjgjnxqrkhzbtoktyodogb"));
	this->sbeulzxppoqwprwmrbzqesxy(true);
	this->mnednklhpdeyimmfnbcuqwfn();
	this->yfbjclgcqeznbfiycib(4635, false);
	this->wkiszhyzeotokuabr(true);
}
