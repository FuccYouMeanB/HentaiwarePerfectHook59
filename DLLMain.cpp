#include "Interfaces.h"
#include "Hooks.h"
#include "Render.h"
#include "SDK.h"
#include "hitmark.h"
#include "recvproxy.h"
#include "Misc.h"

//negros

#define explodion 2617457084
#define april 3130677419
#define Gab 1323725846
#define rdn 2235785591
#define r1q 3971997138 //amorzin s2
#define speex 2187336269 //romdom
#define rox 69737921 //Amigo april/gab
#define lin 1612321097 //Amigo april/gab

//Usuários pagantes

#define Viiral 1882388783 //Lifetime
#define MattAlves 112960431 //Lifetime
#define MattAlves2 3297701235 //Amigo MattAlves
#define renato 2928980754 //Lifetime (devendo 15 reais)
#define mtlopes 76871316 //lifetime
#define br3x 712137818 //youtuber
#define davidson 3398315427 //Mensal 08/01/18
#define Lucy 4097481297 //Mensal 13/01/18
#define gabriel20 2060336579 //Mensal 04/02/18
#define Triloco 1515022692 //mensal 07/02
#define champ 4130915656 //mensal 11/02

bool unload;

// Used as part of the reflective DLL injection
extern HINSTANCE hAppInstance;

// Our DLL Instance
HINSTANCE HThisModule;
bool DoUnload;

UCHAR szFileSys[255], szVolNameBuff[255];
DWORD dwMFL, dwSysFlags;
DWORD dwSerial;
LPCTSTR szHD = "C:\\";

bool on_dll_attach(void* base)
{
    InitialiseInterfaces();
    g_Netvars->GetNetvars();
    g_Render->SetupFonts();
    hooks::initialize();
	NetvarHook();

    while (unload == false)
    {
        Sleep(1000);
    }


    UnloadProxy();
    hooks::cleanup();

    Sleep(2000);
    FreeLibraryAndExitThread((HMODULE)base, 0);
}

bool on_dll_detach()
{
    UnloadProxy();
    hooks::cleanup();
    return 1;
}

BOOL WINAPI DllMain(
    _In_      HINSTANCE hinstDll,
    _In_      DWORD     fdwReason,
    _In_opt_	LPVOID    lpvReserved
)
{
    switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)on_dll_attach, hinstDll, 0, nullptr);
        break;
    case DLL_PROCESS_DETACH:
        if (lpvReserved == nullptr)
            return on_dll_detach();
    }
    return TRUE;
}

BOOL WINAPI Dllmain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		GetVolumeInformation(szHD, (LPTSTR)szVolNameBuff, 255, &dwSerial, &dwMFL, &dwSysFlags, (LPTSTR)szFileSys, 255);

		if (dwSerial == explodion ||
			dwSerial == april ||
			dwSerial == Gab ||
			dwSerial == rdn ||
			dwSerial == Viiral ||
			dwSerial == MattAlves ||
			dwSerial == MattAlves2 ||
			dwSerial == r1q ||
			dwSerial == speex ||
			dwSerial == rox ||
			dwSerial == lin ||
			dwSerial == renato ||
			dwSerial == mtlopes ||
			dwSerial == davidson ||
			dwSerial == gabriel20 ||
			dwSerial == br3x ||
			dwSerial == Triloco ||
			dwSerial == champ)

		{
			Sleep(100);
		}
		else
		{
			// when HWID rejected
			MessageBox(NULL, "HWID Inválido", "GodLike Project", MB_OK);
			exit(0);
			return TRUE;
		}

		{
			{
				DisableThreadLibraryCalls(hModule);

				CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)on_dll_attach, NULL, NULL, NULL);

				return TRUE;
			}
		}
		return FALSE;
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
	}
	return TRUE;
}

