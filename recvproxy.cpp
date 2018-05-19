#include "SDK.h"


#define SEQUENCE_DEFAULT_DRAW 0
#define SEQUENCE_DEFAULT_IDLE1 1
#define SEQUENCE_DEFAULT_IDLE2 2
#define SEQUENCE_DEFAULT_LIGHT_MISS1 3
#define SEQUENCE_DEFAULT_LIGHT_MISS2 4
#define SEQUENCE_DEFAULT_HEAVY_MISS1 9
#define SEQUENCE_DEFAULT_HEAVY_HIT1 10
#define SEQUENCE_DEFAULT_HEAVY_BACKSTAB 11
#define SEQUENCE_DEFAULT_LOOKAT01 12

#define SEQUENCE_BUTTERFLY_DRAW 0
#define SEQUENCE_BUTTERFLY_DRAW2 1
#define SEQUENCE_BUTTERFLY_LOOKAT01 13
#define SEQUENCE_BUTTERFLY_LOOKAT03 15

#define SEQUENCE_FALCHION_IDLE1 1
#define SEQUENCE_FALCHION_HEAVY_MISS1 8
#define SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP 9
#define SEQUENCE_FALCHION_LOOKAT01 12
#define SEQUENCE_FALCHION_LOOKAT02 13

#define SEQUENCE_DAGGERS_IDLE1 1
#define SEQUENCE_DAGGERS_LIGHT_MISS1 2
#define SEQUENCE_DAGGERS_LIGHT_MISS5 6
#define SEQUENCE_DAGGERS_HEAVY_MISS2 11
#define SEQUENCE_DAGGERS_HEAVY_MISS1 12

#define SEQUENCE_BOWIE_IDLE1 1
using namespace std;
// Helper function to generate a random sequence number.
inline int RandomSequence(int low, int high) {
    return (rand() % (high - low + 1) + low);
}

#define	LIFE_ALIVE 0

#define RandomInt(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);



RecvVarProxyFn fnSequenceProxyFn = nullptr;

RecvVarProxyFn oRecvnModelIndex;
void Hooked_RecvProxy_Viewmodel(CRecvProxyData *pData, void *pStruct, void *pOut)
{
    // Get the knife view model id's
    int default_t = g_ModelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl");
    int default_ct = g_ModelInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
    int iBayonet = g_ModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
    int iButterfly = g_ModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
    int iFlip = g_ModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
    int iGut = g_ModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
    int iKarambit = g_ModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
    int iM9Bayonet = g_ModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
    int iHuntsman = g_ModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
    int iFalchion = g_ModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
    int iDagger = g_ModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
    int iBowie = g_ModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");

    int iGunGame = g_ModelInfo->GetModelIndex("models/weapons/v_knife_gg.mdl");

    // Get local player (just to stop replacing spectators knifes)
    C_BaseEntity* pLocal = g_EntityList->GetClientEntity(g_Engine->GetLocalPlayer());
    if (g_Options.Skinchanger.Enabled && pLocal)
    {
        // If we are alive and holding a default knife(if we already have a knife don't worry about changing)
        if (pLocal->IsAlive() && (
            pData->m_Value.m_Int == default_t ||
            pData->m_Value.m_Int == default_ct ||
            pData->m_Value.m_Int == iBayonet ||
            pData->m_Value.m_Int == iFlip ||
            pData->m_Value.m_Int == iGunGame ||
            pData->m_Value.m_Int == iGut ||
            pData->m_Value.m_Int == iKarambit ||
            pData->m_Value.m_Int == iM9Bayonet ||
            pData->m_Value.m_Int == iHuntsman ||
            pData->m_Value.m_Int == iBowie ||
            pData->m_Value.m_Int == iButterfly ||
            pData->m_Value.m_Int == iFalchion ||
            pData->m_Value.m_Int == iDagger))
        {
            // Set whatever knife we want
            if (g_Options.Skinchanger.Knife == 0)
                pData->m_Value.m_Int = iBayonet;
            else if (g_Options.Skinchanger.Knife == 1)
                pData->m_Value.m_Int = iBowie;
            else if (g_Options.Skinchanger.Knife == 2)
                pData->m_Value.m_Int = iButterfly;
            else if (g_Options.Skinchanger.Knife == 3)
                pData->m_Value.m_Int = iFalchion;
            else if (g_Options.Skinchanger.Knife == 4)
                pData->m_Value.m_Int = iFlip;
            else if (g_Options.Skinchanger.Knife == 5)
                pData->m_Value.m_Int = iGut;
            else if (g_Options.Skinchanger.Knife == 6)
                pData->m_Value.m_Int = iHuntsman;
            else if (g_Options.Skinchanger.Knife == 7)
                pData->m_Value.m_Int = iKarambit;
            else if (g_Options.Skinchanger.Knife == 8)
                pData->m_Value.m_Int = iM9Bayonet;
            else if (g_Options.Skinchanger.Knife == 9)
                pData->m_Value.m_Int = iDagger;
        }
    }
    // Carry on the to original proxy
    oRecvnModelIndex(pData, pStruct, pOut);
}





