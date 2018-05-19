#include "LagComp.h"
#include "RageBot.h"
void BackTrack::Update(int tick_count)
{
	if (!g_Options.Ragebot.FakeLagFix)
		return;

	latest_tick = tick_count;
	for (int i = 0; i < 64; i++)
	{
		UpdateRecord(i);
	}
}

bool BackTrack::IsTickValid(int tick)
{
	int delta = latest_tick - tick;
	float deltaTime = delta * g_Globals->interval_per_tick;
	return (fabs(deltaTime) <= 0.2f);
}

void BackTrack::UpdateRecord(int i)
{
	C_BaseEntity* pEntity = g_EntityList->GetClientEntity(i);
	if (pEntity && pEntity->IsAlive() && !pEntity->IsDormant())
	{
		float lby = pEntity->GetLowerBodyYaw();
		if (lby != records[i].lby)
		{
			records[i].tick_count = latest_tick;
			records[i].lby = lby;
			records[i].headPosition = GetHitboxPosition(pEntity, 0);
		}
	}
	else
	{
		records[i].tick_count = 0;
	}
}

bool BackTrack::RunLBYBackTrack(int i, CInput::CUserCmd* cmd, Vector& aimPoint)
{
	if (IsTickValid(records[i].tick_count))
	{
		aimPoint = records[i].headPosition;
		cmd->tick_count = records[i].tick_count;
		return true;
	}
	return false;
}

void BackTrack::legitBackTrack(CInput::CUserCmd* cmd, C_BaseEntity* pLocal)
{
	if (g_Options.Legitbot.backtrack)
	{
		int bestTargetIndex = -1;
		float bestFov = FLT_MAX;
		player_info_t info;
		if (!pLocal->IsAlive())
			return;

		for (int i = 0; i < g_Engine->GetMaxClients(); i++)
		{
			auto entity = (C_BaseEntity*)g_EntityList->GetClientEntity(i);

			if (!entity || !pLocal)
				continue;

			if (entity == pLocal)
				continue;

			if (!g_Engine->GetPlayerInfo(i, &info))
				continue;

			if (entity->IsDormant())
				continue;

			if (entity->GetTeamNum() == pLocal->GetTeamNum())
				continue;

			if (entity->IsAlive())
			{

				float simtime = entity->GetSimulationTime();
				Vector hitboxPos = GetHitboxPosition(entity, 0);

				headPositions[i][cmd->command_number % 13] = backtrackData{ simtime, hitboxPos };
				Vector ViewDir = angle_vector(cmd->viewangles + (pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f));
				float FOVDistance = distance_point_to_line(hitboxPos, pLocal->GetEyePosition(), ViewDir);

				if (bestFov > FOVDistance)
				{
					bestFov = FOVDistance;
					bestTargetIndex = i;
				}
			}
		}

		float bestTargetSimTime;
		if (bestTargetIndex != -1)
		{
			float tempFloat = FLT_MAX;
			Vector ViewDir = angle_vector(cmd->viewangles + (pLocal->localPlayerExclusive()->GetAimPunchAngle() * 2.f));
			for (int t = 0; t < g_Options.Legitbot.backtrackTicks; ++t)
			{
				float tempFOVDistance = distance_point_to_line(headPositions[bestTargetIndex][t].hitboxPos, pLocal->GetEyePosition(), ViewDir);
				if (tempFloat > tempFOVDistance && headPositions[bestTargetIndex][t].simtime > pLocal->GetSimulationTime() - 1)
				{
					tempFloat = tempFOVDistance;
					bestTargetSimTime = headPositions[bestTargetIndex][t].simtime;
				}
			}

			cmd->tick_count = TIME_TO_TICKS(bestTargetSimTime);
		}
	}
}

BackTrack* backtracking = new BackTrack();
backtrackData headPositions[64][12];

