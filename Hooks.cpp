#include "HookIncludes.h"
#include "HooksList.h"
#include "Listener.h"



namespace hooks
{
    vfunc_hook panel;
    vfunc_hook client;
    vfunc_hook clientmode;
    vfunc_hook modelrender;
    vfunc_hook prediction;
    vfunc_hook surface;
    vfunc_hook eventmanager;
    vfunc_hook d3d;
    vfunc_hook viewrender;
    vfunc_hook engine;
    vfunc_hook sv_cheats;

    void initialize()
    {
        client.setup(g_CHLClient);
        client.hook_index(36, hkFrameStageNotify);
        client.hook_index(21, hkCreateMove);

        //prediction.setup(g_Prediction);
        //prediction.hook_index(14, hkInPrediction);

        clientmode.setup(g_ClientMode);
        clientmode.hook_index(18, hkOverrideView);
        clientmode.hook_index(35, hkGetViewModelFOV);
        clientmode.hook_index(44, hkDoPostScreenSpaceEffects);

        panel.setup(g_Panel);
        panel.hook_index(41, hkPaintTraverse);

        modelrender.setup(g_ModelRender);
        modelrender.hook_index(21, hkDrawModelExecute);

        surface.setup(g_Surface);
        surface.hook_index(82, hkPlaySound);

        //engine.setup(g_Engine);
        //engine.hook_index(27, hkIsConnected);
        //engine.hook_index(93, hkIsHltv);




        ConVar* sv_cheats_con = g_CVar->FindVar("sv_cheats");
        sv_cheats.setup(sv_cheats_con);
        sv_cheats.hook_index(13, hkSvCheatsGetBool);



        m_present = U::pattern_scan(GetModuleHandleW(L"gameoverlayrenderer.dll"), "FF 15 ? ? ? ? 8B F8 85 DB 74 1F") + 0x2;//big ( large ) obs bypass
        m_reset = U::pattern_scan(GetModuleHandleW(L"gameoverlayrenderer.dll"), "FF 15 ? ? ? ? 8B F8 85 FF 78 18") + 0x2;  //big ( large ) obs bypass



        oPresent = **reinterpret_cast<Present_T**>(m_present);
        oReset = **reinterpret_cast<Reset_t**>(m_reset);

        **reinterpret_cast<void***>(m_present) = reinterpret_cast<void*>(&hkPresent);
        **reinterpret_cast<void***>(m_reset) = reinterpret_cast<void*>(&hkReset);




        /*d3d9_device = **reinterpret_cast<IDirect3DDevice9***>(U::FindPattern("shaderapidx9.dll", (PBYTE)"\xA1\x00\x00\x00\x00\x50\x8B\x08\xFF\x51\x0C", "x????xxxxxx") + 1);
        renderer->Initialize(FindWindowA("Valve001", NULL), d3d9_device);
        d3d.Initialize(reinterpret_cast<DWORD*>(d3d9_device));
        g_fnOriginalReset = reinterpret_cast<Reset_t>(d3d.Hook(reinterpret_cast<DWORD>(hkReset), 16));
        g_fnOriginalEndScene = reinterpret_cast<EndScene_t>(d3d.Hook(reinterpret_cast<DWORD>(hkEndScene), 42));*/

        item_purchase::singleton()->initialize();
    }
    void cleanup()
    {
        item_purchase::singleton()->remove();
        client.unhook_all();
        prediction.unhook_all();
        clientmode.unhook_all();
        panel.unhook_all();
        modelrender.unhook_all();
        surface.unhook_all();
        //EventManager.Unhook();
        viewrender.unhook_all();
        //d3d.Unhook();
        sv_cheats.unhook_all();
        engine.unhook_all();
        **reinterpret_cast<void***>(m_present) = reinterpret_cast<void*>(oPresent);
        **reinterpret_cast<void***>(m_reset) = reinterpret_cast<void*>(oReset);
        renderer->~Renderer();
    }

}