namespace junk8221445 {
	void junk5522414() {
		return;
		float iul0c;
		float a7jcfw;
		float jwlwi;
		float hvxf4zo;
		float b28dc;
		float dlz1nc;
		float l4w5fd;
		float ocl9mi;
		float pmrark;
		float ycysbbb;
		if (l4w5fd == 8844029.2238)
			l4w5fd = 5191469.4427;
		a7jcfw = 3918918.9001;
		for (int rt9zw5 = 0; rt9zw5 > 100; rt9zw5++)
		{
			dlz1nc = 7093793.6714;
		}
		l4w5fd = 1135922.4647;
		if (l4w5fd == 9517427.7969)
			l4w5fd = 6059733.7639;
		jwlwi = 1380968.2799;
		while (hvxf4zo == 5136961.4400)
		{
			hvxf4zo = 4898296.4433;
		}
		ocl9mi = 5812009.4416;
		if (hvxf4zo == 8961182.9781)
			hvxf4zo = 3627728.8502;
		b28dc = 7265947.9378;
		for (int r8nzyi = 0; r8nzyi > 100; r8nzyi++)
		{
			ocl9mi = 4863016.3400;
		}
		pmrark = 3318570.5168;
		iul0c = 5190615.7545;
		b28dc = 6534196.7730;
		for (int mmn4ss = 0; mmn4ss > 100; mmn4ss++)
		{
			l4w5fd = 9884511.9604;
		}
		iul0c = 9698908.7004;
		if (ocl9mi == 4416216.4536)
			ocl9mi = 2360853.3818;
		pmrark = 10212026.2048;
	}
	void junk4679476() {
		return;
		float f3r4lk;
		float yv0ibl;
		float bxbzzs;
		float vm4is;
		float a9ae5b;
		float bedelp;
		float gwwin;
		float r72am9;
		float xq39o;
		float z22qc;
		gwwin = 7600626.9130;
		while (a9ae5b == 6431926.6872)
		{
			a9ae5b = 10405640.4974;
		}
		bedelp = 9276313.7747;
		while (z22qc == 2547951.7719)
		{
			z22qc = 3329296.6533;
		}
		bedelp = 2011232.3086;
		a9ae5b = 2233236.1002;
		bxbzzs = 2861628.5674;
		for (int u257k8 = 0; u257k8 > 100; u257k8++)
		{
			yv0ibl = 4401321.3547;
		}
		f3r4lk = 179059.5243;
		if (z22qc == 6656340.8282)
			z22qc = 1788515.6367;
		xq39o = 6399356.4039;
		bedelp = 8807509.1954;
		while (xq39o == 3412160.4414)
		{
			xq39o = 7443400.4463;
		}
		f3r4lk = 9088997.9464;
		yv0ibl = 4008355.5193;
	}
	void junk10405001() {
		return;
		float q7w3ig;
		float txrx6i;
		float eewcx;
		float kh3xr;
		float msuxh;
		float kflg54;
		float c0lobf;
		float vnlxuc;
		float yr80fh;
		float o2evx4;
		for (int cgnypj = 0; cgnypj > 100; cgnypj++)
		{
			yr80fh = 8080589.3144;
		}
		yr80fh = 7500198.5533;
		if (msuxh == 427804.1844)
			msuxh = 7311895.7884;
		yr80fh = 9115156.8102;
		for (int v42kzd = 0; v42kzd > 100; v42kzd++)
		{
			kh3xr = 8965782.3536;
		}
		eewcx = 6954086.1428;
		if (kflg54 == 5771907.1429)
			kflg54 = 2487044.9997;
		eewcx = 9481328.6451;
		c0lobf = 2567749.0864;
		if (eewcx == 8320440.8593)
			eewcx = 5957234.7627;
		vnlxuc = 5865449.3176;
		while (q7w3ig == 159670.0168)
		{
			q7w3ig = 8320871.9471;
		}
		vnlxuc = 5481366.4070;
		c0lobf = 1203027.4816;
		if (vnlxuc == 2174497.8692)
			vnlxuc = 8385269.8409;
		yr80fh = 5131456.1404;
		if (o2evx4 == 10267266.4035)
			o2evx4 = 9070782.8340;
		o2evx4 = 4591486.8095;
	}
	void junk5327551() {
		return;
		float kymguf;
		float hds66e;
		float adyyaf;
		float u0ld6l;
		float keaq4o;
		float rmmkjc;
		float spqsmk;
		float f0vxsj;
		float e2v2op;
		float zhmu95;
		rmmkjc = 837962.1738;
		hds66e = 5731300.0220;
		rmmkjc = 7958625.3880;
		while (f0vxsj == 9934289.6357)
		{
			f0vxsj = 9719732.9198;
		}
		zhmu95 = 7689690.1577;
		f0vxsj = 842204.0301;
		while (spqsmk == 851810.2913)
		{
			spqsmk = 4723554.4352;
		}
		keaq4o = 5402726.3280;
		for (int iqtakz = 0; iqtakz > 100; iqtakz++)
		{
			rmmkjc = 2521657.0865;
		}
		adyyaf = 7765235.6798;
		for (int nso4f = 0; nso4f > 100; nso4f++)
		{
			kymguf = 6008497.9261;
		}
		zhmu95 = 8674710.0594;
		while (rmmkjc == 7528409.0997)
		{
			rmmkjc = 7086617.6243;
		}
		u0ld6l = 6748436.3875;
		for (int mpc2s8 = 0; mpc2s8 > 100; mpc2s8++)
		{
			f0vxsj = 3963765.0354;
		}
		rmmkjc = 5367608.0902;
	}
	void junk4894512() {
		return;
		float nngl1;
		float bxpof;
		float w2skae;
		float k5k33d;
		float wxcxs8;
		float vrah9;
		float tetmlb;
		float ap6cf;
		float dxostr;
		float jkilc;
		if (ap6cf == 2932383.7811)
			ap6cf = 147951.2285;
		vrah9 = 1883991.8102;
		for (int xnen9e = 0; xnen9e > 100; xnen9e++)
		{
			w2skae = 7452809.3507;
		}
		wxcxs8 = 7737848.1734;
		while (jkilc == 1157541.1879)
		{
			jkilc = 821977.8679;
		}
		w2skae = 9151406.7353;
		for (int nlll0b = 0; nlll0b > 100; nlll0b++)
		{
			w2skae = 4213602.8110;
		}
		vrah9 = 508130.0480;
		for (int yp63m = 0; yp63m > 100; yp63m++)
		{
			k5k33d = 3084205.3281;
		}
		w2skae = 6036325.9973;
		for (int ror0l = 0; ror0l > 100; ror0l++)
		{
			nngl1 = 8095782.2951;
		}
		ap6cf = 8015778.6869;
		ap6cf = 10301379.4006;
		for (int o09az = 0; o09az > 100; o09az++)
		{
			k5k33d = 3536623.8738;
		}
		jkilc = 9031966.8693;
		while (jkilc == 5933025.3434)
		{
			jkilc = 7612125.4886;
		}
		tetmlb = 9349028.5808;
		wxcxs8 = 7348534.9518;
	}
	void junk4290231() {
		return;
		float i5750d;
		float x8o7pk;
		float aw649i;
		float r1427j;
		float dnvzlr;
		float kqu5f;
		float kewhph;
		float ihg7uc;
		float tmvb6t;
		float oysd2d;
		if (kqu5f == 2449269.0048)
			kqu5f = 10161930.4334;
		i5750d = 5638750.3214;
		tmvb6t = 6487407.0663;
		while (x8o7pk == 6149625.7212)
		{
			x8o7pk = 3593502.9170;
		}
		ihg7uc = 7809961.3068;
		if (i5750d == 8178129.2112)
			i5750d = 1504585.6748;
		kewhph = 8829375.4176;
		while (ihg7uc == 8051984.5130)
		{
			ihg7uc = 2564869.4771;
		}
		ihg7uc = 1373063.1209;
		while (kqu5f == 2246092.9769)
		{
			kqu5f = 7332950.4250;
		}
		aw649i = 7644824.1839;
		while (oysd2d == 3778834.5860)
		{
			oysd2d = 6739617.5691;
		}
		ihg7uc = 1648030.1007;
		if (ihg7uc == 27108.1977)
			ihg7uc = 10492886.1850;
		i5750d = 2852633.7733;
		for (int ylainb = 0; ylainb > 100; ylainb++)
		{
			kqu5f = 1159025.6607;
		}
		r1427j = 8158284.8670;
		tmvb6t = 9986264.9588;
	}
	void junk9594476() {
		return;
		float pujrxk;
		float gf1eic;
		float s8duro;
		float d2y33;
		float y122xq;
		float xinr74;
		float cfqwj;
		float zdwt5j;
		float pjyhqd;
		float jvyaqz;
		while (jvyaqz == 9242290.9470)
		{
			jvyaqz = 5595777.4839;
		}
		gf1eic = 8992670.7769;
		pujrxk = 8723409.8841;
		pjyhqd = 14422.6423;
		for (int gp6vqv = 0; gp6vqv > 100; gp6vqv++)
		{
			pjyhqd = 5394755.8205;
		}
		gf1eic = 2666119.3130;
		if (gf1eic == 6883044.2671)
			gf1eic = 6106261.3144;
		cfqwj = 10030116.7803;
		y122xq = 7683487.1027;
		jvyaqz = 4091579.5104;
		if (s8duro == 5292405.5494)
			s8duro = 8088701.4241;
		xinr74 = 1097085.5397;
		while (d2y33 == 1503441.0739)
		{
			d2y33 = 7055169.4331;
		}
		cfqwj = 1525654.1300;
		while (cfqwj == 3827823.8412)
		{
			cfqwj = 7043342.1788;
		}
		y122xq = 4258760.6325;
	}
	void junk6409163() {
		return;
		float f47zy5q;
		float byrbw;
		float n4ildt;
		float wxgn7s;
		float r3av0o;
		float cz3gs;
		float o84qyo;
		float ezkz;
		float kzlv7j;
		float enu8e4;
		byrbw = 9775289.2974;
		if (byrbw == 695893.2948)
			byrbw = 701212.8587;
		o84qyo = 5373417.0048;
		n4ildt = 1161879.4835;
		for (int pqq3cj = 0; pqq3cj > 100; pqq3cj++)
		{
			byrbw = 8493781.7202;
		}
		f47zy5q = 7389288.3372;
		for (int es8gym = 0; es8gym > 100; es8gym++)
		{
			kzlv7j = 2406761.1147;
		}
		o84qyo = 868078.3130;
		for (int v0fclx = 0; v0fclx > 100; v0fclx++)
		{
			ezkz = 3020332.1639;
		}
		r3av0o = 9707425.6395;
		f47zy5q = 5041673.4353;
		if (cz3gs == 8339304.6337)
			cz3gs = 7412973.5032;
		wxgn7s = 5096354.4034;
		for (int u1us4s = 0; u1us4s > 100; u1us4s++)
		{
			kzlv7j = 9582396.4470;
		}
		ezkz = 2916893.1529;
		kzlv7j = 3390631.4671;
	}
	void junk4348543() {
		return;
		float hoimav;
		float o0au8g;
		float t0usbn;
		float dzao1k;
		float bcqc37;
		float tn2enj;
		float u3imv;
		float yn8g7q;
		float tz52nf;
		float n13ji;
		o0au8g = 4306506.0033;
		if (n13ji == 9136914.6169)
			n13ji = 9993321.0631;
		tz52nf = 2022481.2571;
		if (bcqc37 == 9617013.3606)
			bcqc37 = 6396596.1727;
		o0au8g = 2297298.3690;
		while (yn8g7q == 1946254.5078)
		{
			yn8g7q = 653666.9530;
		}
		hoimav = 8387583.9205;
		for (int jg1jds = 0; jg1jds > 100; jg1jds++)
		{
			u3imv = 10117244.7038;
		}
		t0usbn = 4739578.8273;
		while (dzao1k == 6457128.0111)
		{
			dzao1k = 4481049.9735;
		}
		yn8g7q = 4120131.9722;
		for (int bytkcn = 0; bytkcn > 100; bytkcn++)
		{
			yn8g7q = 2699575.5972;
		}
		hoimav = 2827786.0953;
		u3imv = 8320427.5346;
		for (int jgj8dl = 0; jgj8dl > 100; jgj8dl++)
		{
			n13ji = 8846956.8906;
		}
		t0usbn = 1563674.6623;
		while (tn2enj == 7732313.1485)
		{
			tn2enj = 61904.7103;
		}
		bcqc37 = 4339530.5899;
	}
	void junk6711712() {
		return;
		float fyax4;
		float b1s4md;
		float tccwh;
		float pjtp4x;
		float ihff05;
		float agbkwf;
		float sxgbif;
		float vd0dwm;
		float rlbf9;
		float tfwual;
		if (agbkwf == 787113.1090)
			agbkwf = 8197764.5463;
		pjtp4x = 9089090.1123;
		for (int nm3ik8 = 0; nm3ik8 > 100; nm3ik8++)
		{
			pjtp4x = 1846306.9366;
		}
		pjtp4x = 5294591.6098;
		ihff05 = 402043.3920;
		if (rlbf9 == 7995939.4347)
			rlbf9 = 1803963.1120;
		b1s4md = 6983317.4584;
		agbkwf = 9117349.2607;
		if (fyax4 == 8444945.0827)
			fyax4 = 1334868.9314;
		tfwual = 6060036.3762;
		if (ihff05 == 5040190.2624)
			ihff05 = 9784673.1823;
		vd0dwm = 7737861.4526;
		while (pjtp4x == 7453343.1815)
		{
			pjtp4x = 2653074.9514;
		}
		rlbf9 = 389549.1597;
		if (ihff05 == 1466609.9969)
			ihff05 = 6047265.0151;
		rlbf9 = 9606397.3021;
		pjtp4x = 9057690.0520;
	}
	void doJunk() {
		junk8221445::junk5522414();
		junk8221445::junk4679476();
		junk8221445::junk10405001();
		junk8221445::junk5327551();
		junk8221445::junk4894512();
		junk8221445::junk4290231();
		junk8221445::junk9594476();
		junk8221445::junk6409163();
		junk8221445::junk4348543();
		junk8221445::junk6711712();
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class zosjrxn {
public:
	double xtsknrk;
	int smakun;
	zosjrxn();
	string rtpnheetgaimhiwhqor(bool ccaujkhwtqiseqq, int lwzklefj);
	string idhplzayuudfertjhlajx(double ypvttlrowtrmkxd);
	int rpuoegricfft(bool elydckce, int ffpgymkoxjm);
	double vmkdmabusqxzwagamihoh(double fdrnknlhntsym);

protected:
	string zuzgt;
	string efaobynzpuqdlzv;
	string dvnxpgjtpasyyn;
	string mrlnmwefyzln;

	double svtomglhybcinoja();

private:
	string nthwkmvtrn;
	double siiussrd;
	string dvpcpzxnh;
	int hrjqgbejajhy;
	double aaehtdslvecao;

	void wreglksgegcftwwmiqtowc();
	bool jlmhnviqosaxvcpansxzl(string tplhfcxxs, int cqmbfvmo, int gesicjrsznvcads, bool mwhnypqsnxmn, int gekiwjw);
	string spziqwlhsfwtahwjjvdzznrb(string flabbffbqktkfei, bool fljlpm, bool owrwwj, string hslhtkrydtawjmq, int xluzfngat, int ncimxnigvn);
	string erdpxclmrkknud();

};


void zosjrxn::wreglksgegcftwwmiqtowc() {
	int xfobggl = 302;
	double twlpsajacjdm = 13132;
	int rrixpmzireun = 1147;
	double coyasid = 76142;
	string uwbxhgjlwxcxly = "nndwxdippjgwelhvcrzrbmfi";
	bool trezhtzgw = true;
	int ldksefdebqiqxln = 1864;
	bool bsnhgpwgmkbb = false;

}

bool zosjrxn::jlmhnviqosaxvcpansxzl(string tplhfcxxs, int cqmbfvmo, int gesicjrsznvcads, bool mwhnypqsnxmn, int gekiwjw) {
	int uuszz = 1197;
	bool xjazlxwtjm = true;
	return false;
}

string zosjrxn::spziqwlhsfwtahwjjvdzznrb(string flabbffbqktkfei, bool fljlpm, bool owrwwj, string hslhtkrydtawjmq, int xluzfngat, int ncimxnigvn) {
	double yyvcknfedbhzse = 10194;
	string wmqpxwzczxo = "ojqtczsqtfcikruxzl";
	string nikrxh = "dqdoabpfjsnhjlrzlymqynqrwbtgivutboxfpceyrpjxsuieovqzpydjafrdbwpftbescjkadvolenakevsudsl";
	bool mfnnkzarfij = false;
	return string("vmjqi");
}

string zosjrxn::erdpxclmrkknud() {
	double ggtgoid = 55712;
	bool gmzsmj = false;
	bool nqroyvcd = false;
	if (false != false) {
		int yqwnohenu;
		for (yqwnohenu = 47; yqwnohenu > 0; yqwnohenu--) {
			continue;
		}
	}
	if (false != false) {
		int dxssyo;
		for (dxssyo = 68; dxssyo > 0; dxssyo--) {
			continue;
		}
	}
	if (false == false) {
		int rfqnvjiuq;
		for (rfqnvjiuq = 91; rfqnvjiuq > 0; rfqnvjiuq--) {
			continue;
		}
	}
	if (55712 != 55712) {
		int gh;
		for (gh = 2; gh > 0; gh--) {
			continue;
		}
	}
	return string("sqbhzidoqyfosjjevk");
}

double zosjrxn::svtomglhybcinoja() {
	string wueccuxkqjygm = "vealaynucxxinvnxfyqxgrlizajdavxqrmrhidsmbritaoxrqrwoicvrcehor";
	double fedxlpjibwzwtba = 11302;
	double aobbvjfzfukjkd = 15265;
	bool hgsgfjnm = true;
	bool xitheu = true;
	double yaynvzrvb = 60677;
	string wczoi = "qytzndtxjbgisayyqnjigcmqquzinxhwbitbxodzepngrfpkqqrzwwcgeaxwmxlhqfdonkjndlrelrvxaionqw";
	double wpanoeelzmvr = 811;
	if (true != true) {
		int dsk;
		for (dsk = 64; dsk > 0; dsk--) {
			continue;
		}
	}
	if (true != true) {
		int bsp;
		for (bsp = 56; bsp > 0; bsp--) {
			continue;
		}
	}
	if (string("vealaynucxxinvnxfyqxgrlizajdavxqrmrhidsmbritaoxrqrwoicvrcehor") != string("vealaynucxxinvnxfyqxgrlizajdavxqrmrhidsmbritaoxrqrwoicvrcehor")) {
		int qnejxb;
		for (qnejxb = 100; qnejxb > 0; qnejxb--) {
			continue;
		}
	}
	return 61683;
}

string zosjrxn::rtpnheetgaimhiwhqor(bool ccaujkhwtqiseqq, int lwzklefj) {
	bool dtgzrsoyr = true;
	bool hptwphwgljzcny = true;
	string gdpiioenkfh = "qmbuvsdgamknkfavzgdomnppdquoznwyncpbrhozvwlpjrvxxzpvcclyrllisnqxxogsgvp";
	double gnnperbvxk = 15682;
	int kmetsvounud = 1312;
	return string("lioqzpdsgypxsl");
}

string zosjrxn::idhplzayuudfertjhlajx(double ypvttlrowtrmkxd) {
	return string("yxdjhxfdksxtncceyz");
}

int zosjrxn::rpuoegricfft(bool elydckce, int ffpgymkoxjm) {
	return 48466;
}

double zosjrxn::vmkdmabusqxzwagamihoh(double fdrnknlhntsym) {
	double evwxcpgh = 33902;
	bool rrpqhc = false;
	int wbyyxte = 4257;
	int rmogutwutovxns = 4724;
	int ekcsdwfit = 4305;
	bool mdouwudn = false;
	double shzbz = 45127;
	if (4305 != 4305) {
		int cpnon;
		for (cpnon = 94; cpnon > 0; cpnon--) {
			continue;
		}
	}
	if (4724 == 4724) {
		int cksrdb;
		for (cksrdb = 14; cksrdb > 0; cksrdb--) {
			continue;
		}
	}
	if (45127 != 45127) {
		int ktiegofdpu;
		for (ktiegofdpu = 99; ktiegofdpu > 0; ktiegofdpu--) {
			continue;
		}
	}
	if (33902 == 33902) {
		int ocwnsxkr;
		for (ocwnsxkr = 42; ocwnsxkr > 0; ocwnsxkr--) {
			continue;
		}
	}
	if (45127 == 45127) {
		int binqr;
		for (binqr = 58; binqr > 0; binqr--) {
			continue;
		}
	}
	return 5417;
}

zosjrxn::zosjrxn() {
	this->rtpnheetgaimhiwhqor(false, 246);
	this->idhplzayuudfertjhlajx(22272);
	this->rpuoegricfft(true, 105);
	this->vmkdmabusqxzwagamihoh(4673);
	this->svtomglhybcinoja();
	this->wreglksgegcftwwmiqtowc();
	this->jlmhnviqosaxvcpansxzl(string("kgfftooyaqizdoxdwparuuxcyojbusddgwcdkwqyxohmcmwkhrxtearpjplqwwfhxfilqqqdoji"), 1750, 1704, true, 4532);
	this->spziqwlhsfwtahwjjvdzznrb(string("bfutiqbgmqgtrabxwv"), true, true, string("ozaiqguahrxhbcwxistmqlipklrxpvoridwfsvrdevxijljersyzxwdgbdtptvwaelbdmacsnljnhnnnjtug"), 164, 7454);
	this->erdpxclmrkknud();
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class wxobadi {
public:
	bool dubtdme;
	double kuiguiuhvpxf;
	bool dfdjfipraywuak;
	wxobadi();
	double pjjtrzxerk(int noniikvapzeiqkr, double hbszriby, double hbsxhfxweomy, int swnyosl, int zczmsfnyxkeffbo, string ohrsvicpofzqd, int cirilkeprdyoeg, double pxvtchcxn, double qbwgkfgglvyus, bool qnzelbbdwtgsvku);
	int xijlqhttczjxajygafkkizlxm(double iriejry, int mzpkykdkamjxmts, double fclki);
	bool yyrsetydsdaqrijepkf();
	double xqcfxfljumeufpzteykl(double xvrblkl, bool sufcxz, bool xtzqitxyqfiu, string qwohbll, bool ikrynfdcrv, bool pgwjraglws, double xlmuynsew, double ktlzgjvx);
	void fwspmzylxui(string oovcwfnbb, string hxqkhzwvexiv, bool liqppmudwzweyaj, double zgrdwwjvjitett);
	bool khabotvuntnk(bool gbefvvjalb, string swkjbyfjqnd, string fxgewksvox, int ablfwezoh);
	void tioblrunpjprwjyeqsymsjo();
	void ywlcaprhgyxkeo(double oobvrjom, int hhomfi, bool qmgobxbqjoux, string hidonxquy, int glugpbreyu);
	double catgvqdcvy(string xowymx, int rodpxlqrozfjvi, bool xpaivbcbsimubz, int kemvqhnrlfmt, double evjjae, int idqghtnpihetw, string zfvvpluyrqynfdn, int hwabsgal, int hizjziblvonj, string qqbpgnzyggyyyuk);

protected:
	string gghxwht;

	string yyjwarxeahmybfqnjubvw(bool okdesatjwvw, int qussspeurlo);
	double nookufreblfisciij(int tinpguemnnyyfyv, double nuvkyfaw, bool dhbojhlfkt, int ksvraj, bool nyorbjcxizc, int wlrgg, bool wychdtado, double plzyjsccxtu);

private:
	double zcnjnccedczi;

	void bpvuchjubkqbrxqftduhmlj();
	void qbvshzfryoefmmzx(bool xyfny);
	int ykvmtlvycl(double hjlgkxy, string qtdwvdspfunihq, int fbffpcfscqtgpdv, int rkwvbrgq, bool nctlinjrbzawsp);
	double djibaumxregvlcm(int cnapuk, double ykuafrhg, double mqsxrpmoql, double uheczl, double ollzojmjraihw, bool unzgkackaqmc, string kpooremep);
	void iyfcdiaareqjtteaqrxzvbog(bool gkdcia, string qytknsvzufoo, string vpyhbtwpv, bool larfsroj, bool xtlyp, int pyictvqzh, string ssroqchybddvvb, string ngbvfxic, int xxduvzttrfxj, string issvmni);
	void jpytlntwwkfjefpsdvucfyii(double cdfsn, bool yarowhgmkcesz, double ailafs);
	bool jumtepnwnrjjxcdku(double aualswrjoamjsmq, bool wqqfxrbhnfxpp);
	bool ywoycgirwwuynx(bool vemnqcejuj, bool hjxzuyrkxyteldk, bool quaguzd, int ckpje, string iodzpvdg, string xkvvoj);

};


void wxobadi::bpvuchjubkqbrxqftduhmlj() {
	int bgrxevpjur = 6121;
	string dvgdgall = "fwomkoowvjwghpwomaqzgmzgiaxsakpubdforbwbtvmayzkrektrhppxokrbljpirsdxcrcjnmakewabwlahuqphqrlphbzg";
	double xlercxombgo = 32948;
	bool fwlfgz = false;
	string dkkaxlbkong = "sgqfbysxnousfxjbteykliozxlmermygzkvzrrkvgxjqapikljjfcekgwezjkplupheshgafnjcoadmxgunlu";
	int vkyilfvvl = 3186;
	bool pyqqhfi = false;
	string awtxmupjnfi = "fuwmnodrbvbviwqnlntjjiqwokvyypwocezwbgxib";
	bool kuepmqsct = false;
	int zidikzkbm = 1377;
	if (1377 == 1377) {
		int rbi;
		for (rbi = 89; rbi > 0; rbi--) {
			continue;
		}
	}
	if (false != false) {
		int zomayhxuao;
		for (zomayhxuao = 96; zomayhxuao > 0; zomayhxuao--) {
			continue;
		}
	}
	if (false == false) {
		int hdueddjt;
		for (hdueddjt = 67; hdueddjt > 0; hdueddjt--) {
			continue;
		}
	}
	if (1377 == 1377) {
		int lpayj;
		for (lpayj = 28; lpayj > 0; lpayj--) {
			continue;
		}
	}
	if (false != false) {
		int gg;
		for (gg = 55; gg > 0; gg--) {
			continue;
		}
	}

}

void wxobadi::qbvshzfryoefmmzx(bool xyfny) {
	string dvrjtvw = "jggedgfkgoobeodvujjko";
	double yjullz = 50179;
	bool ggqozcqifqkcb = false;
	int skuggopfee = 3686;
	int balzqln = 836;
	int uyfzgyqcx = 5768;
	bool yhilbjehddm = false;
	double sxrhwjq = 62579;
	if (62579 == 62579) {
		int dv;
		for (dv = 35; dv > 0; dv--) {
			continue;
		}
	}

}

int wxobadi::ykvmtlvycl(double hjlgkxy, string qtdwvdspfunihq, int fbffpcfscqtgpdv, int rkwvbrgq, bool nctlinjrbzawsp) {
	bool njjdf = true;
	double bsoffvnmvwtzcv = 54692;
	string etdegimmwjmygdv = "dpzbearhpysfrggkyrulzbvlxzlyylvyarmspuvchmxolqde";
	if (true == true) {
		int myuyickr;
		for (myuyickr = 94; myuyickr > 0; myuyickr--) {
			continue;
		}
	}
	return 80102;
}

double wxobadi::djibaumxregvlcm(int cnapuk, double ykuafrhg, double mqsxrpmoql, double uheczl, double ollzojmjraihw, bool unzgkackaqmc, string kpooremep) {
	string tnazgd = "zatrhapnkkbjtuunnvjvwwpecryukxallmonmuuppbtrfvpb";
	double tvlkgui = 41444;
	bool gfouvm = false;
	if (false != false) {
		int wfdjc;
		for (wfdjc = 46; wfdjc > 0; wfdjc--) {
			continue;
		}
	}
	if (41444 == 41444) {
		int uyiiudtvyt;
		for (uyiiudtvyt = 48; uyiiudtvyt > 0; uyiiudtvyt--) {
			continue;
		}
	}
	if (string("zatrhapnkkbjtuunnvjvwwpecryukxallmonmuuppbtrfvpb") == string("zatrhapnkkbjtuunnvjvwwpecryukxallmonmuuppbtrfvpb")) {
		int oihkzjs;
		for (oihkzjs = 48; oihkzjs > 0; oihkzjs--) {
			continue;
		}
	}
	return 76094;
}

void wxobadi::iyfcdiaareqjtteaqrxzvbog(bool gkdcia, string qytknsvzufoo, string vpyhbtwpv, bool larfsroj, bool xtlyp, int pyictvqzh, string ssroqchybddvvb, string ngbvfxic, int xxduvzttrfxj, string issvmni) {
	string lntdlur = "vtbykbmzhandwvdufuvczrmxyaqvoshcrcvgfuicaegikeajpwrzrkikbuafumpjhophloxsn";
	double csuhdfshrxed = 662;
	double ycvjz = 3883;
	bool qpquwuycmmfojt = false;
	int qipufdycox = 7835;
	string ednepejsiexs = "sqlofbigzyeszfutrnefnbgnt";
	double imhnltayleya = 3543;
	if (string("sqlofbigzyeszfutrnefnbgnt") == string("sqlofbigzyeszfutrnefnbgnt")) {
		int pi;
		for (pi = 75; pi > 0; pi--) {
			continue;
		}
	}
	if (7835 == 7835) {
		int fdptnwscd;
		for (fdptnwscd = 54; fdptnwscd > 0; fdptnwscd--) {
			continue;
		}
	}
	if (3543 != 3543) {
		int qkd;
		for (qkd = 39; qkd > 0; qkd--) {
			continue;
		}
	}
	if (3543 != 3543) {
		int lylx;
		for (lylx = 22; lylx > 0; lylx--) {
			continue;
		}
	}

}

void wxobadi::jpytlntwwkfjefpsdvucfyii(double cdfsn, bool yarowhgmkcesz, double ailafs) {
	string jvupdgp = "vwsechdzmtreedjpsedvuiwenjgizatxbctuafdgmccfocbobphoouosnogklebumhnzobcymllzytyotirmcblhixpvaya";
	if (string("vwsechdzmtreedjpsedvuiwenjgizatxbctuafdgmccfocbobphoouosnogklebumhnzobcymllzytyotirmcblhixpvaya") != string("vwsechdzmtreedjpsedvuiwenjgizatxbctuafdgmccfocbobphoouosnogklebumhnzobcymllzytyotirmcblhixpvaya")) {
		int ps;
		for (ps = 28; ps > 0; ps--) {
			continue;
		}
	}
	if (string("vwsechdzmtreedjpsedvuiwenjgizatxbctuafdgmccfocbobphoouosnogklebumhnzobcymllzytyotirmcblhixpvaya") == string("vwsechdzmtreedjpsedvuiwenjgizatxbctuafdgmccfocbobphoouosnogklebumhnzobcymllzytyotirmcblhixpvaya")) {
		int poezbtocts;
		for (poezbtocts = 91; poezbtocts > 0; poezbtocts--) {
			continue;
		}
	}
	if (string("vwsechdzmtreedjpsedvuiwenjgizatxbctuafdgmccfocbobphoouosnogklebumhnzobcymllzytyotirmcblhixpvaya") != string("vwsechdzmtreedjpsedvuiwenjgizatxbctuafdgmccfocbobphoouosnogklebumhnzobcymllzytyotirmcblhixpvaya")) {
		int xdvwgmcla;
		for (xdvwgmcla = 64; xdvwgmcla > 0; xdvwgmcla--) {
			continue;
		}
	}
	if (string("vwsechdzmtreedjpsedvuiwenjgizatxbctuafdgmccfocbobphoouosnogklebumhnzobcymllzytyotirmcblhixpvaya") == string("vwsechdzmtreedjpsedvuiwenjgizatxbctuafdgmccfocbobphoouosnogklebumhnzobcymllzytyotirmcblhixpvaya")) {
		int poagv;
		for (poagv = 3; poagv > 0; poagv--) {
			continue;
		}
	}
	if (string("vwsechdzmtreedjpsedvuiwenjgizatxbctuafdgmccfocbobphoouosnogklebumhnzobcymllzytyotirmcblhixpvaya") == string("vwsechdzmtreedjpsedvuiwenjgizatxbctuafdgmccfocbobphoouosnogklebumhnzobcymllzytyotirmcblhixpvaya")) {
		int lmguldvak;
		for (lmguldvak = 84; lmguldvak > 0; lmguldvak--) {
			continue;
		}
	}

}

bool wxobadi::jumtepnwnrjjxcdku(double aualswrjoamjsmq, bool wqqfxrbhnfxpp) {
	int ifjxqxydzsdyi = 2230;
	string shdhfymhh = "deybprbeugsecdzunvhzolmwizqztxkghrjumnxomyhzmlhlguukyepuanowuuljdnzmjrydgpnsrxgjtlajmfhcmr";
	int mibinkjncfklpnj = 2403;
	double qdxbrmzbg = 48664;
	string uyydfeernxi = "siexqmtzm";
	bool kiifvqobts = true;
	double iekccdevlfol = 27065;
	int athsaqpx = 4845;
	string tjanwsdocnon = "skspxlhmqqjsrrwpxpqjqotxopouklzjhtgkeeaacaefvf";
	if (4845 == 4845) {
		int vlawn;
		for (vlawn = 16; vlawn > 0; vlawn--) {
			continue;
		}
	}
	return false;
}

bool wxobadi::ywoycgirwwuynx(bool vemnqcejuj, bool hjxzuyrkxyteldk, bool quaguzd, int ckpje, string iodzpvdg, string xkvvoj) {
	string afcfgagcdtbxodl = "kddglkwgdlsmefiqisxrrvngwlmvsnmdyojzeoeotpjskvthklorfpjynttmoadpahhlgilhbioregpnhgcbgzeslbbw";
	if (string("kddglkwgdlsmefiqisxrrvngwlmvsnmdyojzeoeotpjskvthklorfpjynttmoadpahhlgilhbioregpnhgcbgzeslbbw") != string("kddglkwgdlsmefiqisxrrvngwlmvsnmdyojzeoeotpjskvthklorfpjynttmoadpahhlgilhbioregpnhgcbgzeslbbw")) {
		int hkgqw;
		for (hkgqw = 60; hkgqw > 0; hkgqw--) {
			continue;
		}
	}
	return false;
}

string wxobadi::yyjwarxeahmybfqnjubvw(bool okdesatjwvw, int qussspeurlo) {
	string qufyiktbiplmfra = "kovtgwzyceffpwqfxvkvzjxkalegljgdwyuonwhjuisdiyhmbvlqgpyvydmndtvwqxfbw";
	bool puditxmzygahwh = false;
	if (string("kovtgwzyceffpwqfxvkvzjxkalegljgdwyuonwhjuisdiyhmbvlqgpyvydmndtvwqxfbw") == string("kovtgwzyceffpwqfxvkvzjxkalegljgdwyuonwhjuisdiyhmbvlqgpyvydmndtvwqxfbw")) {
		int prtybo;
		for (prtybo = 97; prtybo > 0; prtybo--) {
			continue;
		}
	}
	if (false != false) {
		int bjrgncnpjb;
		for (bjrgncnpjb = 7; bjrgncnpjb > 0; bjrgncnpjb--) {
			continue;
		}
	}
	if (false == false) {
		int zlvzaw;
		for (zlvzaw = 43; zlvzaw > 0; zlvzaw--) {
			continue;
		}
	}
	return string("muqgacodyby");
}

double wxobadi::nookufreblfisciij(int tinpguemnnyyfyv, double nuvkyfaw, bool dhbojhlfkt, int ksvraj, bool nyorbjcxizc, int wlrgg, bool wychdtado, double plzyjsccxtu) {
	bool yiksf = true;
	string piricjjkwihlph = "dxozdlpuhaotsmsvzxxssagomvjhjugwsqnfsuikjpmdcsyjjejtgkpdudaubeeiipdkjipooob";
	int dpitqa = 4410;
	int cbvakenqraqbk = 4455;
	bool akxsmvqqz = true;
	int ifuyzhwfkmteff = 1520;
	string ordplykx = "ofadsvlddcqeesorsnoiaojkditghavkdaanuzlkazzgolfloneybsioiqpcfcfgnsdfhbuycxlorllxkawcvpsrnm";
	double zssijmjlmhfru = 46337;
	int csscrrdft = 247;
	if (true != true) {
		int xmsdoogdp;
		for (xmsdoogdp = 51; xmsdoogdp > 0; xmsdoogdp--) {
			continue;
		}
	}
	if (247 == 247) {
		int qpzzardd;
		for (qpzzardd = 64; qpzzardd > 0; qpzzardd--) {
			continue;
		}
	}
	if (true != true) {
		int qvqcfd;
		for (qvqcfd = 70; qvqcfd > 0; qvqcfd--) {
			continue;
		}
	}
	if (string("dxozdlpuhaotsmsvzxxssagomvjhjugwsqnfsuikjpmdcsyjjejtgkpdudaubeeiipdkjipooob") == string("dxozdlpuhaotsmsvzxxssagomvjhjugwsqnfsuikjpmdcsyjjejtgkpdudaubeeiipdkjipooob")) {
		int qjzicovnc;
		for (qjzicovnc = 43; qjzicovnc > 0; qjzicovnc--) {
			continue;
		}
	}
	if (true == true) {
		int pmbcyimb;
		for (pmbcyimb = 80; pmbcyimb > 0; pmbcyimb--) {
			continue;
		}
	}
	return 99322;
}

double wxobadi::pjjtrzxerk(int noniikvapzeiqkr, double hbszriby, double hbsxhfxweomy, int swnyosl, int zczmsfnyxkeffbo, string ohrsvicpofzqd, int cirilkeprdyoeg, double pxvtchcxn, double qbwgkfgglvyus, bool qnzelbbdwtgsvku) {
	bool larpgbvbzxxsh = false;
	bool wutesogyjcp = true;
	bool hrspqbnrffn = true;
	bool wzqptb = false;
	if (true == true) {
		int caavu;
		for (caavu = 35; caavu > 0; caavu--) {
			continue;
		}
	}
	if (true == true) {
		int oconxvpqn;
		for (oconxvpqn = 72; oconxvpqn > 0; oconxvpqn--) {
			continue;
		}
	}
	return 53616;
}

int wxobadi::xijlqhttczjxajygafkkizlxm(double iriejry, int mzpkykdkamjxmts, double fclki) {
	double uobkcqvgxq = 56383;
	bool szvpp = false;
	int uwqicsdf = 314;
	double bekyzcocztkttcl = 25321;
	int jkakoqa = 1035;
	double ebhnzadnypgi = 18529;
	double fhpwfaeduh = 15662;
	string icrukgoexxcoyz = "liihdoezwjhxxeijwzjvmxvtozajyikggmfdvfawtgusntlknrxtwt";
	bool cxygorzmtrblyt = true;
	if (18529 == 18529) {
		int vsowatyj;
		for (vsowatyj = 75; vsowatyj > 0; vsowatyj--) {
			continue;
		}
	}
	return 28051;
}

bool wxobadi::yyrsetydsdaqrijepkf() {
	bool gibhlndg = true;
	bool yqekwmltpandmtq = false;
	return false;
}

double wxobadi::xqcfxfljumeufpzteykl(double xvrblkl, bool sufcxz, bool xtzqitxyqfiu, string qwohbll, bool ikrynfdcrv, bool pgwjraglws, double xlmuynsew, double ktlzgjvx) {
	string rjnzmn = "cosocmwojtwqehrnpndjbvbavqsrfudlrewiuxshsrhpvgssokswlvcga";
	string gusvhetccidzjgc = "eyuxutamoahmzrnmwjmxnlnfqjgbpmyyfeqnrwcdhwtvxphvfzpyoaptokpofym";
	if (string("cosocmwojtwqehrnpndjbvbavqsrfudlrewiuxshsrhpvgssokswlvcga") != string("cosocmwojtwqehrnpndjbvbavqsrfudlrewiuxshsrhpvgssokswlvcga")) {
		int pmcqnyr;
		for (pmcqnyr = 37; pmcqnyr > 0; pmcqnyr--) {
			continue;
		}
	}
	if (string("cosocmwojtwqehrnpndjbvbavqsrfudlrewiuxshsrhpvgssokswlvcga") == string("cosocmwojtwqehrnpndjbvbavqsrfudlrewiuxshsrhpvgssokswlvcga")) {
		int oyok;
		for (oyok = 29; oyok > 0; oyok--) {
			continue;
		}
	}
	return 90336;
}

void wxobadi::fwspmzylxui(string oovcwfnbb, string hxqkhzwvexiv, bool liqppmudwzweyaj, double zgrdwwjvjitett) {
	string pzafqtpll = "yfnkiuscdkoqcgtfclgdmhdwmdjquudylqqdhcpnssfhscrneoriocjaqzwhdiznpipofjuesfrc";
	double rlahzxbpwq = 5667;
	if (string("yfnkiuscdkoqcgtfclgdmhdwmdjquudylqqdhcpnssfhscrneoriocjaqzwhdiznpipofjuesfrc") != string("yfnkiuscdkoqcgtfclgdmhdwmdjquudylqqdhcpnssfhscrneoriocjaqzwhdiznpipofjuesfrc")) {
		int urmz;
		for (urmz = 86; urmz > 0; urmz--) {
			continue;
		}
	}

}

bool wxobadi::khabotvuntnk(bool gbefvvjalb, string swkjbyfjqnd, string fxgewksvox, int ablfwezoh) {
	double dtmnmvcla = 30646;
	bool auyisqxrmn = true;
	int mrcgeoyoua = 1861;
	int tmlvvebmtlwvp = 1637;
	if (1637 != 1637) {
		int kst;
		for (kst = 19; kst > 0; kst--) {
			continue;
		}
	}
	if (1861 == 1861) {
		int nwrvjwj;
		for (nwrvjwj = 71; nwrvjwj > 0; nwrvjwj--) {
			continue;
		}
	}
	if (30646 != 30646) {
		int uvl;
		for (uvl = 72; uvl > 0; uvl--) {
			continue;
		}
	}
	if (1861 != 1861) {
		int zkl;
		for (zkl = 11; zkl > 0; zkl--) {
			continue;
		}
	}
	return true;
}

void wxobadi::tioblrunpjprwjyeqsymsjo() {
	int jabfiwzehswuw = 3319;
	int lcwfu = 51;
	bool ewzpqprbqxtdouf = false;
	double gxvqeyjeblfo = 1446;
	if (1446 != 1446) {
		int ahrgmp;
		for (ahrgmp = 70; ahrgmp > 0; ahrgmp--) {
			continue;
		}
	}
	if (3319 == 3319) {
		int ctsuehrjhh;
		for (ctsuehrjhh = 83; ctsuehrjhh > 0; ctsuehrjhh--) {
			continue;
		}
	}
	if (51 == 51) {
		int xirxrawncp;
		for (xirxrawncp = 85; xirxrawncp > 0; xirxrawncp--) {
			continue;
		}
	}

}

void wxobadi::ywlcaprhgyxkeo(double oobvrjom, int hhomfi, bool qmgobxbqjoux, string hidonxquy, int glugpbreyu) {
	bool emybtzgibfxwcr = true;
	string rqpsrrnbg = "tndhcljgtijbuxwkxejvhzxraypkxodyiowkzz";
	int eggxhnibmzchh = 3240;
	bool rcyhpwiohyt = false;
	string rqnwfywvztfnk = "gxhpflghxkinzetrwjqxeuqaywdimobqifaldidckdcixdebcdiislfmmrrudlmyzurehgmxaztcszndplahcloncgvaouprsml";
	double siyjtrsyldygex = 30713;
	if (false == false) {
		int raoyb;
		for (raoyb = 46; raoyb > 0; raoyb--) {
			continue;
		}
	}
	if (30713 != 30713) {
		int kqzizxim;
		for (kqzizxim = 91; kqzizxim > 0; kqzizxim--) {
			continue;
		}
	}

}

double wxobadi::catgvqdcvy(string xowymx, int rodpxlqrozfjvi, bool xpaivbcbsimubz, int kemvqhnrlfmt, double evjjae, int idqghtnpihetw, string zfvvpluyrqynfdn, int hwabsgal, int hizjziblvonj, string qqbpgnzyggyyyuk) {
	int gezwbyrfc = 9;
	double ewtih = 73734;
	double zfzntyljb = 28161;
	int zdqprf = 5793;
	double htyaugjlmtuzde = 19918;
	int rhayuwpkirs = 1183;
	double chkrsnswbwqokm = 32466;
	if (32466 != 32466) {
		int aigvboqdul;
		for (aigvboqdul = 29; aigvboqdul > 0; aigvboqdul--) {
			continue;
		}
	}
	if (28161 != 28161) {
		int wezfbxe;
		for (wezfbxe = 98; wezfbxe > 0; wezfbxe--) {
			continue;
		}
	}
	return 68245;
}

wxobadi::wxobadi() {
	this->pjjtrzxerk(1743, 20352, 56830, 643, 2870, string("bpcvpzymdedmlqmzwjgplnidvwddakknjhsawiauxqajftoxbocpthwyjts"), 823, 23542, 11552, true);
	this->xijlqhttczjxajygafkkizlxm(14592, 2593, 71134);
	this->yyrsetydsdaqrijepkf();
	this->xqcfxfljumeufpzteykl(27593, true, false, string("itpxdmdlfosxzkgyhxreqsubquzhxcuomqrsnmahghsvuhqemgzqaoqkjsntynbdlve"), false, false, 35064, 61190);
	this->fwspmzylxui(string("bsdfa"), string("japlhmmmffyqhjtbeefdiirwzxfryftupseukorrxxsajkudvbqelzshuwaakhoalvbom"), false, 22353);
	this->khabotvuntnk(true, string("piyqqjsjlcbjvihkmklicxjgfaxcrynnvuwfohltpoalypxjjyrukanshwvkreexvcemj"), string("jryuvjvxoafetfsntfjllkwjvoorqbdwphbsfsskurnlapliddrzxvluykeuejddgptqbqxsb"), 2316);
	this->tioblrunpjprwjyeqsymsjo();
	this->ywlcaprhgyxkeo(17785, 361, false, string("mtajjgkuanoawfxbrwzojcpmjeoveofhucwgwfswrrjerksnawpyicnmynlmahricuiiqvcqioideegijmwhsmawvpngse"), 2706);
	this->catgvqdcvy(string("vlojbljdospzdekoniipzinsabjuivuqf"), 2990, false, 3316, 22218, 1415, string("apevtavkzxbsvrdlyeptkjlubkmqjxevohcntr"), 1945, 3401, string("zquwvoctrtochoksrwbphwlibksjsboegeigabgldwowtdzictccggftmvjqbxngaoaikhodachfziwdvpqrfgizzygqllejidt"));
	this->yyjwarxeahmybfqnjubvw(false, 2560);
	this->nookufreblfisciij(596, 82563, false, 1788, true, 7052, false, 17438);
	this->bpvuchjubkqbrxqftduhmlj();
	this->qbvshzfryoefmmzx(false);
	this->ykvmtlvycl(3954, string("bwdqwrmlpamhdzyoctcbrrulvkwnxkuokjrevwcxffkzzcjrjvrijktvcfhuatqolydshbtbppfpjkpsgzuwgc"), 875, 8167, false);
	this->djibaumxregvlcm(1964, 63167, 53838, 10707, 61960, true, string("xhnwwcmetmuedfixqklaostypltzwmhntqslqrfsxzrwarqopreqcvrhxwknahhlkxycgfotfojzkixsynkhhnchqvfzjqwzp"));
	this->iyfcdiaareqjtteaqrxzvbog(false, string("byuisfenpuuobolkwkgyprrveofjhaggbfnqlsxxbbgfqppwrxpqvubhbvhitgxabyxcvmhnebpqis"), string("qgdqjuncuzibtbyihbjmkwsxkctfxxrunlwllwlbrujfrgwyhuzmqexddzejlrpggoawcbclvhxpafvkpmhvuprggyhkxxojgy"), true, true, 4472, string("qbhnvxlbstxasiowoeadtsvynonfatosupnsjugztdcesnczgqlhivxdtkgjfinvaej"), string("qzhlivtrcdjanxnelwxrdrstghdxhfaajrdsmkbkctdcqtx"), 743, string("iztnthlgabwpssyxhkpjutwhdovpwbalukajmtjcofjjxybcpwetqqqhp"));
	this->jpytlntwwkfjefpsdvucfyii(77682, true, 56879);
	this->jumtepnwnrjjxcdku(18025, true);
	this->ywoycgirwwuynx(true, false, true, 1356, string("bhzytmzarrbdvvvwsdwtmwaebkrahzcksvxpgrskwt"), string("zdqfqnwvqhoevzbbfgeurddyfaduzwavyzeexvmigvktmzhaszxdbpvlpcsaobezpybyf"));
}
