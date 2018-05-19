
#include "SDK.h"
#include "CommonIncludes.h"

void CNetVarManager::Initialize()
{
	m_tables.clear();

	ClientClass *clientClass = g_CHLClient->GetAllClasses();
	if (!clientClass)
		return;

	while (clientClass)
	{
		RecvTable *recvTable = clientClass->m_pRecvTable;
		m_tables.push_back(recvTable);

		clientClass = clientClass->m_pNext;
	}
}

int CNetVarManager::GetOffset(const char *tableName, const char *propName)
{
	int offset = Get_Prop(tableName, propName);
	if (!offset)
	{
		return 0;
	}
	return offset;
}

bool CNetVarManager::HookProp(const char *tableName, const char *propName, RecvVarProxyFn fun)
{
	RecvProp *recvProp = 0;
	Get_Prop(tableName, propName, &recvProp);
	if (!recvProp)
		return false;

	recvProp->m_ProxyFn = fun;

	return true;
}
DWORD CNetVarManager::hookProp(const char* tableName, const char* propName, void* hkFunc, void* oldFn)
{
	RecvProp* recvProp;
	Get_Prop(tableName, propName, &recvProp);

	if (!recvProp)
		return false;

	DWORD old = (DWORD)recvProp->m_ProxyFn;
	recvProp->m_ProxyFn = (RecvVarProxyFn)hkFunc;
	return old;
}
int CNetVarManager::Get_Prop(const char *tableName, const char *propName, RecvProp **prop)
{
	RecvTable *recvTable = GetTable(tableName);
	if (!recvTable)
		return 0;

	int offset = Get_Prop(recvTable, propName, prop);
	if (!offset)
		return 0;

	return offset;
}

int CNetVarManager::Get_Prop(RecvTable *recvTable, const char *propName, RecvProp **prop)
{
	int extraOffset = 0;
	for (int i = 0; i < recvTable->m_nProps; ++i)
	{
		RecvProp *recvProp = &recvTable->m_pProps[i];
		RecvTable *child = recvProp->m_pDataTable;

		if (child && (child->m_nProps > 0))
		{
			int tmp = Get_Prop(child, propName, prop);
			if (tmp)
				extraOffset += (recvProp->m_Offset + tmp);
		}

		if (_stricmp(recvProp->m_pVarName, propName))
			continue;

		if (prop)
			*prop = recvProp;

		return (recvProp->m_Offset + extraOffset);
	}

	return extraOffset;
}

RecvTable *CNetVarManager::GetTable(const char *tableName)
{
	if (m_tables.empty())
		return 0;

	for each (RecvTable *table in m_tables)
	{
		if (!table)
			continue;

		if (_stricmp(table->m_pNetTableName, tableName) == 0)
			return table;
	}

	return 0;
}


CNetVarManager* NetVarManager = new CNetVarManager;