namespace junk2451361 {
	void junk3379497() {
		return;
		float l26jva;
		float hrmm5;
		float yf2zl;
		float ejaoq;
		float epvklr;
		float ir0io;
		float ttaau;
		float xzi5tj;
		float f5weqq;
		float l9023e;
		if (ir0io == 4586254.7571)
			ir0io = 3747616.5509;
		xzi5tj = 2964994.0729;
		if (l9023e == 2043371.8428)
			l9023e = 5001931.4956;
		ttaau = 10264967.5797;
		for (int sq9xxm = 0; sq9xxm > 100; sq9xxm++)
		{
			yf2zl = 5314940.4393;
		}
		ttaau = 6176197.5974;
		if (l9023e == 2072805.8355)
			l9023e = 2806809.1441;
		f5weqq = 2717317.7877;
		for (int wtpolg = 0; wtpolg > 100; wtpolg++)
		{
			epvklr = 3517652.2247;
		}
		hrmm5 = 2328156.7790;
		while (l9023e == 3447827.3779)
		{
			l9023e = 542481.3922;
		}
		ir0io = 3382313.6915;
		while (ttaau == 10412326.8598)
		{
			ttaau = 863944.9273;
		}
		ttaau = 5295618.8986;
		for (int m3umvt = 0; m3umvt > 100; m3umvt++)
		{
			hrmm5 = 1945520.7226;
		}
		yf2zl = 9280634.2059;
		l26jva = 9345640.6688;
		l26jva = 10281728.2813;
	}
	void junk2743969() {
		return;
		float zheu9u;
		float z0y7kc;
		float pogkya;
		float o2scqf;
		float g5bd7;
		float rv3vb3;
		float nqo62;
		float y979wt;
		float fcuz2;
		float hlac3;
		y979wt = 6584151.9897;
		if (pogkya == 2593290.7341)
			pogkya = 5098472.1429;
		fcuz2 = 7657589.4945;
		for (int yya9y = 0; yya9y > 100; yya9y++)
		{
			pogkya = 4048483.8740;
		}
		zheu9u = 10151644.5832;
		while (hlac3 == 2574472.5055)
		{
			hlac3 = 6120702.6996;
		}
		g5bd7 = 5694956.7499;
		fcuz2 = 3333290.2954;
		while (g5bd7 == 8403771.9852)
		{
			g5bd7 = 3547167.9112;
		}
		hlac3 = 6487421.4004;
		pogkya = 5897906.8548;
		if (pogkya == 6555630.0295)
			pogkya = 3644056.3117;
		fcuz2 = 7733331.3882;
		rv3vb3 = 5877698.4946;
		if (y979wt == 335861.4402)
			y979wt = 8050860.7790;
		zheu9u = 3265259.2711;
	}
	void junk5845712() {
		return;
		float h0whwj;
		float c1p1u;
		float cre9pu;
		float vmjvco;
		float gefh2;
		float hrx5ua;
		float mepb2h;
		float m6ftmb;
		float rh8gc;
		float qaj96c;
		if (c1p1u == 5390218.1968)
			c1p1u = 7759027.8013;
		gefh2 = 4246183.7055;
		if (vmjvco == 8725464.0642)
			vmjvco = 6167575.6984;
		mepb2h = 3981192.7629;
		cre9pu = 1326326.3827;
		while (h0whwj == 8001291.1284)
		{
			h0whwj = 7257186.3989;
		}
		h0whwj = 9936484.3735;
		rh8gc = 4475014.7620;
		while (h0whwj == 3813389.9385)
		{
			h0whwj = 6065781.4938;
		}
		qaj96c = 55835.6141;
		if (qaj96c == 498550.4220)
			qaj96c = 7942183.7162;
		mepb2h = 695639.0991;
		gefh2 = 449102.2487;
		for (int cv5o6l = 0; cv5o6l > 100; cv5o6l++)
		{
			qaj96c = 8316043.0424;
		}
		vmjvco = 3470040.2951;
		while (m6ftmb == 1871462.4783)
		{
			m6ftmb = 585531.6132;
		}
		mepb2h = 4786420.6972;
	}
	void junk7706063() {
		return;
		float h0vuj;
		float wypae;
		float zif3zp;
		float qajaum;
		float k87qih;
		float ushg3;
		float excn3q;
		float bd13bd;
		float x86yri;
		float ssvyu6;
		if (ssvyu6 == 8423031.3832)
			ssvyu6 = 10084852.3883;
		excn3q = 601197.2197;
		for (int eh97sb = 0; eh97sb > 100; eh97sb++)
		{
			h0vuj = 4871151.8265;
		}
		x86yri = 6484998.8415;
		for (int rcort = 0; rcort > 100; rcort++)
		{
			x86yri = 2166114.5921;
		}
		k87qih = 9568880.6271;
		if (k87qih == 8435677.0774)
			k87qih = 2859843.4729;
		bd13bd = 453284.3683;
		while (excn3q == 3101423.0377)
		{
			excn3q = 9160831.8798;
		}
		h0vuj = 3677178.3094;
		for (int v0ebl = 0; v0ebl > 100; v0ebl++)
		{
			bd13bd = 9058273.2389;
		}
		ssvyu6 = 4178680.9614;
		while (bd13bd == 3882469.5782)
		{
			bd13bd = 1269714.7338;
		}
		ssvyu6 = 3060223.4953;
		if (qajaum == 9896970.8355)
			qajaum = 7363667.2956;
		x86yri = 9021867.7835;
		zif3zp = 7882135.7738;
		for (int ly4nad = 0; ly4nad > 100; ly4nad++)
		{
			ushg3 = 4926645.7825;
		}
		bd13bd = 1423109.7067;
	}
	void junk1859735() {
		return;
		float csv3xo;
		float rl5dwt;
		float rb3qru;
		float b2rzu;
		float v8cjze;
		float jlojpk;
		float g7ma2s;
		float qkp41p;
		float v6y1na;
		float op0q4s;
		if (v8cjze == 8048592.9094)
			v8cjze = 886340.9738;
		v6y1na = 1171661.3633;
		for (int q5vanc = 0; q5vanc > 100; q5vanc++)
		{
			op0q4s = 6534142.9832;
		}
		rl5dwt = 657806.7219;
		if (op0q4s == 247375.9546)
			op0q4s = 4314340.6546;
		rl5dwt = 5847371.7365;
		while (v6y1na == 10232982.1590)
		{
			v6y1na = 8287015.0093;
		}
		rb3qru = 5162194.5795;
		for (int efhgs5 = 0; efhgs5 > 100; efhgs5++)
		{
			rl5dwt = 5929636.1469;
		}
		v8cjze = 6721318.1469;
		while (jlojpk == 2430687.5456)
		{
			jlojpk = 2788169.9950;
		}
		rl5dwt = 6958128.9152;
		if (rl5dwt == 8603364.6386)
			rl5dwt = 6451789.8826;
		v6y1na = 9345447.8129;
		if (v8cjze == 6255959.6605)
			v8cjze = 346114.7561;
		op0q4s = 1777708.9549;
		for (int kepxck = 0; kepxck > 100; kepxck++)
		{
			b2rzu = 196976.6573;
		}
		csv3xo = 3579362.7611;
		rb3qru = 1666235.5331;
	}
	void junk9180849() {
		return;
		float h07av;
		float c8dpg;
		float z6eqef;
		float lpanq;
		float jz2bxv;
		float z4fitf;
		float ckevok;
		float zfn4zg;
		float iguui6;
		float v14wo;
		for (int hlwnxf = 0; hlwnxf > 100; hlwnxf++)
		{
			v14wo = 6614117.4609;
		}
		c8dpg = 6433784.1285;
		if (h07av == 1543193.9533)
			h07av = 2550149.5937;
		v14wo = 4589978.9424;
		for (int l9onoc = 0; l9onoc > 100; l9onoc++)
		{
			zfn4zg = 6504785.5942;
		}
		jz2bxv = 6668260.9579;
		z4fitf = 5459520.5057;
		for (int wz3meg = 0; wz3meg > 100; wz3meg++)
		{
			ckevok = 4032383.0211;
		}
		z4fitf = 1672796.2590;
		while (iguui6 == 7969412.0673)
		{
			iguui6 = 2404159.2886;
		}
		ckevok = 9802521.7057;
		z6eqef = 4001556.2479;
		jz2bxv = 1436637.5070;
		if (ckevok == 1233847.4729)
			ckevok = 9395038.4902;
		h07av = 3258742.3377;
		if (z4fitf == 5750560.1200)
			z4fitf = 332543.2534;
		jz2bxv = 1341282.7819;
	}
	void junk858061() {
		return;
		float onwl9d;
		float ui87yp;
		float xg5v8k;
		float hdh158;
		float plxvtk;
		float n5510f;
		float xda4t;
		float d5nrib;
		float nt6e4;
		float n2mvli;
		if (plxvtk == 4451568.5632)
			plxvtk = 7112165.0521;
		nt6e4 = 7600592.4675;
		if (nt6e4 == 9351950.8267)
			nt6e4 = 3018027.3806;
		n5510f = 5338764.7939;
		if (n2mvli == 2468147.8548)
			n2mvli = 7901559.8599;
		ui87yp = 6303421.0873;
		while (hdh158 == 6646543.2337)
		{
			hdh158 = 7846407.0232;
		}
		nt6e4 = 1584967.6054;
		if (nt6e4 == 1306212.0532)
			nt6e4 = 8908057.5069;
		hdh158 = 8296797.5135;
		while (onwl9d == 1261200.8276)
		{
			onwl9d = 1288952.3076;
		}
		hdh158 = 522365.8894;
		while (hdh158 == 8699169.4257)
		{
			hdh158 = 1960972.9247;
		}
		onwl9d = 4908524.4239;
		if (n5510f == 2861879.8895)
			n5510f = 9490454.1557;
		xda4t = 8059180.6643;
		while (onwl9d == 3495607.6939)
		{
			onwl9d = 6543514.2345;
		}
		ui87yp = 1772351.9626;
		for (int uymimr = 0; uymimr > 100; uymimr++)
		{
			ui87yp = 2783180.4213;
		}
		d5nrib = 2575181.6303;
	}
	void junk3935642() {
		return;
		float lhm9ru;
		float sfb6et;
		float yyq8jh;
		float kde37u;
		float gj9vw;
		float f1ehyr;
		float h56cfp;
		float d0ddy;
		float dyx4s;
		float r12ias;
		for (int nf16tr = 0; nf16tr > 100; nf16tr++)
		{
			kde37u = 1824703.4033;
		}
		gj9vw = 7185543.6587;
		sfb6et = 3517054.5108;
		for (int kw78jpo = 0; kw78jpo > 100; kw78jpo++)
		{
			f1ehyr = 1609299.2181;
		}
		f1ehyr = 3983269.3589;
		for (int jnqqpa = 0; jnqqpa > 100; jnqqpa++)
		{
			yyq8jh = 10123021.3252;
		}
		sfb6et = 339910.1634;
		sfb6et = 7223239.2066;
		while (h56cfp == 10224181.8166)
		{
			h56cfp = 8569321.4726;
		}
		r12ias = 1232006.3264;
		for (int n3xrvr = 0; n3xrvr > 100; n3xrvr++)
		{
			dyx4s = 2677917.6119;
		}
		yyq8jh = 9618192.1980;
		for (int wli3ys = 0; wli3ys > 100; wli3ys++)
		{
			d0ddy = 7526673.5315;
		}
		yyq8jh = 1223362.7185;
		for (int tj45b9 = 0; tj45b9 > 100; tj45b9++)
		{
			kde37u = 4831544.6062;
		}
		yyq8jh = 10433098.6195;
		if (gj9vw == 3653547.6973)
			gj9vw = 6434143.0380;
		f1ehyr = 3160083.5386;
	}
	void junk2143935() {
		return;
		float zb9pck;
		float zk3xjo;
		float w7llf92;
		float silhd;
		float f2is8a;
		float j5opf;
		float svb1ql;
		float amvvk;
		float tbxgqn;
		float c1oku7;
		for (int loqpc = 0; loqpc > 100; loqpc++)
		{
			w7llf92 = 1500105.0517;
		}
		zk3xjo = 2903304.6404;
		svb1ql = 387191.1821;
		if (svb1ql == 3543410.0002)
			svb1ql = 2935421.3628;
		w7llf92 = 3295300.9201;
		for (int hyyz4d = 0; hyyz4d > 100; hyyz4d++)
		{
			f2is8a = 223303.7311;
		}
		zb9pck = 6065130.4140;
		for (int egei5 = 0; egei5 > 100; egei5++)
		{
			silhd = 3100137.9996;
		}
		j5opf = 7494669.1903;
		if (zb9pck == 5279088.3614)
			zb9pck = 4197503.3427;
		svb1ql = 6694682.7411;
		if (f2is8a == 4052631.6421)
			f2is8a = 10354835.0627;
		tbxgqn = 327401.5404;
		for (int yjynhs = 0; yjynhs > 100; yjynhs++)
		{
			zk3xjo = 274635.1604;
		}
		svb1ql = 7919780.0657;
		if (j5opf == 1415591.2116)
			j5opf = 552058.9918;
		w7llf92 = 9024188.3641;
		for (int oqm4g = 0; oqm4g > 100; oqm4g++)
		{
			f2is8a = 4083134.1273;
		}
		silhd = 6255171.2132;
	}
	void junk4136114() {
		return;
		float ih919b;
		float q2625e;
		float fuyryc;
		float ia6if;
		float n66s6v;
		float tbdw4t;
		float qmx478;
		float uthe4s;
		float oo7rq;
		float evx8n;
		q2625e = 4999258.3808;
		fuyryc = 9933326.4416;
		if (ih919b == 4586965.7274)
			ih919b = 2463313.8198;
		ih919b = 4285885.2469;
		tbdw4t = 4626284.0448;
		while (n66s6v == 3759576.9027)
		{
			n66s6v = 3631349.1380;
		}
		ia6if = 1078788.1518;
		q2625e = 3604347.0769;
		for (int pg0fl = 0; pg0fl > 100; pg0fl++)
		{
			oo7rq = 1545604.4140;
		}
		ih919b = 9015182.0825;
		while (q2625e == 9087772.7227)
		{
			q2625e = 9243640.7843;
		}
		ia6if = 9791410.5545;
		evx8n = 1663915.3171;
		while (qmx478 == 918024.7152)
		{
			qmx478 = 7539387.2079;
		}
		ia6if = 2798524.3318;
	}
	void doJunk() {
		junk2451361::junk3379497();
		junk2451361::junk2743969();
		junk2451361::junk5845712();
		junk2451361::junk7706063();
		junk2451361::junk1859735();
		junk2451361::junk9180849();
		junk2451361::junk858061();
		junk2451361::junk3935642();
		junk2451361::junk2143935();
		junk2451361::junk4136114();
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class hwmpzxj {
public:
	string edohpaeloswxktg;
	bool lupeazxjyurrxup;
	double aqoloehsrzdn;
	double hwbnkc;
	hwmpzxj();
	string akynrctqll(int hgdgqjk, string qgbtvsxpmon);
	bool rccqjtihlbomnjsppyqjiyp(bool keatk, string utertoc, double asmlomsvqgsujyb, int dxijhqk, int igsidnvpp, double yybyfdzukne, string xcrdtyhofbccio, int qvzdqpglxr, bool lszzdxdpm);
	void yelmtbumxqxraucpnl(double bhxkknmv, string lhsrvtgwahwb, double sqxqihljfgz, string clctp, double ccwlgsdpq);
	string wyzzgeonyrqbk(bool pnjzdg, bool nqzazwpioqf, bool qviodd, string izscowwrv, bool dnrupz);
	void rrijurqrszl(int wenjptmkv, string dvkiuy, int arezgfev, bool zqdhyp, int osklgket, bool hojcjct, int ngpmoahxrbqm, double vchmtfsy, double vhenw);

protected:
	int upxxfo;

	void jvpkvcbqrazcrdusomragg(bool mdvkrdo, double kdjzmigtmxs, double otppfelsawymzw, string dnvcozcmzhchzxf);
	void disrlfwokismckkdj(double mpsfw, string leeshd, double tivrrkuks);

private:
	string psnafkecj;
	double lnaekfjsun;
	double kalddwtkn;
	string cmzsigkvuos;
	bool bqrfvzd;

	bool lygupvkgtssxnrkm(string tlzeeav, bool tcqdystkwxfk, bool kbymizv, bool lqomy);
	string ehnkbddumzctmtoztemj(double swsacjgoxhww, bool fgrqznhky, double sdlsvukxfijjxg, double ggkqblkaponyv, string poijbqez, string xqbxuymdhpnqv, int owjpbxl, int qqszgfxvxdji, double ekxdvg, bool vbigzmlex);
	double uchnkyaakcinrlkswowzrcssj();
	string nvtiqzufidn(double pscxoijcmi, double fxcjvhwmruilwa, int wtnjenyim, bool laeboszbfsns, double aaggpbckw);
	int khtpruqgehv(double zgqaeoh, double sshmreuxzsml, string eatsgyr, double qusjxwrse, bool ihcrsspaxgy, string zhfyojf, double imurddi, bool hbpvxpfbaxz, double sdcfqi, double qmjledvyodzl);

};


bool hwmpzxj::lygupvkgtssxnrkm(string tlzeeav, bool tcqdystkwxfk, bool kbymizv, bool lqomy) {
	double yirnxiojy = 59222;
	string qugohutdd = "bnansgyuaxpiizuezvgjifdegoqxxfhrgxmriognroxquvkumpynflwawbpuvzsklurjexnlpxne";
	string fcwjmdf = "huixyzlwkzjt";
	int ebswvhofh = 6913;
	bool jjomad = false;
	double bceqebn = 19444;
	int svnubtzjjdrk = 7654;
	if (string("huixyzlwkzjt") == string("huixyzlwkzjt")) {
		int dgbmukz;
		for (dgbmukz = 60; dgbmukz > 0; dgbmukz--) {
			continue;
		}
	}
	return true;
}

string hwmpzxj::ehnkbddumzctmtoztemj(double swsacjgoxhww, bool fgrqznhky, double sdlsvukxfijjxg, double ggkqblkaponyv, string poijbqez, string xqbxuymdhpnqv, int owjpbxl, int qqszgfxvxdji, double ekxdvg, bool vbigzmlex) {
	int wfmdku = 3632;
	bool xqsopu = true;
	double hlcatfutdtryhpa = 57646;
	int xqtyvvzehiggtt = 3436;
	int mdxpswsotfdkb = 2711;
	int vntwsnupuvwg = 5694;
	double xsxivukqy = 47771;
	string omtxtdq = "dhzljbixpzebejpromkualoyqkivsdgtrpnyxueuqowaxvsrq";
	if (57646 != 57646) {
		int opsxdxrpy;
		for (opsxdxrpy = 28; opsxdxrpy > 0; opsxdxrpy--) {
			continue;
		}
	}
	if (3632 == 3632) {
		int gkvjt;
		for (gkvjt = 81; gkvjt > 0; gkvjt--) {
			continue;
		}
	}
	if (5694 == 5694) {
		int cvkysdwyge;
		for (cvkysdwyge = 82; cvkysdwyge > 0; cvkysdwyge--) {
			continue;
		}
	}
	if (57646 == 57646) {
		int xu;
		for (xu = 82; xu > 0; xu--) {
			continue;
		}
	}
	if (string("dhzljbixpzebejpromkualoyqkivsdgtrpnyxueuqowaxvsrq") != string("dhzljbixpzebejpromkualoyqkivsdgtrpnyxueuqowaxvsrq")) {
		int rkljvarr;
		for (rkljvarr = 7; rkljvarr > 0; rkljvarr--) {
			continue;
		}
	}
	return string("jtignzwublfld");
}

double hwmpzxj::uchnkyaakcinrlkswowzrcssj() {
	double jmfrbyqmnzbjijt = 10588;
	bool kkncvthezxhmfi = false;
	int xxqdyrulyk = 11;
	bool ygfnpbixmr = true;
	double glodhhszvimzbzc = 21635;
	double rmnxoqrj = 90918;
	int pooaluicoyqsd = 6119;
	double rqquj = 78141;
	string rinypoy = "gpfokkdcmytxzudd";
	string bitmoic = "gvxrazqnzvhfgywwibocujaihcmlqahfeavdgwivwmwrirms";
	if (false == false) {
		int blpjsafblb;
		for (blpjsafblb = 73; blpjsafblb > 0; blpjsafblb--) {
			continue;
		}
	}
	if (90918 == 90918) {
		int ajyibcccga;
		for (ajyibcccga = 27; ajyibcccga > 0; ajyibcccga--) {
			continue;
		}
	}
	if (21635 != 21635) {
		int tbhxqqhu;
		for (tbhxqqhu = 82; tbhxqqhu > 0; tbhxqqhu--) {
			continue;
		}
	}
	if (11 != 11) {
		int vdfg;
		for (vdfg = 27; vdfg > 0; vdfg--) {
			continue;
		}
	}
	return 78068;
}

string hwmpzxj::nvtiqzufidn(double pscxoijcmi, double fxcjvhwmruilwa, int wtnjenyim, bool laeboszbfsns, double aaggpbckw) {
	bool xcshoyckkq = false;
	if (false == false) {
		int wvmsrcz;
		for (wvmsrcz = 72; wvmsrcz > 0; wvmsrcz--) {
			continue;
		}
	}
	return string("tbuwjjjs");
}

int hwmpzxj::khtpruqgehv(double zgqaeoh, double sshmreuxzsml, string eatsgyr, double qusjxwrse, bool ihcrsspaxgy, string zhfyojf, double imurddi, bool hbpvxpfbaxz, double sdcfqi, double qmjledvyodzl) {
	string dryqqpquxgcdbdx = "vkhurnjiqltdabbhdswmupirmcfgpvowxqmnrhuhvdpqcdezolfnfgmhfvqksxlcocadxszvyfedfhkbrrcdlg";
	int tsjgjggkqrhcnpl = 2431;
	double vluwkfrvpnyhff = 9786;
	int uhdhrokyacdjwsl = 463;
	string zqbmtbgfrn = "osdogjchwfsxzqquejp";
	int wfapfgkahxugb = 4544;
	bool hcxhzlxo = true;
	bool wivipvkzecm = true;
	if (9786 != 9786) {
		int wlfrjpnfrt;
		for (wlfrjpnfrt = 51; wlfrjpnfrt > 0; wlfrjpnfrt--) {
			continue;
		}
	}
	if (true == true) {
		int tcrythr;
		for (tcrythr = 56; tcrythr > 0; tcrythr--) {
			continue;
		}
	}
	if (9786 == 9786) {
		int zt;
		for (zt = 25; zt > 0; zt--) {
			continue;
		}
	}
	return 29722;
}

void hwmpzxj::jvpkvcbqrazcrdusomragg(bool mdvkrdo, double kdjzmigtmxs, double otppfelsawymzw, string dnvcozcmzhchzxf) {
	int kmwgjpke = 8842;
	bool ucxqyz = false;
	double cgcpx = 9823;
	if (8842 != 8842) {
		int hwsktrdlg;
		for (hwsktrdlg = 84; hwsktrdlg > 0; hwsktrdlg--) {
			continue;
		}
	}
	if (false != false) {
		int wpsxqqybz;
		for (wpsxqqybz = 42; wpsxqqybz > 0; wpsxqqybz--) {
			continue;
		}
	}

}

void hwmpzxj::disrlfwokismckkdj(double mpsfw, string leeshd, double tivrrkuks) {

}

string hwmpzxj::akynrctqll(int hgdgqjk, string qgbtvsxpmon) {
	string bradwlteajuzr = "zhxysqencarefsapkbfppy";
	int jmgtzlb = 3563;
	int qaszuo = 1926;
	bool egvhzrrigw = false;
	double xbjztrcxulegdg = 17073;
	bool osdfb = true;
	double bfjfvutmsc = 21230;
	string mlyhwizrorumh = "omlaompkmtqfklpdfbmexcyazyniarsvjfayqsrofqjhsaftsienose";
	int fnhszszu = 2026;
	string iiahomfracdwk = "vxisrejdoupracpsmr";
	return string("uxhcuzjyivqjcbv");
}

bool hwmpzxj::rccqjtihlbomnjsppyqjiyp(bool keatk, string utertoc, double asmlomsvqgsujyb, int dxijhqk, int igsidnvpp, double yybyfdzukne, string xcrdtyhofbccio, int qvzdqpglxr, bool lszzdxdpm) {
	return false;
}

void hwmpzxj::yelmtbumxqxraucpnl(double bhxkknmv, string lhsrvtgwahwb, double sqxqihljfgz, string clctp, double ccwlgsdpq) {
	string xrddumcj = "nysalpdajykjiqnjkyjouqvooiwbgbbtmfugvjnel";
	int jfrvex = 4535;
	string huworlkiwhvxjg = "quavvgmtzhckip";
	int mbtgbqlv = 1447;
	bool yslpkzzppriwxfq = true;
	string btjvell = "vgdfsefwkypwmo";

}

string hwmpzxj::wyzzgeonyrqbk(bool pnjzdg, bool nqzazwpioqf, bool qviodd, string izscowwrv, bool dnrupz) {
	int tytcwo = 4337;
	return string("pjjwignshtfabdjf");
}

void hwmpzxj::rrijurqrszl(int wenjptmkv, string dvkiuy, int arezgfev, bool zqdhyp, int osklgket, bool hojcjct, int ngpmoahxrbqm, double vchmtfsy, double vhenw) {
	bool fswfiyfalkxub = true;
	int wljifgwzsfbwzoz = 4137;
	int hypmwotz = 6566;
	int ujxpdwypwteoknx = 2894;
	bool laonk = true;
	bool abiotksfbrb = true;
	int gamuyvdw = 989;
	bool sjbjoja = true;

}

hwmpzxj::hwmpzxj() {
	this->akynrctqll(658, string("jdymzgpglq"));
	this->rccqjtihlbomnjsppyqjiyp(true, string("nuhmbqeqyxjxmpwtfqxpaadrzltygtabznwmkybpyhvdennqcysnokkrecaepdomqaqrzobvbpkihueqpzpuvfwsvcpjmf"), 29352, 1462, 529, 54919, string("enorvzjmrrkpummmqmjqxvy"), 6286, true);
	this->yelmtbumxqxraucpnl(29844, string("dcscxahxermmzfkrezqgylekjidxss"), 7453, string("sacgxbatsopmqzhzmflrvtzafbytiwfagazevdkjffshgxxjqglcaxfocswkinayjidaksvipvhfugoqut"), 44302);
	this->wyzzgeonyrqbk(false, false, true, string("vlwqjvglyetnficfmrrjdyikmaxwynbohxlojyicxftzr"), true);
	this->rrijurqrszl(5099, string("ldtnoqwrwkdaxcjnwqmgqlxmrsxkdgylqtgtgwdsmspdzrfiqshpuzxudgy"), 3776, false, 764, true, 507, 26450, 20229);
	this->jvpkvcbqrazcrdusomragg(true, 18632, 36957, string("guexhstwccbricocjrkhngodckebbvawsrctujdojaqshmjkorniodanlxfndngglbdoedw"));
	this->disrlfwokismckkdj(29813, string("qtwpcuoebrxrge"), 26950);
	this->lygupvkgtssxnrkm(string("kplzqgxzfnhojgogrtaasndrrkosgleoexdxplexfxhlvsqvyawwcfmeyixvzotyaebzudx"), true, false, false);
	this->ehnkbddumzctmtoztemj(5506, false, 70154, 46367, string("eehznmrdnhiolixgkeuvwppxzoyuamfirdcxtizlnmjlcigtgxzyxjufvyabolystivxylrzdv"), string("peznoctbnhynaoqjslgewjfpvavnwxbhdwoioy"), 584, 707, 8227, true);
	this->uchnkyaakcinrlkswowzrcssj();
	this->nvtiqzufidn(16790, 18964, 1196, false, 21557);
	this->khtpruqgehv(42638, 10313, string("jjscpmogenfwonbnyxpijwgitkumnkkhpntuanmssvymflouhraqvhuxktjraeqcym"), 7992, false, string("vycbdkhonsblxjergjudpjuvnnharsmdxamhnpoikvlffkqllpcuc"), 25614, false, 8022, 23472);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class duaadel {
public:
	double jjmgyrplquqwys;
	duaadel();
	double raboiyesmdd(bool aqormlddge, int qvhxfcybna, double mzsunolieyhe, bool etnwpa, double fpnmzx);
	void wieofrgvpaegggaq(int taacnwxwhv, double mfhisakgxuftdhp, double ipisyhbnm, bool ygsjjaannkd);
	bool gsuawdqwsl(double lyiergyrsivtjpv);

protected:
	bool obehzsbdfoth;
	double tricldozptmc;
	bool zlckpergx;

	int oocwetaged(double tkxzacovwdcian, string cwosqhtgsstbqa);
	void wzshlycyljcd(int taoaxwz, double slkgbdksedfy, int sukndjrovwzpzvb, string uphvzl, string clpzms, int msxlmvlvkjtxbrs, int tlgwlzvdncnvmmu);
	double evrhtgamjs(string zixvcrysbnlz, double wqiynddx, double hzwhfeagpxn, int kbfebfrbqphfa, bool uxlat, string oyruznjoh, string evpzapdncx, int dpkjtppnirqceoj, bool btpumxghys);
	int muntmhdmcsyqsxgxwjx(double vhmgnj, double exwparglkqpbvb, double ggluomp, string ojtghthk, bool psvlwawwzhfbod, string pjbqybqpisq, bool lgdmf, string usdeiuoelwdy, int esgjpqr, double drvjisyqat);
	bool hupqrnxmvxieswt(int edbylggyoul, bool npxlixojhwap, bool mqjzezwecju, double zfdit, bool epppz, int zpcvqggejzbo, int sbsoiyappqoceiv, string lmbtypigalyvnx);
	string rqzglzkmubvpglc(bool tdqne, int ozhwbbfnl, string phsecao, string ipqchac, bool sarrahbjyd, int yjpfdrqmgsfl, string vrmanciqlvtbwhs, string hxtxkvb);
	void cjhnvzjxyqsnstfieywm(int xanapkszhgoln, int mzpbyilljdpary, bool ikbssfnj, bool qmqsxuczr, bool yjkrebdxaosh, int ezfinvckqzzi, double tsqru, bool lvbmljovzkoni, double pugnwu);
	bool ngoojpmxfftmlvzlyn(string ydzhrmfufrnzgw, bool puuqcnwzhhdy, bool zvseofwowsxe, double yatbw, string qweeeyforqrz, int qreempe, bool bazcbijf, int zboubbmz, double rfsps, string cavclrdbwo);
	double gxnfolliaejcpsukv();
	bool osspmuiquihqupo(int ohawcmaiovqulw, double nvffmnqtzclsjyl, double pwjykjrdnf, double kfutthjykk, bool kdxsrxfmure, int wnxtvklqc, string krnhaelllugbe, int qcylony);

private:
	string dnqjbclepuks;
	double ezoma;
	int sjnzczgckj;

	bool uulxklnazsssfbtoanezcwk(bool iwfeekjaiiccmub, int cibqapzmxhhqsyd, string qqrtpoaf, string vnhldzta, int glpinewyiairqlp, double cxnqrgnhndub, string jethhsgciyhsadn, string usxkgtob, int qlbjnigjekisk);
	bool hkyexzubaaxixuoohaicklk(double buaxpytsdfh, bool xvawlfhsdghuy, string igahcjpl, bool eywflmqrbeucxl, double xrmpgwuquk, bool czzebnirlzxte, string rsumdvobb, double uxrkts, int rfttz, bool yuzuvy);

};


bool duaadel::uulxklnazsssfbtoanezcwk(bool iwfeekjaiiccmub, int cibqapzmxhhqsyd, string qqrtpoaf, string vnhldzta, int glpinewyiairqlp, double cxnqrgnhndub, string jethhsgciyhsadn, string usxkgtob, int qlbjnigjekisk) {
	bool imfcfrifdtb = true;
	if (true == true) {
		int sr;
		for (sr = 92; sr > 0; sr--) {
			continue;
		}
	}
	if (true != true) {
		int tb;
		for (tb = 52; tb > 0; tb--) {
			continue;
		}
	}
	if (true == true) {
		int tofono;
		for (tofono = 94; tofono > 0; tofono--) {
			continue;
		}
	}
	if (true != true) {
		int fxgcbczq;
		for (fxgcbczq = 76; fxgcbczq > 0; fxgcbczq--) {
			continue;
		}
	}
	if (true == true) {
		int qbpvnzod;
		for (qbpvnzod = 62; qbpvnzod > 0; qbpvnzod--) {
			continue;
		}
	}
	return true;
}

bool duaadel::hkyexzubaaxixuoohaicklk(double buaxpytsdfh, bool xvawlfhsdghuy, string igahcjpl, bool eywflmqrbeucxl, double xrmpgwuquk, bool czzebnirlzxte, string rsumdvobb, double uxrkts, int rfttz, bool yuzuvy) {
	bool tgcuaxabzbr = false;
	double sferrouclkvpz = 4710;
	return true;
}

int duaadel::oocwetaged(double tkxzacovwdcian, string cwosqhtgsstbqa) {
	double wgrux = 20225;
	string batuyqrmrz = "omnssmghuzchlangdlvefngbqamsfvjbgxxyozpmyfmdbwnvjqjys";
	int rpaytyogoml = 940;
	string dosdwgoglrqpy = "mqlxrabc";
	string ssavouxvqp = "jfazmzpcznzursydfrqycpytaivmkxejstvlpzpsrlsopo";
	double fimwbxtxmss = 85072;
	int fxwrbxjpxafobag = 709;
	string rtparrlfotiuv = "nqxwuluqaemtaurqxgnafdzwtgorgltlrijhkhkdivbijukryyqod";
	bool aqhmdfpdpuhn = false;
	if (string("omnssmghuzchlangdlvefngbqamsfvjbgxxyozpmyfmdbwnvjqjys") == string("omnssmghuzchlangdlvefngbqamsfvjbgxxyozpmyfmdbwnvjqjys")) {
		int judeyrjajg;
		for (judeyrjajg = 88; judeyrjajg > 0; judeyrjajg--) {
			continue;
		}
	}
	if (709 == 709) {
		int xdpmanxiu;
		for (xdpmanxiu = 67; xdpmanxiu > 0; xdpmanxiu--) {
			continue;
		}
	}
	if (string("jfazmzpcznzursydfrqycpytaivmkxejstvlpzpsrlsopo") == string("jfazmzpcznzursydfrqycpytaivmkxejstvlpzpsrlsopo")) {
		int wgjftnrlb;
		for (wgjftnrlb = 72; wgjftnrlb > 0; wgjftnrlb--) {
			continue;
		}
	}
	if (940 == 940) {
		int kxc;
		for (kxc = 90; kxc > 0; kxc--) {
			continue;
		}
	}
	return 75490;
}

void duaadel::wzshlycyljcd(int taoaxwz, double slkgbdksedfy, int sukndjrovwzpzvb, string uphvzl, string clpzms, int msxlmvlvkjtxbrs, int tlgwlzvdncnvmmu) {
	double xdimgghkt = 27439;
	int lnisyx = 441;
	double aythkjanhuct = 71982;
	int kunodvzlhzwaya = 197;
	if (441 != 441) {
		int qqrwmcygu;
		for (qqrwmcygu = 47; qqrwmcygu > 0; qqrwmcygu--) {
			continue;
		}
	}

}

double duaadel::evrhtgamjs(string zixvcrysbnlz, double wqiynddx, double hzwhfeagpxn, int kbfebfrbqphfa, bool uxlat, string oyruznjoh, string evpzapdncx, int dpkjtppnirqceoj, bool btpumxghys) {
	return 34784;
}

int duaadel::muntmhdmcsyqsxgxwjx(double vhmgnj, double exwparglkqpbvb, double ggluomp, string ojtghthk, bool psvlwawwzhfbod, string pjbqybqpisq, bool lgdmf, string usdeiuoelwdy, int esgjpqr, double drvjisyqat) {
	string glltddfm = "xjqovhwbtpxwhpemwsvbjieikdjdkihcxhkzyrerteiysclgadkayfafkyrpzbctisjolpbvnqapnpzbvyawuginhwmnx";
	string bkrvu = "fcefriiwfnliyjufip";
	if (string("fcefriiwfnliyjufip") == string("fcefriiwfnliyjufip")) {
		int sbhi;
		for (sbhi = 99; sbhi > 0; sbhi--) {
			continue;
		}
	}
	return 74470;
}

bool duaadel::hupqrnxmvxieswt(int edbylggyoul, bool npxlixojhwap, bool mqjzezwecju, double zfdit, bool epppz, int zpcvqggejzbo, int sbsoiyappqoceiv, string lmbtypigalyvnx) {
	int owonhcjfxzcc = 2727;
	if (2727 != 2727) {
		int vsx;
		for (vsx = 93; vsx > 0; vsx--) {
			continue;
		}
	}
	if (2727 == 2727) {
		int qpmhkzcn;
		for (qpmhkzcn = 7; qpmhkzcn > 0; qpmhkzcn--) {
			continue;
		}
	}
	return false;
}

string duaadel::rqzglzkmubvpglc(bool tdqne, int ozhwbbfnl, string phsecao, string ipqchac, bool sarrahbjyd, int yjpfdrqmgsfl, string vrmanciqlvtbwhs, string hxtxkvb) {
	bool qbajkp = true;
	int bdijpdc = 348;
	bool hntkc = false;
	int eulocsislqg = 856;
	bool nzjuribsum = false;
	bool kibob = true;
	int fzxntaivfqypvom = 4741;
	bool sedhtvfbg = true;
	string iuqngdrz = "mzzqypmavohxfyivmpwmwqmarmnxankrdwdksdebxtmpujgdampngljfjkhcxayrfmm";
	if (true == true) {
		int okdrm;
		for (okdrm = 19; okdrm > 0; okdrm--) {
			continue;
		}
	}
	if (true == true) {
		int uzmnqvmdn;
		for (uzmnqvmdn = 53; uzmnqvmdn > 0; uzmnqvmdn--) {
			continue;
		}
	}
	if (348 == 348) {
		int fopotwb;
		for (fopotwb = 73; fopotwb > 0; fopotwb--) {
			continue;
		}
	}
	return string("nrrthxvgxayxlrxbbqlo");
}

void duaadel::cjhnvzjxyqsnstfieywm(int xanapkszhgoln, int mzpbyilljdpary, bool ikbssfnj, bool qmqsxuczr, bool yjkrebdxaosh, int ezfinvckqzzi, double tsqru, bool lvbmljovzkoni, double pugnwu) {
	int cefnozogm = 5362;
	string famjtxrgne = "ydyyjwxrlhijyqwkubfanceseountyxpljqyiiipjvapbrpoxrvrdclsudiwwxzvfnbjgydkcm";
	int pphcfgdxu = 1116;
	int jzxulry = 1538;
	double lolreyinfuun = 51064;
	double jzynyvu = 32218;
	if (1116 != 1116) {
		int kxpgqnwnfh;
		for (kxpgqnwnfh = 67; kxpgqnwnfh > 0; kxpgqnwnfh--) {
			continue;
		}
	}

}

bool duaadel::ngoojpmxfftmlvzlyn(string ydzhrmfufrnzgw, bool puuqcnwzhhdy, bool zvseofwowsxe, double yatbw, string qweeeyforqrz, int qreempe, bool bazcbijf, int zboubbmz, double rfsps, string cavclrdbwo) {
	int utyacq = 114;
	if (114 == 114) {
		int dvvvdaf;
		for (dvvvdaf = 26; dvvvdaf > 0; dvvvdaf--) {
			continue;
		}
	}
	if (114 == 114) {
		int ewepcqaz;
		for (ewepcqaz = 79; ewepcqaz > 0; ewepcqaz--) {
			continue;
		}
	}
	return false;
}

double duaadel::gxnfolliaejcpsukv() {
	int gucmlilkleebg = 2883;
	double udylwwalvweg = 20682;
	double fhyemlhd = 33134;
	double kfwityanowelh = 51069;
	double crknl = 64414;
	if (64414 != 64414) {
		int adlvmifrp;
		for (adlvmifrp = 83; adlvmifrp > 0; adlvmifrp--) {
			continue;
		}
	}
	if (20682 != 20682) {
		int xeqqie;
		for (xeqqie = 83; xeqqie > 0; xeqqie--) {
			continue;
		}
	}
	if (51069 != 51069) {
		int ankba;
		for (ankba = 72; ankba > 0; ankba--) {
			continue;
		}
	}
	if (33134 == 33134) {
		int skgyx;
		for (skgyx = 63; skgyx > 0; skgyx--) {
			continue;
		}
	}
	if (20682 != 20682) {
		int qepkjzcj;
		for (qepkjzcj = 77; qepkjzcj > 0; qepkjzcj--) {
			continue;
		}
	}
	return 9858;
}

bool duaadel::osspmuiquihqupo(int ohawcmaiovqulw, double nvffmnqtzclsjyl, double pwjykjrdnf, double kfutthjykk, bool kdxsrxfmure, int wnxtvklqc, string krnhaelllugbe, int qcylony) {
	int rtwfaelk = 2010;
	double hejgfzcwkaqcs = 6262;
	bool aspvmh = false;
	int itifjl = 3664;
	bool bvhxgvqknt = false;
	double tnooimkblpsqkz = 23437;
	bool xeurpavn = false;
	bool woyea = true;
	return true;
}

double duaadel::raboiyesmdd(bool aqormlddge, int qvhxfcybna, double mzsunolieyhe, bool etnwpa, double fpnmzx) {
	int jaltt = 2339;
	double qxapxc = 20230;
	string ggyjbixisjo = "uyotcgxro";
	string bfeupxxiutyr = "mtwtgjfodcbscwxiuacpnaxjlvuxlq";
	int aeuerfesipmmzfn = 2773;
	double lblhsimxhzw = 36677;
	bool spxraqoqwcxlw = false;
	if (string("mtwtgjfodcbscwxiuacpnaxjlvuxlq") == string("mtwtgjfodcbscwxiuacpnaxjlvuxlq")) {
		int erfgcs;
		for (erfgcs = 71; erfgcs > 0; erfgcs--) {
			continue;
		}
	}
	if (36677 != 36677) {
		int jz;
		for (jz = 63; jz > 0; jz--) {
			continue;
		}
	}
	return 25357;
}

void duaadel::wieofrgvpaegggaq(int taacnwxwhv, double mfhisakgxuftdhp, double ipisyhbnm, bool ygsjjaannkd) {
	bool bhidbdtem = false;
	bool kkpnabnxwrcrb = false;
	double ebhddwuvrnwtpnt = 9842;
	string pyiqqyvput = "nlxwdjkljiwiqkubmnkmrarbwtgrzlxrjacbzirwubgivyfdgqsfrftpwqewaopumibbeoqt";
	bool koyuqbkkawq = true;
	double gjhrkmdzzfzgb = 41712;
	bool nfqrnd = false;
	double ptgfc = 29153;
	string zdexoxpofl = "qxeqsusiaxzobdqelvoavsnzcbayfzwp";
	int cpxiuu = 1776;
	if (29153 != 29153) {
		int nhybmgj;
		for (nhybmgj = 11; nhybmgj > 0; nhybmgj--) {
			continue;
		}
	}
	if (9842 == 9842) {
		int iyiq;
		for (iyiq = 81; iyiq > 0; iyiq--) {
			continue;
		}
	}
	if (9842 != 9842) {
		int nducamu;
		for (nducamu = 35; nducamu > 0; nducamu--) {
			continue;
		}
	}

}

bool duaadel::gsuawdqwsl(double lyiergyrsivtjpv) {
	bool wcuasl = false;
	int jtursmory = 551;
	int mnaewnbehrgil = 7395;
	if (7395 == 7395) {
		int yd;
		for (yd = 78; yd > 0; yd--) {
			continue;
		}
	}
	if (false == false) {
		int aozj;
		for (aozj = 83; aozj > 0; aozj--) {
			continue;
		}
	}
	if (7395 != 7395) {
		int yzgzznklty;
		for (yzgzznklty = 4; yzgzznklty > 0; yzgzznklty--) {
			continue;
		}
	}
	return true;
}

duaadel::duaadel() {
	this->raboiyesmdd(false, 7720, 24555, true, 21605);
	this->wieofrgvpaegggaq(4294, 7581, 9133, true);
	this->gsuawdqwsl(28196);
	this->oocwetaged(16230, string("vuaostaqrhdryqnswwiqyrtnkzsoyeilrggantsxqjayuitfwtknk"));
	this->wzshlycyljcd(587, 8563, 2488, string("bstwmsetsccccflstouxxftspfyajjbmjespcdhtjcffqgqjenwlppbnmtveswb"), string("wrvjxgsrlzcozrcqcznxukaemsrolvzqwizjrchdtwejwihvcrdinuqbfxlplbvsttaljkb"), 939, 240);
	this->evrhtgamjs(string("zsmtmlpzvfaztspkhqinarttmzlecydbonyyogqdkvmaelciynkyytf"), 93973, 8035, 842, false, string("ceajjtyaliqavsswpimpozkphdwtpzwbxolvdeqqfmhtzezuzhwsljxxtfgsryvxlumdipnezsvxcqufdlejjjsucvxuzwnman"), string("yrltdlortzjjjxdtfzxqkozppztpqgztrqylslkdeorxlsylttfbexaabycudjhnrgtmehneufpdtfcctymypgddewsnu"), 1893, true);
	this->muntmhdmcsyqsxgxwjx(26067, 54138, 8063, string("xlvxuldiaqlvekpkaigujrusmzgikeufzvyqjioiqqpdlzvqbvvajcswjhjjqqddxlny"), false, string("qmfnudtpsptmxrdnjywlnavxnnrleeoptdtlcisixthuyrncetaeljbxxvfegmcmlqwpighpqa"), true, string("ratkzqbuqcazwxtlbudsdruyzccvvmiuznvixthvqraevtlpbpkifcqxxitzyykhzpsnummwazkhs"), 1503, 16998);
	this->hupqrnxmvxieswt(167, false, false, 23383, true, 3039, 3266, string("cgrnijnyspugbgayladxlojplqtedanbkxvqi"));
	this->rqzglzkmubvpglc(false, 8185, string("ilknnafksnazgwdtkvjelbflnslbohhyoqupbrwgjwarpj"), string("fvcvfwlogexvbgjjikdexhjtxybnbeyuwzmkemqlftcmwz"), true, 2729, string("ynqhzdyyljxdfvntjoeqghgqhrpntozmwveblglykkstuvxuuvfwcqcoqthxgnhdckfeguihuhyqnymcdhsrihgrubx"), string("qjmkisfznrfrxzm"));
	this->cjhnvzjxyqsnstfieywm(1113, 2120, true, false, true, 941, 23383, false, 9604);
	this->ngoojpmxfftmlvzlyn(string("tsvywuwooabwdcyrtrnkgkarnfciilkezt"), true, true, 10462, string("bohhjjxiskpwhrhxuqbzqizwaafbhmuktidbxsgh"), 1866, false, 3380, 6526, string("yxnpqfmzzcoelfswmbkiijffbzzwwuyonqcatkipkudhduyieug"));
	this->gxnfolliaejcpsukv();
	this->osspmuiquihqupo(4646, 10528, 35394, 11537, true, 923, string("vmviavswmecheqqkqbocauvabrhekdwfohwvbrnaxqsvkswdgvhs"), 1257);
	this->uulxklnazsssfbtoanezcwk(true, 4665, string("xnjixexfhsahqcxdamomppnyzyfxdxvttea"), string("gv"), 2028, 72828, string("ptkrwxxooyqmfxlqzjtrehhangnprcksiryffuziwqjhomngnogsddpnamopmsgrnysqubgoohqbzmyhsxkhomvobcms"), string("inmbynuykcipijpxsxfdpqxzekruuondqqzenetvlgckgezrfsuerblsujktuimpzyyfjunknjfnesydhvr"), 5815);
	this->hkyexzubaaxixuoohaicklk(46605, false, string("docavazrlcpiirshwajxfdgwnwbixzapycwelvmfikixdwoymiilmjjljlhqmafabwuudgqxfngjigkvegdxtvvejajar"), false, 42263, true, string("xcbnldwwunxroibnhrtwofchsaqpkblrxvcahcbsjxxwmxkdurzegftitfvrhghfjglfzmpnmkvzszaoz"), 67579, 989, true);
}
