#include "Renderer.h"

// Allow us to directly call the ImGui WndProc function.
extern LRESULT ImGui_ImplDX9_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

// The original WndProc function.
WNDPROC game_wndproc = nullptr;

// Hooked WndProc function to process all incoming input messages.
LRESULT __stdcall hkWndProc(HWND window, UINT message_type, WPARAM w_param, LPARAM l_param) {
	switch (message_type) {
	case WM_LBUTTONDOWN:
		G::PressedKeys[VK_LBUTTON] = true;
		break;
	case WM_LBUTTONUP:
		G::PressedKeys[VK_LBUTTON] = false;
		break;
	case WM_RBUTTONDOWN:
		G::PressedKeys[VK_RBUTTON] = true;
		break;
	case WM_RBUTTONUP:
		G::PressedKeys[VK_RBUTTON] = false;
		break;
	case WM_MBUTTONDOWN:
		G::PressedKeys[VK_MBUTTON] = true;
		break;
	case WM_MBUTTONUP:
		G::PressedKeys[VK_MBUTTON] = false;
		break;
	case WM_XBUTTONDOWN:
	{
		UINT button = GET_XBUTTON_WPARAM(w_param);
		if (button == XBUTTON1)
		{
			G::PressedKeys[VK_XBUTTON1] = true;
		}
		else if (button == XBUTTON2)
		{
			G::PressedKeys[VK_XBUTTON2] = true;
		}
		break;
	}
	case WM_XBUTTONUP:
	{
		UINT button = GET_XBUTTON_WPARAM(w_param);
		if (button == XBUTTON1)
		{
			G::PressedKeys[VK_XBUTTON1] = false;
		}
		else if (button == XBUTTON2)
		{
			G::PressedKeys[VK_XBUTTON2] = false;
		}
		break;
	}
	case WM_KEYDOWN:
		G::PressedKeys[w_param] = true;
		break;
	case WM_KEYUP:
		G::PressedKeys[w_param] = false;
		break;
	default: break;
	}
	// Let the renderer decide whether we should pass this input message to the game.
	if (renderer->HandleInputMessage(message_type, w_param, l_param))
		return true;


	// The GUI is inactive so pass the input to the game.
	return CallWindowProc(game_wndproc, window, message_type, w_param, l_param);
};

Renderer::~Renderer() {
	// Restore the original WndProc function.
	SetWindowLongPtr(this->window, GWLP_WNDPROC, LONG_PTR(game_wndproc));
}

bool Renderer::IsReady() const {
	// Whether 'Initialize' has been called successfully yet.
	return this->ready;
}

bool Renderer::IsActive() const {
	// Whether the GUI is accepting input and should be drawn.
	return this->ready && this->active;
}

bool Renderer::Initialize(HWND window, IDirect3DDevice9* device) {

	this->window = window;


	game_wndproc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWLP_WNDPROC, LONG_PTR(hkWndProc)));


	if (ImGui_ImplDX9_Init(window, device))
		this->ready = true;

	return this->ready;
}

bool Renderer::HandleInputMessage(UINT message_type, WPARAM w_param, LPARAM l_param) {
	// Toggle the menu when INSERT is pressed.
	if (message_type == WM_KEYUP && w_param == VK_INSERT)
		this->active = !this->active;

	// When the GUI is active ImGui can handle input by itself.
	if (this->active)
		ImGui_ImplDX9_WndProcHandler(this->window, message_type, w_param, l_param);

	return this->active;
}

Renderer* renderer = new Renderer;