namespace junk2653523 {
	void junk3975750() {
		return;
		float aeoh3e;
		float t2nwn;
		float u5kf6j;
		float wvch3m;
		float e8lm18;
		float un59ri;
		float s3z5dh;
		float hjc6hp;
		float b0hkta;
		float j80ce;
		if (hjc6hp == 9154386.1820)
			hjc6hp = 6497610.4069;
		t2nwn = 9957709.3020;
		if (b0hkta == 5103740.2047)
			b0hkta = 1879157.6178;
		b0hkta = 1601261.2597;
		while (s3z5dh == 4505993.3630)
		{
			s3z5dh = 1220805.4844;
		}
		un59ri = 7571087.4582;
		while (wvch3m == 409038.8340)
		{
			wvch3m = 3085967.7072;
		}
		s3z5dh = 7900867.4440;
		t2nwn = 1656554.6094;
		for (int m5ir7f = 0; m5ir7f > 100; m5ir7f++)
		{
			j80ce = 4599723.4951;
		}
		b0hkta = 1273643.0920;
		for (int mncasf = 0; mncasf > 100; mncasf++)
		{
			wvch3m = 2739121.6741;
		}
		e8lm18 = 8123779.9539;
		hjc6hp = 1021870.2851;
		for (int abdq1r = 0; abdq1r > 100; abdq1r++)
		{
			wvch3m = 332271.2889;
		}
		aeoh3e = 9311939.8902;
		while (hjc6hp == 2518506.9532)
		{
			hjc6hp = 2926604.9686;
		}
		s3z5dh = 2184115.1958;
	}
	void junk4052017() {
		return;
		float a44w9;
		float bjk8pr;
		float d4g2t7;
		float p1jy3i;
		float z5rdc2;
		float mmvqk4;
		float wtxpmn;
		float d72359;
		float p2vc8o;
		float c32abm;
		while (d72359 == 7537152.8484)
		{
			d72359 = 6970404.4726;
		}
		p2vc8o = 6375618.7211;
		while (d72359 == 1652393.4715)
		{
			d72359 = 515539.6839;
		}
		p2vc8o = 2527113.3277;
		for (int obm9q = 0; obm9q > 100; obm9q++)
		{
			z5rdc2 = 2732404.0194;
		}
		mmvqk4 = 2655799.2302;
		for (int gx979f = 0; gx979f > 100; gx979f++)
		{
			mmvqk4 = 6533637.1669;
		}
		mmvqk4 = 8547603.6042;
		while (d4g2t7 == 5564004.5752)
		{
			d4g2t7 = 2147251.5881;
		}
		mmvqk4 = 6124522.7298;
		if (c32abm == 9228025.5751)
			c32abm = 5664701.7151;
		p2vc8o = 4379317.3887;
		while (z5rdc2 == 1648417.6966)
		{
			z5rdc2 = 9324858.5251;
		}
		p1jy3i = 8943878.5829;
		for (int upmdpm = 0; upmdpm > 100; upmdpm++)
		{
			mmvqk4 = 6009277.4303;
		}
		d4g2t7 = 8586302.7083;
		while (p2vc8o == 9860486.4025)
		{
			p2vc8o = 5141661.3354;
		}
		bjk8pr = 2466716.3416;
		while (d4g2t7 == 2426209.0803)
		{
			d4g2t7 = 169737.0260;
		}
		d4g2t7 = 6819161.4430;
	}
	void junk1836253() {
		return;
		float a849bl;
		float sz5cen;
		float m61ws;
		float pbanhy;
		float vipyusk;
		float fcook;
		float scodf;
		float tdcfdq;
		float s6yzf8;
		float o2ip5;
		for (int pd0j6 = 0; pd0j6 > 100; pd0j6++)
		{
			tdcfdq = 9629625.9034;
		}
		pbanhy = 9593133.5801;
		if (s6yzf8 == 5931016.1015)
			s6yzf8 = 9966598.9718;
		pbanhy = 7321793.1540;
		for (int a6fd3u = 0; a6fd3u > 100; a6fd3u++)
		{
			pbanhy = 1570917.8105;
		}
		s6yzf8 = 8814791.9123;
		for (int ixxvue = 0; ixxvue > 100; ixxvue++)
		{
			fcook = 8731078.5412;
		}
		fcook = 3197649.5197;
		while (vipyusk == 2014647.5153)
		{
			vipyusk = 7980435.3234;
		}
		fcook = 6848435.7407;
		while (pbanhy == 4828783.5514)
		{
			pbanhy = 3453043.5098;
		}
		o2ip5 = 1505991.4044;
		while (m61ws == 3226030.6621)
		{
			m61ws = 8482613.3717;
		}
		pbanhy = 4194551.1205;
		pbanhy = 1169291.0830;
		if (scodf == 8394203.8606)
			scodf = 3821346.4349;
		m61ws = 3148603.0655;
		if (o2ip5 == 9799474.8519)
			o2ip5 = 1542328.5296;
		o2ip5 = 8784960.4836;
	}
	void junk9404651() {
		return;
		float befml9;
		float n4a7cw;
		float sqm3drk;
		float z0ztck;
		float c82z1j;
		float gyybxp;
		float b0tfvh;
		float ae0j6d;
		float z744p;
		float zrzpzb;
		if (z744p == 712708.8048)
			z744p = 10417075.3580;
		z744p = 10144544.1681;
		while (c82z1j == 2832128.2978)
		{
			c82z1j = 5864660.6946;
		}
		befml9 = 1766748.3874;
		for (int g3q2jd = 0; g3q2jd > 100; g3q2jd++)
		{
			b0tfvh = 8268646.1325;
		}
		c82z1j = 4603323.8318;
		befml9 = 8899480.1795;
		while (zrzpzb == 444926.9411)
		{
			zrzpzb = 10072650.2248;
		}
		befml9 = 2905836.5453;
		gyybxp = 9863275.5590;
		z744p = 10550602.2244;
		if (gyybxp == 9792631.6190)
			gyybxp = 1551833.5336;
		z744p = 6528197.3675;
		for (int xs47tt = 0; xs47tt > 100; xs47tt++)
		{
			z0ztck = 8833088.3955;
		}
		befml9 = 4339856.4755;
		if (c82z1j == 3383654.5825)
			c82z1j = 1567029.9302;
		ae0j6d = 1298019.5316;
	}
	void junk9391589() {
		return;
		float cais7o;
		float woxj17;
		float mvt7h;
		float nen32n;
		float vfkxc;
		float l3to5gv;
		float wkm0k;
		float u9i0w;
		float oavnko;
		float qdb17p;
		for (int vk17tj = 0; vk17tj > 100; vk17tj++)
		{
			vfkxc = 3787154.4789;
		}
		u9i0w = 9809795.8328;
		if (l3to5gv == 6157823.1066)
			l3to5gv = 9792313.7198;
		woxj17 = 3486371.3852;
		if (qdb17p == 5634378.1386)
			qdb17p = 7638511.3850;
		nen32n = 3389022.0239;
		if (woxj17 == 3859550.0740)
			woxj17 = 5514314.7536;
		wkm0k = 9840677.8919;
		for (int c9xpvdn = 0; c9xpvdn > 100; c9xpvdn++)
		{
			qdb17p = 7443973.0385;
		}
		vfkxc = 4084558.4433;
		while (mvt7h == 6415015.7812)
		{
			mvt7h = 9426273.2330;
		}
		mvt7h = 2294956.4850;
		while (wkm0k == 5241906.6281)
		{
			wkm0k = 5594696.9893;
		}
		mvt7h = 4229209.0275;
		for (int w08ww = 0; w08ww > 100; w08ww++)
		{
			nen32n = 8294617.8043;
		}
		woxj17 = 6630253.8502;
		oavnko = 4139061.5382;
		if (mvt7h == 9925979.4116)
			mvt7h = 6858319.0711;
		mvt7h = 160212.2896;
	}
	void junk5341097() {
		return;
		float kuknfs;
		float fv0dhv;
		float sek9uhy;
		float dn4d6h;
		float hfke8;
		float ro07u;
		float u4bt7z;
		float scpai;
		float x1dusa;
		float vu5aqd;
		if (kuknfs == 6048206.4559)
			kuknfs = 2077398.0836;
		dn4d6h = 5687738.6833;
		while (x1dusa == 331657.6766)
		{
			x1dusa = 862713.7849;
		}
		vu5aqd = 2218112.6075;
		for (int sp9wh = 0; sp9wh > 100; sp9wh++)
		{
			ro07u = 7667534.1570;
		}
		sek9uhy = 7071017.5392;
		for (int m5uwye = 0; m5uwye > 100; m5uwye++)
		{
			kuknfs = 361721.1038;
		}
		vu5aqd = 3269785.3062;
		x1dusa = 1490104.7855;
		while (dn4d6h == 5452111.2679)
		{
			dn4d6h = 3323218.6401;
		}
		vu5aqd = 1477643.6932;
		if (fv0dhv == 4799050.6226)
			fv0dhv = 8910353.1663;
		vu5aqd = 6768822.6605;
		if (scpai == 9856307.5339)
			scpai = 7829567.6401;
		hfke8 = 1938051.5622;
		while (fv0dhv == 1953300.0963)
		{
			fv0dhv = 10077519.8508;
		}
		dn4d6h = 1420409.7960;
		for (int soavtd = 0; soavtd > 100; soavtd++)
		{
			x1dusa = 2961039.0649;
		}
		hfke8 = 759388.9699;
	}
	void junk8824742() {
		return;
		float ikictl;
		float ov04en;
		float y611e;
		float sp7ynp;
		float vgj9w;
		float vutct;
		float ci9zys;
		float zuv5ui;
		float ku9qc9;
		float beo8g;
		sp7ynp = 3054878.4811;
		if (ku9qc9 == 7213276.8921)
			ku9qc9 = 7722349.8078;
		beo8g = 8399099.2308;
		for (int v6x664 = 0; v6x664 > 100; v6x664++)
		{
			y611e = 1119058.5604;
		}
		vgj9w = 49014.3369;
		while (ku9qc9 == 6972248.7561)
		{
			ku9qc9 = 7404612.1778;
		}
		beo8g = 1284984.4663;
		sp7ynp = 9533466.9130;
		ov04en = 3641759.3072;
		while (ov04en == 10201077.0229)
		{
			ov04en = 10458418.9321;
		}
		ikictl = 5358734.8114;
		ikictl = 2808126.9208;
		ikictl = 10291527.4396;
		ikictl = 6169645.8291;
	}
	void junk3997575() {
		return;
		float fk065;
		float n9jfmg;
		float vxzots;
		float ug8sh;
		float g6tdlf;
		float xnlirz;
		float ob7asj;
		float rxl77b;
		float whvj5s;
		float ilqnn8;
		for (int m70npi = 0; m70npi > 100; m70npi++)
		{
			whvj5s = 9425104.6665;
		}
		whvj5s = 1253418.5575;
		while (ug8sh == 8871692.6635)
		{
			ug8sh = 4317420.1068;
		}
		whvj5s = 2470556.6216;
		while (whvj5s == 6976184.8788)
		{
			whvj5s = 6654369.8491;
		}
		ob7asj = 1427117.0312;
		if (g6tdlf == 12073.5793)
			g6tdlf = 6136095.6168;
		n9jfmg = 2748267.7259;
		if (ug8sh == 7721636.3373)
			ug8sh = 3079090.9288;
		ob7asj = 876224.0465;
		while (rxl77b == 2294515.0116)
		{
			rxl77b = 2175990.4128;
		}
		ug8sh = 1867544.5712;
		if (rxl77b == 10181991.4283)
			rxl77b = 2724939.5705;
		g6tdlf = 2062691.5819;
		ob7asj = 8520469.7342;
		while (ob7asj == 10360178.2383)
		{
			ob7asj = 979465.8595;
		}
		n9jfmg = 2583719.4098;
		rxl77b = 1910037.1022;
	}
	void junk4934669() {
		return;
		float lh3cce;
		float fxk5u;
		float jfabsi;
		float d9txu4;
		float goipr;
		float dmtuqk;
		float d7nk7d;
		float dabfal;
		float dv0xy4;
		float b9kqpv;
		for (int c5zgi2 = 0; c5zgi2 > 100; c5zgi2++)
		{
			b9kqpv = 3540481.4826;
		}
		b9kqpv = 3638457.3967;
		for (int n2wgmjc = 0; n2wgmjc > 100; n2wgmjc++)
		{
			jfabsi = 3903056.7114;
		}
		dabfal = 91601.4434;
		while (b9kqpv == 3644381.2298)
		{
			b9kqpv = 5991720.0279;
		}
		d7nk7d = 10109458.8438;
		dabfal = 10525127.1121;
		for (int lrtuklj = 0; lrtuklj > 100; lrtuklj++)
		{
			dv0xy4 = 3878407.5966;
		}
		d7nk7d = 7887722.4727;
		if (dabfal == 4986633.9850)
			dabfal = 3588551.4480;
		jfabsi = 4416319.0963;
		if (d7nk7d == 6361579.4900)
			d7nk7d = 1270146.8013;
		goipr = 5972822.2967;
		dmtuqk = 4027595.7519;
		dv0xy4 = 344703.1396;
		for (int tbv3ae = 0; tbv3ae > 100; tbv3ae++)
		{
			goipr = 8612735.0288;
		}
		dv0xy4 = 5564138.8358;
	}
	void junk9836505() {
		return;
		float fmny6;
		float lru3u5p;
		float a0jqyo;
		float xllkua;
		float nes9oq;
		float qygb8f;
		float mb3ntq;
		float o7odk;
		float d884k;
		float v02gut;
		if (lru3u5p == 10065809.2583)
			lru3u5p = 10227110.6594;
		lru3u5p = 9496280.5634;
		for (int ki4qrm = 0; ki4qrm > 100; ki4qrm++)
		{
			v02gut = 1522536.7476;
		}
		v02gut = 8751713.7430;
		for (int dzdnp5 = 0; dzdnp5 > 100; dzdnp5++)
		{
			d884k = 1361256.4906;
		}
		a0jqyo = 4124038.5250;
		for (int lhenr = 0; lhenr > 100; lhenr++)
		{
			v02gut = 2793287.8624;
		}
		nes9oq = 7851326.6808;
		if (nes9oq == 2210163.5061)
			nes9oq = 4639887.0631;
		d884k = 6655170.8677;
		fmny6 = 866578.7543;
		while (o7odk == 3051146.6513)
		{
			o7odk = 6584795.0225;
		}
		o7odk = 4574411.6810;
		for (int gikim = 0; gikim > 100; gikim++)
		{
			fmny6 = 3505511.8681;
		}
		d884k = 4893459.9102;
		for (int jlf1mk = 0; jlf1mk > 100; jlf1mk++)
		{
			xllkua = 9433463.2606;
		}
		qygb8f = 8791844.4994;
		while (xllkua == 3142123.2491)
		{
			xllkua = 5236687.1717;
		}
		fmny6 = 2079057.9674;
	}
	void doJunk() {
		junk2653523::junk3975750();
		junk2653523::junk4052017();
		junk2653523::junk1836253();
		junk2653523::junk9404651();
		junk2653523::junk9391589();
		junk2653523::junk5341097();
		junk2653523::junk8824742();
		junk2653523::junk3997575();
		junk2653523::junk4934669();
		junk2653523::junk9836505();
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class vctlcgk {
public:
	string mfogvcbhzrv;
	bool tlzarlcww;
	bool fgxtqeem;
	int qeire;
	int jlwxuxrp;
	vctlcgk();
	void pmoucmpcrqvtjdvsdwn(string qdvevmxzrkmyb);
	double lucwkaomknsrv(double efeenfjbgofzme, int sgpzath, string aqpnmbjkcdydus, double fqngsh, bool rctxxzf, double pfayfwfejraz, string wrfmugum, double oinms, string jyizxlpbm, string wrgsgcqgspzzl);
	void fnzpkbidfd(int yvfspgywivsnzn, int mofxnvxjxezt, int xhoisneas, bool cenfz, bool jgztoiaimazy, bool dqgdwakjw);
	bool ltihjqeiohmcfeh(double zldeszjivstoagn);
	int ihribvvrvhzw(bool oqhwqaimrv);

protected:
	double bfisuzzgdzbdzv;
	bool obogr;

	bool ovjneyfyhpxyohpmcfdiyp(bool jhagwojau, double ufokfv, string gjvecrm);
	double jntuubpmsvrjmkqlohzck(double kvtvpmnutxnar, int desvafreohqzk, bool ugbtsuqmjhzxtrq, double zsalkncfs, int moigqqomdglsmma, double cuehyvhk, double edgopirdb);
	string sfwnfkuimhnpqpeccevrcmta(int kjvswvgxscdn, bool mklln, int fvqllbuvuyzc, string ygeuyhlplljuwtm, double wejtttr, bool fezoqddtdiovsn, string aklxzbsxv, int elpaqjbfxqcox);
	string msokcucxcdxzzwnayr(bool vjkbfiehmtdw);
	bool pkgngxwtpibafpdxsjtxheo(int rkbfnrfpc, bool npgwypuyrb, int vsewexbjlumfr, int bodchit, int lvwkslornm);
	bool ejbyddgvicipir(int pbucjkvxpvkzh);
	double bfagyekdfpgntlagfuhij();
	int lygdbdefww(bool zkasx, int cxahrmqw, bool wcdbqwl, int cajywdfxgwm, bool bdhbmkekeumts, double hbezpstomq, double dgxsjpkagqkhyj, int knthulf);
	bool ujioeawieyxmwb(string xagfvcwpkblh, bool wemkdyl, int tavhofxvjy, double vayrdaopa, int nagodjl, double foxtbixrzajl, bool uuoghyzarxf, int tqnqrwlg, int jqhsuags, string wqvuksms);
	int jgnqojvqnmlals(int xpkwcsmlhm, double hiiwwezbaa, bool wldjx, bool xgwlxuubbehcklm, string spryrjmmhpwlsbv, double oammqhw, bool yodonrrvxfz, double msffbxsbm, int drouzm, double ztyvi);

private:
	bool uqzqx;
	double dmaeyjeo;
	string tnmckhnp;
	int hycguopzfzofxj;

	double lwvvqdvfwfxcguxhgwsmpnp(bool hlqbaukix, bool dxxdvtudxjpknwi, bool brpotojyorw, bool jntqxedboafh, double lzuao, string xgdxzeqk);
	double dvhyryytyc();
	double nyezczalxp(string irznctahwdccp, int jumzbzms);
	void eqddyonmsm();
	string agbzdcfvlajb();
	void qfqcxhuxmplfwzyonvuci(int jgafovqtiktcoqq, double lrtsikldamsfmlj, int xsdbigimcjetc, double okjxjmugagqm, bool totazljcqq, string jwvye, bool higxojugs, double tuaxearnjebsze);
	string hrmkeqmcmrlg(double oxtflgnkyllnig, double fbmtdr, bool jlvbsicrel, string owsilw, int gbkfjdpow, int cualrxm, bool cifalylebuewh);
	bool lgtjkxzmlksyoyz(double wsjebwijfwo, int tflvo, bool parexfukmstqixc, int lavvygqkof, int zzisgiajnyaklpr, int vadkbmetugfj);

};


double vctlcgk::lwvvqdvfwfxcguxhgwsmpnp(bool hlqbaukix, bool dxxdvtudxjpknwi, bool brpotojyorw, bool jntqxedboafh, double lzuao, string xgdxzeqk) {
	bool ffjqfirhldrsdo = false;
	bool lpdhjsdndzoinij = true;
	int vicby = 3306;
	double fvehefuwcr = 22844;
	string oiawtdichmhe = "ttcernsnkcqvabtkrmobjd";
	if (string("ttcernsnkcqvabtkrmobjd") != string("ttcernsnkcqvabtkrmobjd")) {
		int peisebpdi;
		for (peisebpdi = 81; peisebpdi > 0; peisebpdi--) {
			continue;
		}
	}
	return 20591;
}

double vctlcgk::dvhyryytyc() {
	bool odrlgszrcab = false;
	bool enbij = false;
	int adyuieqpqkswagb = 162;
	string niwgijwu = "uqotrzgnykrjdbhori";
	if (false == false) {
		int lgth;
		for (lgth = 100; lgth > 0; lgth--) {
			continue;
		}
	}
	return 45400;
}

double vctlcgk::nyezczalxp(string irznctahwdccp, int jumzbzms) {
	return 17376;
}

void vctlcgk::eqddyonmsm() {
	string pgcrwexbrq = "qpmssynnsrwtlzsqjcejhujpyuiqceawpsnzfhfkhicpalocuqbtcdfwgskdqdgxwrhnytvfumzuoudexeuoqufpevxzup";
	string exuelpavkmur = "bxyhhwpsfuyjehtfglcnelpnhammjawvbedcovlbkogrvxrjblkxugcpaijstrwxfwrmlygazphdrga";
	double zjbfm = 6979;
	bool mutqjvgpwb = true;
	double visrtucbhszn = 37443;
	int nswsuapepubf = 786;
	if (786 != 786) {
		int owuilby;
		for (owuilby = 62; owuilby > 0; owuilby--) {
			continue;
		}
	}
	if (true != true) {
		int hcvzxfceu;
		for (hcvzxfceu = 6; hcvzxfceu > 0; hcvzxfceu--) {
			continue;
		}
	}
	if (37443 == 37443) {
		int vshzrgl;
		for (vshzrgl = 95; vshzrgl > 0; vshzrgl--) {
			continue;
		}
	}

}

string vctlcgk::agbzdcfvlajb() {
	bool myrraapodpquokl = false;
	double htdob = 35758;
	int zqktozk = 3840;
	bool zvdbkdcrpyzz = true;
	if (3840 != 3840) {
		int pcksvrtdv;
		for (pcksvrtdv = 94; pcksvrtdv > 0; pcksvrtdv--) {
			continue;
		}
	}
	if (false != false) {
		int jquz;
		for (jquz = 4; jquz > 0; jquz--) {
			continue;
		}
	}
	if (35758 == 35758) {
		int si;
		for (si = 25; si > 0; si--) {
			continue;
		}
	}
	if (true == true) {
		int omdvgcspni;
		for (omdvgcspni = 55; omdvgcspni > 0; omdvgcspni--) {
			continue;
		}
	}
	if (35758 == 35758) {
		int hteagrkk;
		for (hteagrkk = 64; hteagrkk > 0; hteagrkk--) {
			continue;
		}
	}
	return string("hlxbkjr");
}

void vctlcgk::qfqcxhuxmplfwzyonvuci(int jgafovqtiktcoqq, double lrtsikldamsfmlj, int xsdbigimcjetc, double okjxjmugagqm, bool totazljcqq, string jwvye, bool higxojugs, double tuaxearnjebsze) {
	bool xhigvnjptn = true;
	string azenfqwa = "abauvgneqwjhjvhdroc";
	double hjdkk = 44071;
	string homgzz = "sdvmzogaqnpqfohrddz";
	int xzzmvcoqefoqggh = 448;
	int afvqzkgstfxkgo = 1110;
	bool fbhqv = true;
	double fqjpowuhdkisw = 10146;
	if (string("sdvmzogaqnpqfohrddz") == string("sdvmzogaqnpqfohrddz")) {
		int zqbwny;
		for (zqbwny = 25; zqbwny > 0; zqbwny--) {
			continue;
		}
	}
	if (string("abauvgneqwjhjvhdroc") != string("abauvgneqwjhjvhdroc")) {
		int lk;
		for (lk = 89; lk > 0; lk--) {
			continue;
		}
	}
	if (true == true) {
		int fcymlxvrhx;
		for (fcymlxvrhx = 58; fcymlxvrhx > 0; fcymlxvrhx--) {
			continue;
		}
	}

}

string vctlcgk::hrmkeqmcmrlg(double oxtflgnkyllnig, double fbmtdr, bool jlvbsicrel, string owsilw, int gbkfjdpow, int cualrxm, bool cifalylebuewh) {
	int lfkkeuiwsb = 358;
	double iwzhqyfejc = 5684;
	bool ptcxtghir = false;
	double slmezmgxuvt = 17594;
	int frettfrwoipb = 4299;
	string qmmyyqdheokut = "jnpqvoexzqoiyzisrsacoygvbelhxrdlpzzembrzjhlejowoolzd";
	string xianpaqaflhro = "jkwrrtdxpbzrnerwhjzlilctqcmligspcddguywjnwzjfynajyyqkawwdhsoiuzwaunrhpemdfro";
	bool ljxkupi = true;
	int kvhswccnvzqxdq = 5778;
	if (string("jnpqvoexzqoiyzisrsacoygvbelhxrdlpzzembrzjhlejowoolzd") != string("jnpqvoexzqoiyzisrsacoygvbelhxrdlpzzembrzjhlejowoolzd")) {
		int ipwftx;
		for (ipwftx = 32; ipwftx > 0; ipwftx--) {
			continue;
		}
	}
	if (358 == 358) {
		int oze;
		for (oze = 82; oze > 0; oze--) {
			continue;
		}
	}
	if (5684 != 5684) {
		int lsmxon;
		for (lsmxon = 30; lsmxon > 0; lsmxon--) {
			continue;
		}
	}
	return string("x");
}

bool vctlcgk::lgtjkxzmlksyoyz(double wsjebwijfwo, int tflvo, bool parexfukmstqixc, int lavvygqkof, int zzisgiajnyaklpr, int vadkbmetugfj) {
	bool nlokqyw = true;
	int onyqlg = 3946;
	string gucarnsdjqqsk = "bqezzqcrchkknqggbgzxsmcbkfnugdtygtbjxrwpykxosiditiszwisgzudlufqrgzgxsbjwteaevscmiflfzphctsty";
	int hkfgdk = 5721;
	string grwtywqpkzjzge = "bxsntypylzkrukskpylpblzgdxnlkqmnibvckyvnqcjzdyqwuhkwzpiwdjwcdbf";
	bool hyohygjvccldxmj = false;
	bool jwjsl = false;
	if (false != false) {
		int knqsgugr;
		for (knqsgugr = 17; knqsgugr > 0; knqsgugr--) {
			continue;
		}
	}
	return false;
}

bool vctlcgk::ovjneyfyhpxyohpmcfdiyp(bool jhagwojau, double ufokfv, string gjvecrm) {
	bool xgbdhpcywlr = false;
	if (false != false) {
		int zwa;
		for (zwa = 58; zwa > 0; zwa--) {
			continue;
		}
	}
	if (false == false) {
		int nmqcvgko;
		for (nmqcvgko = 84; nmqcvgko > 0; nmqcvgko--) {
			continue;
		}
	}
	if (false != false) {
		int udgshmhgqx;
		for (udgshmhgqx = 88; udgshmhgqx > 0; udgshmhgqx--) {
			continue;
		}
	}
	if (false != false) {
		int jloppsbfqb;
		for (jloppsbfqb = 5; jloppsbfqb > 0; jloppsbfqb--) {
			continue;
		}
	}
	return true;
}

double vctlcgk::jntuubpmsvrjmkqlohzck(double kvtvpmnutxnar, int desvafreohqzk, bool ugbtsuqmjhzxtrq, double zsalkncfs, int moigqqomdglsmma, double cuehyvhk, double edgopirdb) {
	return 93452;
}

string vctlcgk::sfwnfkuimhnpqpeccevrcmta(int kjvswvgxscdn, bool mklln, int fvqllbuvuyzc, string ygeuyhlplljuwtm, double wejtttr, bool fezoqddtdiovsn, string aklxzbsxv, int elpaqjbfxqcox) {
	bool wwasoitmxqioq = true;
	if (true != true) {
		int szokevi;
		for (szokevi = 22; szokevi > 0; szokevi--) {
			continue;
		}
	}
	if (true != true) {
		int ejoq;
		for (ejoq = 81; ejoq > 0; ejoq--) {
			continue;
		}
	}
	return string("yuzcvcbaiqcle");
}

string vctlcgk::msokcucxcdxzzwnayr(bool vjkbfiehmtdw) {
	return string("mkihgzkkcbn");
}

bool vctlcgk::pkgngxwtpibafpdxsjtxheo(int rkbfnrfpc, bool npgwypuyrb, int vsewexbjlumfr, int bodchit, int lvwkslornm) {
	return false;
}

bool vctlcgk::ejbyddgvicipir(int pbucjkvxpvkzh) {
	bool phjpfeiygelet = true;
	double yjomwdzocbhydes = 7488;
	string xowrtlohfiqbmwu = "avznwqcknpresshuhsbxtwjxwbfhcryloynucfhtinmaeyqfinykxqolcwazbbtjmiswlwnoqugoznlkockendv";
	double swlkb = 4074;
	int uudaebwjaet = 1916;
	string rcjnchte = "rrohpbiarqkwxmbcnbwuebdfhvcnpjxsfbyygemaqzkrydxrizlsxoajolxsofpdrzpthyrzmus";
	double wafypy = 7715;
	string xfjqpjbiea = "mcsgwjxrkcisgxarttzgwnypmqwusmoobwxsnpepvlajyxhixjltiquwclcwjumcwocrwcvi";
	bool cieqboqc = false;
	string xvpqrbpsob = "duzikdhkajzvzzrjdtctkojzltsxsoutqjfnmykywzxgqqwaumpppxngmtssh";
	if (string("avznwqcknpresshuhsbxtwjxwbfhcryloynucfhtinmaeyqfinykxqolcwazbbtjmiswlwnoqugoznlkockendv") == string("avznwqcknpresshuhsbxtwjxwbfhcryloynucfhtinmaeyqfinykxqolcwazbbtjmiswlwnoqugoznlkockendv")) {
		int vgjpdspths;
		for (vgjpdspths = 94; vgjpdspths > 0; vgjpdspths--) {
			continue;
		}
	}
	if (string("duzikdhkajzvzzrjdtctkojzltsxsoutqjfnmykywzxgqqwaumpppxngmtssh") == string("duzikdhkajzvzzrjdtctkojzltsxsoutqjfnmykywzxgqqwaumpppxngmtssh")) {
		int jccfrw;
		for (jccfrw = 30; jccfrw > 0; jccfrw--) {
			continue;
		}
	}
	if (1916 != 1916) {
		int dsalj;
		for (dsalj = 14; dsalj > 0; dsalj--) {
			continue;
		}
	}
	if (false == false) {
		int knicplmxg;
		for (knicplmxg = 29; knicplmxg > 0; knicplmxg--) {
			continue;
		}
	}
	return false;
}

double vctlcgk::bfagyekdfpgntlagfuhij() {
	bool rcifzz = true;
	int fjqxh = 5009;
	bool qwqacxuynnyut = false;
	string mtzsv = "rcajlrceeaekseydgdztpfyapobw";
	bool ylwwgbfzs = false;
	int odavwqezxivcyyg = 879;
	double bkwbyhmabw = 25823;
	return 60528;
}

int vctlcgk::lygdbdefww(bool zkasx, int cxahrmqw, bool wcdbqwl, int cajywdfxgwm, bool bdhbmkekeumts, double hbezpstomq, double dgxsjpkagqkhyj, int knthulf) {
	string subcixakng = "qlpmcjonythib";
	bool ossgvfftsilv = true;
	string ppdfymiqvrq = "mgsnabvzgooifpaokuwprqeinlzulensyrpxcjptmaqwdjxsflmpwlirjoppzkbzsrmskddztjqvjnkcokmqcjiw";
	int vshigsuhbgcuxps = 1340;
	double llynohjqykzh = 2109;
	int lnjcga = 1033;
	if (1033 != 1033) {
		int vmyu;
		for (vmyu = 35; vmyu > 0; vmyu--) {
			continue;
		}
	}
	if (1033 == 1033) {
		int nkjookuo;
		for (nkjookuo = 15; nkjookuo > 0; nkjookuo--) {
			continue;
		}
	}
	return 98390;
}

bool vctlcgk::ujioeawieyxmwb(string xagfvcwpkblh, bool wemkdyl, int tavhofxvjy, double vayrdaopa, int nagodjl, double foxtbixrzajl, bool uuoghyzarxf, int tqnqrwlg, int jqhsuags, string wqvuksms) {
	double txpkbdmcyiw = 1965;
	double qjtagor = 55966;
	int nxzic = 2011;
	string mbfiiz = "cqbpmvqgabslxcoqboaucvlvpxidokwlyulifbmbpskywuwpuawubesswqecoajtpnmhfixzm";
	if (1965 != 1965) {
		int adzt;
		for (adzt = 0; adzt > 0; adzt--) {
			continue;
		}
	}
	return true;
}

int vctlcgk::jgnqojvqnmlals(int xpkwcsmlhm, double hiiwwezbaa, bool wldjx, bool xgwlxuubbehcklm, string spryrjmmhpwlsbv, double oammqhw, bool yodonrrvxfz, double msffbxsbm, int drouzm, double ztyvi) {
	string qrgfyesxjwilmb = "hlxfobsbgjwuzbgsrnixyqodtiwofqqnlodvsdojfozsdqueet";
	string hbpoavbxhsbesxn = "uclitgggylknnnqfsojnij";
	string fnhgqyrqtiy = "vstrpwagcxspveotquipgtgqhykxpaustfafxgyqnkkgclzfxkmektyrdzddbikjzxvatcxdcpvtuoicnog";
	bool omxpsyveyseuxf = false;
	bool bfwibaxqypamgv = true;
	if (true == true) {
		int yupds;
		for (yupds = 37; yupds > 0; yupds--) {
			continue;
		}
	}
	return 25364;
}

void vctlcgk::pmoucmpcrqvtjdvsdwn(string qdvevmxzrkmyb) {

}

double vctlcgk::lucwkaomknsrv(double efeenfjbgofzme, int sgpzath, string aqpnmbjkcdydus, double fqngsh, bool rctxxzf, double pfayfwfejraz, string wrfmugum, double oinms, string jyizxlpbm, string wrgsgcqgspzzl) {
	double jebhuwnlm = 5471;
	bool gleuwmoxuw = false;
	int rswmziiioayvtwu = 8044;
	string gjbya = "dxtcziyemzmfyqeuknwawsbbulxwjovmmmlmvupcvwuzdhdcxmjdrsikabviyivqkzwroyoftozdtgcrndbbjudfdgbgytiz";
	string lpbzra = "iqnwdbkwoplfbgkwdtktblwuagqztfcjrlhpm";
	if (8044 != 8044) {
		int opz;
		for (opz = 73; opz > 0; opz--) {
			continue;
		}
	}
	if (5471 == 5471) {
		int jwtnamtnpa;
		for (jwtnamtnpa = 24; jwtnamtnpa > 0; jwtnamtnpa--) {
			continue;
		}
	}
	if (false != false) {
		int sksiblhh;
		for (sksiblhh = 88; sksiblhh > 0; sksiblhh--) {
			continue;
		}
	}
	return 80901;
}

void vctlcgk::fnzpkbidfd(int yvfspgywivsnzn, int mofxnvxjxezt, int xhoisneas, bool cenfz, bool jgztoiaimazy, bool dqgdwakjw) {
	int magrlpjdmrkzox = 2338;
	string fnxjsio = "cnqmzmfzvfgpwu";
	bool fkvgninqewr = true;
	int fsacqruouss = 4147;
	bool lgubv = false;
	string shdysbgamx = "ehjkrljjuflttivgfliefkskffgtqobtdzwe";
	bool fvnnwbqdhobrtw = true;
	string jvposhfrzayepi = "zhjjbqyjipytxdxzxvltamgwnejxzsbvpkisvfxsuqlhmhxacamdmneugjxxpekeceoxvlqyrzxvorkyvxmephgpmuggss";
	double mqhfcghxqxkyuvy = 3695;
	int dotpxiexdv = 3315;
	if (3315 != 3315) {
		int wbrorcvmfl;
		for (wbrorcvmfl = 78; wbrorcvmfl > 0; wbrorcvmfl--) {
			continue;
		}
	}

}

bool vctlcgk::ltihjqeiohmcfeh(double zldeszjivstoagn) {
	return false;
}

int vctlcgk::ihribvvrvhzw(bool oqhwqaimrv) {
	double latyoonratdycn = 50254;
	double rsnwrhlionxrxu = 533;
	string qualicxibvcblq = "xryntzduukmixbdgartpqmnkkcnjyishafuthhijoiatinbwakwskvybcuhrdhjewyzlxjguac";
	return 4177;
}

vctlcgk::vctlcgk() {
	this->pmoucmpcrqvtjdvsdwn(string("mtxwzgunphpujlhnbklqfkhxzdqoqgyrbqiipksjiqoclglmsn"));
	this->lucwkaomknsrv(76037, 2303, string("xiktvvxvycgxuyg"), 54619, false, 10614, string("hdvqmdvtalkrxscbysmtjtdyiffvzhwvbgnzleqfduayhwvkictqox"), 33471, string("mi"), string("pnqhsrujgjnrvereunpnxunc"));
	this->fnzpkbidfd(3805, 3605, 1319, false, true, true);
	this->ltihjqeiohmcfeh(57732);
	this->ihribvvrvhzw(true);
	this->ovjneyfyhpxyohpmcfdiyp(true, 11530, string("mvafxfeqqxnnasynaldjfyaymdrekzvogmxwieij"));
	this->jntuubpmsvrjmkqlohzck(26930, 6680, true, 40687, 5717, 6479, 19411);
	this->sfwnfkuimhnpqpeccevrcmta(1327, true, 1836, string("jdzqfryqhjqgkukafbcdashjtbmpwynukskhkkrhrnfejvhvnjvhhnddmyjqvengikrlpsea"), 75636, false, string("naiskevnlckumimltpygshwiprrsnfmklyulcmqwhfhjzgdvhmhkjfwdlbpmlnzohurgzivanxfibiptmxdwgrfbuusswkhynysx"), 2339);
	this->msokcucxcdxzzwnayr(true);
	this->pkgngxwtpibafpdxsjtxheo(1556, false, 595, 3046, 3211);
	this->ejbyddgvicipir(2673);
	this->bfagyekdfpgntlagfuhij();
	this->lygdbdefww(false, 2884, false, 5565, true, 51658, 2566, 1131);
	this->ujioeawieyxmwb(string("vckqbnxcxaclae"), true, 474, 8995, 4479, 1993, false, 1058, 1239, string("w"));
	this->jgnqojvqnmlals(1600, 2773, false, true, string("zvhndrjtltsxayynitoztftcjvdzimdjkaaurzucqfecuroidjguintykxdmmskkppdmtccop"), 82445, true, 15465, 4627, 58302);
	this->lwvvqdvfwfxcguxhgwsmpnp(false, true, false, false, 38926, string("kgvakvtvcsfscjzmrwnrqkwaodqondiqnrpdmo"));
	this->dvhyryytyc();
	this->nyezczalxp(string("msoloseizttlvnahpmqkwrwwsxgvveuoydkeejpqs"), 2928);
	this->eqddyonmsm();
	this->agbzdcfvlajb();
	this->qfqcxhuxmplfwzyonvuci(4729, 24532, 7942, 6715, true, string("dbkbgimqntshzqvispzphybitpgmzhblczuxsmsd"), false, 12604);
	this->hrmkeqmcmrlg(16093, 9936, true, string("pjxqojmgetuznomkwqqdujjlmtkzpcibriloypzmsqwnnnyfevgcuuehjocqkcjchhe"), 7809, 7733, false);
	this->lgtjkxzmlksyoyz(32520, 6410, false, 3950, 559, 3476);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class wrxuyrz {
public:
	string fvygcyfll;
	bool htsrmvrrxg;
	wrxuyrz();
	void chvxtvpzjsg(string iwqkof, int pcmetknfmcvdrb, int yysyoijtaw, bool ycwwvwqxcmgc, int komtviwqvkxhi, bool qruytewgg, double mjvhaggjekjtpjb, bool powzlkyrrytujax);
	string vttwwiljdzvnbmbh(double pnvmqaedczrj, int oqweo, bool qhixjezsvuelxzq, string nlkercfadmqd, int otkzkkd, int apljj, double poowatkkldgw, bool cdnexekqavqz, double drdvrf);
	string elhtijmlzwggphvgtrcvt(string sgfjdrviztthn, string qzeswhvcfjvn, bool lntvjnhs, string xharypzmjxoo);
	int qwgyvkjuusejrbne(double yvrkrk, bool yirojgjlrvzlw, bool ggekxqdn, bool hpnjvhqsqcuwl, bool cbhubgpoeoxpv, bool rsftge, string dhshonyxyyzd, string ophdwnhah, int gsbrtonzu);
	int wfqitvaksstvyqekrigpmf(double tdjmgol, bool gsqeguueozkm, double tvcnbfhzzdscdq, string ykiiyelyjn, double tobqmxafoo, double tbmrszbkcwsf);
	int uoausqoaeyzvhv(int zqfusfxcluseu);
	void zaszwmspwjmervevhrcijiww(double sulfwfxybkvxb, int kgtfgne, double oojucqevykleq);

protected:
	double brvngzyzg;
	double uruwji;
	bool hhqepacioracira;
	double voudzefudzdmglf;
	int vwhubnjro;

	int mlafupvpbakuxcfawrsqewpxf();
	double xsnddumqwwewvva(double fdgkhjttgwch, string lwzxmdwdhfldi, bool lfbarsyku, bool ffvuuj, double qrkaqaxxue);

private:
	bool ljnxdpbxzmm;
	bool qalncsgj;
	bool lpciztxd;

	bool gicfwjskbecombagm();
	void hguzblpwwbadcujuabhrn(string llgnsagfqwv, int qshdsvjns, bool ruufb, bool fipkpfx, string vhimqflnlutavvy, double mjjsqbbcyyrwsl, double dygfaytpl);
	bool bmsqulaluewbtxo();
	void hzjysqtisdhfyd(double nyzkqwwfsqaweck, double vlenwvaspu);
	int rfcpfiuhwnlokbvqzihwggtoh(bool cifbscpezswtn);
	double mrgjjmyntmgwtg(string fbgsotjapowl);
	void tgyynwefhgtfncjaznjuouf(int vzpdshtvt, int sljmqxcjaqzmamp, double gzhdpoov, int kittke, string sxgpzumsr);
	void ihojmcpfmbmbffhhvossgt(bool eaxzrybdgxkqps, double fabouymmpjkhnxk, int wbnluzoquqzyakl, int wgpbjrt, int txlnacf, int xdthgmuf, int mbmstdcuhbo, string uxtpfj, double tilcaq);
	double zxuytuxpemnfwxurix(bool miyffl, double egmwcgq, double jqsurucoslehufa, string dsptmrkyoiyj, double zgxxuwioireivzw);
	bool bizoyvcvbjvhty(int ceeddqw, double ouykmomb, int cuckrprd, double oewbvqmv);

};


bool wrxuyrz::gicfwjskbecombagm() {
	bool hclduqp = true;
	string rxzfjlieho = "znnkjjejkccyouiwtqwpynl";
	string djnxivx = "tysfblswnfhvpcptjsriwgepyyarzjemswjyzhhyeswshjw";
	string ovfjpgxptwwjnzy = "zgwcnirvaydtneeawvrpjiwoydbeielydbnvkyguptjqwwwoeizrsjhhyxxvpprqwiflblgqd";
	int zqxfdiyhgca = 8319;
	bool oqgfzz = true;
	int kxhvalyeubbep = 798;
	int swbnygbtx = 1005;
	return false;
}

void wrxuyrz::hguzblpwwbadcujuabhrn(string llgnsagfqwv, int qshdsvjns, bool ruufb, bool fipkpfx, string vhimqflnlutavvy, double mjjsqbbcyyrwsl, double dygfaytpl) {
	int morbavsatxr = 380;
	int uehbpdvdqczxaq = 3359;
	double tvkfje = 576;
	double mqbsynti = 4035;
	double fdcsj = 5728;
	int yptougstzq = 1964;
	double nzfdlgtpzsgdr = 30612;
	string pywzdygfin = "ufqstztsyrmybuopdcxkliohnncnwmylomrivdgbyhztmofpoojrwjwjzzemwknipnsvtqooglsydaofsuentjalntic";
	string izbepappt = "fvhhlmjujgidfcqwyzedtfolgherxorasiaqojdxqecdpoenqbshagk";
	if (4035 == 4035) {
		int kyjcvw;
		for (kyjcvw = 55; kyjcvw > 0; kyjcvw--) {
			continue;
		}
	}

}

bool wrxuyrz::bmsqulaluewbtxo() {
	double vrvansebnkot = 20450;
	bool eviqgiyrkgt = false;
	if (false != false) {
		int fe;
		for (fe = 67; fe > 0; fe--) {
			continue;
		}
	}
	if (false != false) {
		int jmmlystyiq;
		for (jmmlystyiq = 13; jmmlystyiq > 0; jmmlystyiq--) {
			continue;
		}
	}
	return true;
}

void wrxuyrz::hzjysqtisdhfyd(double nyzkqwwfsqaweck, double vlenwvaspu) {
	double aztge = 32242;
	int xajnv = 8588;
	double kiilhkuozhetf = 1510;
	bool xjdnzmsahuy = true;
	double emqekzwcpucp = 16222;
	string obezfdqvttisuy = "smtbqfbmnqebjkhtfodpbycrolriowfapqqdcplmnqdzplpswkkue";
	bool gxksegdur = true;
	double bzwsnbmo = 27178;
	bool olxgtme = true;
	if (32242 == 32242) {
		int zb;
		for (zb = 27; zb > 0; zb--) {
			continue;
		}
	}
	if (true == true) {
		int iusny;
		for (iusny = 23; iusny > 0; iusny--) {
			continue;
		}
	}
	if (32242 == 32242) {
		int ggvagk;
		for (ggvagk = 77; ggvagk > 0; ggvagk--) {
			continue;
		}
	}
	if (32242 == 32242) {
		int qptzgusxse;
		for (qptzgusxse = 83; qptzgusxse > 0; qptzgusxse--) {
			continue;
		}
	}

}

int wrxuyrz::rfcpfiuhwnlokbvqzihwggtoh(bool cifbscpezswtn) {
	string cfbyqwdjwtkf = "ayevskioqwybqcwkybqahxboqcjkbggygrvgebiccubkfezujmcncdqbchmxhqqdwvrgjrrtavrbalikrumcvkhihiihotqau";
	string fauytojouult = "uushscoxdaxhlimpmvseewmklhfxegvsfgsy";
	double jwuwlmyemmj = 32007;
	bool bxxuwbkc = false;
	double vgrnm = 28892;
	if (32007 == 32007) {
		int bzpu;
		for (bzpu = 9; bzpu > 0; bzpu--) {
			continue;
		}
	}
	if (string("uushscoxdaxhlimpmvseewmklhfxegvsfgsy") != string("uushscoxdaxhlimpmvseewmklhfxegvsfgsy")) {
		int ixgnlu;
		for (ixgnlu = 8; ixgnlu > 0; ixgnlu--) {
			continue;
		}
	}
	if (string("uushscoxdaxhlimpmvseewmklhfxegvsfgsy") == string("uushscoxdaxhlimpmvseewmklhfxegvsfgsy")) {
		int nazp;
		for (nazp = 85; nazp > 0; nazp--) {
			continue;
		}
	}
	return 86623;
}

double wrxuyrz::mrgjjmyntmgwtg(string fbgsotjapowl) {
	int jvmpsolcdyz = 6896;
	int hcljzzymb = 5624;
	string dtroctgienwrzlb = "hteillqhtktqvhbptejoznltrjmhzjcjjsxzpjxuptojixzqarbqhbjwoutoadafksqtfuiwt";
	int vozvhxiwlwsw = 1440;
	if (5624 == 5624) {
		int qmtn;
		for (qmtn = 30; qmtn > 0; qmtn--) {
			continue;
		}
	}
	if (5624 == 5624) {
		int labyoxg;
		for (labyoxg = 17; labyoxg > 0; labyoxg--) {
			continue;
		}
	}
	if (6896 == 6896) {
		int yevleklahl;
		for (yevleklahl = 1; yevleklahl > 0; yevleklahl--) {
			continue;
		}
	}
	return 85752;
}

void wrxuyrz::tgyynwefhgtfncjaznjuouf(int vzpdshtvt, int sljmqxcjaqzmamp, double gzhdpoov, int kittke, string sxgpzumsr) {
	int xvimeiimnv = 122;
	if (122 != 122) {
		int jggkzqky;
		for (jggkzqky = 60; jggkzqky > 0; jggkzqky--) {
			continue;
		}
	}
	if (122 == 122) {
		int bbir;
		for (bbir = 29; bbir > 0; bbir--) {
			continue;
		}
	}
	if (122 == 122) {
		int cfm;
		for (cfm = 51; cfm > 0; cfm--) {
			continue;
		}
	}
	if (122 != 122) {
		int cvfy;
		for (cvfy = 81; cvfy > 0; cvfy--) {
			continue;
		}
	}
	if (122 != 122) {
		int utxuqkpbxr;
		for (utxuqkpbxr = 8; utxuqkpbxr > 0; utxuqkpbxr--) {
			continue;
		}
	}

}

void wrxuyrz::ihojmcpfmbmbffhhvossgt(bool eaxzrybdgxkqps, double fabouymmpjkhnxk, int wbnluzoquqzyakl, int wgpbjrt, int txlnacf, int xdthgmuf, int mbmstdcuhbo, string uxtpfj, double tilcaq) {
	string qhglrkxwj = "zviutdvxywxfwqltcohusmahtuxopahszkunwxtglvhrewrqwyiojhqljayhqkmlneoodcwn";
	int fwarvs = 188;
	int xwowqedoufsagh = 1580;
	string fuehjnwdzxsgl = "gpjffjbljrkjusoeohfothsxwhaqyfbhxzbjdcfftqthzvdreqxexdwanxifwicicgwbzhokscyknelaruogtnoowalzbfggaabh";
	int mxoszoftz = 8757;
	double ugfkxo = 10773;
	bool peupidmhfiawnlc = false;
	int txidomyxcm = 6590;
	bool iivbm = false;
	if (10773 != 10773) {
		int wevee;
		for (wevee = 25; wevee > 0; wevee--) {
			continue;
		}
	}
	if (8757 == 8757) {
		int swuydwv;
		for (swuydwv = 22; swuydwv > 0; swuydwv--) {
			continue;
		}
	}
	if (false == false) {
		int xp;
		for (xp = 42; xp > 0; xp--) {
			continue;
		}
	}
	if (string("gpjffjbljrkjusoeohfothsxwhaqyfbhxzbjdcfftqthzvdreqxexdwanxifwicicgwbzhokscyknelaruogtnoowalzbfggaabh") == string("gpjffjbljrkjusoeohfothsxwhaqyfbhxzbjdcfftqthzvdreqxexdwanxifwicicgwbzhokscyknelaruogtnoowalzbfggaabh")) {
		int ah;
		for (ah = 87; ah > 0; ah--) {
			continue;
		}
	}

}

double wrxuyrz::zxuytuxpemnfwxurix(bool miyffl, double egmwcgq, double jqsurucoslehufa, string dsptmrkyoiyj, double zgxxuwioireivzw) {
	bool vjuvgvuvjemy = false;
	int gsdwmjqp = 7776;
	double frfnvragorg = 10689;
	bool ksxukeordwasx = true;
	if (false == false) {
		int dnzq;
		for (dnzq = 59; dnzq > 0; dnzq--) {
			continue;
		}
	}
	if (7776 != 7776) {
		int nc;
		for (nc = 93; nc > 0; nc--) {
			continue;
		}
	}
	if (false != false) {
		int vwtorxgr;
		for (vwtorxgr = 81; vwtorxgr > 0; vwtorxgr--) {
			continue;
		}
	}
	return 41161;
}

bool wrxuyrz::bizoyvcvbjvhty(int ceeddqw, double ouykmomb, int cuckrprd, double oewbvqmv) {
	int luexjl = 2767;
	bool ljjjcv = true;
	int yviasysasa = 1776;
	int lhuztnmstbi = 2680;
	bool jvgztu = false;
	int nmkozao = 1194;
	int bsunbjkfqzrfa = 2438;
	if (1194 != 1194) {
		int mkkd;
		for (mkkd = 75; mkkd > 0; mkkd--) {
			continue;
		}
	}
	if (1194 == 1194) {
		int tuzato;
		for (tuzato = 86; tuzato > 0; tuzato--) {
			continue;
		}
	}
	return false;
}

int wrxuyrz::mlafupvpbakuxcfawrsqewpxf() {
	return 31032;
}

double wrxuyrz::xsnddumqwwewvva(double fdgkhjttgwch, string lwzxmdwdhfldi, bool lfbarsyku, bool ffvuuj, double qrkaqaxxue) {
	string pjxda = "meoqgpjkzlocajgouscmoveluizyjvznmgmuyzqmutdgwnkccolkjnedsrmoyxgrbfqtwfaqzzlmszwhrmgiinvjuxknsfk";
	double bpbuxj = 21199;
	string zafzkmspcrv = "kcuzjqsqwrdaayaogfxkhdvimbypmgrkrcrooxsszibqwwqgnfxsojgs";
	int jfisgwgwcjnzn = 2124;
	bool ztvxvnxho = true;
	double ptndcysb = 22420;
	string tokydwigrfqddh = "jixhcffyztulyoseqygnjuyqjaqldyxtzhpzdynpiv";
	double soeftda = 1359;
	double wsoiashjbwhlfo = 47058;
	return 93308;
}

void wrxuyrz::chvxtvpzjsg(string iwqkof, int pcmetknfmcvdrb, int yysyoijtaw, bool ycwwvwqxcmgc, int komtviwqvkxhi, bool qruytewgg, double mjvhaggjekjtpjb, bool powzlkyrrytujax) {
	string ruendek = "sozvpgmmwlidhmyrjrpnpkbrccxyttzhhdmlnkrcndfadgxsag";
	double olhiixbedb = 8319;
	bool zkioqfskxtiwud = false;
	double xgvukubhsj = 7418;
	double odhcimcpjpiem = 87988;
	int lybjpxcebxuepqd = 6605;
	int hekmdhjrpf = 6307;
	string nnlwqvu = "jheymmnxdpqmizolzzlvcextpyswwrehacdflnvjkqtoraup";
	string jtejci = "viyjswrnajedqtqdloenwhahexahfqxoeylz";
	int quqdadnhbspbr = 2750;

}

string wrxuyrz::vttwwiljdzvnbmbh(double pnvmqaedczrj, int oqweo, bool qhixjezsvuelxzq, string nlkercfadmqd, int otkzkkd, int apljj, double poowatkkldgw, bool cdnexekqavqz, double drdvrf) {
	int hjqoe = 2546;
	double ewlayjnagio = 21451;
	bool ornpfncrjhfro = false;
	int miytywfc = 3975;
	if (3975 != 3975) {
		int ipxzfcjaj;
		for (ipxzfcjaj = 0; ipxzfcjaj > 0; ipxzfcjaj--) {
			continue;
		}
	}
	if (3975 == 3975) {
		int lrbsryo;
		for (lrbsryo = 38; lrbsryo > 0; lrbsryo--) {
			continue;
		}
	}
	if (21451 != 21451) {
		int twpl;
		for (twpl = 31; twpl > 0; twpl--) {
			continue;
		}
	}
	if (21451 != 21451) {
		int qqoiszzyev;
		for (qqoiszzyev = 86; qqoiszzyev > 0; qqoiszzyev--) {
			continue;
		}
	}
	return string("dz");
}

string wrxuyrz::elhtijmlzwggphvgtrcvt(string sgfjdrviztthn, string qzeswhvcfjvn, bool lntvjnhs, string xharypzmjxoo) {
	int weewdqpwecqsnx = 1139;
	bool rdsxmudbhsftauo = false;
	string otdzat = "suvrgaqpannwcxuoynvyantdqudgglfdgrnuigawdrddyndxjamdvvromtxzgwsjdhoeqtmtronlgrtxqhutdjncktt";
	double edgzidhvzmz = 66307;
	double unickjbqhzccua = 18100;
	double ohpgxkupvougit = 9765;
	double pknoddomffxnc = 31049;
	string hiwusg = "msyiyqxccui";
	bool xbqybntejkljprc = false;
	if (false != false) {
		int gctqmucl;
		for (gctqmucl = 13; gctqmucl > 0; gctqmucl--) {
			continue;
		}
	}
	return string("tagtwhmce");
}

int wrxuyrz::qwgyvkjuusejrbne(double yvrkrk, bool yirojgjlrvzlw, bool ggekxqdn, bool hpnjvhqsqcuwl, bool cbhubgpoeoxpv, bool rsftge, string dhshonyxyyzd, string ophdwnhah, int gsbrtonzu) {
	int hrusqgvbgsp = 3826;
	string jboktnc = "qgmmxsgcediujkzokeqwhcpykueqyonmlmrfllijxqpxqsnsuvwkofofzbipljchsjzjpyhhfnhrtttvxfapflruipjamfyzwj";
	double kaxqauvehews = 17210;
	int bufswaehhxqlnsh = 2110;
	if (2110 == 2110) {
		int wwlcttx;
		for (wwlcttx = 2; wwlcttx > 0; wwlcttx--) {
			continue;
		}
	}
	if (17210 == 17210) {
		int vzn;
		for (vzn = 62; vzn > 0; vzn--) {
			continue;
		}
	}
	return 39466;
}

int wrxuyrz::wfqitvaksstvyqekrigpmf(double tdjmgol, bool gsqeguueozkm, double tvcnbfhzzdscdq, string ykiiyelyjn, double tobqmxafoo, double tbmrszbkcwsf) {
	bool vmykquvnhkk = false;
	int jpnsk = 2396;
	bool oxraojgyxpkmd = true;
	int hfxwuhgfx = 1210;
	if (2396 == 2396) {
		int in;
		for (in = 100; in > 0; in--) {
			continue;
		}
	}
	if (true == true) {
		int lbkdfph;
		for (lbkdfph = 28; lbkdfph > 0; lbkdfph--) {
			continue;
		}
	}
	if (true != true) {
		int xlprhwkqkf;
		for (xlprhwkqkf = 44; xlprhwkqkf > 0; xlprhwkqkf--) {
			continue;
		}
	}
	if (1210 != 1210) {
		int rppcccbwz;
		for (rppcccbwz = 99; rppcccbwz > 0; rppcccbwz--) {
			continue;
		}
	}
	if (false != false) {
		int mvoeb;
		for (mvoeb = 80; mvoeb > 0; mvoeb--) {
			continue;
		}
	}
	return 49462;
}

int wrxuyrz::uoausqoaeyzvhv(int zqfusfxcluseu) {
	string lxjim = "utuejjsunphldslqyrtdluhxbfsujcjeokhxarwekrzjeqnwhmvwmlgnsjjfayxwzymuvhge";
	double clofjroy = 24088;
	string grubtyij = "dn";
	string oldgdzqhdsxvklh = "yxbgojigklcxpmawoxeftggtykcanuybuvnfmmtmhjcpvnzusgbequpzwuhflwdlxdlkgxfvjumbigbbuoiloruspchzb";
	double exuukbevwgcvj = 2819;
	if (2819 == 2819) {
		int erjq;
		for (erjq = 48; erjq > 0; erjq--) {
			continue;
		}
	}
	if (string("dn") == string("dn")) {
		int bwafageaeo;
		for (bwafageaeo = 44; bwafageaeo > 0; bwafageaeo--) {
			continue;
		}
	}
	return 59429;
}

void wrxuyrz::zaszwmspwjmervevhrcijiww(double sulfwfxybkvxb, int kgtfgne, double oojucqevykleq) {
	int mlddfkfxjzojan = 22;
	double cknlfv = 40667;
	bool lawbahgxgnsh = false;
	double owetk = 20784;
	string bovbimuhupphk = "aiasmzwpyavbqbxtatfclovzdrxpbunjuzwqasrwvsboaasvqmabh";
	bool pinffsqmjgrbyw = true;
	string fppdfpkqxi = "gizwnsuadocxyrtzymenrbulavghhiivk";
	if (true != true) {
		int kzjhyjac;
		for (kzjhyjac = 98; kzjhyjac > 0; kzjhyjac--) {
			continue;
		}
	}
	if (string("aiasmzwpyavbqbxtatfclovzdrxpbunjuzwqasrwvsboaasvqmabh") != string("aiasmzwpyavbqbxtatfclovzdrxpbunjuzwqasrwvsboaasvqmabh")) {
		int ilq;
		for (ilq = 41; ilq > 0; ilq--) {
			continue;
		}
	}
	if (40667 == 40667) {
		int sevwavldp;
		for (sevwavldp = 81; sevwavldp > 0; sevwavldp--) {
			continue;
		}
	}
	if (false == false) {
		int dkii;
		for (dkii = 6; dkii > 0; dkii--) {
			continue;
		}
	}
	if (40667 == 40667) {
		int yaew;
		for (yaew = 98; yaew > 0; yaew--) {
			continue;
		}
	}

}

wrxuyrz::wrxuyrz() {
	this->chvxtvpzjsg(string("iuhottbaqnxllndauqqfymwkfwuggegxbgydrjrvompmgcvutuqegzgjkahfrpzfnodvqjtcjchhogcmducystnfxffmnr"), 1512, 1876, true, 933, false, 34210, false);
	this->vttwwiljdzvnbmbh(39158, 997, false, string("gnbbtagcftszmcmfqzapkpnr"), 4036, 528, 18311, false, 35997);
	this->elhtijmlzwggphvgtrcvt(string("yapoiubyewiwscpvryzpay"), string("mcxqnnt"), false, string("ovegxqxbcwtfsrsatdodsdqjrwdsqudnxgttwtacddjssszeqixzctoiupasyipxntenrvtzbpkhhsjssbrpavdpnpukrf"));
	this->qwgyvkjuusejrbne(4059, false, true, true, true, true, string("egqouxbdov"), string("llajrifnzpavkxbdynstnwbjfutrvuoeynzwnrjbwhzciegphqfidkojmfgiamwixorgs"), 2106);
	this->wfqitvaksstvyqekrigpmf(7278, false, 29200, string("eplyxmczoyelkqybedfxbxkwidzxqvmjjpwznqqceopipewtjspenkdnxrppzfyjxhgilnrcrbuvqvgymxxu"), 34668, 9529);
	this->uoausqoaeyzvhv(5659);
	this->zaszwmspwjmervevhrcijiww(15376, 1012, 58378);
	this->mlafupvpbakuxcfawrsqewpxf();
	this->xsnddumqwwewvva(16295, string("skairjtjnaificyooubhfmzpiqkyjowuckwrgqgsqfdwuaggdahhnuvxlfqjxwxymnuvgefrtzsxhasecoomubicxlklnqfi"), true, false, 25728);
	this->gicfwjskbecombagm();
	this->hguzblpwwbadcujuabhrn(string("sfxurbsavgyzmscdcjffllznpeuxxyzqupoiqnyyqxztrlvhaci"), 84, true, true, string("royifgicrceexxlysrexbkhhcigmlpcfhskkio"), 6799, 10023);
	this->bmsqulaluewbtxo();
	this->hzjysqtisdhfyd(5362, 39554);
	this->rfcpfiuhwnlokbvqzihwggtoh(false);
	this->mrgjjmyntmgwtg(string("wynfwsojvheilubenjgzxljctrhmzbsejcaz"));
	this->tgyynwefhgtfncjaznjuouf(1006, 3087, 3680, 5117, string("sngnycetxebansmdrskowsiokgevnaubxnddatqqzoxkxozyesmavocthsjpwcll"));
	this->ihojmcpfmbmbffhhvossgt(true, 55477, 572, 1574, 4031, 5413, 590, string("rhgipks"), 7026);
	this->zxuytuxpemnfwxurix(false, 13217, 33873, string("uyoisodxgygologmgwdoliuoxnthtfcqbqthnzwvlafcrenphnnxdefxsidgqjjrjdzmhnmoklpiobptcxajfsdi"), 5042);
	this->bizoyvcvbjvhty(903, 3586, 5132, 12484);
}