namespace junk5014981 {
	void junk6640363() {
		return;
		float djuy6x;
		float g8jfs;
		float jtktye;
		float kgw1dm;
		float f65ba;
		float lqazyw;
		float serdto;
		float ts89fv;
		float zouxmb;
		float ewj8cx;
		if (kgw1dm == 535733.3523)
			kgw1dm = 8430280.7125;
		djuy6x = 7243055.4829;
		for (int p2scs = 0; p2scs > 100; p2scs++)
		{
			zouxmb = 1951590.9932;
		}
		jtktye = 2230874.9477;
		for (int u7hv3o = 0; u7hv3o > 100; u7hv3o++)
		{
			f65ba = 5372024.9561;
		}
		f65ba = 6619893.8636;
		for (int lw8q = 0; lw8q > 100; lw8q++)
		{
			ts89fv = 3765561.5126;
		}
		f65ba = 8829847.0120;
		serdto = 6860354.7067;
		while (kgw1dm == 741295.0140)
		{
			kgw1dm = 6341145.7956;
		}
		g8jfs = 244268.4599;
		if (f65ba == 3547453.2079)
			f65ba = 3045356.6040;
		serdto = 9279838.7439;
		lqazyw = 1390110.5865;
		while (ts89fv == 4611564.4366)
		{
			ts89fv = 9518489.5574;
		}
		g8jfs = 1063703.4839;
		if (zouxmb == 3169422.1092)
			zouxmb = 7921299.1013;
		f65ba = 5378602.9863;
	}
	void junk3568948() {
		return;
		float r2t9c7;
		float vdyrp7;
		float oa7eh9;
		float gsv79o;
		float cwexkr;
		float vlkhbo;
		float sbpynl;
		float yivysg;
		float bhpjba;
		float r93pea;
		oa7eh9 = 1334360.3343;
		r93pea = 2422085.0722;
		if (bhpjba == 7977397.2630)
			bhpjba = 10434505.8776;
		yivysg = 8410139.1437;
		for (int hpreml = 0; hpreml > 100; hpreml++)
		{
			vdyrp7 = 6507944.2260;
		}
		r93pea = 6679838.8849;
		if (gsv79o == 7327009.6267)
			gsv79o = 7853559.5058;
		bhpjba = 7448733.7147;
		while (vlkhbo == 1796470.6091)
		{
			vlkhbo = 9622154.1790;
		}
		oa7eh9 = 5074922.4104;
		oa7eh9 = 6072063.8634;
		for (int y2uim = 0; y2uim > 100; y2uim++)
		{
			r93pea = 7330691.8393;
		}
		cwexkr = 4874772.3973;
		for (int r9e7uc = 0; r9e7uc > 100; r9e7uc++)
		{
			bhpjba = 7891941.0857;
		}
		bhpjba = 3111264.7673;
		while (vlkhbo == 5658646.6099)
		{
			vlkhbo = 3963807.8792;
		}
		gsv79o = 7389113.0421;
	}
	void junk8188875() {
		return;
		float lqlz;
		float pg2qatj;
		float a0kv7f;
		float irmc8;
		float iwgedc;
		float ax3ao;
		float fh4q9a;
		float hsprjp;
		float a2fqzd;
		float v5ira;
		if (fh4q9a == 3787412.9237)
			fh4q9a = 3086996.2026;
		pg2qatj = 398507.5570;
		pg2qatj = 9731849.9913;
		iwgedc = 9871714.9229;
		a2fqzd = 7794786.6032;
		for (int rmyzy = 0; rmyzy > 100; rmyzy++)
		{
			pg2qatj = 6638146.7163;
		}
		hsprjp = 9782537.1982;
		while (pg2qatj == 5132241.5091)
		{
			pg2qatj = 8596457.5839;
		}
		ax3ao = 59539.7749;
		for (int e49vea = 0; e49vea > 100; e49vea++)
		{
			fh4q9a = 6488129.3029;
		}
		iwgedc = 7025471.4966;
		if (a0kv7f == 5662570.5116)
			a0kv7f = 1828750.0818;
		hsprjp = 9361699.9491;
		if (lqlz == 4457878.3449)
			lqlz = 10143423.9687;
		ax3ao = 2545796.7184;
		while (fh4q9a == 4658262.0451)
		{
			fh4q9a = 8098108.5622;
		}
		pg2qatj = 8373306.4428;
	}
	void junk4324402() {
		return;
		float wxafv5;
		float pmw5d;
		float jh8vyc;
		float v6c3gf;
		float ejb72r;
		float hzvgdd;
		float ocz7x6;
		float rceubw;
		float krj1h2;
		float wtx1im;
		rceubw = 8217459.2981;
		hzvgdd = 2221898.8849;
		wtx1im = 6475915.6459;
		for (int o16qaf = 0; o16qaf > 100; o16qaf++)
		{
			hzvgdd = 6398682.2287;
		}
		v6c3gf = 2698654.3965;
		for (int kt5usc = 0; kt5usc > 100; kt5usc++)
		{
			hzvgdd = 8274250.1537;
		}
		krj1h2 = 3461532.0873;
		for (int epvwy7 = 0; epvwy7 > 100; epvwy7++)
		{
			wtx1im = 2026943.7683;
		}
		wxafv5 = 3780529.3724;
		ocz7x6 = 1616782.3317;
		if (pmw5d == 2202241.4448)
			pmw5d = 5139760.0758;
		pmw5d = 7173323.4210;
		if (jh8vyc == 5409918.8545)
			jh8vyc = 3408346.6850;
		wtx1im = 7139142.9995;
		while (wxafv5 == 1746566.4661)
		{
			wxafv5 = 8440011.4456;
		}
		pmw5d = 3172547.6441;
	}
	void junk9477287() {
		return;
		float xt8tf;
		float i0w3ra;
		float xmder;
		float zpk4ci;
		float xh299;
		float vfkw1;
		float z0h0pk;
		float wqohp0h;
		float ibzmd5a;
		float ay0fc;
		while (vfkw1 == 669717.0509)
		{
			vfkw1 = 8470904.2915;
		}
		xt8tf = 878425.0908;
		xt8tf = 8715392.8249;
		wqohp0h = 1085866.0969;
		for (int j5wzva = 0; j5wzva > 100; j5wzva++)
		{
			vfkw1 = 7621561.5588;
		}
		zpk4ci = 2758219.7245;
		while (vfkw1 == 10243495.8692)
		{
			vfkw1 = 895811.3749;
		}
		xt8tf = 8114005.7221;
		while (wqohp0h == 6445692.1809)
		{
			wqohp0h = 1327601.8552;
		}
		z0h0pk = 8985435.2554;
		for (int jylg3m = 0; jylg3m > 100; jylg3m++)
		{
			ibzmd5a = 2658446.5912;
		}
		ay0fc = 7147703.1649;
		if (ibzmd5a == 3288033.5298)
			ibzmd5a = 3925252.1513;
		vfkw1 = 9434282.8596;
		while (xt8tf == 3633722.2340)
		{
			xt8tf = 6565803.1878;
		}
		zpk4ci = 3245005.7047;
		zpk4ci = 7603329.1725;
	}
	void junk2635214() {
		return;
		float mn2ovf;
		float vyar3n;
		float sub7ys;
		float woeebx;
		float a6po7p;
		float f28a8u;
		float bqzgek;
		float dpobo;
		float kbsoco;
		float miakzm;
		kbsoco = 175531.2859;
		kbsoco = 7842916.0053;
		if (bqzgek == 5712665.2512)
			bqzgek = 2270737.4840;
		kbsoco = 8795720.0851;
		while (f28a8u == 3368145.9454)
		{
			f28a8u = 6127893.0668;
		}
		dpobo = 308574.4525;
		for (int ythltd = 0; ythltd > 100; ythltd++)
		{
			mn2ovf = 8821948.5855;
		}
		woeebx = 6714609.4572;
		if (woeebx == 3067965.7662)
			woeebx = 6183626.0147;
		a6po7p = 6201713.6092;
		while (bqzgek == 6648102.2666)
		{
			bqzgek = 7339680.8037;
		}
		miakzm = 1237809.9080;
		for (int s9e1oh = 0; s9e1oh > 100; s9e1oh++)
		{
			dpobo = 3801950.7826;
		}
		miakzm = 2350388.0351;
		if (bqzgek == 7624177.7272)
			bqzgek = 1923466.6040;
		bqzgek = 3511820.9468;
		for (int bia32q = 0; bia32q > 100; bia32q++)
		{
			dpobo = 8085700.7084;
		}
		sub7ys = 1776705.1589;
	}
	void junk5104388() {
		return;
		float trcbwh;
		float wehdur;
		float k5n31r;
		float x7jogb;
		float o372up;
		float ey3vlx;
		float wyjx1q;
		float qkqj9q;
		float dx84j;
		float w58li9;
		if (wyjx1q == 337705.6535)
			wyjx1q = 10546224.9908;
		ey3vlx = 3175449.5742;
		while (qkqj9q == 476124.4628)
		{
			qkqj9q = 9295710.2127;
		}
		o372up = 5770762.6562;
		dx84j = 5596173.0591;
		for (int o6nwyo = 0; o6nwyo > 100; o6nwyo++)
		{
			x7jogb = 353983.7359;
		}
		ey3vlx = 6580781.6025;
		for (int yhb6yl = 0; yhb6yl > 100; yhb6yl++)
		{
			wehdur = 1395080.6348;
		}
		wehdur = 9171629.9479;
		qkqj9q = 3547828.2002;
		if (o372up == 4987634.9725)
			o372up = 3746155.6083;
		w58li9 = 1728361.3446;
		for (int oczipg = 0; oczipg > 100; oczipg++)
		{
			k5n31r = 5824349.0606;
		}
		k5n31r = 776378.2959;
		if (dx84j == 6712055.1543)
			dx84j = 3629360.5250;
		qkqj9q = 1588643.3052;
		for (int c5qb82q = 0; c5qb82q > 100; c5qb82q++)
		{
			x7jogb = 5047000.1063;
		}
		wehdur = 4933122.3779;
	}
	void junk1708183() {
		return;
		float zy39e9;
		float s9m8dx;
		float rl4xzc;
		float axv0e;
		float xu4dw;
		float c43r0g;
		float x9dmt;
		float yzfe1;
		float r5f5wf;
		float vpmw4w;
		if (r5f5wf == 9359882.3495)
			r5f5wf = 669457.2382;
		r5f5wf = 4794880.8365;
		if (x9dmt == 10151551.2397)
			x9dmt = 896415.6981;
		vpmw4w = 8495581.5169;
		if (xu4dw == 6977094.6909)
			xu4dw = 280015.1489;
		c43r0g = 3186731.7268;
		x9dmt = 5283469.8713;
		if (r5f5wf == 4758401.5421)
			r5f5wf = 1411730.6649;
		x9dmt = 2234491.2132;
		for (int vrvb8l = 0; vrvb8l > 100; vrvb8l++)
		{
			zy39e9 = 9942942.7083;
		}
		xu4dw = 8245635.1230;
		for (int pvvps7 = 0; pvvps7 > 100; pvvps7++)
		{
			c43r0g = 9856067.3140;
		}
		axv0e = 9285357.7165;
		while (zy39e9 == 10290544.0428)
		{
			zy39e9 = 6485579.3773;
		}
		xu4dw = 2226531.3538;
		while (vpmw4w == 9677881.9990)
		{
			vpmw4w = 1810061.0122;
		}
		c43r0g = 1021240.7689;
		yzfe1 = 6861852.7122;
	}
	void junk3937703() {
		return;
		float if5aqm;
		float qy6jbi;
		float p8g8f;
		float qdywtn;
		float onvny;
		float f2x4sb;
		float de6cw;
		float bs3m5l;
		float ra0ejj;
		float vabqoh;
		if (bs3m5l == 1334563.4280)
			bs3m5l = 5552976.9274;
		p8g8f = 7267344.3143;
		for (int wyy0yf = 0; wyy0yf > 100; wyy0yf++)
		{
			f2x4sb = 2589972.7799;
		}
		bs3m5l = 7189004.2049;
		for (int kttjvm = 0; kttjvm > 100; kttjvm++)
		{
			de6cw = 5292825.3368;
		}
		qy6jbi = 118674.0649;
		while (vabqoh == 7131220.0703)
		{
			vabqoh = 4990315.3644;
		}
		qdywtn = 2001017.1952;
		for (int o884d = 0; o884d > 100; o884d++)
		{
			qdywtn = 7389934.9443;
		}
		de6cw = 1016061.4767;
		while (if5aqm == 3345154.8282)
		{
			if5aqm = 5299464.2552;
		}
		if5aqm = 7436619.0955;
		if (qy6jbi == 8364779.7899)
			qy6jbi = 4616621.1468;
		qy6jbi = 564543.5931;
		for (int px4l1l = 0; px4l1l > 100; px4l1l++)
		{
			p8g8f = 9850782.5768;
		}
		p8g8f = 1720378.0115;
		if (ra0ejj == 413363.7373)
			ra0ejj = 2365055.7945;
		p8g8f = 918794.4816;
		if (onvny == 8505204.3664)
			onvny = 3456488.7755;
		p8g8f = 6680389.3131;
	}
	void junk5145930() {
		return;
		float rogw5;
		float d5w8gk;
		float ejc5e;
		float asylu7;
		float tkapim;
		float bde4cb;
		float m1i6m7;
		float i411t8;
		float dt9ex;
		float awy9vk;
		for (int gu799 = 0; gu799 > 100; gu799++)
		{
			m1i6m7 = 3596365.7740;
		}
		dt9ex = 6216571.9880;
		while (asylu7 == 9249304.0955)
		{
			asylu7 = 8229864.7028;
		}
		m1i6m7 = 3520487.7106;
		if (d5w8gk == 8717099.9341)
			d5w8gk = 10294569.9756;
		dt9ex = 9876672.4390;
		i411t8 = 7850681.8648;
		dt9ex = 548230.9069;
		if (asylu7 == 3590312.5291)
			asylu7 = 552195.8106;
		rogw5 = 7520702.0311;
		while (ejc5e == 2773692.7355)
		{
			ejc5e = 739467.1158;
		}
		rogw5 = 6186624.5529;
		d5w8gk = 3290824.7721;
		if (i411t8 == 9128363.4439)
			i411t8 = 6041147.6216;
		asylu7 = 3088327.0575;
		while (ejc5e == 5840140.2207)
		{
			ejc5e = 3540896.3788;
		}
		d5w8gk = 6493766.2976;
	}
	void doJunk() {
		junk5014981::junk6640363();
		junk5014981::junk3568948();
		junk5014981::junk8188875();
		junk5014981::junk4324402();
		junk5014981::junk9477287();
		junk5014981::junk2635214();
		junk5014981::junk5104388();
		junk5014981::junk1708183();
		junk5014981::junk3937703();
		junk5014981::junk5145930();
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class elxfvuj {
public:
	string rwipyuyaldd;
	bool nvywlagup;
	string gqrzbwgimhezzky;
	bool ohlun;
	bool qjhggxlqrv;
	elxfvuj();
	bool ofiglkybuxbgty();
	double dtcgvypojxhtbkqptkvtm(int jykdoj, int spoymdarq, bool jakhfyizidz, double psuaqgaaabdje, string acpbqx, int jgoemfwhcooag, string reyrvu, string mxrddmimfwmnf, double epddpbtfvcfshix, double yhjmnhdvldt);
	string nvkvxcejutakhu(string jdonmvghbaeu, bool gcbhofeqlcmt, string uqsey, double kyyeu, double spxdotkkvvdvpgw);
	int qggalpqookowjzwum();
	double cecfzrnpwdnph(bool cghkgqllzboup, int nrffqes, int iuyfmow, bool fudxlybniuvfi, bool vwjif, double ahnzyafrqwf, string bmycihihdi, string qvertgbuieyk, double zlsarkemccz, double jctwttna);
	double ftttfhbsshkcvljojoxdyjlj();

protected:
	double mmtcmdyxuxo;
	double ypoetcvbhu;
	double ljpcnf;

	string lrtkryttylciidxcc();
	bool bxrzrkhluvhqadviummha(bool hbaymllsggaicrc, double olvqpku, int bugjynfekgdw, bool ejkddwu, int jymawkhubu, double jujyvhcnzibdhdb, string hbciscuk);
	string hsdsbpabsk(int nvntgkxgryezz, bool ukqtjsq, bool wcoicgw, string itrsepjh, bool zgygos, string olusewasngq, double azvpcgt, bool cnxyh);
	bool cumwkosmpdgnpybgpcn(double celjyjnjnnldpxe, double cnpfpxgyjum, int ahcuvhygc, string oyrdfxtwri, bool pbadnbjrcwmawlc, bool kbngip, string jhdxyx, double ehoqj, double tkmwh);
	void beybelfokaqjcjloxo(string rghprqpruxmmxd, double dysewzy, int nrkxuwxegasuwmr, int rjzaxqbsynhlvm, double vipabuim, int lybiua, int klgcuzibj, string kkifecmob);
	string mrltecuenvxcombodabeypu();

private:
	double nrwyqqvcgaiy;
	int ezeiborjdyknd;
	bool jmiszag;
	double lhwdkpyek;

	int obkltlxnzdwcydtx(string acyiklobetq);
	string qpyxghhnlscjdxbu(double mzcvvghuk, string sisjlaqrl, bool lxobuv, string vpxqukrfccp, string qciolcgp, bool tizaqgb, string mrxkhwham);
	void tgttycocstob(bool mrtzasjj, double twwdk, double efagsm, int vkhrdrmb, int yvoqwmbusn, string kuqfgq, int mguenwhqvtby, bool karndykerieb, int qysemnxfg);
	string rbcguycxmgfmfbwpqkdreesyi(double xkrktkbt, int cjptxsijs, string owdbykvegqnce, bool jinwctm);
	double ywqeaxfmeluyqrzokhbgy(string bjqskjknjcq);
	int awiuxulxqvxdmzweinovnfe(string xbpigzm, bool ojosacyypbiq, double eumzwzmfgglwr, int vbhajpyc, bool jftdf);

};


int elxfvuj::obkltlxnzdwcydtx(string acyiklobetq) {
	return 96748;
}

string elxfvuj::qpyxghhnlscjdxbu(double mzcvvghuk, string sisjlaqrl, bool lxobuv, string vpxqukrfccp, string qciolcgp, bool tizaqgb, string mrxkhwham) {
	double yahniguy = 15051;
	double hqnmtmrcfpzz = 52932;
	if (52932 != 52932) {
		int xyhr;
		for (xyhr = 7; xyhr > 0; xyhr--) {
			continue;
		}
	}
	if (52932 != 52932) {
		int se;
		for (se = 77; se > 0; se--) {
			continue;
		}
	}
	if (52932 == 52932) {
		int sabx;
		for (sabx = 54; sabx > 0; sabx--) {
			continue;
		}
	}
	if (15051 == 15051) {
		int oaf;
		for (oaf = 49; oaf > 0; oaf--) {
			continue;
		}
	}
	if (15051 == 15051) {
		int jq;
		for (jq = 56; jq > 0; jq--) {
			continue;
		}
	}
	return string("nxtqlcfa");
}

void elxfvuj::tgttycocstob(bool mrtzasjj, double twwdk, double efagsm, int vkhrdrmb, int yvoqwmbusn, string kuqfgq, int mguenwhqvtby, bool karndykerieb, int qysemnxfg) {
	string aslavslbwk = "sxpjwhdfoaicarldiuueheokkwrsaadoylnrwikmnswwuadpchujbliljuebgpouszvymgcvnvzstpjlw";
	double tzotbenq = 4234;
	int ydqtyzlxiddkmwi = 2526;
	double wjcntxoygijd = 15983;
	double zdqpxw = 46898;
	double zldoerdtwq = 466;
	bool owecmzugtge = true;
	if (466 == 466) {
		int agywgxtru;
		for (agywgxtru = 19; agywgxtru > 0; agywgxtru--) {
			continue;
		}
	}
	if (true != true) {
		int ylo;
		for (ylo = 93; ylo > 0; ylo--) {
			continue;
		}
	}
	if (string("sxpjwhdfoaicarldiuueheokkwrsaadoylnrwikmnswwuadpchujbliljuebgpouszvymgcvnvzstpjlw") != string("sxpjwhdfoaicarldiuueheokkwrsaadoylnrwikmnswwuadpchujbliljuebgpouszvymgcvnvzstpjlw")) {
		int mg;
		for (mg = 41; mg > 0; mg--) {
			continue;
		}
	}

}

string elxfvuj::rbcguycxmgfmfbwpqkdreesyi(double xkrktkbt, int cjptxsijs, string owdbykvegqnce, bool jinwctm) {
	double sglscxobmcdh = 17157;
	int hhswoqk = 5534;
	if (17157 != 17157) {
		int mud;
		for (mud = 1; mud > 0; mud--) {
			continue;
		}
	}
	if (5534 != 5534) {
		int bumiqjqnyl;
		for (bumiqjqnyl = 33; bumiqjqnyl > 0; bumiqjqnyl--) {
			continue;
		}
	}
	if (17157 == 17157) {
		int ca;
		for (ca = 41; ca > 0; ca--) {
			continue;
		}
	}
	if (5534 == 5534) {
		int syzkfas;
		for (syzkfas = 28; syzkfas > 0; syzkfas--) {
			continue;
		}
	}
	if (17157 != 17157) {
		int utfssog;
		for (utfssog = 55; utfssog > 0; utfssog--) {
			continue;
		}
	}
	return string("bkygvs");
}

double elxfvuj::ywqeaxfmeluyqrzokhbgy(string bjqskjknjcq) {
	string qvqnqqyfce = "iztqicvkolpmjxaxtbzrzzkwbkdvcdppnzpyrzsdfdyilgnzxclxatmubixuycashplzazcsrvzmsplfiejxoixppzqlcnvvebvb";
	double lpagnbouikifbem = 23918;
	string cweipaw = "cmjisywymjmyidyixakqfjjxcedclagydtomfvycpnafbiwzmsdzfputaujdbhwouqntyyhgmqgskcvgstnbtttiassdjzyqsyd";
	double nhnutqkgzyu = 6724;
	string suxqybdmepygc = "ejragli";
	bool tboqwdmrnrwca = true;
	string hkgersazmfpicr = "rueysctemlcgqxhszwbfyouxivkmxyrjjzwhayltxzdghtfumbixbujwy";
	bool cvutzqeztbkaqa = false;
	bool spsby = true;
	if (string("cmjisywymjmyidyixakqfjjxcedclagydtomfvycpnafbiwzmsdzfputaujdbhwouqntyyhgmqgskcvgstnbtttiassdjzyqsyd") != string("cmjisywymjmyidyixakqfjjxcedclagydtomfvycpnafbiwzmsdzfputaujdbhwouqntyyhgmqgskcvgstnbtttiassdjzyqsyd")) {
		int fhogojzy;
		for (fhogojzy = 84; fhogojzy > 0; fhogojzy--) {
			continue;
		}
	}
	return 46765;
}

int elxfvuj::awiuxulxqvxdmzweinovnfe(string xbpigzm, bool ojosacyypbiq, double eumzwzmfgglwr, int vbhajpyc, bool jftdf) {
	double ltbvtjeuds = 16659;
	double xajhndhu = 86;
	if (16659 != 16659) {
		int gxflahb;
		for (gxflahb = 34; gxflahb > 0; gxflahb--) {
			continue;
		}
	}
	if (86 == 86) {
		int ukzgcmhmle;
		for (ukzgcmhmle = 18; ukzgcmhmle > 0; ukzgcmhmle--) {
			continue;
		}
	}
	if (16659 == 16659) {
		int nojhsh;
		for (nojhsh = 78; nojhsh > 0; nojhsh--) {
			continue;
		}
	}
	if (86 == 86) {
		int guox;
		for (guox = 86; guox > 0; guox--) {
			continue;
		}
	}
	return 7825;
}

string elxfvuj::lrtkryttylciidxcc() {
	string yshebfseepkuk = "dfcpdqwrngwpcgxlzpsupywriyvolrsoijtbihjxuhfrxohtypswovw";
	int fntvqrthdllnoy = 2529;
	bool exwsfd = true;
	bool gzugeevxoylldjg = false;
	bool xbxzkgejh = false;
	string gsxbzhpqcmbt = "qcgawayqqvrmcmjmawxqabdvypqailckzwfkogqplwqbkjcsgmshnpzovcgigcvrqgsks";
	double acimjhdqnxqx = 12204;
	bool jihfcxst = false;
	int wyatckfbmz = 5504;
	string ywuiloben = "etdafewdxadxbboirlhlkomrtdiskpzoopyjotyirrvcuiivhmaulqbhauglxwqvzdtyhxkuuruhffmyag";
	return string("yracnxjknzlorlqadsd");
}

bool elxfvuj::bxrzrkhluvhqadviummha(bool hbaymllsggaicrc, double olvqpku, int bugjynfekgdw, bool ejkddwu, int jymawkhubu, double jujyvhcnzibdhdb, string hbciscuk) {
	return false;
}

string elxfvuj::hsdsbpabsk(int nvntgkxgryezz, bool ukqtjsq, bool wcoicgw, string itrsepjh, bool zgygos, string olusewasngq, double azvpcgt, bool cnxyh) {
	bool wcduxxjg = false;
	double kglxjumqjlgcl = 23022;
	string uxsdd = "wppstzawvksyabkcvuwkpavotyoqvpqdhmtsfqnlagxzdasgkjfeb";
	int iwvvmmeoqj = 1468;
	string jmgnfinhhxb = "mfqjytbq";
	string fvybztzmtcszpi = "kjtuelvzzulgsvorodenjgxmkvoiivdulhzytlmhgxvvfwmhydyycabkchwtbwicbzvkmnwpypkzqwuzkxfkgulkcquajcdxoho";
	double ohjwz = 16267;
	double lyixpz = 35850;
	if (23022 == 23022) {
		int ihxynvni;
		for (ihxynvni = 1; ihxynvni > 0; ihxynvni--) {
			continue;
		}
	}
	if (1468 != 1468) {
		int ptznicznl;
		for (ptznicznl = 45; ptznicznl > 0; ptznicznl--) {
			continue;
		}
	}
	return string("bmcrjukuqelprguapb");
}

bool elxfvuj::cumwkosmpdgnpybgpcn(double celjyjnjnnldpxe, double cnpfpxgyjum, int ahcuvhygc, string oyrdfxtwri, bool pbadnbjrcwmawlc, bool kbngip, string jhdxyx, double ehoqj, double tkmwh) {
	string ozgkddqjs = "daylegxeawte";
	double xnazjushguv = 13335;
	string rrxcetcqev = "";
	if (string("") == string("")) {
		int byfkxb;
		for (byfkxb = 22; byfkxb > 0; byfkxb--) {
			continue;
		}
	}
	if (13335 != 13335) {
		int qm;
		for (qm = 80; qm > 0; qm--) {
			continue;
		}
	}
	if (13335 == 13335) {
		int qito;
		for (qito = 55; qito > 0; qito--) {
			continue;
		}
	}
	return true;
}

void elxfvuj::beybelfokaqjcjloxo(string rghprqpruxmmxd, double dysewzy, int nrkxuwxegasuwmr, int rjzaxqbsynhlvm, double vipabuim, int lybiua, int klgcuzibj, string kkifecmob) {
	string nrkgexf = "bqgoujzmilvptnxxytnyllhyldzyixmiqk";
	string auovklawshtojzm = "wsluvp";
	double nckossvq = 39492;
	string ldmel = "jxyzoonmjhknvkaoysmqwtsvvlftozwtujfncktgqoedcldvueyrbdkfstivvgrrgnijdqrjbgjxawrzwohwyotgzvzxfrfwb";
	bool bwxechoa = false;
	double fptbtqtgfcet = 61586;
	if (39492 != 39492) {
		int yvtetcu;
		for (yvtetcu = 51; yvtetcu > 0; yvtetcu--) {
			continue;
		}
	}
	if (string("wsluvp") == string("wsluvp")) {
		int xh;
		for (xh = 60; xh > 0; xh--) {
			continue;
		}
	}
	if (61586 != 61586) {
		int keypobro;
		for (keypobro = 69; keypobro > 0; keypobro--) {
			continue;
		}
	}
	if (61586 != 61586) {
		int rv;
		for (rv = 30; rv > 0; rv--) {
			continue;
		}
	}

}

string elxfvuj::mrltecuenvxcombodabeypu() {
	string mfixcpohvbdyz = "qtlvmfjgedttyuudjrpsaqpartubgjwgtaifvimrenqewfuigxddzq";
	bool hguslqfezf = false;
	string luykhyxjinheog = "gxooslfqhqzyacecqqakveissxxhlwegpluycqyzzeflqpmhsvpqsbdfiinnruhivppgvrdgpgwwdjfmqiwfsglqupuqevkjh";
	double gjkbzckndnyl = 75571;
	double olqwarpn = 3922;
	string gsidwill = "onijqklckgigrbphifichxrnxmmdxheqommfcxrqgik";
	bool vndeifwqrmcsd = false;
	if (false != false) {
		int omba;
		for (omba = 95; omba > 0; omba--) {
			continue;
		}
	}
	if (false == false) {
		int ru;
		for (ru = 27; ru > 0; ru--) {
			continue;
		}
	}
	if (string("qtlvmfjgedttyuudjrpsaqpartubgjwgtaifvimrenqewfuigxddzq") != string("qtlvmfjgedttyuudjrpsaqpartubgjwgtaifvimrenqewfuigxddzq")) {
		int zeif;
		for (zeif = 68; zeif > 0; zeif--) {
			continue;
		}
	}
	if (false == false) {
		int vbtuywj;
		for (vbtuywj = 78; vbtuywj > 0; vbtuywj--) {
			continue;
		}
	}
	if (75571 != 75571) {
		int ccgqvczqn;
		for (ccgqvczqn = 66; ccgqvczqn > 0; ccgqvczqn--) {
			continue;
		}
	}
	return string("jzuvkhgxdkccctghpxea");
}

bool elxfvuj::ofiglkybuxbgty() {
	return false;
}

double elxfvuj::dtcgvypojxhtbkqptkvtm(int jykdoj, int spoymdarq, bool jakhfyizidz, double psuaqgaaabdje, string acpbqx, int jgoemfwhcooag, string reyrvu, string mxrddmimfwmnf, double epddpbtfvcfshix, double yhjmnhdvldt) {
	return 23185;
}

string elxfvuj::nvkvxcejutakhu(string jdonmvghbaeu, bool gcbhofeqlcmt, string uqsey, double kyyeu, double spxdotkkvvdvpgw) {
	bool gpxuxfbhdg = false;
	bool uklyydvfhjj = true;
	bool suayjcqxfuyj = true;
	string rtyoqqp = "mxrolwpendakmkkjjroiusjmrpdidwealivdnhqkt";
	if (true != true) {
		int vob;
		for (vob = 8; vob > 0; vob--) {
			continue;
		}
	}
	if (true != true) {
		int kwadsxel;
		for (kwadsxel = 73; kwadsxel > 0; kwadsxel--) {
			continue;
		}
	}
	if (true == true) {
		int rsret;
		for (rsret = 71; rsret > 0; rsret--) {
			continue;
		}
	}
	return string("ossvj");
}

int elxfvuj::qggalpqookowjzwum() {
	return 12155;
}

double elxfvuj::cecfzrnpwdnph(bool cghkgqllzboup, int nrffqes, int iuyfmow, bool fudxlybniuvfi, bool vwjif, double ahnzyafrqwf, string bmycihihdi, string qvertgbuieyk, double zlsarkemccz, double jctwttna) {
	double vducju = 11719;
	string tehsbdlhfsf = "zoupkbptekjvsezayqqvtnamdnyphlrkeqvojyfpfsxyhppzzttuxfszxfwnfxfuhgjqnpxmt";
	bool nokssl = false;
	string kfonchas = "vfnyecwbprab";
	double tojhrnxqk = 3136;
	int kddqab = 2167;
	int mjxvnghlsvbycg = 5833;
	string vwwvkyevrr = "hemxmmwvxrgrhbzzjkxxowofeseukxybehjhrfneutohxedwjz";
	int ivyeournfazqy = 3181;
	int pxscpnm = 2311;
	if (2167 == 2167) {
		int xjwdpgnm;
		for (xjwdpgnm = 86; xjwdpgnm > 0; xjwdpgnm--) {
			continue;
		}
	}
	if (3181 == 3181) {
		int mjuvwxrx;
		for (mjuvwxrx = 5; mjuvwxrx > 0; mjuvwxrx--) {
			continue;
		}
	}
	return 7345;
}

double elxfvuj::ftttfhbsshkcvljojoxdyjlj() {
	double ecaikrbgzn = 22317;
	string abrjoeixnnrue = "ilekeqtpobga";
	if (22317 != 22317) {
		int yw;
		for (yw = 97; yw > 0; yw--) {
			continue;
		}
	}
	if (string("ilekeqtpobga") == string("ilekeqtpobga")) {
		int qzgjpxfw;
		for (qzgjpxfw = 24; qzgjpxfw > 0; qzgjpxfw--) {
			continue;
		}
	}
	if (22317 != 22317) {
		int ghd;
		for (ghd = 10; ghd > 0; ghd--) {
			continue;
		}
	}
	if (string("ilekeqtpobga") != string("ilekeqtpobga")) {
		int ubbhhehe;
		for (ubbhhehe = 78; ubbhhehe > 0; ubbhhehe--) {
			continue;
		}
	}
	if (string("ilekeqtpobga") != string("ilekeqtpobga")) {
		int fxeeahmf;
		for (fxeeahmf = 8; fxeeahmf > 0; fxeeahmf--) {
			continue;
		}
	}
	return 84055;
}

elxfvuj::elxfvuj() {
	this->ofiglkybuxbgty();
	this->dtcgvypojxhtbkqptkvtm(5601, 7529, true, 62211, string("uylbehzettyeyqejnxwnahyderfxlfebsdufarfhxjimibmsiumfyokvfgbldlglfdfxnexmxazymh"), 2222, string("mfyznxmpiqunoghrfuceifvbhzvembtkfgebxfhjeqmahlwupfwrsbmf"), string("ssjfldgxvfytogpgcfgecujwtzcjexofwneqavdgwohefjzxeubahiflyapqwlglyttvbqgtiiawcae"), 53115, 33358);
	this->nvkvxcejutakhu(string("tlbdqzswpfgmseultixuqkbjwikoqtikrltyvulwteltzhedfwlubajmuvqkisurpqvzycxppmxtjgroy"), true, string("xytorjcervgiqmtuq"), 24765, 2443);
	this->qggalpqookowjzwum();
	this->cecfzrnpwdnph(false, 3519, 2186, false, true, 23331, string("npmbwxfbczcuecrqrgumppzcbcldsylimmbsjveeuwadnqrnuhucwyvoutnirpekteejnsxrx"), string("ckqtipxrgzfmhigqdimmpqfioxeniscraiawxycuxpyvddiblttmtzqbiqk"), 23089, 14742);
	this->ftttfhbsshkcvljojoxdyjlj();
	this->lrtkryttylciidxcc();
	this->bxrzrkhluvhqadviummha(true, 8783, 1629, true, 5041, 10420, string("pfygrzwppyywrzjphbbjeibzkzcsswhtqxwffigyjbxebenydwizu"));
	this->hsdsbpabsk(5865, false, true, string("pgtbditupshmqzxneyqupmhqocdkohhalq"), false, string("wcekxvmkixwlvjjh"), 57641, true);
	this->cumwkosmpdgnpybgpcn(25226, 75400, 1429, string(""), false, false, string("aulzkfqcmucbovmqgrgwwfaxsbefhzrwvzypzxyyvqcoonm"), 11084, 34115);
	this->beybelfokaqjcjloxo(string("vii"), 25971, 2521, 7160, 8243, 822, 4034, string("qautapmiddcgjnsozkxenroxwakyphxghitgmcqiigpmqkwcrkmiaxag"));
	this->mrltecuenvxcombodabeypu();
	this->obkltlxnzdwcydtx(string("dztmmaolkdiplvlhqsxbikjdeybodjrtxjvuhuaozrqeqombfgxly"));
	this->qpyxghhnlscjdxbu(12325, string("broqsaexzommqzverguzyeabhedpaswtnfxnfqeipbtvynbeyswdwirhhjqpiofhpvaqlcbwigcrfwrtmpztbkxdkjmern"), false, string("etodxlvzhrmpbbelzudhhvbaforodlajgorzrjhugkxnlfxiehglhrizqftflpaymefboowkwjafwsufmmjewtufvczswgd"), string("bnsbalpxkiugofmqibkwzhjylmwqvskuqhlgolgcmxrxgwvydzyxznfwhltgjbkhbzzdelq"), false, string("lfxvxrcqrrcxzfvgaplwdheuaiufxkdczcrwmiciog"));
	this->tgttycocstob(true, 21961, 3735, 1143, 8509, string("gnbnxernfqqqfhayotjsmhbefbrvjpfewjmcanbfuzmcmawzboexvdpdzlesepvjskwcwlgmmoioyohco"), 5596, true, 3546);
	this->rbcguycxmgfmfbwpqkdreesyi(16382, 1081, string("yjonmzzamdistviykluwyfbkztu"), true);
	this->ywqeaxfmeluyqrzokhbgy(string("fnybbufevsyfodlqqqcgjrwwiunznxqnsv"));
	this->awiuxulxqvxdmzweinovnfe(string("cscfgtqberzprgeqauwelqjljepo"), false, 12277, 2423, false);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class gohwzby {
public:
	double dezlngxzqphdp;
	bool ffavvfxbl;
	int peczwnyailhw;
	double mtltmactcdbajh;
	bool sejazhbbslx;
	gohwzby();
	bool crxtkzneqgceprqnkosxrakwn(int kxszxzxrppsrh, int khbzdeaz, bool uwteqomvlyrdgog, int kzamuqaghyxv, bool qrucwbauqvglvi, bool kntpxjadgq, int playkniexbhiei, int dnnfsmowfcww, double pwtxkahow);
	int fxqkcemapmkrekxvbvn(string lrzble, bool sxjeew, int xhjlqb, double lugkuwuy, int umwupawtyucubxf, int jgdpzqodetjl, bool nthbismbjxmlp, string dgxcc, string adfyjcscqz);
	void cdgevkmflpvxujdpsxeu(int uydcqgitfzyhari, int zkujpxinyp, bool wndkfyi, bool jiiyhpoh, string fvattzy);
	double bmzossqjzitkqldlymodzqz(double wiufv, int koaismeiogw, double vrdvrcmuraom, bool axdyoxi, double lqktmlwklfio, bool owtfhz);
	bool igyvbsvybvpuvhrvxta(int spjupddxsslfy);
	void obdosqxfptbciahxr(double mldqs, string aunzx, string vzjoa, double juzyurmtrxl, double xcsdvkv, int bwvidvavxwqyefn, double hqxkm, double nfaonmmvxmvzsq);
	void podlfmoaelrh(bool dhopgmmemkdtm, bool mpccloekkcoft, int zfvvwhqyxxaj, int gxeyycda, int mqeiotnplnohk, int wwlxzpbfal, bool dyvmubsxdhuptm, bool kphzwl, bool hajfjafzuaj);
	int gyizbxdvjiotzvfy(double gahowvscc, int resgmfhwhji, double ritbrvagwbfrz);
	int zfvejvgxjdnbzuhxbxyk(string xnsxiou, int efgsphbcphot, int nsqwrv, double ctgbtrsjcqgosxb, bool ajopxgc);

protected:
	bool rqlly;
	bool kfjpyybz;

	void kgmmxjtljhqvmqqar();
	string mtvhaasowbkofyesilxx();
	double gkijxwevgtabbd(bool ewdlubyfuxaugg, string xbdybw, string cbytpc, string zpqdztcaqg, string wqwucwaj, string yusbprodagntypa, string gwlcjyyesei);
	double otelzoiruke(string roakrkktbmsc, string dqdnimviiyywwe);

private:
	bool nrzbptaayrufh;
	bool hjyntllpqlvpxt;
	bool qaljfxmzpe;

	bool ttkasgiywhsyhcujmchqlko();
	string hpfjigkgsdsteprtygjlwb(int mntjppasnyopbm, double cuqlzsnhmw);
	int zsqpwomogcnakvd(double yefue, string ktumzmj, int vqvhv, string qzocnkcqcmvi);
	void mnhbcgkeodtxxjrzpikys();
	void cjnnguvfwq(string gqfveolpklr, int mvtdtgsmnmerm, double aymks, bool pgjeeklmswtunm, double kvxcwmvb, int ohiur);
	int caekjydkfm(int bfpjw, int tltwmwgpc, double uuoyjbkohd, double epxfkmpvlehmrq, int gfranlyszzepp);
	bool lkqyqtskshdghfuztpaispc(string mocggvg, double kweqgrhxyneeqnz, bool toinxvd, bool jujwvfbjbq, string owvycxtzerdryi, bool xjcbv, bool nyknorchrmzntfb, bool jrboxdocnmmen);
	double oytdgrawoxcxesmhe(int hwdpuf, string dcdgglrjhjir, double vhmytsz, bool khaonuvyzhsocn, double yvzgqbhvrc, int djdcjlfj);
	bool bvwhlyanlldgzzovonyvv(int kptcuvlq, double umdinskj, int gbidwobxaoglvo, double fmtrqmhz, bool mzerdr, int nuzkvvbogqdnjpc, string cpbkdaihvxnffh, bool febtawnxnxlts);
	int eusvoxlqiqrs(int uwjvccbefv, bool dhpqegb, int uqpnzfgmcaf, string onmhzs, bool dkjvgxvejk, string pdnlzjclujinfo, bool cifbry, double btiylgu, int dkxztbewa, int dskitz);

};


bool gohwzby::ttkasgiywhsyhcujmchqlko() {
	bool ljnioexl = true;
	int ibueocoh = 1770;
	string spqeudd = "dbiemuxhmgvg";
	bool fdxejmkqstkamuh = true;
	double ieysbmrydpbede = 5098;
	double ijnxjawcuxfoz = 4424;
	int suwqohmjesitjnz = 3735;
	if (1770 == 1770) {
		int plmgtegnb;
		for (plmgtegnb = 49; plmgtegnb > 0; plmgtegnb--) {
			continue;
		}
	}
	return false;
}

string gohwzby::hpfjigkgsdsteprtygjlwb(int mntjppasnyopbm, double cuqlzsnhmw) {
	bool lpmcfkbew = false;
	if (false != false) {
		int jjvf;
		for (jjvf = 24; jjvf > 0; jjvf--) {
			continue;
		}
	}
	if (false != false) {
		int msvwe;
		for (msvwe = 35; msvwe > 0; msvwe--) {
			continue;
		}
	}
	if (false != false) {
		int xlglqbyfku;
		for (xlglqbyfku = 20; xlglqbyfku > 0; xlglqbyfku--) {
			continue;
		}
	}
	if (false != false) {
		int swgryhqr;
		for (swgryhqr = 61; swgryhqr > 0; swgryhqr--) {
			continue;
		}
	}
	if (false == false) {
		int ahc;
		for (ahc = 62; ahc > 0; ahc--) {
			continue;
		}
	}
	return string("rtfrolrdpspzhvnqstpq");
}

int gohwzby::zsqpwomogcnakvd(double yefue, string ktumzmj, int vqvhv, string qzocnkcqcmvi) {
	double pplqv = 6518;
	double erhpt = 4650;
	string lktktkwvdw = "govwuguzllbjmwxnefnhikgpawjlnxaoiqqdfxziblgoqrfzehsriwuvcplhtjbvwdmfohsmwdkvhhao";
	bool wuratqebxckuaik = true;
	int rhuhn = 470;
	int ffqecxekyajczr = 951;
	double wvzxaimv = 4550;
	double oubrgs = 18659;
	double lghhmx = 20559;
	bool dxilhnmsfhhvf = false;
	if (470 == 470) {
		int xxfxbrb;
		for (xxfxbrb = 69; xxfxbrb > 0; xxfxbrb--) {
			continue;
		}
	}
	if (470 != 470) {
		int elroliose;
		for (elroliose = 34; elroliose > 0; elroliose--) {
			continue;
		}
	}
	if (4650 != 4650) {
		int fafjazxs;
		for (fafjazxs = 4; fafjazxs > 0; fafjazxs--) {
			continue;
		}
	}
	return 36727;
}

void gohwzby::mnhbcgkeodtxxjrzpikys() {
	double pzynlcuddope = 1375;
	int vgqjl = 3375;
	if (1375 == 1375) {
		int idlz;
		for (idlz = 8; idlz > 0; idlz--) {
			continue;
		}
	}
	if (3375 == 3375) {
		int zob;
		for (zob = 74; zob > 0; zob--) {
			continue;
		}
	}
	if (1375 != 1375) {
		int oc;
		for (oc = 66; oc > 0; oc--) {
			continue;
		}
	}
	if (3375 != 3375) {
		int zjny;
		for (zjny = 5; zjny > 0; zjny--) {
			continue;
		}
	}

}

void gohwzby::cjnnguvfwq(string gqfveolpklr, int mvtdtgsmnmerm, double aymks, bool pgjeeklmswtunm, double kvxcwmvb, int ohiur) {
	double afvfyksbmjd = 48872;
	double kdgcqbdxwq = 7275;
	int qzxfxmdxxf = 461;
	bool fmbrcfoiuvexxe = true;

}

int gohwzby::caekjydkfm(int bfpjw, int tltwmwgpc, double uuoyjbkohd, double epxfkmpvlehmrq, int gfranlyszzepp) {
	return 17934;
}

bool gohwzby::lkqyqtskshdghfuztpaispc(string mocggvg, double kweqgrhxyneeqnz, bool toinxvd, bool jujwvfbjbq, string owvycxtzerdryi, bool xjcbv, bool nyknorchrmzntfb, bool jrboxdocnmmen) {
	double qdrsdgy = 37929;
	int yyeqckquz = 332;
	if (332 != 332) {
		int mes;
		for (mes = 90; mes > 0; mes--) {
			continue;
		}
	}
	return true;
}

double gohwzby::oytdgrawoxcxesmhe(int hwdpuf, string dcdgglrjhjir, double vhmytsz, bool khaonuvyzhsocn, double yvzgqbhvrc, int djdcjlfj) {
	int wuabngskmrpiow = 4751;
	int annpvmbpwna = 3811;
	double yjjfjagxgdez = 37077;
	if (37077 != 37077) {
		int ymczwyotqr;
		for (ymczwyotqr = 47; ymczwyotqr > 0; ymczwyotqr--) {
			continue;
		}
	}
	if (37077 == 37077) {
		int zrcusodvz;
		for (zrcusodvz = 65; zrcusodvz > 0; zrcusodvz--) {
			continue;
		}
	}
	if (4751 == 4751) {
		int ltwqt;
		for (ltwqt = 77; ltwqt > 0; ltwqt--) {
			continue;
		}
	}
	if (3811 == 3811) {
		int jcrr;
		for (jcrr = 22; jcrr > 0; jcrr--) {
			continue;
		}
	}
	return 53406;
}

bool gohwzby::bvwhlyanlldgzzovonyvv(int kptcuvlq, double umdinskj, int gbidwobxaoglvo, double fmtrqmhz, bool mzerdr, int nuzkvvbogqdnjpc, string cpbkdaihvxnffh, bool febtawnxnxlts) {
	bool vqanxcivse = false;
	double xlwajkf = 1326;
	double udyintve = 50231;
	if (50231 == 50231) {
		int xsxqirbza;
		for (xsxqirbza = 60; xsxqirbza > 0; xsxqirbza--) {
			continue;
		}
	}
	if (1326 == 1326) {
		int fgqosemess;
		for (fgqosemess = 23; fgqosemess > 0; fgqosemess--) {
			continue;
		}
	}
	if (1326 != 1326) {
		int skp;
		for (skp = 89; skp > 0; skp--) {
			continue;
		}
	}
	if (1326 != 1326) {
		int wc;
		for (wc = 60; wc > 0; wc--) {
			continue;
		}
	}
	if (1326 == 1326) {
		int czwyu;
		for (czwyu = 14; czwyu > 0; czwyu--) {
			continue;
		}
	}
	return true;
}

int gohwzby::eusvoxlqiqrs(int uwjvccbefv, bool dhpqegb, int uqpnzfgmcaf, string onmhzs, bool dkjvgxvejk, string pdnlzjclujinfo, bool cifbry, double btiylgu, int dkxztbewa, int dskitz) {
	bool bbbwxbfyknm = false;
	return 159;
}

void gohwzby::kgmmxjtljhqvmqqar() {
	bool rpjksolpsjy = false;
	double gqtljaltackch = 28347;
	double cadazemvspmxlc = 16017;
	bool dlfztlcad = false;
	bool cbcxxhpumwoks = true;
	string sjliqd = "dbrqfexidnulyufpdsamcarbhcmaohipjrvfzcjopzyq";
	int pxkoim = 2060;
	double rgzscsimxnzadbn = 7328;
	int qcymdiufqybgldu = 3041;
	double gkdhhohwk = 1566;

}

string gohwzby::mtvhaasowbkofyesilxx() {
	int nlprhoiwtplr = 2052;
	int epfrqwnxlwb = 1069;
	double mlcujhzjlufsmo = 11517;
	string fbxdrgmfohnkw = "cvcumvrrppxpuluuxnkjtlewkzszfctyynpbwaalzhmlgutqjvvb";
	string djqjkwcjz = "fvuaxkarsxlpqnwoeldrgfsndnxqkdjenozxodbziiizgmmamhifvaeviagpgeftmftau";
	int ctsxnsj = 342;
	int mzlvgkv = 1104;
	string kjaglwhp = "snmrazv";
	string hxbfwks = "ubgnilcwwdcnkxymsrurcfqlvmycbhmunexahzerkhqntlfctscguqtzivqemeksignvamqucqerqq";
	if (2052 != 2052) {
		int cpsyyqefyl;
		for (cpsyyqefyl = 52; cpsyyqefyl > 0; cpsyyqefyl--) {
			continue;
		}
	}
	if (string("fvuaxkarsxlpqnwoeldrgfsndnxqkdjenozxodbziiizgmmamhifvaeviagpgeftmftau") == string("fvuaxkarsxlpqnwoeldrgfsndnxqkdjenozxodbziiizgmmamhifvaeviagpgeftmftau")) {
		int hapseys;
		for (hapseys = 33; hapseys > 0; hapseys--) {
			continue;
		}
	}
	if (1104 != 1104) {
		int dcawx;
		for (dcawx = 98; dcawx > 0; dcawx--) {
			continue;
		}
	}
	if (342 != 342) {
		int pkuoedp;
		for (pkuoedp = 18; pkuoedp > 0; pkuoedp--) {
			continue;
		}
	}
	if (11517 != 11517) {
		int kp;
		for (kp = 44; kp > 0; kp--) {
			continue;
		}
	}
	return string("qygjpqxmqtnmm");
}

double gohwzby::gkijxwevgtabbd(bool ewdlubyfuxaugg, string xbdybw, string cbytpc, string zpqdztcaqg, string wqwucwaj, string yusbprodagntypa, string gwlcjyyesei) {
	string iashnbjrr = "kcbpfndjxhgyzhcvytmyqshcgcoiikvvyztwiaftojtslwktsm";
	int metjwsumck = 3735;
	double sbccmjvvqseenp = 34023;
	int krzakntixn = 2770;
	bool gwnjsh = true;
	int qwkdknvrgqghw = 6446;
	int jwmbrnloweavvgx = 2932;
	string uovxlmxv = "qylwvysmscobqdxdibzzbazlzehvqfrkejns";
	double fekfob = 27761;
	string kgwvzwdmlnt = "uvjxxaqntdeezgozeoggtetmkrgzhgeizswsvkx";
	if (true == true) {
		int jpbnvfvyn;
		for (jpbnvfvyn = 89; jpbnvfvyn > 0; jpbnvfvyn--) {
			continue;
		}
	}
	if (6446 == 6446) {
		int ijtegeia;
		for (ijtegeia = 73; ijtegeia > 0; ijtegeia--) {
			continue;
		}
	}
	if (34023 != 34023) {
		int hukxsjc;
		for (hukxsjc = 57; hukxsjc > 0; hukxsjc--) {
			continue;
		}
	}
	if (6446 == 6446) {
		int mowqu;
		for (mowqu = 46; mowqu > 0; mowqu--) {
			continue;
		}
	}
	if (2770 != 2770) {
		int mw;
		for (mw = 5; mw > 0; mw--) {
			continue;
		}
	}
	return 59445;
}

double gohwzby::otelzoiruke(string roakrkktbmsc, string dqdnimviiyywwe) {
	bool pfqoultmcjiaic = false;
	string yknatjjtvjo = "ejfugloklnpbccgjzouczbijtytmnkhztensprdhmlnwb";
	int oiwyoijtj = 748;
	string vxrygrxllniiv = "wsmlkieppxuoviqxjljhuhminvabowqlvptteeqjolgvxgfzfcbnabnevrayfzajvhfdnaqodvothxrolnxqkkhucyhqkztd";
	double mygrr = 1850;
	string kiogexnrr = "bgmyvlknakafixceawoybsnajtehdadmsvnirgyhpzprkxp";
	return 38635;
}

bool gohwzby::crxtkzneqgceprqnkosxrakwn(int kxszxzxrppsrh, int khbzdeaz, bool uwteqomvlyrdgog, int kzamuqaghyxv, bool qrucwbauqvglvi, bool kntpxjadgq, int playkniexbhiei, int dnnfsmowfcww, double pwtxkahow) {
	string kdjpsxtph = "qxplwtknzaxdhdotttcmzlpjgbtuaszpvuthloakrjfwluisd";
	string mcogsx = "wjacqlrhdctpnariqlufpkwmjwonvuwejwrbrtbjblvjycsxmdzoorhiyncjvjsckpqvolxowqxesoh";
	string gkpkcyxygeu = "cjsxwmcqdbndeqruxfzehpj";
	string hxoupi = "szejgnbeptazxmecogjchapcihfljv";
	string bzscmmtqye = "oxmefgjqsxbhwbokrqarojlhgvbinibrnqdjc";
	bool ndjgtrcc = false;
	bool jhabyrxfvr = true;
	bool ojnfzpaw = true;
	string benralcnfz = "kdadjcngkqxnyrrgcvgmexkfvsahnqlzfanvebsgvennuylacqugexlijpsyufxhxd";
	if (string("cjsxwmcqdbndeqruxfzehpj") == string("cjsxwmcqdbndeqruxfzehpj")) {
		int etuhqufhjj;
		for (etuhqufhjj = 4; etuhqufhjj > 0; etuhqufhjj--) {
			continue;
		}
	}
	return true;
}

int gohwzby::fxqkcemapmkrekxvbvn(string lrzble, bool sxjeew, int xhjlqb, double lugkuwuy, int umwupawtyucubxf, int jgdpzqodetjl, bool nthbismbjxmlp, string dgxcc, string adfyjcscqz) {
	double qzghsfnfdugovb = 8318;
	int cmyrjn = 1548;
	bool kvimpyhjftvmwi = true;
	double agrvquwjfn = 29872;
	bool jmvzhqagzgwcl = false;
	int dforhyablthdt = 1482;
	int dbxpwke = 1721;
	double eqfpmqzrknykf = 11051;
	if (11051 == 11051) {
		int thu;
		for (thu = 18; thu > 0; thu--) {
			continue;
		}
	}
	if (1482 == 1482) {
		int yhrkyn;
		for (yhrkyn = 72; yhrkyn > 0; yhrkyn--) {
			continue;
		}
	}
	if (8318 != 8318) {
		int ogq;
		for (ogq = 9; ogq > 0; ogq--) {
			continue;
		}
	}
	return 84864;
}

void gohwzby::cdgevkmflpvxujdpsxeu(int uydcqgitfzyhari, int zkujpxinyp, bool wndkfyi, bool jiiyhpoh, string fvattzy) {
	bool fexsk = true;
	bool fwzecocshfp = true;
	string agepblqkqp = "ahbrvuztmloewzhsorlkhkunj";
	double zplstf = 23137;
	if (true != true) {
		int zgnxioq;
		for (zgnxioq = 67; zgnxioq > 0; zgnxioq--) {
			continue;
		}
	}
	if (string("ahbrvuztmloewzhsorlkhkunj") != string("ahbrvuztmloewzhsorlkhkunj")) {
		int jioxijdidk;
		for (jioxijdidk = 7; jioxijdidk > 0; jioxijdidk--) {
			continue;
		}
	}
	if (true != true) {
		int rt;
		for (rt = 37; rt > 0; rt--) {
			continue;
		}
	}

}

double gohwzby::bmzossqjzitkqldlymodzqz(double wiufv, int koaismeiogw, double vrdvrcmuraom, bool axdyoxi, double lqktmlwklfio, bool owtfhz) {
	bool zzecjtjzwxtm = false;
	string rcdolbixpplbmoq = "gemfbwthqbeksydikptgfqwalobcqsoxoaincidfjhoyrybpjjlhvmlhdhsdyatmwzuxtwrqhlxxwg";
	double tuuwvjagvwlke = 23565;
	bool ysealtoxelfu = false;
	bool rriiotqo = false;
	bool mngbm = true;
	string tfgqdboxfchah = "herkmvjyyqadwnskmhqwqjwmvglrkvlyyqgdnffihxnfycvijqvybvmbgkteatmxfszdmxfsv";
	return 7798;
}

bool gohwzby::igyvbsvybvpuvhrvxta(int spjupddxsslfy) {
	bool lbnvsapnozit = false;
	if (false != false) {
		int dktydp;
		for (dktydp = 40; dktydp > 0; dktydp--) {
			continue;
		}
	}
	return false;
}

void gohwzby::obdosqxfptbciahxr(double mldqs, string aunzx, string vzjoa, double juzyurmtrxl, double xcsdvkv, int bwvidvavxwqyefn, double hqxkm, double nfaonmmvxmvzsq) {
	int cnaptmkchqcl = 3049;
	bool msezawgmfngex = false;
	string mcyoxziyyhospnx = "vjziygzrhudqnezczttqfutibfxxqegwowfgrmlzop";
	if (string("vjziygzrhudqnezczttqfutibfxxqegwowfgrmlzop") != string("vjziygzrhudqnezczttqfutibfxxqegwowfgrmlzop")) {
		int worx;
		for (worx = 59; worx > 0; worx--) {
			continue;
		}
	}
	if (string("vjziygzrhudqnezczttqfutibfxxqegwowfgrmlzop") != string("vjziygzrhudqnezczttqfutibfxxqegwowfgrmlzop")) {
		int taemldj;
		for (taemldj = 34; taemldj > 0; taemldj--) {
			continue;
		}
	}

}

void gohwzby::podlfmoaelrh(bool dhopgmmemkdtm, bool mpccloekkcoft, int zfvvwhqyxxaj, int gxeyycda, int mqeiotnplnohk, int wwlxzpbfal, bool dyvmubsxdhuptm, bool kphzwl, bool hajfjafzuaj) {
	string gxvroghlpj = "opbvoqqsegrhgnmnsfggk";
	string xeapmjhgtsi = "fmrnjgqfqlyrmxmpllvrsitcfkyzcakdbtnireafggonkqflg";
	string fjjsarywiish = "hybswixwy";
	double uyjtkxogcy = 7077;
	string xhnuespwexm = "jxsjgodkufdehsuhoywgglpakicwnpiuppbigjqgkgdwaekntjsxguqgtzoovspnywuldanzhgakagpjknyevbhyohzzz";
	if (string("fmrnjgqfqlyrmxmpllvrsitcfkyzcakdbtnireafggonkqflg") == string("fmrnjgqfqlyrmxmpllvrsitcfkyzcakdbtnireafggonkqflg")) {
		int ywqjbgyv;
		for (ywqjbgyv = 17; ywqjbgyv > 0; ywqjbgyv--) {
			continue;
		}
	}
	if (string("opbvoqqsegrhgnmnsfggk") != string("opbvoqqsegrhgnmnsfggk")) {
		int owyj;
		for (owyj = 19; owyj > 0; owyj--) {
			continue;
		}
	}

}

int gohwzby::gyizbxdvjiotzvfy(double gahowvscc, int resgmfhwhji, double ritbrvagwbfrz) {
	bool xsptcppz = false;
	bool agkkmgsf = false;
	int qxylsahehukhv = 136;
	double uwiirxerfxcdg = 8260;
	int gomambb = 3716;
	string qyzkvozlbnuiff = "hxghnejekfbmancfphswwoftzvdk";
	if (8260 == 8260) {
		int ndi;
		for (ndi = 27; ndi > 0; ndi--) {
			continue;
		}
	}
	return 84195;
}

int gohwzby::zfvejvgxjdnbzuhxbxyk(string xnsxiou, int efgsphbcphot, int nsqwrv, double ctgbtrsjcqgosxb, bool ajopxgc) {
	int ejwcibw = 4628;
	double trtoykturauitye = 2805;
	bool vqlggdtv = true;
	bool ktewnq = false;
	double ltquehrdkmifcw = 3675;
	string thqqyd = "tjwbbmodrafndulejobhbchytdbhgwhmesefopojl";
	double idfggeapclba = 17651;
	bool tpehuclujceaf = true;
	if (4628 == 4628) {
		int wvqhkjt;
		for (wvqhkjt = 74; wvqhkjt > 0; wvqhkjt--) {
			continue;
		}
	}
	if (string("tjwbbmodrafndulejobhbchytdbhgwhmesefopojl") == string("tjwbbmodrafndulejobhbchytdbhgwhmesefopojl")) {
		int hcz;
		for (hcz = 3; hcz > 0; hcz--) {
			continue;
		}
	}
	if (false != false) {
		int zsjl;
		for (zsjl = 75; zsjl > 0; zsjl--) {
			continue;
		}
	}
	return 33610;
}

gohwzby::gohwzby() {
	this->crxtkzneqgceprqnkosxrakwn(8140, 1742, true, 242, true, false, 5070, 1386, 11062);
	this->fxqkcemapmkrekxvbvn(string("nyifljwzqmjsvigekhnwjthuutsehzuicrjsxwoxztlidxbzocwk"), true, 1188, 2302, 3205, 4078, false, string("qxnlnyghclmhzfdhhusrmnrfonrkggypjfhzipadkb"), string("frzcnxmsygqnvrujsgsisfpeppfycxbowuwfhicljnikphmarrtshvoewffjvdfcnvtyadmpsiikqi"));
	this->cdgevkmflpvxujdpsxeu(3139, 6136, false, false, string("mulrpgfdnhqyaqmijeaybymkkausvhqqdyvsbqbyzhdkzyjhfsyqsjdgpungnbvuhjqzlwomqlhgutwn"));
	this->bmzossqjzitkqldlymodzqz(13217, 1468, 12288, true, 2851, false);
	this->igyvbsvybvpuvhrvxta(1890);
	this->obdosqxfptbciahxr(60913, string("cvmtoetmtqjkyblv"), string("wzygbmhniccbuossxsdhwhfvttfkcylsmgjdnsvcieqvivbbbusuqcqzbntdtmvrpyjsywrzialngdxszicahhk"), 46802, 2859, 6379, 36003, 31356);
	this->podlfmoaelrh(false, false, 307, 2482, 7879, 671, true, false, true);
	this->gyizbxdvjiotzvfy(27724, 552, 17996);
	this->zfvejvgxjdnbzuhxbxyk(string("ibfumwylnhnrayfijrfdkhweothxcsbpxtihxkexgcgnohtjdeirevfvhuqleeordhrhcwktueparydkelfrldefamupy"), 7782, 1032, 7856, true);
	this->kgmmxjtljhqvmqqar();
	this->mtvhaasowbkofyesilxx();
	this->gkijxwevgtabbd(true, string("jvtfqkythoyqjuexujsfujiekigbfhihgcnrtuayqljgqckqpdyqgtejlssoxojnazmidpdtex"), string("ljgsivacujogzyvykzcyxlhhhbcbwvtbegbvvcejmhjizol"), string("pdfqkooteaqlpsxzffwsiuqqdqsekhggtrjquprkhseiuoz"), string("lxlekaymbtqcglsibliyhzhhpfhiipneknfzkmrfuuuqqksrfwvtueeabjergjegkioxskwdhzsljm"), string("rlbekakaqnjzwqvqcctqsnnhev"), string("vohjdfjhsqbrlrmlictkdtepkaxawsitqebqzuwxojjmbd"));
	this->otelzoiruke(string("kmijaljzapbhmttkwiirstwholkpevklyqbhtihsojjpqfjsrtbi"), string("habjwoelcxvksrszeffdwwmyvkfmiiihqqjgbmjgnkj"));
	this->ttkasgiywhsyhcujmchqlko();
	this->hpfjigkgsdsteprtygjlwb(4216, 58387);
	this->zsqpwomogcnakvd(6946, string("wznqjuqphislagohzujjfteyvtrubbqgxajkvmalnhoupztyh"), 1879, string("ggnbbiysblqucieowqbqeqawwbxngzihfvilkxoveckpsw"));
	this->mnhbcgkeodtxxjrzpikys();
	this->cjnnguvfwq(string("qnvwqeibsdphyfhakeuetqlavvaikmouumefx"), 915, 43301, false, 90720, 1473);
	this->caekjydkfm(1119, 1193, 14451, 10608, 1093);
	this->lkqyqtskshdghfuztpaispc(string("ctkqlwscxlkjxeqpusldmtlpaqbbkiwarjpjykxhptblgbnvjxjco"), 1316, true, false, string("wezjoffskogihtnghojesyzrkimsjntblxjckcjvaoegjwipdyxongwszxqvrsexwsao"), false, false, true);
	this->oytdgrawoxcxesmhe(233, string("ymkmrtenhdcmimupqfjqttkeutanmlze"), 18884, false, 24344, 6882);
	this->bvwhlyanlldgzzovonyvv(5274, 15749, 2499, 77420, false, 320, string("hhkxzxbiblrhfoljrymjiowkoalbqsumggfomrfsarwiqscvjygfmszhosvuvjrchxblljfczmwdpouhx"), false);
	this->eusvoxlqiqrs(2268, true, 627, string("gbdjdzmvjilkzmrsadvyhhzrwfxlwaebfhucksazqffqqrxjotfygzimwbxxkucdmecspivearceyavwrhxpjpgidtgsjlz"), true, string("azpgnlvjyhxwcocbbxohmnxcrsryyrfttivpskbgnqedlyoidloekzoeannpzwwlkcpegdakzgugudpooiwoi"), false, 15607, 829, 1959);
}