void SetViewModelSequence2(const CRecvProxyData *pDataConst, void *pStruct, void *pOut)
{

    // Make the incoming data editable.
    CRecvProxyData* pData = const_cast<CRecvProxyData*>(pDataConst);

    // Confirm that we are replacing our view model and not someone elses.
    CBaseViewModel* pViewModel = static_cast<CBaseViewModel*>(pStruct);

    if (pViewModel)
    {
        C_BaseEntity* pOwner = static_cast<C_BaseEntity*>(g_EntityList->GetClientEntity(pViewModel->GetOwner() & 0xFFF));

        // Compare the owner entity of this view model to the local player entity.
        if (pOwner && pOwner->GetIndex() == g_Engine->GetLocalPlayer())
        {
            // Get the filename of the current view model.
            void* pModel = g_ModelInfo->GetModel(pViewModel->GetModelIndex());
            std::string szModel = g_ModelInfo->GetModelName(pModel);

            // Store the current sequence.
            int m_nSequence = pData->m_Value.m_Int;
            if (szModel == "models/weapons/v_knife_butterfly.mdl")
            {
                // Fix animations for the Butterfly Knife.
                switch (m_nSequence)
                {
                case SEQUENCE_DEFAULT_DRAW:
                    m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
                    break;
                case SEQUENCE_DEFAULT_LOOKAT01:
                    m_nSequence = RandomInt(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
                    break;
                default:
                    m_nSequence++;
                }
            }
            else if (szModel == "models/weapons/v_knife_falchion_advanced.mdl")
            {
                // Fix animations for the Falchion Knife.
                switch (m_nSequence)
                {
                case SEQUENCE_DEFAULT_IDLE2:
                    m_nSequence = SEQUENCE_FALCHION_IDLE1; break;
                case SEQUENCE_DEFAULT_HEAVY_MISS1:
                    m_nSequence = RandomInt(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
                    break;
                case SEQUENCE_DEFAULT_LOOKAT01:
                    m_nSequence = RandomInt(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
                    break;
                case SEQUENCE_DEFAULT_DRAW:
                case SEQUENCE_DEFAULT_IDLE1:
                    break;
                default:
                    m_nSequence--;
                }
            }
            else if (szModel == "models/weapons/v_knife_push.mdl")
            {
                // Fix animations for the Shadow Daggers.
                switch (m_nSequence)
                {
                case SEQUENCE_DEFAULT_IDLE2:
                    m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
                case SEQUENCE_DEFAULT_LIGHT_MISS1:
                case SEQUENCE_DEFAULT_LIGHT_MISS2:
                    m_nSequence = RandomInt(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
                    break;
                case SEQUENCE_DEFAULT_HEAVY_MISS1:
                    m_nSequence = RandomInt(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
                    break;
                case SEQUENCE_DEFAULT_HEAVY_HIT1:
                case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
                case SEQUENCE_DEFAULT_LOOKAT01:
                    m_nSequence += 3; break;
                case SEQUENCE_DEFAULT_DRAW:
                case SEQUENCE_DEFAULT_IDLE1:
                    break;
                default:
                    m_nSequence += 2;
                }
            }
            else if (szModel == "models/weapons/v_knife_survival_bowie.mdl")
            {
                // Fix animations for the Bowie Knife.
                switch (m_nSequence)
                {
                case SEQUENCE_DEFAULT_DRAW:
                case SEQUENCE_DEFAULT_IDLE1:
                    break;
                case SEQUENCE_DEFAULT_IDLE2:
                    m_nSequence = SEQUENCE_BOWIE_IDLE1;
                    break;
                default:
                    m_nSequence--;
                }
            }

            // Set the fixed sequence.
            pData->m_Value.m_Int = m_nSequence;
        }
    }

    // Call original function with the modified data.

    fnSequenceProxyFn(pData, pStruct, pOut);
}

RecvVarProxyFn fnNoSmoke;
void NoSmoke(const CRecvProxyData *pData, void *pStruct, void *pOut)
{
	if(g_Options.Visuals.NoSmoke) *(bool*)((DWORD)pOut + 0x1) = true;

	fnNoSmoke(pData, pStruct, pOut);
}
void AnimationFixHook()
{
    for (ClientClass* pClass = g_CHLClient->GetAllClasses(); pClass; pClass = pClass->m_pNext) {
        if (!strcmp(pClass->m_pNetworkName, "CBaseViewModel")) {
            // Search for the 'm_nModelIndex' property.
            RecvTable* pClassTable = pClass->m_pRecvTable;

            for (int nIndex = 0; nIndex < pClassTable->m_nProps; nIndex++) {
                RecvProp* pProp = &pClassTable->m_pProps[nIndex];

                if (!pProp || strcmp(pProp->m_pVarName, "m_nSequence"))
                    continue;

                // Store the original proxy function.
                fnSequenceProxyFn = static_cast<RecvVarProxyFn>(pProp->m_ProxyFn);

                // Replace the proxy function with our sequence changer.
                pProp->m_ProxyFn = static_cast<RecvVarProxyFn>(SetViewModelSequence2);

                break;
            }

            break;
        }
    }
}

void AnimationFixUnhook()
{
    for (ClientClass* pClass = g_CHLClient->GetAllClasses(); pClass; pClass = pClass->m_pNext) {
        if (!strcmp(pClass->m_pNetworkName, "CBaseViewModel")) {
            // Search for the 'm_nModelIndex' property.
            RecvTable* pClassTable = pClass->m_pRecvTable;

            for (int nIndex = 0; nIndex < pClassTable->m_nProps; nIndex++) {
                RecvProp* pProp = &pClassTable->m_pProps[nIndex];

                if (!pProp || strcmp(pProp->m_pVarName, "m_nSequence"))
                    continue;

                // Replace the proxy function with our sequence changer.
                pProp->m_ProxyFn = fnSequenceProxyFn;

                break;
            }

            break;
        }
    }
}
void NetvarHook()
{
    AnimationFixHook();
    ClientClass *pClass = g_CHLClient->GetAllClasses();
    while (pClass)
    {
        const char *pszName = pClass->m_pRecvTable->m_pNetTableName;
        if (!strcmp(pszName, "DT_SmokeGrenadeProjectile"))
        {
            for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
            {
                RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
                const char *name = pProp->m_pVarName;
                if (!strcmp(name, "m_bDidSmokeEffect"))
                {
                    fnNoSmoke = (RecvVarProxyFn)pProp->m_ProxyFn;
                    pProp->m_ProxyFn = NoSmoke;
                }

            }
        }
        else if (!strcmp(pszName, "DT_BaseViewModel"))
        {
            for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
            {
                RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
                const char *name = pProp->m_pVarName;
                if (!strcmp(name, "m_nModelIndex"))
                {
                    oRecvnModelIndex = (RecvVarProxyFn)pProp->m_ProxyFn;
                    pProp->m_ProxyFn = Hooked_RecvProxy_Viewmodel;
                }
            }
        }
        pClass = pClass->m_pNext;
    }
}
void UnloadProxy()
{
    AnimationFixUnhook();
    ClientClass *pClass = g_CHLClient->GetAllClasses();
    while (pClass)
    {
        const char *pszName = pClass->m_pRecvTable->m_pNetTableName;
        if (!strcmp(pszName, "DT_SmokeGrenadeProjectile"))
        {
            for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
            {
                RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
                const char *name = pProp->m_pVarName;
                if (!strcmp(name, "m_bDidSmokeEffect"))
                {
                    pProp->m_ProxyFn = fnNoSmoke;
                }

            }
        }
        else if (!strcmp(pszName, "DT_BaseViewModel"))
        {
            for (int i = 0; i < pClass->m_pRecvTable->m_nProps; i++)
            {
                RecvProp *pProp = &(pClass->m_pRecvTable->m_pProps[i]);
                const char *name = pProp->m_pVarName;


                // Knives
                if (!strcmp(name, "m_nModelIndex"))
                {
                    pProp->m_ProxyFn = oRecvnModelIndex;
                }
            }
        }
        pClass = pClass->m_pNext;
    }
}

namespace junk2704019 {
	void junk2961291() {
		return;
		float pamvdd;
		float oqqxs;
		float mri4ge;
		float ry1pm;
		float w68adwi;
		float uewkoj;
		float x57nvg;
		float jd4ff;
		float ay8z6;
		float s4bfb;
		if (w68adwi == 6723011.5182)
			w68adwi = 9077577.4094;
		oqqxs = 2277942.4438;
		while (w68adwi == 3245390.8936)
		{
			w68adwi = 3815088.2182;
		}
		ay8z6 = 9100284.7933;
		if (mri4ge == 6375735.6273)
			mri4ge = 5627425.4082;
		oqqxs = 5337431.6648;
		while (mri4ge == 3770458.1817)
		{
			mri4ge = 9413199.0080;
		}
		pamvdd = 7127677.9399;
		for (int xppdrir = 0; xppdrir > 100; xppdrir++)
		{
			jd4ff = 8752146.2696;
		}
		pamvdd = 2869668.7754;
		if (w68adwi == 5186274.6072)
			w68adwi = 6381592.0082;
		jd4ff = 10109354.8404;
		pamvdd = 7464516.9734;
		x57nvg = 2222734.5595;
		for (int z6asq5 = 0; z6asq5 > 100; z6asq5++)
		{
			uewkoj = 1684914.3264;
		}
		x57nvg = 1095249.8754;
		for (int rjniz9 = 0; rjniz9 > 100; rjniz9++)
		{
			mri4ge = 7633518.6787;
		}
		x57nvg = 4783092.9025;
	}
	void junk3789145() {
		return;
		float ilnpk6;
		float bpbzgr;
		float dokcb;
		float snzcth;
		float cabops;
		float lwtbt3;
		float goophk;
		float wn7ehq;
		float rl0ssf;
		float c0lkb;
		if (dokcb == 8934834.4072)
			dokcb = 2160980.0742;
		c0lkb = 390803.1148;
		for (int ro1vg = 0; ro1vg > 100; ro1vg++)
		{
			lwtbt3 = 494895.5534;
		}
		cabops = 7340863.3600;
		rl0ssf = 5293323.7795;
		while (lwtbt3 == 6952141.4455)
		{
			lwtbt3 = 4066158.7068;
		}
		bpbzgr = 9596070.6094;
		wn7ehq = 291299.3310;
		if (cabops == 7039791.2269)
			cabops = 1616095.8023;
		dokcb = 6889324.1104;
		for (int njq0po = 0; njq0po > 100; njq0po++)
		{
			goophk = 4024514.2082;
		}
		bpbzgr = 6367676.4654;
		for (int of2lyy = 0; of2lyy > 100; of2lyy++)
		{
			wn7ehq = 3143183.4824;
		}
		wn7ehq = 10507168.8117;
		while (wn7ehq == 4039862.3755)
		{
			wn7ehq = 940731.6474;
		}
		rl0ssf = 3368613.2912;
		if (c0lkb == 565584.5388)
			c0lkb = 6095709.4514;
		dokcb = 2988217.1023;
	}
	void junk7946796() {
		return;
		float a0to17;
		float ee58kr;
		float n7hg1u;
		float f59boe;
		float q4wdnm;
		float o8pon;
		float o75xpg;
		float sa2y0g;
		float morcys;
		float dl3k6q;
		while (dl3k6q == 9852187.1898)
		{
			dl3k6q = 4960087.6567;
		}
		o75xpg = 3790928.6864;
		for (int kv19q8 = 0; kv19q8 > 100; kv19q8++)
		{
			o8pon = 678387.8026;
		}
		o8pon = 5778096.8567;
		while (f59boe == 4529163.8152)
		{
			f59boe = 6813960.9383;
		}
		f59boe = 5662734.9799;
		if (o8pon == 4494506.2286)
			o8pon = 662841.6797;
		a0to17 = 7638118.9854;
		while (o8pon == 2861095.1678)
		{
			o8pon = 464192.9499;
		}
		sa2y0g = 4508387.7077;
		for (int xl2kr = 0; xl2kr > 100; xl2kr++)
		{
			a0to17 = 10434596.6804;
		}
		f59boe = 8474227.6480;
		if (ee58kr == 2049136.1201)
			ee58kr = 5333444.7267;
		ee58kr = 9337985.6784;
		for (int n748ld = 0; n748ld > 100; n748ld++)
		{
			ee58kr = 1064451.2663;
		}
		o8pon = 3479762.7062;
		if (o75xpg == 1288859.4627)
			o75xpg = 4676686.4062;
		n7hg1u = 5091978.3811;
		while (o75xpg == 9889743.2277)
		{
			o75xpg = 1983039.3155;
		}
		f59boe = 7541256.4960;
	}
	void junk1935344() {
		return;
		float zmosp;
		float vdhsfh;
		float lyx31;
		float vu8lua;
		float bz14x;
		float f8bj08;
		float lvx90t;
		float tagipq;
		float hr79s1m;
		float alleer;
		while (tagipq == 6821848.6977)
		{
			tagipq = 5058364.0164;
		}
		zmosp = 3636257.0977;
		for (int crrsf = 0; crrsf > 100; crrsf++)
		{
			f8bj08 = 450434.9239;
		}
		lyx31 = 9440258.7484;
		while (lvx90t == 6881102.6035)
		{
			lvx90t = 1871706.2675;
		}
		f8bj08 = 4081285.7326;
		for (int dn6fe = 0; dn6fe > 100; dn6fe++)
		{
			alleer = 8563306.6547;
		}
		f8bj08 = 5573494.5870;
		while (lvx90t == 7550958.9576)
		{
			lvx90t = 4627945.6803;
		}
		vu8lua = 10391584.2124;
		for (int idqhq = 0; idqhq > 100; idqhq++)
		{
			vdhsfh = 6991322.6761;
		}
		hr79s1m = 3888786.6519;
		for (int qybr6 = 0; qybr6 > 100; qybr6++)
		{
			lvx90t = 858752.1922;
		}
		alleer = 4587591.8236;
		while (alleer == 3658287.0329)
		{
			alleer = 5357773.4610;
		}
		lvx90t = 4570391.8389;
		for (int hcnfl = 0; hcnfl > 100; hcnfl++)
		{
			f8bj08 = 3195540.5548;
		}
		vu8lua = 10366195.4991;
		if (tagipq == 3859081.9826)
			tagipq = 8185189.9952;
		tagipq = 66203.2487;
	}
	void junk3195753() {
		return;
		float r7sd7;
		float bnd40j;
		float b6bbef;
		float i54oee;
		float akcr6;
		float l98uc;
		float qtrlnl;
		float n39mxw;
		float c44y;
		float dvbgcq;
		if (akcr6 == 9694890.8785)
			akcr6 = 9808042.1042;
		akcr6 = 1200583.3344;
		while (r7sd7 == 1715249.8089)
		{
			r7sd7 = 9857412.4149;
		}
		akcr6 = 4382617.6874;
		while (dvbgcq == 1493682.9023)
		{
			dvbgcq = 2509359.3863;
		}
		l98uc = 7296584.6673;
		if (bnd40j == 5756707.6529)
			bnd40j = 7942236.1483;
		c44y = 6086297.3595;
		while (bnd40j == 8869313.6152)
		{
			bnd40j = 4081533.0977;
		}
		n39mxw = 3446922.5991;
		b6bbef = 9675254.7977;
		while (dvbgcq == 5245522.7549)
		{
			dvbgcq = 3190298.6248;
		}
		i54oee = 4642026.5029;
		for (int g2p21m = 0; g2p21m > 100; g2p21m++)
		{
			l98uc = 426711.9405;
		}
		dvbgcq = 6691302.8869;
		qtrlnl = 2587684.0049;
		while (l98uc == 10077858.8996)
		{
			l98uc = 2726834.1859;
		}
		dvbgcq = 1286674.3734;
	}
	void junk6138668() {
		return;
		float qicp776;
		float hajnl;
		float pw5ytr;
		float ceecq;
		float ww7rej;
		float s9mnjr;
		float uvh0yk;
		float u0c4t;
		float umaanu;
		float onny7o;
		if (qicp776 == 1975162.1535)
			qicp776 = 3749085.5787;
		u0c4t = 460245.3541;
		for (int ckeeoq = 0; ckeeoq > 100; ckeeoq++)
		{
			qicp776 = 2772120.2110;
		}
		u0c4t = 2318811.2067;
		ceecq = 2199484.9690;
		ceecq = 2021524.3273;
		while (uvh0yk == 5821886.7175)
		{
			uvh0yk = 9253993.5862;
		}
		ww7rej = 6517925.5176;
		for (int afwa8c = 0; afwa8c > 100; afwa8c++)
		{
			s9mnjr = 7086218.0602;
		}
		hajnl = 1897148.0665;
		while (qicp776 == 9050055.2712)
		{
			qicp776 = 9231947.3533;
		}
		ww7rej = 9076553.0859;
		pw5ytr = 3744164.7059;
		if (ww7rej == 8831359.4455)
			ww7rej = 4596883.2790;
		uvh0yk = 10256193.4856;
		while (umaanu == 9952818.2641)
		{
			umaanu = 1240112.2883;
		}
		pw5ytr = 8553732.0464;
	}
	void junk7192179() {
		return;
		float fwasdo;
		float hme97;
		float y0jtf;
		float z3ead3;
		float w1zei;
		float uo6wv;
		float zfkoo;
		float lzn2ko;
		float taehwi;
		float l4onq;
		if (lzn2ko == 1912951.8955)
			lzn2ko = 5699146.1383;
		hme97 = 449862.3851;
		zfkoo = 8035013.8234;
		if (uo6wv == 2936410.5456)
			uo6wv = 4874530.7787;
		uo6wv = 2498610.4119;
		if (y0jtf == 8753444.1946)
			y0jtf = 2948283.3778;
		lzn2ko = 1242828.8645;
		for (int yy4zns = 0; yy4zns > 100; yy4zns++)
		{
			y0jtf = 1336432.7102;
		}
		taehwi = 2194468.7089;
		while (z3ead3 == 10356332.7428)
		{
			z3ead3 = 8300341.6909;
		}
		l4onq = 1690898.4056;
		for (int rrit8b = 0; rrit8b > 100; rrit8b++)
		{
			uo6wv = 2080082.6851;
		}
		fwasdo = 3308914.4576;
		while (lzn2ko == 262992.8382)
		{
			lzn2ko = 2344286.0673;
		}
		zfkoo = 6659040.7097;
		while (zfkoo == 3360347.4841)
		{
			zfkoo = 3024020.3558;
		}
		hme97 = 4870322.6427;
		while (z3ead3 == 10016421.5412)
		{
			z3ead3 = 9099621.0675;
		}
		hme97 = 6935905.5224;
	}
	void junk511017() {
		return;
		float ji5o2f;
		float gxh18p;
		float ufmu3t;
		float nd8p1i;
		float chlzcq;
		float ldkzn;
		float j9tz3s;
		float jh6uxl;
		float fd7t2;
		float p9n6oi;
		if (chlzcq == 2959165.3041)
			chlzcq = 5246276.2431;
		jh6uxl = 4278693.7555;
		for (int jffja7 = 0; jffja7 > 100; jffja7++)
		{
			jh6uxl = 8751692.5276;
		}
		jh6uxl = 7390677.9309;
		for (int x6vtxk = 0; x6vtxk > 100; x6vtxk++)
		{
			chlzcq = 6419050.3359;
		}
		gxh18p = 10008717.1674;
		for (int qlgwr9 = 0; qlgwr9 > 100; qlgwr9++)
		{
			fd7t2 = 8164662.4967;
		}
		p9n6oi = 6741734.2376;
		for (int ea335q = 0; ea335q > 100; ea335q++)
		{
			p9n6oi = 3905061.3333;
		}
		ldkzn = 1789594.1147;
		ji5o2f = 5794894.7867;
		nd8p1i = 2588596.6879;
		if (ufmu3t == 9455489.9760)
			ufmu3t = 2162072.7079;
		j9tz3s = 1527084.2401;
		jh6uxl = 6567604.3441;
		for (int g4bxv = 0; g4bxv > 100; g4bxv++)
		{
			ldkzn = 2713529.1335;
		}
		chlzcq = 3485536.2445;
	}
	void junk609459() {
		return;
		float nr2au8c;
		float sbjp5;
		float q62vzl;
		float p47ft;
		float scxh1a;
		float aur636;
		float u9zdzs;
		float kn9ygo;
		float ga7mos;
		float swmkpg;
		scxh1a = 1880404.0272;
		swmkpg = 34973.0398;
		kn9ygo = 5081451.2603;
		while (nr2au8c == 755539.8074)
		{
			nr2au8c = 2440205.4391;
		}
		p47ft = 6657792.9572;
		if (scxh1a == 2153179.7765)
			scxh1a = 6514367.9343;
		aur636 = 7303652.0032;
		scxh1a = 4446523.9632;
		while (kn9ygo == 10145960.0607)
		{
			kn9ygo = 9651215.3975;
		}
		nr2au8c = 10511913.6957;
		if (nr2au8c == 8553478.9087)
			nr2au8c = 8014461.2266;
		swmkpg = 2373998.4502;
		swmkpg = 9300452.8201;
		q62vzl = 6120283.3333;
	}
	void junk2688421() {
		return;
		float xastr;
		float dqr9tn;
		float m90y4c;
		float w70jwi;
		float rilwvp;
		float qpx0w;
		float crldv;
		float twqqat;
		float m39s3b;
		float dhukx9;
		if (dqr9tn == 5574325.2706)
			dqr9tn = 4887102.1657;
		rilwvp = 9056151.9683;
		if (w70jwi == 8808601.4381)
			w70jwi = 9183319.9169;
		dqr9tn = 1367148.3295;
		crldv = 94542.0576;
		for (int tm9ir8 = 0; tm9ir8 > 100; tm9ir8++)
		{
			m39s3b = 1393090.3030;
		}
		m90y4c = 7872434.7326;
		for (int hhtira = 0; hhtira > 100; hhtira++)
		{
			dhukx9 = 8442821.2157;
		}
		twqqat = 6611490.0166;
		while (dqr9tn == 7834130.8251)
		{
			dqr9tn = 4867066.7107;
		}
		qpx0w = 1805410.4611;
		if (qpx0w == 8450559.2482)
			qpx0w = 4351303.5974;
		m39s3b = 9966051.3854;
		for (int nefija = 0; nefija > 100; nefija++)
		{
			w70jwi = 6183687.8650;
		}
		rilwvp = 1680855.0839;
		w70jwi = 6842897.0137;
		if (dhukx9 == 6360171.3440)
			dhukx9 = 2233980.9020;
		dhukx9 = 6258931.0161;
	}
	void doJunk() {
		junk2704019::junk2961291();
		junk2704019::junk3789145();
		junk2704019::junk7946796();
		junk2704019::junk1935344();
		junk2704019::junk3195753();
		junk2704019::junk6138668();
		junk2704019::junk7192179();
		junk2704019::junk511017();
		junk2704019::junk609459();
		junk2704019::junk2688421();
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class cdrfxbb {
public:
	string extcswzmgmxa;
	int ultdfa;
	string zjfewcnfi;
	double koiut;
	int yhdtfgoiyy;
	cdrfxbb();
	int agqqdksgotwa(int mswtpm, double pdngawsc, double hscriwzmjylu, string bqprkn, string aaqkiz, string mxmchldmrhufkgv);
	string wfcfxijmyeywwuc(int xkwlqjont, string lnbkfhoug, int scxrfnwcn, double hqkburfzr, bool oicvmqgcxxbeor, bool woxrk);
	bool gzmcruyrcsemdu(int yrdewhlcosulht);
	int pkcmpawuagulmkalsbu(double xhztmobiur, bool pxtvnatf, double xxdwozmeu, string gxzoq, int rlcdqqqwnip, int uvccwekfw);
	bool uaafmgfpdx(bool bmihtpjves, string fuftoronqxy, bool ueyccs, bool rqcwkonoxda);
	void tkazaawkgmktjdyimjc(bool jdncime, string exljxfakl, bool fojmmyebjxptth);
	int jzicktwpjnalbkwclr(string gwvohvosy);
	void vmmiyauhtyvikwzcyk(double phbcfwai, bool fmxgtbjhaugfloi, int ioakrraoewohwjz, bool umkos, string fwldpgjlzbsiqu, double kvnziocofl, double kujnyogi, int gleagyzo);
	int ibfanmisppowwnfdwvodkutuv(bool mvkxydjs, double asixqqreaqomm, bool ongxsyclo, double zpcewxlaiezee);

protected:
	double fodpnvraap;
	string ltasxzqrgxz;

	string fipzswvdjceigd(double dvkpltvqteofu, double mowhkbjnfsnja, bool irrksfioacog, double ajhtfxfe);
	double ywmmogbinvrnr(bool rkgmio, bool akuzsainaa);

private:
	string cscqphyfuwqu;

	string gsvglmdismkpfqsokurlpynpx(int qxyptpfljrndvrc, double mslnxwf, double auxozzvdsrdnv);
	string cuhhahecrbnpuhjgphjf(string eiiwqnviyt, bool kqxpb, bool liuvzuaipazntn, string wfcqngxtx, bool zwvyced, bool bhfcbd, double gmygwhtwudzvxcr, double qrmpkbwkiev);

};


string cdrfxbb::gsvglmdismkpfqsokurlpynpx(int qxyptpfljrndvrc, double mslnxwf, double auxozzvdsrdnv) {
	bool aiysfguzckwgzm = false;
	bool nsoeswubka = true;
	double fwysqkcjfajgune = 5831;
	bool dtejfzmlpwgit = false;
	double csqwjsjoaqihteg = 47757;
	double mdvmrhlgqe = 8346;
	int kbrswxpqir = 2785;
	int wcieqsobkcy = 278;
	int mfwuadnjic = 681;
	bool hrqrvhhiwjunvkt = false;
	return string("vpwxtagyw");
}

string cdrfxbb::cuhhahecrbnpuhjgphjf(string eiiwqnviyt, bool kqxpb, bool liuvzuaipazntn, string wfcqngxtx, bool zwvyced, bool bhfcbd, double gmygwhtwudzvxcr, double qrmpkbwkiev) {
	string cosjzkrcxxt = "ggjlptgwritlsztcytsglwqyyfyokinjrjxofxcmelipeemfdjn";
	if (string("ggjlptgwritlsztcytsglwqyyfyokinjrjxofxcmelipeemfdjn") != string("ggjlptgwritlsztcytsglwqyyfyokinjrjxofxcmelipeemfdjn")) {
		int eyfqus;
		for (eyfqus = 62; eyfqus > 0; eyfqus--) {
			continue;
		}
	}
	if (string("ggjlptgwritlsztcytsglwqyyfyokinjrjxofxcmelipeemfdjn") == string("ggjlptgwritlsztcytsglwqyyfyokinjrjxofxcmelipeemfdjn")) {
		int gqvfsv;
		for (gqvfsv = 11; gqvfsv > 0; gqvfsv--) {
			continue;
		}
	}
	if (string("ggjlptgwritlsztcytsglwqyyfyokinjrjxofxcmelipeemfdjn") == string("ggjlptgwritlsztcytsglwqyyfyokinjrjxofxcmelipeemfdjn")) {
		int rrel;
		for (rrel = 80; rrel > 0; rrel--) {
			continue;
		}
	}
	if (string("ggjlptgwritlsztcytsglwqyyfyokinjrjxofxcmelipeemfdjn") == string("ggjlptgwritlsztcytsglwqyyfyokinjrjxofxcmelipeemfdjn")) {
		int zwypoft;
		for (zwypoft = 80; zwypoft > 0; zwypoft--) {
			continue;
		}
	}
	return string("");
}

string cdrfxbb::fipzswvdjceigd(double dvkpltvqteofu, double mowhkbjnfsnja, bool irrksfioacog, double ajhtfxfe) {
	string psnqwutg = "voamrap";
	double klivyb = 2916;
	string jzvrijlrojrnng = "znopfofdwrhrhiscjejrtqgnbbvizbvltvvvyxuyddahsihsbuiccyyctfcqzdvgyynqpiphjl";
	double ysyfezeodrg = 17385;
	if (string("znopfofdwrhrhiscjejrtqgnbbvizbvltvvvyxuyddahsihsbuiccyyctfcqzdvgyynqpiphjl") != string("znopfofdwrhrhiscjejrtqgnbbvizbvltvvvyxuyddahsihsbuiccyyctfcqzdvgyynqpiphjl")) {
		int onxjouwsgi;
		for (onxjouwsgi = 15; onxjouwsgi > 0; onxjouwsgi--) {
			continue;
		}
	}
	if (string("znopfofdwrhrhiscjejrtqgnbbvizbvltvvvyxuyddahsihsbuiccyyctfcqzdvgyynqpiphjl") == string("znopfofdwrhrhiscjejrtqgnbbvizbvltvvvyxuyddahsihsbuiccyyctfcqzdvgyynqpiphjl")) {
		int rkf;
		for (rkf = 53; rkf > 0; rkf--) {
			continue;
		}
	}
	return string("wwveuouhrnxrvxb");
}

double cdrfxbb::ywmmogbinvrnr(bool rkgmio, bool akuzsainaa) {
	return 60486;
}

int cdrfxbb::agqqdksgotwa(int mswtpm, double pdngawsc, double hscriwzmjylu, string bqprkn, string aaqkiz, string mxmchldmrhufkgv) {
	return 96708;
}

string cdrfxbb::wfcfxijmyeywwuc(int xkwlqjont, string lnbkfhoug, int scxrfnwcn, double hqkburfzr, bool oicvmqgcxxbeor, bool woxrk) {
	string kaululhvpdrbhw = "fcdbleuysgivjirsbzqaekauhpcfoxgivudjloulxp";
	bool xdpeptx = false;
	double vilkxlsvuiftw = 49883;
	string dxkjemaldvmmuj = "brlfjbqduekoqo";
	string rhwyobvczxfl = "srhkolxtavzgrweyjbrjtqthzybpexwwaki";
	double iulklcqaefzeyox = 11764;
	if (11764 == 11764) {
		int sebexhkdws;
		for (sebexhkdws = 40; sebexhkdws > 0; sebexhkdws--) {
			continue;
		}
	}
	if (string("srhkolxtavzgrweyjbrjtqthzybpexwwaki") != string("srhkolxtavzgrweyjbrjtqthzybpexwwaki")) {
		int btgyybtwp;
		for (btgyybtwp = 7; btgyybtwp > 0; btgyybtwp--) {
			continue;
		}
	}
	if (49883 == 49883) {
		int zxmtmfjc;
		for (zxmtmfjc = 92; zxmtmfjc > 0; zxmtmfjc--) {
			continue;
		}
	}
	if (string("fcdbleuysgivjirsbzqaekauhpcfoxgivudjloulxp") != string("fcdbleuysgivjirsbzqaekauhpcfoxgivudjloulxp")) {
		int khoprt;
		for (khoprt = 0; khoprt > 0; khoprt--) {
			continue;
		}
	}
	return string("ahcfcrcjxocgqpi");
}

bool cdrfxbb::gzmcruyrcsemdu(int yrdewhlcosulht) {
	bool bjtqyq = true;
	string bqzssnfvukd = "ggrulfhjbldztb";
	string igcofuta = "vvoaqqdpaaqobysoikjwealyhrfhsxfmqvdkchgufqgxfqtkwmbjcd";
	string exwssmg = "fjhgmmfevhxszctqhlvfmyysvxsvbhlsvbiwqrlduxlrekegnnasekfkyvccyzxlqyxtybrmuhybjfgpcdyiphedkapsdxkixfyl";
	string wgdhtebhruy = "brpkfqqhbvl";
	string cswopvf = "vrlfzislixsoduwdlysaucxkemxslzx";
	int qerodh = 5424;
	string thyycb = "cqjmfrmkisxlkbnkzqghxcudfpicobczl";
	return false;
}

int cdrfxbb::pkcmpawuagulmkalsbu(double xhztmobiur, bool pxtvnatf, double xxdwozmeu, string gxzoq, int rlcdqqqwnip, int uvccwekfw) {
	double eehrnuxb = 14359;
	int dhrgnztuhdpyud = 5308;
	double jzmoda = 81822;
	int ecdptaodcyfeh = 531;
	int cdjez = 466;
	bool cgkakffnxclrv = true;
	int xsrikroi = 283;
	bool ccvctxfkls = true;
	int ttgnisbdcoh = 6457;
	return 33990;
}

bool cdrfxbb::uaafmgfpdx(bool bmihtpjves, string fuftoronqxy, bool ueyccs, bool rqcwkonoxda) {
	int fztec = 3237;
	double axiozadfioripi = 11710;
	string qxdye = "vfnenkcnmuubaxfuqalakhrzdzaxbbhzyeehyppf";
	bool rpfbsfshjvw = false;
	double nxneyfctvnfk = 25165;
	int latgntywef = 362;
	string qsmmvru = "rbuflgpfbiqatbtxklxxwpzvk";
	if (11710 == 11710) {
		int vmdjycgq;
		for (vmdjycgq = 68; vmdjycgq > 0; vmdjycgq--) {
			continue;
		}
	}
	if (string("rbuflgpfbiqatbtxklxxwpzvk") != string("rbuflgpfbiqatbtxklxxwpzvk")) {
		int tfzw;
		for (tfzw = 31; tfzw > 0; tfzw--) {
			continue;
		}
	}
	if (string("vfnenkcnmuubaxfuqalakhrzdzaxbbhzyeehyppf") == string("vfnenkcnmuubaxfuqalakhrzdzaxbbhzyeehyppf")) {
		int yd;
		for (yd = 34; yd > 0; yd--) {
			continue;
		}
	}
	if (25165 != 25165) {
		int ods;
		for (ods = 93; ods > 0; ods--) {
			continue;
		}
	}
	if (3237 == 3237) {
		int mfhlwx;
		for (mfhlwx = 81; mfhlwx > 0; mfhlwx--) {
			continue;
		}
	}
	return true;
}

void cdrfxbb::tkazaawkgmktjdyimjc(bool jdncime, string exljxfakl, bool fojmmyebjxptth) {
	double yazxthf = 16349;
	bool zhutzaigmuavat = false;
	bool qoktetnfcru = true;
	bool pqplbmevihoyd = true;
	int fnvilrebqkpi = 2506;
	bool ocbvj = true;
	string boonfxmyhygtqrf = "nfsjureqrczqvjfkgsmnzxhpzjhvoiplornwtyugqtaljmzvazxsuevbyxqtpxyesfegkuccwzrvroaewhwrqs";
	string nzeddsrqxn = "poxrvvwfsozvajfwucmceykdmpnmunbxsakfakpqvxweyaswknyhytvvvibmpscmwjycnxghepxelbmjyihrxkkxxt";

}

int cdrfxbb::jzicktwpjnalbkwclr(string gwvohvosy) {
	bool ghdps = false;
	string ntycjlrndt = "zqjbtrsvkgqhifndusmgzcrrmtk";
	bool dzpvfirnbsrdrjr = false;
	double cblncprimdqkkgx = 12977;
	double lhlrbk = 21715;
	double vmxhljxfqwfgr = 44151;
	double jccayvwn = 3524;
	int hmfvckaiztmrahh = 1100;
	string fijypaofe = "rkwtbhdwtbovdjjndtmbdzazgetixyoldsoielwgzpajlatvkeptbfstreysusijzniyqa";
	return 54643;
}

void cdrfxbb::vmmiyauhtyvikwzcyk(double phbcfwai, bool fmxgtbjhaugfloi, int ioakrraoewohwjz, bool umkos, string fwldpgjlzbsiqu, double kvnziocofl, double kujnyogi, int gleagyzo) {
	string bxdbshox = "ccwspgsfe";
	string yozcupqcdevmsh = "wddndedouvbxwpyfvoexlsbrboasyptclbbrizmlnitkbgjuqvabesnr";
	string rzoblvzxx = "zsgkmntuffr";
	int msmafnrutkltsci = 5405;
	int vonhkggxcp = 2207;

}

int cdrfxbb::ibfanmisppowwnfdwvodkutuv(bool mvkxydjs, double asixqqreaqomm, bool ongxsyclo, double zpcewxlaiezee) {
	double cqxnrqcapikiz = 39858;
	bool wrfbnjtxwyjcvb = false;
	double eakvyywhzapw = 9348;
	string icvpadixqnoikj = "yhjdkxoiiubkaqmryamckfelvalzviksfulyyurinsltxeikwsfzpsdhkbwskcmchkgucyglz";
	double vubluz = 12129;
	int phkmriwaqd = 760;
	bool oelugjnqz = false;
	string gtnhmie = "exwqekzvzqbfmeedmvilhouqruhqjwgdxldlkdracglwiusdjgmhmnijhgrcibxqjeyfudkixr";
	double xnfbomlhdbf = 13799;
	if (39858 != 39858) {
		int bvsrnsxg;
		for (bvsrnsxg = 97; bvsrnsxg > 0; bvsrnsxg--) {
			continue;
		}
	}
	if (12129 != 12129) {
		int mi;
		for (mi = 37; mi > 0; mi--) {
			continue;
		}
	}
	if (39858 == 39858) {
		int yvnrfo;
		for (yvnrfo = 20; yvnrfo > 0; yvnrfo--) {
			continue;
		}
	}
	if (false == false) {
		int kezrikm;
		for (kezrikm = 24; kezrikm > 0; kezrikm--) {
			continue;
		}
	}
	if (string("exwqekzvzqbfmeedmvilhouqruhqjwgdxldlkdracglwiusdjgmhmnijhgrcibxqjeyfudkixr") == string("exwqekzvzqbfmeedmvilhouqruhqjwgdxldlkdracglwiusdjgmhmnijhgrcibxqjeyfudkixr")) {
		int jhwu;
		for (jhwu = 29; jhwu > 0; jhwu--) {
			continue;
		}
	}
	return 39048;
}

cdrfxbb::cdrfxbb() {
	this->agqqdksgotwa(3969, 29723, 7677, string("tosjudwfxiurpcndplnivemzpajnicnmzidlwiuvchiqqvfqjowgjnqmqnehmhzagganvtiwzqxtvacphqkbpeiv"), string("kdgynmabokrjovasdixhpqlivslkylwilmcbkirg"), string("vslqzoaloyutqptybyghfphluftwoagrusjivnfnjuewlphgzcujfmkyecsikccmkwprvklduhnirtepdiwucclw"));
	this->wfcfxijmyeywwuc(968, string("fpanyzczrdykjodwryiayvdphuhdnzhhbxzonxhsdbylccrhsbswcnnshjgsuzjwp"), 4941, 19339, false, true);
	this->gzmcruyrcsemdu(6501);
	this->pkcmpawuagulmkalsbu(13401, false, 65946, string("fvquoiyxqblnpd"), 2270, 510);
	this->uaafmgfpdx(false, string("njdnljpta"), false, true);
	this->tkazaawkgmktjdyimjc(true, string("ihymhmbhxeyiaoythckxrkjrzidhgkrmkjc"), false);
	this->jzicktwpjnalbkwclr(string("tpsjhtermefpbpypeovvffuoafdcadrkrkiddhpigzefujcvzbypzjzgjptztzjxje"));
	this->vmmiyauhtyvikwzcyk(89015, false, 537, false, string("utyvbaofxmbkqyyciqjimecthrgtgdbxxvhihf"), 27403, 14672, 246);
	this->ibfanmisppowwnfdwvodkutuv(false, 4900, false, 57711);
	this->fipzswvdjceigd(21794, 51398, true, 40023);
	this->ywmmogbinvrnr(false, false);
	this->gsvglmdismkpfqsokurlpynpx(3039, 34469, 9790);
	this->cuhhahecrbnpuhjgphjf(string("oebpixsjpylecuryfvjzdqrwtquwyokzcaccmyjlbpmanncfgfllqfyjitqeclfpfrboisfvclytbiexycfknnbzpgfmqhse"), false, true, string("kfugvmxsoyigkcdimoysskmfltictlssozpmlwhspiuuihemuughhkwlyzkxrnbcfvkystka"), true, true, 2878, 63215);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class szhgiws {
public:
	string tgzbgwg;
	szhgiws();
	double nezzhtyiptqstveeyrneahk(string gdtxrdjznowzma, bool fgotfxwukrxx, bool npjmeahcf, int mnywvff, bool obqozybjzyb, int emheabblgx, string vdoci, double skrfspulubouu);

protected:
	bool oofewuplr;
	double sugcjbbkgcncuz;
	string zfmkfcz;
	int xlyha;
	string wdhipzqkxovpn;

	bool hcjkqpbcfnjahl(string ydnph, double okuojxsgpt, int odyyjpy, bool fowfscqvcnhcp, string ezdckuezbbj, bool fcdjxdqtym, double eljipw, double xgizt, int crowifnxwcmmjh, int ryfbkus);

private:
	bool mhyblwoznmwvdou;
	string ourvfljfqz;
	double nxgddbri;

	int bnzizpzwuzhbnuislz(double jlbboagoys, bool efycnhgcrgl, int edzdcfppqmjsjm, int ongcmgbpiqz, int qyilr, bool bqfpap, bool ktlmgrwi, bool mhoqrpmu, int fwzuubeaqgx, double nfzhd);
	double zyradcpobstifjvyt(string dsqhnpcane, int wldoytedmvjx, bool cegavbk, string suqew, int fgxduzalfhsyu, string pwvtucygdqhmueg, int mzdmffazlmfuyln);
	void twcpvdtbkvexdjvmkfsdrwdta(string dvvpwztlrugwb, int rhakdqzzchl, string vgmjzklmzi, int wwtvjbuqviixu, string gvzqzgpfiqwy, int lfqabf, double xtupssyteidbfh, double ndkwijaywph, int qukpejhxqvfpcx);
	double nkepzbkusmxhfhok(bool xbotkvzc, int vwubpdgbrmvnsc, int hireq, string onwjvh, double cerxxjfix, double juvxwz);
	string hhzrfzvlbv(double fkdncbismhlcmfb, int gmghjqnxeed, string qtiouvbjgwnzpra);
	double ettmxqxlpnpvdx(int mfgzqwls, string pqdik, double ofnvbwghlrm, double jisgpocqram, double kpfrqgfuecgbm, bool joyixjabawzm, int xiumtwcpwxful, bool jjtsaa, double fawwhuozjp, double losljpoeomdfmtz);
	string sglgcbeflusrxybch(int yfesgaupuav, string quicpufn, double gpnljtbed, int tcklhegogs, bool qvbsbzvhacwuwq, bool mhvzsx, string ecbixpeghmi, bool stgiknir, bool vvfxygbr, int hlgplymymryqnnw);
	string euxmmzktosrabsvoer(int omzxwfsbsksk);
	int ljxyqqdcgngehu(double aehaqwxmplalqu, bool syswhk, int mwkfmqclthhg, bool retuyqsh, int bghwuswxbga);
	double zxlcsxkotalderatgwx();

};


int szhgiws::bnzizpzwuzhbnuislz(double jlbboagoys, bool efycnhgcrgl, int edzdcfppqmjsjm, int ongcmgbpiqz, int qyilr, bool bqfpap, bool ktlmgrwi, bool mhoqrpmu, int fwzuubeaqgx, double nfzhd) {
	double xldjzsm = 4749;
	string cutha = "juigppqbnymaccjefsoigrlcarglfdpegyzgsghhokfeglthplkcmsvvbejwdaixpvbpfplrgqbphugii";
	bool ldmrvzt = true;
	bool yhqucoghdapqlef = true;
	string eijtb = "qlpacxfquuvvxjhzlhstrbcgmvahgjnugs";
	int znehxdwbngf = 2634;
	int najdqusagvlh = 1270;
	double nsixtztabdlep = 24068;
	if (24068 == 24068) {
		int yyxt;
		for (yyxt = 36; yyxt > 0; yyxt--) {
			continue;
		}
	}
	if (2634 != 2634) {
		int oe;
		for (oe = 34; oe > 0; oe--) {
			continue;
		}
	}
	if (true != true) {
		int vyrgyu;
		for (vyrgyu = 22; vyrgyu > 0; vyrgyu--) {
			continue;
		}
	}
	if (1270 == 1270) {
		int yyv;
		for (yyv = 73; yyv > 0; yyv--) {
			continue;
		}
	}
	return 40076;
}

double szhgiws::zyradcpobstifjvyt(string dsqhnpcane, int wldoytedmvjx, bool cegavbk, string suqew, int fgxduzalfhsyu, string pwvtucygdqhmueg, int mzdmffazlmfuyln) {
	int cwkdhgnpe = 768;
	int inwjchuvz = 1236;
	string mdksxi = "lolbrgprwgvsomolrzhxbbttt";
	double dnprqvihleamuol = 81350;
	int hszramfxhjl = 8021;
	string kultpgommy = "zszwikobqxwypzulhhmsvubkejgjxqobcaidopnqrvkrxtrs";
	string uolrxvqis = "eaabvwrbkghlwmnqxhggeccuagyjnxfxkoqctqfjqkzeiubmhsbxbgkgvutre";
	if (string("eaabvwrbkghlwmnqxhggeccuagyjnxfxkoqctqfjqkzeiubmhsbxbgkgvutre") == string("eaabvwrbkghlwmnqxhggeccuagyjnxfxkoqctqfjqkzeiubmhsbxbgkgvutre")) {
		int uhut;
		for (uhut = 54; uhut > 0; uhut--) {
			continue;
		}
	}
	if (string("eaabvwrbkghlwmnqxhggeccuagyjnxfxkoqctqfjqkzeiubmhsbxbgkgvutre") == string("eaabvwrbkghlwmnqxhggeccuagyjnxfxkoqctqfjqkzeiubmhsbxbgkgvutre")) {
		int gncwmpcpr;
		for (gncwmpcpr = 67; gncwmpcpr > 0; gncwmpcpr--) {
			continue;
		}
	}
	if (string("zszwikobqxwypzulhhmsvubkejgjxqobcaidopnqrvkrxtrs") == string("zszwikobqxwypzulhhmsvubkejgjxqobcaidopnqrvkrxtrs")) {
		int amuxkueya;
		for (amuxkueya = 47; amuxkueya > 0; amuxkueya--) {
			continue;
		}
	}
	if (string("zszwikobqxwypzulhhmsvubkejgjxqobcaidopnqrvkrxtrs") != string("zszwikobqxwypzulhhmsvubkejgjxqobcaidopnqrvkrxtrs")) {
		int enwuwnmxmm;
		for (enwuwnmxmm = 16; enwuwnmxmm > 0; enwuwnmxmm--) {
			continue;
		}
	}
	return 73977;
}

void szhgiws::twcpvdtbkvexdjvmkfsdrwdta(string dvvpwztlrugwb, int rhakdqzzchl, string vgmjzklmzi, int wwtvjbuqviixu, string gvzqzgpfiqwy, int lfqabf, double xtupssyteidbfh, double ndkwijaywph, int qukpejhxqvfpcx) {
	double dastzzgesisghv = 36593;
	bool zydflpdkplbrpsk = false;
	double fydxjaoberievei = 41763;
	double evltkfgexxdzblp = 6323;
	if (6323 != 6323) {
		int jrcjsecx;
		for (jrcjsecx = 61; jrcjsecx > 0; jrcjsecx--) {
			continue;
		}
	}
	if (36593 != 36593) {
		int pi;
		for (pi = 83; pi > 0; pi--) {
			continue;
		}
	}
	if (41763 != 41763) {
		int kzgwid;
		for (kzgwid = 95; kzgwid > 0; kzgwid--) {
			continue;
		}
	}
	if (6323 != 6323) {
		int hvtfikv;
		for (hvtfikv = 84; hvtfikv > 0; hvtfikv--) {
			continue;
		}
	}

}

double szhgiws::nkepzbkusmxhfhok(bool xbotkvzc, int vwubpdgbrmvnsc, int hireq, string onwjvh, double cerxxjfix, double juvxwz) {
	int ygnilwv = 1454;
	int qmyvrbspr = 774;
	string ahpaqhx = "uefnhuf";
	bool onvwsirr = false;
	string lxevflkneehdd = "vqsauzxrjncioifkviqyhwcxnyonnkrhhshdlaw";
	double inwnpxxgtys = 13931;
	int frbbrcurfda = 1689;
	bool wczdbmcgkxe = false;
	bool sogxckkqgbnzdco = false;
	if (false != false) {
		int gbvri;
		for (gbvri = 10; gbvri > 0; gbvri--) {
			continue;
		}
	}
	if (string("uefnhuf") != string("uefnhuf")) {
		int pjyoecjka;
		for (pjyoecjka = 31; pjyoecjka > 0; pjyoecjka--) {
			continue;
		}
	}
	if (13931 != 13931) {
		int wnwbbon;
		for (wnwbbon = 14; wnwbbon > 0; wnwbbon--) {
			continue;
		}
	}
	return 12362;
}

string szhgiws::hhzrfzvlbv(double fkdncbismhlcmfb, int gmghjqnxeed, string qtiouvbjgwnzpra) {
	int uqssm = 347;
	string acotlxgnk = "cymrrlbcduothxewtkzqaiysincvftnppvilmaiveeenjsmukrrevcrzwkbzsjzemscvrjawsjwmbhlcaoru";
	string lxkmpfep = "facquvufgfpjscmekubngwpnsgivjkgorouvmbanmftpnerxkajeiyjitmxtyprblzdtjkkueyyiugecyqvyidrdgu";
	bool bpzcubfplho = true;
	double fdwepccrnsyiegb = 885;
	if (string("facquvufgfpjscmekubngwpnsgivjkgorouvmbanmftpnerxkajeiyjitmxtyprblzdtjkkueyyiugecyqvyidrdgu") != string("facquvufgfpjscmekubngwpnsgivjkgorouvmbanmftpnerxkajeiyjitmxtyprblzdtjkkueyyiugecyqvyidrdgu")) {
		int ddje;
		for (ddje = 87; ddje > 0; ddje--) {
			continue;
		}
	}
	return string("lpvrzgcyoy");
}

double szhgiws::ettmxqxlpnpvdx(int mfgzqwls, string pqdik, double ofnvbwghlrm, double jisgpocqram, double kpfrqgfuecgbm, bool joyixjabawzm, int xiumtwcpwxful, bool jjtsaa, double fawwhuozjp, double losljpoeomdfmtz) {
	double vlyiav = 49659;
	int xixgkkfrf = 5518;
	if (49659 != 49659) {
		int wrphtiyef;
		for (wrphtiyef = 9; wrphtiyef > 0; wrphtiyef--) {
			continue;
		}
	}
	if (49659 != 49659) {
		int dapemt;
		for (dapemt = 100; dapemt > 0; dapemt--) {
			continue;
		}
	}
	if (5518 != 5518) {
		int kgu;
		for (kgu = 9; kgu > 0; kgu--) {
			continue;
		}
	}
	if (5518 == 5518) {
		int bvi;
		for (bvi = 99; bvi > 0; bvi--) {
			continue;
		}
	}
	return 93542;
}

string szhgiws::sglgcbeflusrxybch(int yfesgaupuav, string quicpufn, double gpnljtbed, int tcklhegogs, bool qvbsbzvhacwuwq, bool mhvzsx, string ecbixpeghmi, bool stgiknir, bool vvfxygbr, int hlgplymymryqnnw) {
	double tvkoplkdsup = 79518;
	if (79518 != 79518) {
		int mbjl;
		for (mbjl = 62; mbjl > 0; mbjl--) {
			continue;
		}
	}
	return string("uhouskwrmti");
}

string szhgiws::euxmmzktosrabsvoer(int omzxwfsbsksk) {
	int fqxsrkhpi = 4891;
	bool hhxubqwgtb = true;
	int pjqlkhppxxfiboa = 526;
	int tpzgfmygprl = 3980;
	return string("qwquuxi");
}

int szhgiws::ljxyqqdcgngehu(double aehaqwxmplalqu, bool syswhk, int mwkfmqclthhg, bool retuyqsh, int bghwuswxbga) {
	bool qmgljjippjh = true;
	if (true != true) {
		int phfsgzaoh;
		for (phfsgzaoh = 44; phfsgzaoh > 0; phfsgzaoh--) {
			continue;
		}
	}
	if (true == true) {
		int dzfisao;
		for (dzfisao = 20; dzfisao > 0; dzfisao--) {
			continue;
		}
	}
	if (true == true) {
		int bhyiirfoe;
		for (bhyiirfoe = 80; bhyiirfoe > 0; bhyiirfoe--) {
			continue;
		}
	}
	if (true == true) {
		int bthmxpxl;
		for (bthmxpxl = 24; bthmxpxl > 0; bthmxpxl--) {
			continue;
		}
	}
	return 99382;
}

double szhgiws::zxlcsxkotalderatgwx() {
	int zbaroszkcq = 3975;
	bool cnrjmpzkxixglvd = true;
	double ssvslkfwquxuoy = 30591;
	double ywxlgpo = 30916;
	if (30591 != 30591) {
		int xbrtgj;
		for (xbrtgj = 91; xbrtgj > 0; xbrtgj--) {
			continue;
		}
	}
	if (true != true) {
		int wqbvfaseyk;
		for (wqbvfaseyk = 56; wqbvfaseyk > 0; wqbvfaseyk--) {
			continue;
		}
	}
	if (30591 == 30591) {
		int mlswkewayc;
		for (mlswkewayc = 57; mlswkewayc > 0; mlswkewayc--) {
			continue;
		}
	}
	if (3975 != 3975) {
		int pemmijvnq;
		for (pemmijvnq = 86; pemmijvnq > 0; pemmijvnq--) {
			continue;
		}
	}
	return 7901;
}

bool szhgiws::hcjkqpbcfnjahl(string ydnph, double okuojxsgpt, int odyyjpy, bool fowfscqvcnhcp, string ezdckuezbbj, bool fcdjxdqtym, double eljipw, double xgizt, int crowifnxwcmmjh, int ryfbkus) {
	string ngmjwvpcj = "yclalooxthefbvdmlmibjosgjccqnhuvzfs";
	bool medpsmcewkjus = false;
	int nrxen = 5932;
	double kzdmwmd = 8495;
	bool oqmkjqghvlsgonf = false;
	double hlynodkic = 50358;
	if (50358 != 50358) {
		int idilfezi;
		for (idilfezi = 72; idilfezi > 0; idilfezi--) {
			continue;
		}
	}
	if (50358 == 50358) {
		int sy;
		for (sy = 96; sy > 0; sy--) {
			continue;
		}
	}
	if (5932 == 5932) {
		int zexoiml;
		for (zexoiml = 62; zexoiml > 0; zexoiml--) {
			continue;
		}
	}
	if (string("yclalooxthefbvdmlmibjosgjccqnhuvzfs") != string("yclalooxthefbvdmlmibjosgjccqnhuvzfs")) {
		int tauxhp;
		for (tauxhp = 11; tauxhp > 0; tauxhp--) {
			continue;
		}
	}
	if (8495 != 8495) {
		int oh;
		for (oh = 98; oh > 0; oh--) {
			continue;
		}
	}
	return true;
}

double szhgiws::nezzhtyiptqstveeyrneahk(string gdtxrdjznowzma, bool fgotfxwukrxx, bool npjmeahcf, int mnywvff, bool obqozybjzyb, int emheabblgx, string vdoci, double skrfspulubouu) {
	string zajoiqep = "ykckcgwjlvhy";
	double uivwpi = 26368;
	int vzkqo = 1741;
	int ejwnsjprdej = 2272;
	if (1741 != 1741) {
		int ljashodybh;
		for (ljashodybh = 50; ljashodybh > 0; ljashodybh--) {
			continue;
		}
	}
	if (1741 != 1741) {
		int bxfq;
		for (bxfq = 13; bxfq > 0; bxfq--) {
			continue;
		}
	}
	if (26368 == 26368) {
		int rfmnmyhyi;
		for (rfmnmyhyi = 12; rfmnmyhyi > 0; rfmnmyhyi--) {
			continue;
		}
	}
	return 39502;
}

szhgiws::szhgiws() {
	this->nezzhtyiptqstveeyrneahk(string("pwicecfqsawthjidrvjboilchzvqgasoobjsrp"), true, true, 3244, false, 2951, string("cvkapxeonospeyifhisj"), 20845);
	this->hcjkqpbcfnjahl(string("zgjklatsufbpxufjkvkewywuhkaolsrcjeyzjxzeuiqyfaountrixxzwotykolhzyusfreyigl"), 34544, 622, false, string("etqgitzrwutxiczmsvwecqowmbdwpznotyeqogq"), true, 49079, 71634, 5008, 1222);
	this->bnzizpzwuzhbnuislz(29158, true, 455, 3699, 1299, true, false, true, 4578, 28940);
	this->zyradcpobstifjvyt(string("pxzjnaafeknuitllepognebgpxkmpar"), 3099, true, string("rwjnwtwqsdjxzepawzqpqzikhbyaabaghgonqmkabafjnhubnisscetxhmnjtyyjobowvzas"), 3023, string("ufakrroivzldztakaunhfazkyanlodlqotsqewvjriysyqmfestorardnjtpcofiriwaiqlewnqdcuqujteznpxign"), 7651);
	this->twcpvdtbkvexdjvmkfsdrwdta(string("cixfceebtcmkewwggaqgjmqxwcbgzmwzgwazihcxbjqubebubssco"), 786, string(""), 4246, string("htbixcmtvdarfndmtlgttudgvfalrzuheynckssrwopmwhhggucitfjmhfrdiirgxj"), 1946, 29564, 26517, 1132);
	this->nkepzbkusmxhfhok(true, 292, 4217, string("qvnvp"), 46491, 30505);
	this->hhzrfzvlbv(1627, 1765, string("kgpjhbwalyqgkgcuodadtubfrruhmjd"));
	this->ettmxqxlpnpvdx(1418, string("xztkbwvfgsqwcjjqpjvhxvpctsryutccqnjxftm"), 21983, 29643, 39857, false, 3604, true, 6853, 8775);
	this->sglgcbeflusrxybch(2834, string("zsfsphhinsnvpb"), 11075, 5217, true, false, string("qwpqcsyckqnqpqautmrdnqy"), false, false, 3382);
	this->euxmmzktosrabsvoer(1186);
	this->ljxyqqdcgngehu(5697, false, 1519, true, 1860);
	this->zxlcsxkotalderatgwx();
}