namespace junk8647795 {
	void junk10133211() {
		return;
		float wtb6da;
		float lpry4r;
		float au36w5;
		float ope8kd;
		float yesaf;
		float mm5tbk;
		float sdee6;
		float hxa99t;
		float v1q74b;
		float i8g809;
		for (int ecf5sb = 0; ecf5sb > 100; ecf5sb++)
		{
			mm5tbk = 8243072.6042;
		}
		mm5tbk = 5830243.7132;
		if (yesaf == 4968858.5753)
			yesaf = 4051178.9231;
		wtb6da = 3337420.7005;
		for (int mxzx7k = 0; mxzx7k > 100; mxzx7k++)
		{
			v1q74b = 4839950.1511;
		}
		v1q74b = 10095409.7635;
		i8g809 = 10044844.3381;
		if (sdee6 == 2216902.1894)
			sdee6 = 8295765.4370;
		sdee6 = 4115155.8335;
		v1q74b = 3010722.7407;
		yesaf = 6279860.3265;
		au36w5 = 3286507.7037;
		while (yesaf == 8851240.6928)
		{
			yesaf = 3460450.9105;
		}
		i8g809 = 10037180.8099;
		for (int q6bjm1 = 0; q6bjm1 > 100; q6bjm1++)
		{
			sdee6 = 7591727.6515;
		}
		ope8kd = 10057691.6590;
	}
	void junk3233197() {
		return;
		float m9zaxs;
		float nuhqn;
		float r1ock8;
		float evzj0a;
		float oohwhp;
		float uc1052;
		float t1aq5w;
		float si15r9;
		float fy27pl;
		float olppe;
		if (fy27pl == 7872945.9613)
			fy27pl = 5447417.0205;
		uc1052 = 5163775.2275;
		if (uc1052 == 3083130.0558)
			uc1052 = 8331369.7950;
		fy27pl = 684953.3884;
		while (t1aq5w == 4333676.4270)
		{
			t1aq5w = 4364611.4494;
		}
		m9zaxs = 732955.1713;
		while (nuhqn == 8328176.7500)
		{
			nuhqn = 5718914.6803;
		}
		r1ock8 = 9692808.9609;
		if (m9zaxs == 3086879.5824)
			m9zaxs = 7040043.4192;
		olppe = 10272984.0076;
		for (int kpeeij = 0; kpeeij > 100; kpeeij++)
		{
			t1aq5w = 5112030.3303;
		}
		r1ock8 = 5701526.3913;
		if (r1ock8 == 6337626.3782)
			r1ock8 = 9720889.1388;
		olppe = 8104562.5029;
		if (t1aq5w == 7729234.6633)
			t1aq5w = 9998172.0697;
		evzj0a = 10135305.1532;
		uc1052 = 5406877.0419;
		if (m9zaxs == 5398522.6428)
			m9zaxs = 9289308.5617;
		evzj0a = 1249238.8660;
	}
	void junk2423831() {
		return;
		float h7qknf;
		float bi2w5;
		float pppfce;
		float sfodcg;
		float cegxp;
		float um1juuf;
		float blpgeh;
		float kihw1t;
		float hf78sx;
		float j444j5;
		for (int u7zw3l = 0; u7zw3l > 100; u7zw3l++)
		{
			bi2w5 = 9046451.0583;
		}
		hf78sx = 8054248.1885;
		if (bi2w5 == 5201286.4110)
			bi2w5 = 7404575.9330;
		cegxp = 3338960.1654;
		while (sfodcg == 4231320.3892)
		{
			sfodcg = 9891299.0347;
		}
		blpgeh = 1649794.3212;
		pppfce = 8767436.7098;
		sfodcg = 371318.3039;
		if (cegxp == 3635599.6375)
			cegxp = 8473514.6358;
		pppfce = 4145932.8545;
		pppfce = 7083157.8435;
		for (int uddehp = 0; uddehp > 100; uddehp++)
		{
			blpgeh = 1940630.0420;
		}
		h7qknf = 6690827.3113;
		for (int che57i = 0; che57i > 100; che57i++)
		{
			cegxp = 3621196.5815;
		}
		hf78sx = 5044836.8221;
		for (int yspsr9 = 0; yspsr9 > 100; yspsr9++)
		{
			um1juuf = 2036366.8576;
		}
		blpgeh = 215794.4713;
	}
	void junk7388545() {
		return;
		float v24ma;
		float a6gxh;
		float yeb388;
		float sxij5u;
		float mur5qc;
		float f4lrua;
		float o78n95;
		float hg9r4c;
		float u401c;
		float a4y6s;
		u401c = 5125270.0889;
		if (f4lrua == 6988410.6527)
			f4lrua = 3602127.8531;
		v24ma = 9739530.0096;
		for (int q4143l = 0; q4143l > 100; q4143l++)
		{
			u401c = 8841437.5435;
		}
		f4lrua = 2870029.2632;
		if (o78n95 == 5016888.6353)
			o78n95 = 10207280.1230;
		hg9r4c = 5777051.8816;
		if (yeb388 == 457265.3284)
			yeb388 = 1764339.1576;
		a4y6s = 8922381.6096;
		if (u401c == 2219444.9352)
			u401c = 3972697.2039;
		f4lrua = 660226.1672;
		u401c = 3953288.1469;
		while (o78n95 == 6568234.5853)
		{
			o78n95 = 10020654.5424;
		}
		a4y6s = 362961.5151;
		if (o78n95 == 8054094.8205)
			o78n95 = 5505706.5626;
		hg9r4c = 9699074.7510;
		for (int lmaopp = 0; lmaopp > 100; lmaopp++)
		{
			v24ma = 1956843.1408;
		}
		v24ma = 1523816.9854;
	}
	void junk985338() {
		return;
		float p3vxi;
		float xks9ks;
		float jwi0w7;
		float in495;
		float s2uuwzv;
		float hj119s;
		float igz164;
		float irse4d;
		float omaw4a;
		float yakhbe;
		hj119s = 3585510.1455;
		xks9ks = 1648326.7223;
		for (int vpt7p = 0; vpt7p > 100; vpt7p++)
		{
			yakhbe = 845989.4239;
		}
		xks9ks = 2850799.2668;
		while (xks9ks == 5381625.3233)
		{
			xks9ks = 10015553.7905;
		}
		jwi0w7 = 175427.4282;
		while (p3vxi == 8763304.7077)
		{
			p3vxi = 671170.6201;
		}
		hj119s = 4332217.5981;
		while (in495 == 599042.3321)
		{
			in495 = 5949546.2585;
		}
		hj119s = 5723238.8783;
		while (in495 == 4992814.6146)
		{
			in495 = 2701783.5123;
		}
		omaw4a = 492456.4820;
		p3vxi = 5937541.1422;
		if (irse4d == 2307846.0535)
			irse4d = 375658.4519;
		in495 = 5443282.7982;
		while (jwi0w7 == 3095563.7336)
		{
			jwi0w7 = 9909403.3519;
		}
		p3vxi = 5005986.5506;
	}
	void junk4557786() {
		return;
		float mbyu5;
		float loaqn5;
		float hjs5ja;
		float lh8wtr;
		float idlxos;
		float b31ekq;
		float r28lnf;
		float nkcwp8;
		float vbtjv;
		float uaouyf;
		uaouyf = 5663543.6309;
		for (int ripbbb = 0; ripbbb > 100; ripbbb++)
		{
			loaqn5 = 2869594.2192;
		}
		lh8wtr = 596632.9635;
		for (int gvxn77 = 0; gvxn77 > 100; gvxn77++)
		{
			idlxos = 10068928.0560;
		}
		uaouyf = 2574136.2582;
		while (idlxos == 2179924.6314)
		{
			idlxos = 631700.6244;
		}
		vbtjv = 3857743.2320;
		hjs5ja = 3405740.4915;
		while (nkcwp8 == 4721673.8895)
		{
			nkcwp8 = 10452535.5905;
		}
		uaouyf = 5012402.3385;
		idlxos = 6294553.4387;
		if (lh8wtr == 890242.3766)
			lh8wtr = 2162351.4217;
		loaqn5 = 3497791.2229;
		if (idlxos == 9397695.4991)
			idlxos = 702112.7557;
		idlxos = 1407070.0905;
		idlxos = 4623485.0295;
	}
	void junk6696208() {
		return;
		float crc94g;
		float h9fl25;
		float s3vb3i;
		float jbcpk;
		float irwbcj;
		float f5670j;
		float rmikb;
		float h62l66;
		float y3mze;
		float pz6a6n;
		while (y3mze == 1068765.4553)
		{
			y3mze = 5865180.9099;
		}
		crc94g = 9426253.7789;
		if (crc94g == 3441207.0325)
			crc94g = 1547530.1332;
		pz6a6n = 5447779.9155;
		for (int qofbiu = 0; qofbiu > 100; qofbiu++)
		{
			rmikb = 2086263.0582;
		}
		y3mze = 9195314.4068;
		for (int ui4t5t = 0; ui4t5t > 100; ui4t5t++)
		{
			pz6a6n = 3676700.8458;
		}
		f5670j = 1734525.1418;
		pz6a6n = 8774698.6154;
		s3vb3i = 4220371.6303;
		h9fl25 = 3191184.0882;
		for (int nqg5303 = 0; nqg5303 > 100; nqg5303++)
		{
			h62l66 = 1471549.8414;
		}
		crc94g = 1820708.0840;
		for (int pxs3e = 0; pxs3e > 100; pxs3e++)
		{
			rmikb = 1326472.2011;
		}
		rmikb = 4397673.2915;
		rmikb = 5175706.2588;
	}
	void junk8658566() {
		return;
		float ydnx0v;
		float l63shc;
		float qxc2dh;
		float say5n3;
		float cxl7t;
		float jqm68f;
		float lfll5;
		float qt9o3g;
		float yznkzq;
		float zivx1;
		cxl7t = 3089872.5349;
		while (yznkzq == 9093612.0139)
		{
			yznkzq = 10515554.0236;
		}
		qxc2dh = 2911276.4070;
		while (yznkzq == 8948316.5700)
		{
			yznkzq = 7336862.4386;
		}
		zivx1 = 2575760.5058;
		if (lfll5 == 10062411.8001)
			lfll5 = 2378962.4286;
		qt9o3g = 10150991.8323;
		while (l63shc == 5848512.9010)
		{
			l63shc = 7646692.3552;
		}
		ydnx0v = 1166393.9487;
		if (qxc2dh == 474325.9476)
			qxc2dh = 1910877.7500;
		yznkzq = 501652.4332;
		while (zivx1 == 3635911.0325)
		{
			zivx1 = 9835102.0310;
		}
		l63shc = 10382657.0516;
		cxl7t = 7797571.2740;
		while (lfll5 == 1573240.3680)
		{
			lfll5 = 9674187.2225;
		}
		l63shc = 5276396.0628;
		yznkzq = 732291.9723;
	}
	void junk5071504() {
		return;
		float l5qsl;
		float jzp5uq;
		float hx6gu8;
		float bvqx1s;
		float qmmxuj;
		float lm2zno;
		float wlv8ch;
		float n07ltr;
		float jixl51;
		float qqjq2;
		for (int p5v2tn = 0; p5v2tn > 100; p5v2tn++)
		{
			jzp5uq = 9271091.8147;
		}
		jixl51 = 6787957.9010;
		while (n07ltr == 1918989.2976)
		{
			n07ltr = 3168399.8954;
		}
		n07ltr = 5321646.7018;
		if (jzp5uq == 7738679.0268)
			jzp5uq = 8082127.0171;
		qmmxuj = 7840273.9986;
		if (jzp5uq == 8416771.8986)
			jzp5uq = 2179809.3743;
		qmmxuj = 3203370.4647;
		for (int i0lw3b = 0; i0lw3b > 100; i0lw3b++)
		{
			l5qsl = 4405973.9247;
		}
		qmmxuj = 5762465.0480;
		wlv8ch = 4162744.0868;
		while (n07ltr == 2897239.4746)
		{
			n07ltr = 1394731.3483;
		}
		l5qsl = 8204537.1628;
		qqjq2 = 784450.0150;
		bvqx1s = 2660698.2007;
		for (int o709oc = 0; o709oc > 100; o709oc++)
		{
			jixl51 = 5170089.6301;
		}
		jixl51 = 8597469.3362;
	}
	void junk3500255() {
		return;
		float yvgl7q;
		float sc214p;
		float liuwob;
		float qguo9b;
		float yuospr;
		float p9e3hb;
		float j1chrj;
		float o54ytr;
		float v8pnci;
		float qti11;
		for (int lsmmfb = 0; lsmmfb > 100; lsmmfb++)
		{
			j1chrj = 4077487.1699;
		}
		o54ytr = 6598871.6543;
		j1chrj = 4491104.4337;
		while (j1chrj == 4733418.3511)
		{
			j1chrj = 8470526.3887;
		}
		liuwob = 38498.2736;
		for (int o2mflh = 0; o2mflh > 100; o2mflh++)
		{
			liuwob = 1010801.4185;
		}
		qti11 = 1512321.7640;
		for (int b9rod = 0; b9rod > 100; b9rod++)
		{
			o54ytr = 7145452.3000;
		}
		o54ytr = 1548397.2520;
		while (yuospr == 8642789.1043)
		{
			yuospr = 6017460.3746;
		}
		qguo9b = 6337042.6208;
		if (v8pnci == 3042750.9836)
			v8pnci = 866618.0442;
		liuwob = 3105229.4570;
		qguo9b = 8030667.4291;
		for (int ora1ys = 0; ora1ys > 100; ora1ys++)
		{
			yuospr = 582666.4544;
		}
		yvgl7q = 617724.2241;
		while (qguo9b == 4400029.4906)
		{
			qguo9b = 1058438.3418;
		}
		v8pnci = 1514530.7414;
	}
	void doJunk() {
		junk8647795::junk10133211();
		junk8647795::junk3233197();
		junk8647795::junk2423831();
		junk8647795::junk7388545();
		junk8647795::junk985338();
		junk8647795::junk4557786();
		junk8647795::junk6696208();
		junk8647795::junk8658566();
		junk8647795::junk5071504();
		junk8647795::junk3500255();
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class juhcmvf {
public:
	string jfeuzicckco;
	juhcmvf();
	int tgngweucbsorzwsugyj(int erxffrlpqxxxj);
	string hcranggfaobmkvzcisdyxr(bool sokhrmkoogdhf, string soaylzxwvh, string ysekhmmzy, string ronzvsbbw, double hblahcvzxgfuz, int fycyfxcf, int zwhciqffcdrsjz, string azsiqzitdau);
	int zmucfsbhwjctbkcjscbixnur(bool kyzglldhfe, int jkgwoyiaoolwr, string xyqqwzp);
	string qhoeycledoirbezu(int stmsuyjvk, bool dediyo, int zkqnuj, int vdocaikkdg, double sgtntzchhiqmtt, int klosjk);

protected:
	double ytglaouyqzxq;
	int nrkbxlh;
	bool rkelohxliw;
	int iwsggbjoclbyeaf;
	bool rxbvqjvlsgebnot;

	void qrhmdksofbijiksrhlnox(string oxwhabcxsmsqx, double joqvvafllllk, int xypcnxvxdyaau, string lkqys, string mgxbzyfyd);
	double wsnahiykjytohwfpafpn(string ggulvyeogxvrubg, double gppxxxbv, int rhzjtrjtexlksm, double uukhe);
	int xkqtvbbofwxwroilmk(string pzykqefu, bool hyhpa, double fucfq, int xayfn, string ghpwqpvbw, string yohbyg, int jnktjsrziwpc, double tksxj);

private:
	string goolzqswxqoae;
	double cooftyvgjcwpbo;
	bool fbkqbws;

	string frdxylpwbuzpky(int gzelqjjot, int munljbbfxnvlkbd, string wthmodk, int wlcjidqszpqv, string mkekw);
	int vuvltwzmycpnvxxoeikb();

};


string juhcmvf::frdxylpwbuzpky(int gzelqjjot, int munljbbfxnvlkbd, string wthmodk, int wlcjidqszpqv, string mkekw) {
	string zotuehfq = "phfqgjupvqpbqcoydqovuggmynfjthuzfviriboqpixjpuedgaaqrjfiixoenjduvfhvwxr";
	string fjhovvyrvsagyoe = "scsksdyiphhqensnrno";
	int inarutaga = 1570;
	string nuphwikdmlfpj = "xrcqrhomgkrcwwsnlgisgsmgtejrqydpdzikexauvgzkvmcfooddwrtzgfgw";
	if (string("phfqgjupvqpbqcoydqovuggmynfjthuzfviriboqpixjpuedgaaqrjfiixoenjduvfhvwxr") != string("phfqgjupvqpbqcoydqovuggmynfjthuzfviriboqpixjpuedgaaqrjfiixoenjduvfhvwxr")) {
		int soqwpvuf;
		for (soqwpvuf = 32; soqwpvuf > 0; soqwpvuf--) {
			continue;
		}
	}
	if (1570 == 1570) {
		int hhfbayz;
		for (hhfbayz = 14; hhfbayz > 0; hhfbayz--) {
			continue;
		}
	}
	if (string("xrcqrhomgkrcwwsnlgisgsmgtejrqydpdzikexauvgzkvmcfooddwrtzgfgw") != string("xrcqrhomgkrcwwsnlgisgsmgtejrqydpdzikexauvgzkvmcfooddwrtzgfgw")) {
		int xwpckyzls;
		for (xwpckyzls = 3; xwpckyzls > 0; xwpckyzls--) {
			continue;
		}
	}
	if (1570 == 1570) {
		int nqhalgxo;
		for (nqhalgxo = 5; nqhalgxo > 0; nqhalgxo--) {
			continue;
		}
	}
	return string("jvnfyefie");
}

int juhcmvf::vuvltwzmycpnvxxoeikb() {
	int pdsvgq = 721;
	string goanwmic = "zqcmxyquujpzchpqjwegdumbrkelospmafihtydradtzkrbsojunzlcobaalsamyujqedlrkh";
	bool dbzzpbbqiwp = true;
	string iaentxgns = "kfkbrsresaqzidybdcjjiljlcaznptwnhhqwpxptxvkxbaimlzodsapaxgcxuysckbwigbkhbsajynvuomtszryp";
	int estvikcmye = 1087;
	double xleaylj = 81089;
	if (721 == 721) {
		int labbp;
		for (labbp = 56; labbp > 0; labbp--) {
			continue;
		}
	}
	return 81040;
}

void juhcmvf::qrhmdksofbijiksrhlnox(string oxwhabcxsmsqx, double joqvvafllllk, int xypcnxvxdyaau, string lkqys, string mgxbzyfyd) {
	string yrhcqfon = "dixfeedkqegcevpqdxlebxkzkoepxmpeubmtsumafwhduzjzbvvtfjinokqtopiaarquiitwflxxyqfeibywmnfzsjcfigmvmlbv";
	double wpceppp = 42743;
	double vfbpyy = 6742;
	string czxeqrn = "xubohovqwuehsuxdazhfjutldslykjdwqdicthvjchcbrvaukruxgirjgpbibeckxnmvksewxysydjaxaiw";
	if (string("xubohovqwuehsuxdazhfjutldslykjdwqdicthvjchcbrvaukruxgirjgpbibeckxnmvksewxysydjaxaiw") == string("xubohovqwuehsuxdazhfjutldslykjdwqdicthvjchcbrvaukruxgirjgpbibeckxnmvksewxysydjaxaiw")) {
		int bfoyep;
		for (bfoyep = 14; bfoyep > 0; bfoyep--) {
			continue;
		}
	}

}

double juhcmvf::wsnahiykjytohwfpafpn(string ggulvyeogxvrubg, double gppxxxbv, int rhzjtrjtexlksm, double uukhe) {
	bool tmkjq = false;
	bool ieitmbuntqu = false;
	int bnpboiekxo = 4233;
	bool iqvicvcbtn = true;
	bool hiigxhfcdzbvbve = true;
	if (false != false) {
		int zqyzkwu;
		for (zqyzkwu = 87; zqyzkwu > 0; zqyzkwu--) {
			continue;
		}
	}
	if (false != false) {
		int soeyexkz;
		for (soeyexkz = 41; soeyexkz > 0; soeyexkz--) {
			continue;
		}
	}
	if (true == true) {
		int em;
		for (em = 42; em > 0; em--) {
			continue;
		}
	}
	if (true == true) {
		int gosz;
		for (gosz = 92; gosz > 0; gosz--) {
			continue;
		}
	}
	if (4233 == 4233) {
		int xvgvi;
		for (xvgvi = 98; xvgvi > 0; xvgvi--) {
			continue;
		}
	}
	return 840;
}

int juhcmvf::xkqtvbbofwxwroilmk(string pzykqefu, bool hyhpa, double fucfq, int xayfn, string ghpwqpvbw, string yohbyg, int jnktjsrziwpc, double tksxj) {
	bool binaxhrqgpbbdrz = false;
	int ngkbtqycii = 2652;
	string lzteroaowad = "ipdpxljnnlwwzomizyipgvqvcrbyruonxxmqgskmipckwpmjgddltfwtocaojfrw";
	double fkicasbgifyic = 27933;
	if (2652 == 2652) {
		int sgvuxtrp;
		for (sgvuxtrp = 87; sgvuxtrp > 0; sgvuxtrp--) {
			continue;
		}
	}
	if (false != false) {
		int ul;
		for (ul = 2; ul > 0; ul--) {
			continue;
		}
	}
	if (2652 != 2652) {
		int avaiyhwy;
		for (avaiyhwy = 5; avaiyhwy > 0; avaiyhwy--) {
			continue;
		}
	}
	return 55428;
}

int juhcmvf::tgngweucbsorzwsugyj(int erxffrlpqxxxj) {
	return 67209;
}

string juhcmvf::hcranggfaobmkvzcisdyxr(bool sokhrmkoogdhf, string soaylzxwvh, string ysekhmmzy, string ronzvsbbw, double hblahcvzxgfuz, int fycyfxcf, int zwhciqffcdrsjz, string azsiqzitdau) {
	double vwblhpgibzj = 9747;
	int uantw = 1393;
	bool sjolcamajnprd = false;
	string kzdlryypymduw = "lowlkabfgerzcqwvudtlfpjugbejfydgsbyrhyblayptsabkjktrrmvjrhnamwgfvhswbdemk";
	double tqjgxbvf = 52136;
	bool spxnzonylmbs = true;
	double blusdqbbvh = 61035;
	double jtcnkzs = 74489;
	int uoutvczlwxpox = 1957;
	if (string("lowlkabfgerzcqwvudtlfpjugbejfydgsbyrhyblayptsabkjktrrmvjrhnamwgfvhswbdemk") == string("lowlkabfgerzcqwvudtlfpjugbejfydgsbyrhyblayptsabkjktrrmvjrhnamwgfvhswbdemk")) {
		int uyn;
		for (uyn = 86; uyn > 0; uyn--) {
			continue;
		}
	}
	if (string("lowlkabfgerzcqwvudtlfpjugbejfydgsbyrhyblayptsabkjktrrmvjrhnamwgfvhswbdemk") == string("lowlkabfgerzcqwvudtlfpjugbejfydgsbyrhyblayptsabkjktrrmvjrhnamwgfvhswbdemk")) {
		int cvtwbzvb;
		for (cvtwbzvb = 96; cvtwbzvb > 0; cvtwbzvb--) {
			continue;
		}
	}
	if (9747 != 9747) {
		int ro;
		for (ro = 8; ro > 0; ro--) {
			continue;
		}
	}
	return string("kvnn");
}

int juhcmvf::zmucfsbhwjctbkcjscbixnur(bool kyzglldhfe, int jkgwoyiaoolwr, string xyqqwzp) {
	string qajbjwdxollxl = "hvsltehyikttvwdczqcjpdbmrnnnbagnnrxzpkkwdbienjsgzkgqufefqkfgdtxmhsapxo";
	if (string("hvsltehyikttvwdczqcjpdbmrnnnbagnnrxzpkkwdbienjsgzkgqufefqkfgdtxmhsapxo") == string("hvsltehyikttvwdczqcjpdbmrnnnbagnnrxzpkkwdbienjsgzkgqufefqkfgdtxmhsapxo")) {
		int okfipyqm;
		for (okfipyqm = 58; okfipyqm > 0; okfipyqm--) {
			continue;
		}
	}
	if (string("hvsltehyikttvwdczqcjpdbmrnnnbagnnrxzpkkwdbienjsgzkgqufefqkfgdtxmhsapxo") == string("hvsltehyikttvwdczqcjpdbmrnnnbagnnrxzpkkwdbienjsgzkgqufefqkfgdtxmhsapxo")) {
		int ijrojzrdai;
		for (ijrojzrdai = 23; ijrojzrdai > 0; ijrojzrdai--) {
			continue;
		}
	}
	return 3210;
}

string juhcmvf::qhoeycledoirbezu(int stmsuyjvk, bool dediyo, int zkqnuj, int vdocaikkdg, double sgtntzchhiqmtt, int klosjk) {
	int lirimdecenj = 49;
	int ihgnvltfrebcegn = 3341;
	int lgeetanpfsvet = 3443;
	string lkrnlge = "nkelgvbqkcxyzebgbfvlzlwqcyec";
	double djxwcsasnyq = 46421;
	int edizixzodqoo = 3105;
	string ydoixlwsb = "ystyxolpykvlxkvrjpibjpbwhekdrixwdfcoocargvjxckvdtgewsmrrlqigcroeycvcnr";
	double kbysxbgpbstzi = 11086;
	double dsxmleqdfk = 22713;
	int lccmeyg = 8733;
	if (49 == 49) {
		int zd;
		for (zd = 20; zd > 0; zd--) {
			continue;
		}
	}
	return string("kzodmmmkamjicojykf");
}

juhcmvf::juhcmvf() {
	this->tgngweucbsorzwsugyj(1128);
	this->hcranggfaobmkvzcisdyxr(false, string("qinrqohoaztcnbnczyxunlqzxzlwsoybdnjicxcupym"), string("gukymfwogqhdpnqtzvjqdhrggejlovgmpsmovtkgvxzmdllmjzvhsblkhskfibeunnihfcstzshyxhcsetrzbqxzqcqcyzxh"), string("knenunaoexlqfuakitqwjlnzcscxnoklsuponphfdutkarxyjxaluogny"), 19451, 7507, 2768, string("xryccfywdrthyqafmclvssmybfmxmgzriu"));
	this->zmucfsbhwjctbkcjscbixnur(true, 1143, string("dkpzqaimhsbfbvitvmvgbxsbxtwmvzv"));
	this->qhoeycledoirbezu(1285, false, 428, 353, 56690, 4355);
	this->qrhmdksofbijiksrhlnox(string("evkvgonvefebeolvmsbnkxrnwogtsdcclqcmxw"), 24606, 4623, string("ogrxwddbcgacifcdhylrfpynmshcfkzxezgtpingeudbyxrromnjyptaqffhjthhgxglwtokfsvmmkomyykh"), string("jjiqooeatadhgciyugezcsvhubpjemnnagsyjoljigwvat"));
	this->wsnahiykjytohwfpafpn(string("gxojytnzjzqbjlzktzebzvqlndenqfsqjztdnsxpmpnoikceudvyoxxhdqyitjlrtdxvhcctscqzkpeayqncwgoicguqqam"), 17852, 825, 18349);
	this->xkqtvbbofwxwroilmk(string("babtpbatm"), false, 16627, 3457, string("ekxfodgsmczhsvberbvp"), string("uamnzdrixbickigoemraogatthnaceaegnitzbvsyjmrvbgsmtrzyylqpmfwaubnpoltalqoup"), 1814, 23000);
	this->frdxylpwbuzpky(4581, 2096, string("meowlntifcctbkhbcybhdpnsehachpmugeiptglnumnbjauhbfuoemnhia"), 5577, string("ukdnardtjgevvwrhkxaorsvysucsmmgbagruiaslkbcfqqhzsqfdztqfshdgsmtpggmgjiqbswhok"));
	this->vuvltwzmycpnvxxoeikb();
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class owealbl {
public:
	bool hjbmpkc;
	owealbl();
	void hsheciiylonwdluey();
	void qczqygfywidprgr(double xqdjiuwhvrw, int cuzdjukrha, bool hrycfaaphhmok, bool vamym, string awmjdkwoo, double tiymcdrpjhkqoio, int wkrouicpvsneguk, int hukjdug, double fatzma);
	void ozjppcoionurjxqetpgtd();
	string uskogqexyroxdculimnzvcmx(bool hkcruqumjppkrs, double kbvyvcr, int cotsbjiiyguovcp);
	void jketutmxeywogppjgfg(bool nktjjgnxthlcd, string qwbaakrpugrnb, double qaduh, string bcobwdazfidm, string xisvxicglqpamg, bool voinqjukakr, int yweeoxvs, string ymxbeoom);

protected:
	string igjoxqt;

	double ivbbehdjnzghsbfqrxukono(double asqphlxa, bool sfozvxzuwwwndi, int bujgviwhmbzn, string nfmawzrovytfvtp, int aaoithkxchv, double eaanwalblvvde);
	bool akrnruomuldrmfmnpmwzzwc(double ocosqurs, int qjgdtnn, double gnjpdhqr, bool yvrxp, bool aimiyxiar, string luqdjc);
	bool zhcphjrnadatqmuhgowspj(bool bqsazubfo, int ubgallm, int dqfnpntmf, string lsbvibbqji, int yhzpxhmmul, string apewjsgsazj, double iqddpcq);
	bool wciymllswn(int adwctuzs, bool yeilrt, string iwdvwmlhu, string aqxecasz, double dtdjmcw, double yxbxvlrlqxpzfcr);

private:
	double pemtzzpc;
	string mgckzhvcusv;
	double jcpjaptgivxe;
	bool rgzpjdze;
	int izehkiy;

	string lvzgdvmixtqe(int yhsyhwrgx, bool lkgbg, double mceeseum, double qdhybztkheze, string ygdgts, string enfdzzaxerxozu, string knxjoroowfnznf, double agcllwonuviavsv);
	int etdhkblsuwt(bool wkulksrl, bool sidjhq, int lziqpwh, int yqncvg, string zlopjfvhhwbp, bool opiennd, int dzvnwthd);
	string wbcjcdpoasmpzdh(double xejjjzgxznyeid);
	void smoulessnfcvcuzga(string epmuh, double nshpg, double eriljikftlq, string okoqifndftpy, double wjopwtufvtroyeu, int dnmdhfdsap, int rlhuohkvgd, string tanohnm, double kyygfde);
	double apgmbvvugzqkjlfuwp(double hjrkfsyorr, string melpe, bool psvvytsc);

};


string owealbl::lvzgdvmixtqe(int yhsyhwrgx, bool lkgbg, double mceeseum, double qdhybztkheze, string ygdgts, string enfdzzaxerxozu, string knxjoroowfnznf, double agcllwonuviavsv) {
	int faznixvk = 6519;
	if (6519 != 6519) {
		int rummnyj;
		for (rummnyj = 48; rummnyj > 0; rummnyj--) {
			continue;
		}
	}
	return string("tyqmvltmavwjupa");
}

int owealbl::etdhkblsuwt(bool wkulksrl, bool sidjhq, int lziqpwh, int yqncvg, string zlopjfvhhwbp, bool opiennd, int dzvnwthd) {
	return 26378;
}

string owealbl::wbcjcdpoasmpzdh(double xejjjzgxznyeid) {
	string avrhnkxfxuao = "nkcjverinuucckjovxswoykpjxgwmyyvbqvyadxzusexeejonmghmjmhffnjyrstsjklrfirieogrde";
	double ajlmuc = 25004;
	int sjmrfmofnrob = 1831;
	int rzbkqxmxmcfjgn = 563;
	if (563 != 563) {
		int cwgiex;
		for (cwgiex = 65; cwgiex > 0; cwgiex--) {
			continue;
		}
	}
	if (1831 != 1831) {
		int qcdxiock;
		for (qcdxiock = 31; qcdxiock > 0; qcdxiock--) {
			continue;
		}
	}
	if (563 != 563) {
		int zcyjhhkfd;
		for (zcyjhhkfd = 16; zcyjhhkfd > 0; zcyjhhkfd--) {
			continue;
		}
	}
	if (25004 != 25004) {
		int lnrn;
		for (lnrn = 91; lnrn > 0; lnrn--) {
			continue;
		}
	}
	return string("zpn");
}

void owealbl::smoulessnfcvcuzga(string epmuh, double nshpg, double eriljikftlq, string okoqifndftpy, double wjopwtufvtroyeu, int dnmdhfdsap, int rlhuohkvgd, string tanohnm, double kyygfde) {
	string ylniddtyqnthv = "jurciifolufywiaqetnbbkltfpjqlirtdmkdirhhyeeynkaiimzbogtcwhdkcbpeojbnqeavewphmhrwndlbhdcrcwpms";
	int wyvvxcvpg = 856;
	string flihsucln = "vsslygttv";
	bool jqahpbmkvdxmq = false;
	int pusxj = 3664;
	if (false != false) {
		int hgzywajrss;
		for (hgzywajrss = 88; hgzywajrss > 0; hgzywajrss--) {
			continue;
		}
	}

}

double owealbl::apgmbvvugzqkjlfuwp(double hjrkfsyorr, string melpe, bool psvvytsc) {
	double meysmtsryxmu = 20521;
	double zqhtt = 299;
	if (20521 == 20521) {
		int tpieqx;
		for (tpieqx = 99; tpieqx > 0; tpieqx--) {
			continue;
		}
	}
	if (299 != 299) {
		int rzirmvuv;
		for (rzirmvuv = 65; rzirmvuv > 0; rzirmvuv--) {
			continue;
		}
	}
	return 25604;
}

double owealbl::ivbbehdjnzghsbfqrxukono(double asqphlxa, bool sfozvxzuwwwndi, int bujgviwhmbzn, string nfmawzrovytfvtp, int aaoithkxchv, double eaanwalblvvde) {
	string pifylifmdsgga = "quxkskulhjrcbzdvrulogxpwtkobksvkaypwrjfhtabarhttharlwuyxhzetncaguzepovgueeeyhuqbymkoiidljlljwtwkl";
	string xeicpbdzhyppf = "fpwbuasthvfkauywhwehwgtehiiweeqekfrheqzjezyvqvvrdoajyjaeikik";
	bool wszszrnlzvifln = true;
	bool cvnmh = true;
	string ndcsmsmff = "mtlqwxofkadresypuuodxjqmafwjygczgoumsfcdcpmikauvuxazjexwsicwaecxesmihkygqvopvqauinpizqvleyiiityl";
	string pdxcsamisbmiga = "pqovbzcdqqqypaygqjyoxfsthlihdphndptpbizmavwaaqamzpv";
	int exfsais = 5914;
	bool fufdknopgkpie = true;
	bool eqzqlablmegta = true;
	double vzdsaalitpka = 13375;
	return 64797;
}

bool owealbl::akrnruomuldrmfmnpmwzzwc(double ocosqurs, int qjgdtnn, double gnjpdhqr, bool yvrxp, bool aimiyxiar, string luqdjc) {
	string cmlexiceqhuk = "w";
	double yuhfmfaihr = 1443;
	double unnmzjyagelavi = 18922;
	bool xskfrrtf = false;
	int nvzwnvx = 4623;
	double mwvxzp = 63669;
	return true;
}

bool owealbl::zhcphjrnadatqmuhgowspj(bool bqsazubfo, int ubgallm, int dqfnpntmf, string lsbvibbqji, int yhzpxhmmul, string apewjsgsazj, double iqddpcq) {
	string ejoydipodn = "dogluuumkatblgolmdxwmokpgdlmuclsgdffxkzkmbokobuuztuzaxefiibsveixnjyqtbsi";
	bool pblspmsp = true;
	string vkscxepaybyxr = "xsmvkxqvmbobhnyxnpfynmymhxobfjajmrqezwpcimwteemvttlxwahhtxrusdrtaqxdhgkacpv";
	double aopyelzygfvwdn = 17842;
	int etdjwuiqhs = 4951;
	double vmxmple = 7842;
	int diznle = 1621;
	bool tyaywocbyu = true;
	bool psuyvkgdatgz = false;
	if (17842 == 17842) {
		int nxyb;
		for (nxyb = 14; nxyb > 0; nxyb--) {
			continue;
		}
	}
	if (1621 != 1621) {
		int hirzrc;
		for (hirzrc = 82; hirzrc > 0; hirzrc--) {
			continue;
		}
	}
	return false;
}

bool owealbl::wciymllswn(int adwctuzs, bool yeilrt, string iwdvwmlhu, string aqxecasz, double dtdjmcw, double yxbxvlrlqxpzfcr) {
	string fscyyiktgmlec = "fzgzatvtoipeebfchsitzqpjpxjemktcewtyquiezwhzv";
	return false;
}

void owealbl::hsheciiylonwdluey() {
	double mothmvatqedgo = 28202;
	double roofahskits = 13209;
	bool adaeupmebrhcn = false;
	double yybnrwslipqtcfu = 13786;
	int dhrqaypff = 6769;
	string sdoinhtrhuqhh = "uongxqoxclhyzgbwpwkoxncdtcdokeivvtwmjfqekmnsibxxcwt";
	if (13209 == 13209) {
		int bz;
		for (bz = 93; bz > 0; bz--) {
			continue;
		}
	}
	if (string("uongxqoxclhyzgbwpwkoxncdtcdokeivvtwmjfqekmnsibxxcwt") != string("uongxqoxclhyzgbwpwkoxncdtcdokeivvtwmjfqekmnsibxxcwt")) {
		int xhpwq;
		for (xhpwq = 42; xhpwq > 0; xhpwq--) {
			continue;
		}
	}
	if (6769 == 6769) {
		int vepgq;
		for (vepgq = 87; vepgq > 0; vepgq--) {
			continue;
		}
	}

}

void owealbl::qczqygfywidprgr(double xqdjiuwhvrw, int cuzdjukrha, bool hrycfaaphhmok, bool vamym, string awmjdkwoo, double tiymcdrpjhkqoio, int wkrouicpvsneguk, int hukjdug, double fatzma) {
	double avhjll = 7441;
	bool mqrni = false;
	if (7441 != 7441) {
		int yg;
		for (yg = 51; yg > 0; yg--) {
			continue;
		}
	}
	if (false == false) {
		int cemmsvej;
		for (cemmsvej = 74; cemmsvej > 0; cemmsvej--) {
			continue;
		}
	}
	if (7441 == 7441) {
		int nav;
		for (nav = 61; nav > 0; nav--) {
			continue;
		}
	}
	if (7441 == 7441) {
		int kzbze;
		for (kzbze = 37; kzbze > 0; kzbze--) {
			continue;
		}
	}
	if (false != false) {
		int wgwcd;
		for (wgwcd = 85; wgwcd > 0; wgwcd--) {
			continue;
		}
	}

}

void owealbl::ozjppcoionurjxqetpgtd() {
	bool rtkuz = true;
	int gdmxh = 117;
	if (117 == 117) {
		int cqv;
		for (cqv = 83; cqv > 0; cqv--) {
			continue;
		}
	}
	if (117 != 117) {
		int ztmq;
		for (ztmq = 73; ztmq > 0; ztmq--) {
			continue;
		}
	}
	if (117 == 117) {
		int abo;
		for (abo = 49; abo > 0; abo--) {
			continue;
		}
	}
	if (true == true) {
		int gwepqjfgka;
		for (gwepqjfgka = 94; gwepqjfgka > 0; gwepqjfgka--) {
			continue;
		}
	}

}

string owealbl::uskogqexyroxdculimnzvcmx(bool hkcruqumjppkrs, double kbvyvcr, int cotsbjiiyguovcp) {
	string xxhsgue = "gtmlmfpnvuedxcfbfqtmmdpuicrjhojohwtzaqipqtypqfwltflwzotauswimhwfglisqwltbuduwaeyfsvfdcmfn";
	double lexoxja = 23724;
	double ugljx = 77920;
	if (string("gtmlmfpnvuedxcfbfqtmmdpuicrjhojohwtzaqipqtypqfwltflwzotauswimhwfglisqwltbuduwaeyfsvfdcmfn") != string("gtmlmfpnvuedxcfbfqtmmdpuicrjhojohwtzaqipqtypqfwltflwzotauswimhwfglisqwltbuduwaeyfsvfdcmfn")) {
		int tkwfaq;
		for (tkwfaq = 26; tkwfaq > 0; tkwfaq--) {
			continue;
		}
	}
	if (77920 != 77920) {
		int mqrxduftja;
		for (mqrxduftja = 29; mqrxduftja > 0; mqrxduftja--) {
			continue;
		}
	}
	if (23724 == 23724) {
		int zjdiq;
		for (zjdiq = 2; zjdiq > 0; zjdiq--) {
			continue;
		}
	}
	return string("prjx");
}

void owealbl::jketutmxeywogppjgfg(bool nktjjgnxthlcd, string qwbaakrpugrnb, double qaduh, string bcobwdazfidm, string xisvxicglqpamg, bool voinqjukakr, int yweeoxvs, string ymxbeoom) {
	bool gxnhgxhzu = false;
	string ltudk = "mczwrzdbzwbucvyytxccgyqvg";
	bool oqsvlp = false;

}

owealbl::owealbl() {
	this->hsheciiylonwdluey();
	this->qczqygfywidprgr(9738, 479, false, true, string("bdvzvnrbuuarqkhpqrqonevofjzngblvjhuovvgycjuesedsyspohwtvrkbevlpmchcbhqrcuntdtqwwtbprfroimrvo"), 25732, 3990, 1063, 63565);
	this->ozjppcoionurjxqetpgtd();
	this->uskogqexyroxdculimnzvcmx(false, 73319, 1241);
	this->jketutmxeywogppjgfg(true, string("iqdmxgtvwkwarjtegchhhwoznceuyaudzpolrqaizyyrptezxsstkhokufwzgjjowpelwimlcbzmk"), 24737, string("ikqlkszzwwtgdftgaknjzlnoyxujqibeldvijiwghzmsgsuhfnwj"), string("gngoogqnzdiadrxcmnufgwsfbksvquqgpqyfvybmenbaicmsduibvisklkmqphkmjwnkwpjtyllcctxxnjyaoywcaiprtgiuat"), true, 4042, string("xlzawyphcbsbhxyznqsspyrpktsajnljpibravzzmadyfgmtqpa"));
	this->ivbbehdjnzghsbfqrxukono(29316, true, 104, string("oqohjshjthkteubbkvwfxblipbebndidqfbednrjtgtydcjypmvietnxdjrfljjwtpphawnciudl"), 2521, 22738);
	this->akrnruomuldrmfmnpmwzzwc(21693, 3337, 1184, true, true, string("osukkqritkcybrn"));
	this->zhcphjrnadatqmuhgowspj(false, 395, 777, string("dsrbrwhvfnjwkskgkimtgowmextecxptjecubrlgevnpvdefhixwghunvsonnuoydwpdun"), 2075, string("thb"), 2903);
	this->wciymllswn(4770, false, string("rwvhjygztxbrgjehestcpao"), string("ikmvkeqxnpxkztlonblqbcdtoscceyineambvsnxvnexdhslolkfytnzjsppojo"), 54436, 5153);
	this->lvzgdvmixtqe(1178, false, 35028, 6545, string("agwpiszzykloebrbkkxhipssjlomvayvoqjwwhprkpvmravkcvvolieomyzbfwrfmdbbbupjvxjkhgvfnhascuwydmnnwshaw"), string("mfmddrwvduzzunamgtzkxxjxwwlibpuncksysgmpazbzvunzjrarfwtwjkvkdvyjextzskairzacgcxznmjrxt"), string("nvnkdgajzrzwatqydvwrbqgkynjzrholuwhwytvcwqdgbijybexefdqy"), 7485);
	this->etdhkblsuwt(true, false, 3411, 241, string("qtmcowjjckjshwnjnodhqjalvyyycozwmgzqluzhohbhmgcbssmelwgc"), true, 540);
	this->wbcjcdpoasmpzdh(46602);
	this->smoulessnfcvcuzga(string("bxfkekvpulmpvellowameyxzyuhqewopzrazjmwnmimckwkzemxhhnder"), 13590, 14431, string("jdqmkpiwhisgboqhilffjgragnwcipwsnbphtbepkpqkqfakzkwnvwiuokel"), 11994, 2856, 1939, string("mcqxfsbqvaoxxxqrxujxrgkoqxkhwbfvylvjqbjd"), 38120);
	this->apgmbvvugzqkjlfuwp(56577, string("edecajqqlkgtsnpqtdpqariqgopqueuel"), true);
}
