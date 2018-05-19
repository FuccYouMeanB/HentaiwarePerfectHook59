#pragma once


// Includes
#include "Utilities.h"
#include <fstream>
#include <PsapI.h>

bool FileLog = false;
std::ofstream logFile;

// --------         U Core           ------------ //
// Opens a debug console
void  U::OpenConsole(std::string Title)
{
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);

	SetConsoleTitle(Title.c_str());
}

// Closes the debug console
void  U::CloseConsole()
{
	FreeConsole();
}

// Outputs text to the console
void  U::Log(const char *fmt, ...)
{
	if (!fmt) return; //if the passed string is null return
	if (strlen(fmt) < 2) return;

	//Set up va_list and buffer to hold the params 
	va_list va_alist;
	char logBuf[256] = { 0 };

	//Do sprintf with the parameters
	va_start(va_alist, fmt);
	_vsnprintf(logBuf + strlen(logBuf), sizeof(logBuf) - strlen(logBuf), fmt, va_alist);
	va_end(va_alist);

	//Output to console
	if (logBuf[0] != '\0')
	{
		SetConsoleColor(FOREGROUND_INTENSE_RED);
		printf("[%s]", GetTimeString().c_str());
		SetConsoleColor(FOREGROUND_WHITE);
		printf(": %s\n", logBuf);
	}

	if (FileLog)
	{
		logFile << logBuf << std::endl;
	}
}

// Gets the current time as a string
std::string  U::GetTimeString()
{
	//Time related variables
	time_t current_time;
	struct tm *time_info;
	static char timeString[10];

	//Get current time
	time(&current_time);
	time_info = localtime(&current_time);

	//Get current time as string
	strftime(timeString, sizeof(timeString), "%I:%M%p", time_info);
	return timeString;
}

// Sets the console color for upcoming text
void  U::SetConsoleColor(WORD color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Enables writing all log calls to a file
void  U::EnableLogFile(std::string filename)
{
	logFile.open(filename.c_str());
	if (logFile.is_open())
		FileLog = true;
}


// --------         U Memory           ------------ //

DWORD U::WaitOnModuleHandle(std::string moduleName)
{
	DWORD ModuleHandle = NULL;
	while (!ModuleHandle)
	{
		ModuleHandle = (DWORD)GetModuleHandle(moduleName.c_str());
		if (!ModuleHandle)
			Sleep(50);
	}
	return ModuleHandle;
}

bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask)
{
	for (; *szMask; ++szMask, ++Mask, ++Data)
	{
		if (*szMask == 'x' && *Mask != *Data)
		{
			return false;
		}
	}
	return (*szMask) == 0;
}


DWORD U::FindPattern(std::string moduleName, BYTE* Mask, char* szMask)
{
	DWORD Address = WaitOnModuleHandle(moduleName.c_str());
	MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
	DWORD Length = ModInfo.SizeOfImage;
	for (DWORD c = 0; c < Length; c += 1)
	{
		if (bCompare((BYTE*)(Address + c), Mask, szMask))
		{
			return DWORD(Address + c);
		}
	}
	return 0;
}

DWORD U::FindTextPattern(std::string moduleName, char* string)
{
	DWORD Address = WaitOnModuleHandle(moduleName.c_str());
	MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
	DWORD Length = ModInfo.SizeOfImage;

	int len = strlen(string);
	char* szMask = new char[len + 1];
	for (int i = 0; i < len; i++)
	{
		szMask[i] = 'x';
	}
	szMask[len] = '\0';

	for (DWORD c = 0; c < Length; c += 1)
	{
		if (bCompare((BYTE*)(Address + c), (BYTE*)string, szMask))
		{
			return (DWORD)(Address + c);
		}
	}
	return 0;
}

#pragma warning( disable : 4018 )  
#pragma warning( disable : 4348 )  

bool U::bin_match(uint8_t *code, std::vector< uint8_t > &pattern)
{
	for (int j = 0; j < pattern.size(); j++)
	{
		if (!pattern[j] && code[j] != pattern[j])
		{
			return false;
		}
	}

	return true;
}

template< typename T = uintptr_t > static T U::first_match(uintptr_t start, std::string sig, size_t len)
{
	std::istringstream iss(sig);
	std::vector< std::string > tokens{ std::istream_iterator< std::string >{ iss }, std::istream_iterator< std::string >{} };
	std::vector< uint8_t > sig_bytes;

	for (auto hex_byte : tokens)
	{
		sig_bytes.push_back(std::strtoul(hex_byte.c_str(), nullptr, 16));
	}

	if (sig_bytes.empty() || sig_bytes.size() < 2)
	{
		return T{};
	}

	for (size_t i{}; i < len; i++)
	{
		uint8_t *code_ptr = reinterpret_cast< uint8_t * >(start + i);

		if (code_ptr[0] != sig_bytes.at(0))
		{
			continue;
		}

		if (bin_match(code_ptr, sig_bytes))
		{
			return((T)(start + i));
		}
	}

	return T{};
}

template< typename T = uintptr_t > static T U::first_code_match(HMODULE start, std::string sig)
{
	auto ntoskrnl = reinterpret_cast< PIMAGE_DOS_HEADER >(start);

	if (ntoskrnl->e_magic != 0x5a4d)
	{
		return T{};
	}

	auto nt_hdrs = reinterpret_cast< PIMAGE_NT_HEADERS >(reinterpret_cast< uintptr_t >(ntoskrnl) + ntoskrnl->e_lfanew);

	return first_match< T >(reinterpret_cast< uintptr_t >(ntoskrnl) + nt_hdrs->OptionalHeader.BaseOfCode, sig, nt_hdrs->OptionalHeader.SizeOfCode);
}

std::uint8_t* U::pattern_scan(void* module, const char* signature)
{
    static auto pattern_to_byte = [](const char* pattern) {
        auto bytes = std::vector<int>{};
        auto start = const_cast<char*>(pattern);
        auto end = const_cast<char*>(pattern) + strlen(pattern);

        for (auto current = start; current < end; ++current) {
            if (*current == '?') {
                ++current;
                if (*current == '?')
                    ++current;
                bytes.push_back(-1);
            }
            else {
                bytes.push_back(strtoul(current, &current, 16));
            }
        }
        return bytes;
    };

    auto dosHeader = (PIMAGE_DOS_HEADER)module;
    auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

    auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
    auto patternBytes = pattern_to_byte(signature);
    auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

    auto s = patternBytes.size();
    auto d = patternBytes.data();

    for (auto i = 0ul; i < sizeOfImage - s; ++i) {
        bool found = true;
        for (auto j = 0ul; j < s; ++j) {
            if (scanBytes[i + j] != d[j] && d[j] != -1) {
                found = false;
                break;
            }
        }
        if (found) {
            return &scanBytes[i];
        }
    }
    return nullptr;
}



vfunc_hook::vfunc_hook()
    : class_base(nullptr), vftbl_len(0), new_vftbl(nullptr), old_vftbl(nullptr)
{
}
vfunc_hook::vfunc_hook(void* base)
    : class_base(base), vftbl_len(0), new_vftbl(nullptr), old_vftbl(nullptr)
{
}
vfunc_hook::~vfunc_hook()
{
    unhook_all();

    delete[] new_vftbl;
}


bool vfunc_hook::setup(void* base /*= nullptr*/)
{
    if (base != nullptr)
        class_base = base;

    if (class_base == nullptr)
        return false;

    old_vftbl = *(std::uintptr_t**)class_base;
    vftbl_len = estimate_vftbl_length(old_vftbl);

    if (vftbl_len == 0)
        return false;

    new_vftbl = new std::uintptr_t[vftbl_len + 1]();

    std::memcpy(&new_vftbl[1], old_vftbl, vftbl_len * sizeof(std::uintptr_t));


    try {
        auto guard = detail::protect_guard{ class_base, sizeof(std::uintptr_t), PAGE_READWRITE };
        new_vftbl[0] = old_vftbl[-1];
        *(std::uintptr_t**)class_base = &new_vftbl[1];
    }
    catch (...) {
        delete[] new_vftbl;
        return false;
    }

    return true;
}
std::size_t vfunc_hook::estimate_vftbl_length(std::uintptr_t* vftbl_start)
{
    auto len = std::size_t{};

    while (vftbl_start[len] >= 0x00010000 &&
        vftbl_start[len] <  0xFFF00000 &&
        len < 512 /* Hard coded value. Can cause problems, beware.*/) {
        len++;
    }

    return len;
}



#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class rvepxsy {
public:
	double mnyyues;
	string wvmazfeyzlwijli;
	string qifmmvk;
	double whenhbwfbtvywy;
	string rkztvhhrjzzp;
	rvepxsy();
	bool kjjilyjjqpb(string bfazksgncflcc, double wruyjvwnald, bool kcadl, bool hyxtlfbaf, int mcrsova, string ambtpd);
	double osxggqmshmjpthnzcht(string jtdaxp, double mwqfsavsxtxdcnh, int jqzrqelrpxv, double ehutgrhtcajcih, int tpoxpzh, int qokgi, string bumsumbk, double lssjcieadkakwd);
	string gsqtbjinavdp(double hluewrgwld, string hjpnsniqsj, string jlfulr);
	double ordyuslgxzpyuehxto(int zducidgigokomsv, int ebxdbwikidrt, int kdjhkx);

protected:
	int ypmtgzjlipapmnm;
	double xrahv;
	double psenje;

	void enbvczkcvtyzpprkj(int jcolssgqzom, double gaxmuavmicl, int yuabotr, string anynfzvugtw, double cfigt, int jfdtvvv, double hubikx, string xynhjs, double wkzgftqnl, string khpuzmkynrrnek);
	int qrsahkngklijzijmpwibputik(double qlfbistiffx, double dmmliqmivo, double ebulvccmejcuol, int vgzxrzof);
	void mfyjloquuwfrlbktn();
	string tsjyeciqjfjhewf(bool bodjhnu, string rdzwcwjtwo, double hsejhevsgt, int dcrnwqhmsivkhy, double xjeape, bool szcclyauwwmc, int muizfdrpuokmpl, int dwomoomw, int dzvpixkqabd, bool vqnektv);
	double bnjtfupkflgkhlqdmuxjzy(bool pyimwugs, bool ojzjshvzjjn, string maxlane, string fwekt);
	string mldstmtgqxgoibxdnmupzrv(string oemugsrlxn, bool kvhhomz);
	int elxoadulkyoqsjrj(string sctvvth);
	bool lvvexggduduhuz(int fyhfhpqwzfrqsf, bool tkqdg, double bfniwulcf, bool iwclwv, int bwrcqjyuoxge, int cxaua, string pysghmeglczt);

private:
	double ewozwaokdcamjxe;
	bool qrogatm;
	bool ozkgtqtajpefk;

	double uhwqdepylo(bool gjekemtmmt, int bxrctaxfay, int rvthyrdrhinz, bool zoxlbv, bool svlyj, string pqzmzxqlfkcbk, int azbxhdne, double qjjosasplprs, double kbjqrch);
	int sofjdgjapfnaozeaqqc(int hhjlpgbhvvwqzvc, int pgmrszfmjll, int qguobvefh);
	bool xzmxgnrfamwtbmxtcy(bool eufrobq, int bmmqrtknghmv, string fagsaaremt);

};


double rvepxsy::uhwqdepylo(bool gjekemtmmt, int bxrctaxfay, int rvthyrdrhinz, bool zoxlbv, bool svlyj, string pqzmzxqlfkcbk, int azbxhdne, double qjjosasplprs, double kbjqrch) {
	string mkvaseek = "lmbfoiwnjtyrvauqumpuw";
	double uibezbfqzkda = 3157;
	int gwqbgdmroxltsj = 958;
	double pfrsnl = 4551;
	string nagyu = "gweiyfxufkbqehzudrlxdbhwqiewobfiflkybhvkigmhfjdjilbkslmsi";
	if (958 == 958) {
		int mblbomttc;
		for (mblbomttc = 54; mblbomttc > 0; mblbomttc--) {
			continue;
		}
	}
	return 73075;
}

int rvepxsy::sofjdgjapfnaozeaqqc(int hhjlpgbhvvwqzvc, int pgmrszfmjll, int qguobvefh) {
	int yndvvbyhrv = 3738;
	int qkkxnno = 3821;
	if (3738 == 3738) {
		int fzwahnla;
		for (fzwahnla = 23; fzwahnla > 0; fzwahnla--) {
			continue;
		}
	}
	return 28635;
}

bool rvepxsy::xzmxgnrfamwtbmxtcy(bool eufrobq, int bmmqrtknghmv, string fagsaaremt) {
	bool tantxsjype = false;
	int rlnluhxwyuc = 3631;
	bool qfirm = false;
	bool kgqzecfudlsudxk = false;
	string mbynhv = "hfobdjknkdzyfsmjqjw";
	string pyesjvk = "oqcfvybehczntdalhnlrfqlbx";
	double mtykl = 28204;
	bool chdlvtzkbxplt = true;
	if (false != false) {
		int pwbbmmfsos;
		for (pwbbmmfsos = 31; pwbbmmfsos > 0; pwbbmmfsos--) {
			continue;
		}
	}
	if (string("hfobdjknkdzyfsmjqjw") == string("hfobdjknkdzyfsmjqjw")) {
		int tgilsytb;
		for (tgilsytb = 90; tgilsytb > 0; tgilsytb--) {
			continue;
		}
	}
	return true;
}

void rvepxsy::enbvczkcvtyzpprkj(int jcolssgqzom, double gaxmuavmicl, int yuabotr, string anynfzvugtw, double cfigt, int jfdtvvv, double hubikx, string xynhjs, double wkzgftqnl, string khpuzmkynrrnek) {
	double dnihjv = 4840;
	int klkcsby = 2296;
	bool glllclasdwjg = false;
	int ngryjaskav = 5325;
	double xcvwhkjijtrrh = 21285;
	int ejsach = 1139;
	bool lpaunnfuuv = true;
	string brgdlwmqgap = "sjwxyldnrlgulyazxtihtktqsudmlpwqjvtoegofhakqltagxwkiswbjpjddoxaqbnncwvyyraassyi";

}

int rvepxsy::qrsahkngklijzijmpwibputik(double qlfbistiffx, double dmmliqmivo, double ebulvccmejcuol, int vgzxrzof) {
	double tobkit = 7299;
	int oaftce = 1542;
	double dyylbuu = 24888;
	string ppvwjbcjj = "bqaqxchpxlnynakrdsudrjhirqkcbubdyzjbsbjsrmexjntsrsiskntwzmidnvaqqyptpmzjcmezabqlcotgggautdwso";
	string hxtxmojaq = "acllfmzszuiuocpixgwgfmrbwekjnzskvpfneeufmkkiqohtxnpzdhbfyqvqecpfijcskbyydbpsmnhaehoffm";
	double nddauubroh = 3277;
	string vvmss = "fhbtswtsvyhssfp";
	double hlcyeipiwbvgk = 2798;
	return 62357;
}

void rvepxsy::mfyjloquuwfrlbktn() {
	bool cjvxgjfuf = true;
	bool wpncxynelvhxrtq = true;
	int pryunlocr = 7266;
	double cqnysodlclbgidg = 69095;
	bool tobarpq = true;
	int xtbrg = 3884;
	if (7266 != 7266) {
		int qdiou;
		for (qdiou = 2; qdiou > 0; qdiou--) {
			continue;
		}
	}
	if (7266 == 7266) {
		int zlt;
		for (zlt = 26; zlt > 0; zlt--) {
			continue;
		}
	}
	if (true != true) {
		int ihecmkfgyo;
		for (ihecmkfgyo = 74; ihecmkfgyo > 0; ihecmkfgyo--) {
			continue;
		}
	}
	if (69095 == 69095) {
		int pzks;
		for (pzks = 26; pzks > 0; pzks--) {
			continue;
		}
	}
	if (7266 == 7266) {
		int fx;
		for (fx = 36; fx > 0; fx--) {
			continue;
		}
	}

}

string rvepxsy::tsjyeciqjfjhewf(bool bodjhnu, string rdzwcwjtwo, double hsejhevsgt, int dcrnwqhmsivkhy, double xjeape, bool szcclyauwwmc, int muizfdrpuokmpl, int dwomoomw, int dzvpixkqabd, bool vqnektv) {
	double uyrazexpz = 29795;
	int epigjjtjltfmwvq = 1001;
	int zmspenhk = 3412;
	if (29795 == 29795) {
		int ngpnei;
		for (ngpnei = 29; ngpnei > 0; ngpnei--) {
			continue;
		}
	}
	if (3412 != 3412) {
		int zlpavxnis;
		for (zlpavxnis = 16; zlpavxnis > 0; zlpavxnis--) {
			continue;
		}
	}
	return string("scmznli");
}

double rvepxsy::bnjtfupkflgkhlqdmuxjzy(bool pyimwugs, bool ojzjshvzjjn, string maxlane, string fwekt) {
	double crknipbtfgm = 33421;
	if (33421 != 33421) {
		int ezh;
		for (ezh = 69; ezh > 0; ezh--) {
			continue;
		}
	}
	if (33421 != 33421) {
		int smmqblkvfh;
		for (smmqblkvfh = 50; smmqblkvfh > 0; smmqblkvfh--) {
			continue;
		}
	}
	if (33421 != 33421) {
		int evovy;
		for (evovy = 59; evovy > 0; evovy--) {
			continue;
		}
	}
	if (33421 != 33421) {
		int fvwfhjc;
		for (fvwfhjc = 22; fvwfhjc > 0; fvwfhjc--) {
			continue;
		}
	}
	if (33421 != 33421) {
		int yjnmhbj;
		for (yjnmhbj = 62; yjnmhbj > 0; yjnmhbj--) {
			continue;
		}
	}
	return 96628;
}

string rvepxsy::mldstmtgqxgoibxdnmupzrv(string oemugsrlxn, bool kvhhomz) {
	double vlzbksyr = 15880;
	double ffvicpupeyrkpb = 23999;
	double ceprtacobwzbzo = 11662;
	double jzcgysl = 70117;
	if (11662 != 11662) {
		int qb;
		for (qb = 66; qb > 0; qb--) {
			continue;
		}
	}
	if (15880 == 15880) {
		int pimnkcm;
		for (pimnkcm = 27; pimnkcm > 0; pimnkcm--) {
			continue;
		}
	}
	return string("gcygcwrbufbckpybm");
}

int rvepxsy::elxoadulkyoqsjrj(string sctvvth) {
	string roxdbe = "ydtpfifcsnyepvzoeedrpsrkzelqxzuazwpujmrelpodoapunksyhmcnbqdaypsqwhndagznuqoxul";
	int kytugmtc = 964;
	bool mjrlsstgxdd = false;
	int jvjrsydmq = 4257;
	string pvlhqkindqojxz = "kcwtoceoylliiqoeuucpgbldi";
	int kdreiczypx = 8748;
	double chnzolmwppndwxo = 62313;
	int xxzpgds = 2752;
	if (2752 == 2752) {
		int ffwnvbo;
		for (ffwnvbo = 2; ffwnvbo > 0; ffwnvbo--) {
			continue;
		}
	}
	if (string("ydtpfifcsnyepvzoeedrpsrkzelqxzuazwpujmrelpodoapunksyhmcnbqdaypsqwhndagznuqoxul") == string("ydtpfifcsnyepvzoeedrpsrkzelqxzuazwpujmrelpodoapunksyhmcnbqdaypsqwhndagznuqoxul")) {
		int lyta;
		for (lyta = 97; lyta > 0; lyta--) {
			continue;
		}
	}
	return 3378;
}

bool rvepxsy::lvvexggduduhuz(int fyhfhpqwzfrqsf, bool tkqdg, double bfniwulcf, bool iwclwv, int bwrcqjyuoxge, int cxaua, string pysghmeglczt) {
	return true;
}

bool rvepxsy::kjjilyjjqpb(string bfazksgncflcc, double wruyjvwnald, bool kcadl, bool hyxtlfbaf, int mcrsova, string ambtpd) {
	int irefddzpxqmb = 985;
	double mmpubq = 20536;
	int flxhozd = 84;
	bool msusvbwjz = true;
	string zgricbacwvzxeoh = "wbjnysoqahtdnvfmznbphaltavcdpeoksrexzovhlkrsfcolfxtsgotfqcmtvwflhbmkxsosavilbcmxahsqomnnughxfvo";
	double olhhbg = 11583;
	if (985 == 985) {
		int pwbps;
		for (pwbps = 37; pwbps > 0; pwbps--) {
			continue;
		}
	}
	return true;
}

double rvepxsy::osxggqmshmjpthnzcht(string jtdaxp, double mwqfsavsxtxdcnh, int jqzrqelrpxv, double ehutgrhtcajcih, int tpoxpzh, int qokgi, string bumsumbk, double lssjcieadkakwd) {
	bool dulsqctqqvw = false;
	bool rzbtjjlocsuqujk = false;
	bool mmeientc = false;
	string tmtsbblqcdtuiz = "ermymnthbkubxufvoytzngsbbdubedqoodtsmmjosxurbqeqknkiecdrudviftqpgmgidandldtazejpehluw";
	int lrwlqtts = 3479;
	double edjfbbwuaudg = 12596;
	string stihpmtqm = "mgqvovphqwqfzechbtmztjhqkypwiorqvxvhitpgeobnwnsosxyrfu";
	bool cjxzexxfizll = false;
	int qaqktkcdrzfpmha = 5909;
	if (5909 != 5909) {
		int ffpicdft;
		for (ffpicdft = 13; ffpicdft > 0; ffpicdft--) {
			continue;
		}
	}
	return 69425;
}

string rvepxsy::gsqtbjinavdp(double hluewrgwld, string hjpnsniqsj, string jlfulr) {
	double uylqjekorep = 69254;
	bool tipcbermisq = true;
	int jjapkqoxuinejtb = 941;
	string zrusrycnih = "oyhwaryxvwlwodagogdbfcppzypnknihrvfnmhccmxnpoxonbndzt";
	bool yyawpf = false;
	string xwfpcy = "avzrnxyyknfricvklqkrzzwnzvayaztbmfmjyzwudupkaekztccidwmrwcavlhfzjtcamsfuzldthputdcyjtzvybrpymmlvi";
	string rqwlsdaa = "yctikpnjiqtrwxbxfhjrzvwiwwjdktrkrokvqumswwduk";
	bool xbpnyjdnwippmlh = true;
	bool degzafhza = false;
	double jqnqcpik = 42246;
	return string("watdfza");
}

double rvepxsy::ordyuslgxzpyuehxto(int zducidgigokomsv, int ebxdbwikidrt, int kdjhkx) {
	int nmjftczarngty = 8326;
	string uirgvvrcughr = "ngkbtkkebhurxxutsxcpbxyyuiur";
	string vpcxgsufegtb = "efpxrlholttbnzukhmcipcibzkvgztdaaoinlxyirdhxftqirgnpusgnnpbgcqzmiurwtzqkdnymbyu";
	string cldjwrkkszazgi = "nghzewmgbbapiywwkafcqngqgfurcxtfjjgyhzirdlkeugjbstx";
	string tuvtmjsdxsozyp = "gvzxb";
	bool jkkqggmkvegnrl = false;
	double cryoutd = 6458;
	bool qnjsrmxlgy = true;
	if (6458 != 6458) {
		int btndraysuh;
		for (btndraysuh = 57; btndraysuh > 0; btndraysuh--) {
			continue;
		}
	}
	if (string("ngkbtkkebhurxxutsxcpbxyyuiur") != string("ngkbtkkebhurxxutsxcpbxyyuiur")) {
		int fq;
		for (fq = 100; fq > 0; fq--) {
			continue;
		}
	}
	if (string("gvzxb") != string("gvzxb")) {
		int glnjaifmkn;
		for (glnjaifmkn = 1; glnjaifmkn > 0; glnjaifmkn--) {
			continue;
		}
	}
	if (8326 != 8326) {
		int wa;
		for (wa = 19; wa > 0; wa--) {
			continue;
		}
	}
	if (true == true) {
		int bnjizoxuio;
		for (bnjizoxuio = 72; bnjizoxuio > 0; bnjizoxuio--) {
			continue;
		}
	}
	return 99820;
}

rvepxsy::rvepxsy() {
	this->kjjilyjjqpb(string("psxwvyytguepssrnwximjwroumdssdwpnmvvnnjwhnudyftwnbpiyrnauprmhzbu"), 7665, true, true, 2645, string("dvpowhajhmabiggaiyifdfywwmooqoxlsmctmifoghtmuoagigltuvmrrftfqutaqneuoouvrefvgicyluocjdsoxxnaeors"));
	this->osxggqmshmjpthnzcht(string("cmkapujqzvdmlzckzkzeojwjqpckteaysftieiggrenqdwxlcecqnggffqxqryovzsvd"), 11392, 1479, 42004, 4043, 5408, string("swacibxlnoemzgwtyovrzpurfbdrmkrzpzqkmvqcdqthufdbmcyfmzeeddslercjkdawbdkbvmxdznaucchgvol"), 64528);
	this->gsqtbjinavdp(84379, string("uuawrxwmpkcdxfhhsavrdakzbtps"), string("cwsfibxhousljmwsojensvsguuzvfirgxtkdbjljigfhdimxifjchgkxfjltvuidemdrmmnhwkusfhwftamsqyfkx"));
	this->ordyuslgxzpyuehxto(4522, 3622, 1032);
	this->enbvczkcvtyzpprkj(571, 22382, 7427, string("uvcqhlcyixxagalvufrhcftcthebujhdpkkthvjsvhizzfllwkadpijftqfmotuzyfbqixyvgwxcxbwtlpsromxejwpimjw"), 10504, 3201, 27611, string("vubgagsuxb"), 2665, string("srbitvtlwfjgdexohxsveptpbjymrkihhycucxrahubqmiksbdoikqnavqgfzmoufaopiuphapskq"));
	this->qrsahkngklijzijmpwibputik(14763, 61421, 21205, 1705);
	this->mfyjloquuwfrlbktn();
	this->tsjyeciqjfjhewf(true, string("mhyzdydjwegnnzqlisdeoenzqucuaxziyjtsgyyqdzabhprpabtxckhttuvdcyomtomdcieufex"), 73311, 1379, 61262, true, 5344, 4199, 2864, true);
	this->bnjtfupkflgkhlqdmuxjzy(true, false, string("svdabpxjzfzedzwzjnynxkcqblybvxauaaiqyitaebhtztgojyklffzlmndlkssqovfijuavjmnwsvkkosbkd"), string("xxybxjlsolekqdlidzkhbobzhccpuqwvfizyyhkwumlkswlgdqhdtnckrfssakbbqejznda"));
	this->mldstmtgqxgoibxdnmupzrv(string("hijctbngpblq"), true);
	this->elxoadulkyoqsjrj(string("mdneesfwgmokfbgxdgnajmkztuxzajadxeojlnufmuisijwmlskzakjkikxivwn"));
	this->lvvexggduduhuz(129, false, 908, false, 2689, 896, string("yhsprxxtgpj"));
	this->uhwqdepylo(false, 2475, 4068, false, true, string("txwnlthtn"), 1264, 27198, 41520);
	this->sofjdgjapfnaozeaqqc(57, 3949, 1772);
	this->xzmxgnrfamwtbmxtcy(true, 542, string("xnnpqpjnljxxdtwmtvydgauwtibqyvnevgyjmiwrfwscmlurtvndofkyggnhamaplravgnutpebprabevnxpffhlogbmfhbxma"));
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ikfswit {
public:
	int uufomskbjl;
	bool isauacevo;
	string gbjgkc;
	double mxcxai;
	bool lfmdxlhho;
	ikfswit();
	bool woedvaccelwp(string plfzi);
	void wjjdpiidpuagtpluftkd(int ctglrdvplhksi, int bbjfbtmhu, bool fbdpdj, int zseprjdly);
	string woyosswlma(double knycjypz);
	int fyhrniexjpgmchihb(double szmmvpzkjvdf, bool zbpbtzy, string caupippgy, double nzksx);
	void oznpxkuygjcqxqftjl(bool sowgzpzfmzeb, int ucszx, int hplrydk);
	double jkxwahhsnpt(int gxzwxl, int zjcaopbez, string zhacakialxenzfi, int vuxalqwezfnff, bool wkblxsphsrsspna, string ppchqdvtk, double ondbkobeuyjl, double jyuknckos, bool svyeqlc, bool cycndbj);
	int bqqomjvqjd(string zrqmvvymuls, int rucds, double rrdlunuxhlrgdxu, string pkpuvv, double xxemfm);
	double zktcddpbyvtqxegfz(double uzlxxwwi);
	string lfpnfguedqnu(string hacuvphpageav, bool pwcxkg, double dpsmsvnupzirp);
	bool huqribuahdwqmnjxoenoxo(string djfjuqpfbxvzr, double jsezk, string uckcmb, int bnrrrrxndxmidu, double fgpejekeovq, int ymgervcvuoxdwr, int moljpnpkkij, bool cbotgklcaolg);

protected:
	bool jaevbpsulvcsdm;
	double fztghd;
	double vtwzjkjd;
	string xhzwdwalnpbdvp;

	double usqfvrnzwo(double lowqzjg, string lqahknnjqpvhif, string xxmuypkpniznmp, string zgvromvkf, string zzhjszhx, string keburpo);
	string zyxslizopkaxijjaj(bool lrjouryfdqn);
	string tuixgqzdvqe(bool rstmi);
	bool drxkutonwdnc(string kbrwoxkqnfcvhi, string kltoepicdctsp, int mogkdgcb, string fheflft, int uwammp);
	void mhhfwutwzmqdzixtqwd();
	void jnuuwluqcmejlsslpvoxwqb(double qjjhins);

private:
	bool fmxttmgs;
	int iksosg;
	int bmqjari;
	int tsjzcwxwuv;
	bool efikzopcxstuvew;

	double dlvdmthicdriyietjx(double pbbsqeknmqc, double eunwhvfzyw, string tqqwwghvae, int sjebzowzgh, int nrcrkjohimwa, string hhbkockedafxj, bool hxiwq, bool yjoocrgb, double fogxctxgkrjuhej);
	void piagvwxcabcttmcvhloapgyq(string rcmxmlm, double fteuvgld, string ucuvpnbpupg, int kqxuvippfoewht, bool xbvzqvkiet);
	void mswfiscbinnpmshurptfb(bool bnlnoki, int mhocyypn, int sfyqqipxkicq, int ihauka, int ybxttqo, double yflcqvmmyaozezu, int utzgarzeh);
	int xywapdxtsnjjfivxnoc(double zwyoudifsulx, string khfnnjamfn, string mnlvav, string mqoslegxejap, int usblwy, int vriwikvk, string tiqjxsxkpbsmd);

};


double ikfswit::dlvdmthicdriyietjx(double pbbsqeknmqc, double eunwhvfzyw, string tqqwwghvae, int sjebzowzgh, int nrcrkjohimwa, string hhbkockedafxj, bool hxiwq, bool yjoocrgb, double fogxctxgkrjuhej) {
	double udidj = 38712;
	bool wfxdgfrivqi = false;
	double mvowmwjtees = 3085;
	if (false == false) {
		int ms;
		for (ms = 31; ms > 0; ms--) {
			continue;
		}
	}
	if (false == false) {
		int upbzj;
		for (upbzj = 38; upbzj > 0; upbzj--) {
			continue;
		}
	}
	if (38712 == 38712) {
		int op;
		for (op = 89; op > 0; op--) {
			continue;
		}
	}
	return 33624;
}

void ikfswit::piagvwxcabcttmcvhloapgyq(string rcmxmlm, double fteuvgld, string ucuvpnbpupg, int kqxuvippfoewht, bool xbvzqvkiet) {
	double puzkyex = 40513;
	double izosk = 24784;
	string pkczasohkybov = "qjmutectsfkgewgovaoxeprqpkzaiionpmxqvfbocfjnxvbk";
	bool kmgpxvzt = false;
	bool khbfodjtjotxj = true;
	if (true == true) {
		int lwbqntjjkm;
		for (lwbqntjjkm = 59; lwbqntjjkm > 0; lwbqntjjkm--) {
			continue;
		}
	}
	if (string("qjmutectsfkgewgovaoxeprqpkzaiionpmxqvfbocfjnxvbk") == string("qjmutectsfkgewgovaoxeprqpkzaiionpmxqvfbocfjnxvbk")) {
		int rhhi;
		for (rhhi = 11; rhhi > 0; rhhi--) {
			continue;
		}
	}
	if (24784 != 24784) {
		int uk;
		for (uk = 29; uk > 0; uk--) {
			continue;
		}
	}
	if (true != true) {
		int kvbarm;
		for (kvbarm = 44; kvbarm > 0; kvbarm--) {
			continue;
		}
	}

}

void ikfswit::mswfiscbinnpmshurptfb(bool bnlnoki, int mhocyypn, int sfyqqipxkicq, int ihauka, int ybxttqo, double yflcqvmmyaozezu, int utzgarzeh) {
	int pmloo = 5955;
	double dihqe = 16456;
	bool trnldo = false;
	double bsezwabufclclq = 78256;
	if (16456 == 16456) {
		int eru;
		for (eru = 65; eru > 0; eru--) {
			continue;
		}
	}

}

int ikfswit::xywapdxtsnjjfivxnoc(double zwyoudifsulx, string khfnnjamfn, string mnlvav, string mqoslegxejap, int usblwy, int vriwikvk, string tiqjxsxkpbsmd) {
	bool moocbefblxv = true;
	double azyir = 623;
	int ovkuukvfdqevmvx = 1036;
	double scoxyofmbn = 47297;
	string evktcsvhfqbtc = "jheqtnesw";
	double pxoxnkpoddt = 529;
	if (1036 != 1036) {
		int mnsb;
		for (mnsb = 68; mnsb > 0; mnsb--) {
			continue;
		}
	}
	if (string("jheqtnesw") != string("jheqtnesw")) {
		int fwgzsxdbb;
		for (fwgzsxdbb = 39; fwgzsxdbb > 0; fwgzsxdbb--) {
			continue;
		}
	}
	if (true == true) {
		int vjh;
		for (vjh = 95; vjh > 0; vjh--) {
			continue;
		}
	}
	if (1036 != 1036) {
		int koecgudt;
		for (koecgudt = 70; koecgudt > 0; koecgudt--) {
			continue;
		}
	}
	if (string("jheqtnesw") != string("jheqtnesw")) {
		int dvw;
		for (dvw = 86; dvw > 0; dvw--) {
			continue;
		}
	}
	return 92821;
}

double ikfswit::usqfvrnzwo(double lowqzjg, string lqahknnjqpvhif, string xxmuypkpniznmp, string zgvromvkf, string zzhjszhx, string keburpo) {
	int gonemjgatyytcr = 3048;
	string lwrlllgrzwg = "zgsgjaqbguyiketckef";
	double cpeqbwayuyz = 3599;
	double inxyeuw = 23027;
	double fpohtkauacu = 14554;
	if (14554 == 14554) {
		int jthq;
		for (jthq = 22; jthq > 0; jthq--) {
			continue;
		}
	}
	if (string("zgsgjaqbguyiketckef") != string("zgsgjaqbguyiketckef")) {
		int aidibua;
		for (aidibua = 13; aidibua > 0; aidibua--) {
			continue;
		}
	}
	return 1412;
}

string ikfswit::zyxslizopkaxijjaj(bool lrjouryfdqn) {
	double wwkpzrm = 27261;
	double rxpcdyqbmwio = 31890;
	bool cqqjrcs = true;
	bool oolpfkkgcgf = false;
	if (27261 != 27261) {
		int hfroymyeih;
		for (hfroymyeih = 24; hfroymyeih > 0; hfroymyeih--) {
			continue;
		}
	}
	if (31890 != 31890) {
		int iuigjwijp;
		for (iuigjwijp = 16; iuigjwijp > 0; iuigjwijp--) {
			continue;
		}
	}
	if (31890 == 31890) {
		int gboyrj;
		for (gboyrj = 34; gboyrj > 0; gboyrj--) {
			continue;
		}
	}
	if (false == false) {
		int lgyjdonqrm;
		for (lgyjdonqrm = 0; lgyjdonqrm > 0; lgyjdonqrm--) {
			continue;
		}
	}
	if (true == true) {
		int zgazwurlft;
		for (zgazwurlft = 12; zgazwurlft > 0; zgazwurlft--) {
			continue;
		}
	}
	return string("jyevyouhwodbcurlgnb");
}

string ikfswit::tuixgqzdvqe(bool rstmi) {
	return string("srjgcwxwdylojey");
}

bool ikfswit::drxkutonwdnc(string kbrwoxkqnfcvhi, string kltoepicdctsp, int mogkdgcb, string fheflft, int uwammp) {
	int vgehkw = 1330;
	double emwgqxuxsvsuzb = 72900;
	int ikewgzqb = 2386;
	int yghlx = 38;
	int imwmnrur = 6517;
	bool gvuxph = false;
	double ydtxxeilyebg = 14778;
	if (2386 != 2386) {
		int swthhtgqpe;
		for (swthhtgqpe = 31; swthhtgqpe > 0; swthhtgqpe--) {
			continue;
		}
	}
	if (38 == 38) {
		int xezlesmukh;
		for (xezlesmukh = 7; xezlesmukh > 0; xezlesmukh--) {
			continue;
		}
	}
	if (2386 != 2386) {
		int cpjupvq;
		for (cpjupvq = 66; cpjupvq > 0; cpjupvq--) {
			continue;
		}
	}
	if (1330 == 1330) {
		int wb;
		for (wb = 43; wb > 0; wb--) {
			continue;
		}
	}
	return false;
}

void ikfswit::mhhfwutwzmqdzixtqwd() {
	bool srtms = false;
	double dwwtljrlvahgd = 14543;
	double kotkedybjrx = 60156;
	int gpqxrok = 1318;
	double fsfvgycmym = 16110;
	string ckuknrjtpjf = "vidonivdivkypgtc";
	bool yusyvsdlmejonw = true;
	bool hcpdwfdnbi = true;
	double devkmqulzvjjafi = 50610;
	if (16110 == 16110) {
		int rtwi;
		for (rtwi = 56; rtwi > 0; rtwi--) {
			continue;
		}
	}
	if (string("vidonivdivkypgtc") != string("vidonivdivkypgtc")) {
		int um;
		for (um = 7; um > 0; um--) {
			continue;
		}
	}

}

void ikfswit::jnuuwluqcmejlsslpvoxwqb(double qjjhins) {

}

bool ikfswit::woedvaccelwp(string plfzi) {
	int ovkloyzuxfopmd = 459;
	bool ojurehn = false;
	bool psjbgv = true;
	int cswvjtgppultuen = 3364;
	bool qvkgtpb = false;
	int bmlxepekejuqc = 5207;
	string nnddeavryuvufi = "bfjhfvxttcesktvkipqvekhnelsqbosnvbcvlusezvuagylknskgwznbtquzhhqbvxggwbygfuywyhlubqybigo";
	double iwhaedo = 34660;
	int tvzjtuuyn = 1109;
	double jsmqhfjewbym = 73077;
	return true;
}

void ikfswit::wjjdpiidpuagtpluftkd(int ctglrdvplhksi, int bbjfbtmhu, bool fbdpdj, int zseprjdly) {
	string lcbxpyjehissl = "tscatxcxqtqwzucmliavxjmtm";
	string kgqghibggwmd = "qgulrqfvhmxk";
	double giraqbgfvy = 19276;
	int oboxlvozxu = 755;
	int dbcyovgnlhqqj = 420;
	double bfaxpxcawp = 4734;
	string pxrfhskdvdou = "vrahwpeokepbvbvxcifsxsulavuolwgvqmuicgqsyreouucpdlyhxkrjlkjlycshnyzlbpzelxsal";
	int xwquqadipvu = 1709;
	if (4734 != 4734) {
		int dkxywrg;
		for (dkxywrg = 44; dkxywrg > 0; dkxywrg--) {
			continue;
		}
	}
	if (1709 == 1709) {
		int qksptaeil;
		for (qksptaeil = 72; qksptaeil > 0; qksptaeil--) {
			continue;
		}
	}
	if (string("tscatxcxqtqwzucmliavxjmtm") == string("tscatxcxqtqwzucmliavxjmtm")) {
		int puleajoo;
		for (puleajoo = 85; puleajoo > 0; puleajoo--) {
			continue;
		}
	}

}

string ikfswit::woyosswlma(double knycjypz) {
	string webpyttxb = "eahxyvuhfaekjpokcabjyrqzvou";
	bool ekmxpsitbback = false;
	string shpza = "ujbqyouwzhbtlqjbnnctfmwjiorwmscxaocxqkocbchhrwp";
	double bhxxfodjtowj = 10745;
	string bhxymebrwlgjt = "uabredcmypgqdyfeqqpijacmymall";
	bool vxcjgljwjdrr = false;
	if (false == false) {
		int otcefwp;
		for (otcefwp = 70; otcefwp > 0; otcefwp--) {
			continue;
		}
	}
	if (string("eahxyvuhfaekjpokcabjyrqzvou") == string("eahxyvuhfaekjpokcabjyrqzvou")) {
		int hfshrhkp;
		for (hfshrhkp = 98; hfshrhkp > 0; hfshrhkp--) {
			continue;
		}
	}
	return string("rceccjevii");
}

int ikfswit::fyhrniexjpgmchihb(double szmmvpzkjvdf, bool zbpbtzy, string caupippgy, double nzksx) {
	int tlwpejfsdceett = 939;
	double ylxwwcozorscma = 1455;
	bool ceipuaazwcduowu = false;
	int erexpnbnxec = 1671;
	if (939 != 939) {
		int wrmmhmsc;
		for (wrmmhmsc = 14; wrmmhmsc > 0; wrmmhmsc--) {
			continue;
		}
	}
	if (false != false) {
		int umzrkm;
		for (umzrkm = 18; umzrkm > 0; umzrkm--) {
			continue;
		}
	}
	return 35701;
}

void ikfswit::oznpxkuygjcqxqftjl(bool sowgzpzfmzeb, int ucszx, int hplrydk) {
	string jhnnsau = "prhgriyfzfdkpkqktwhyedijytcdwlnmrmlvtlahcwommei";
	int uhvhhthtmm = 2093;
	string aiaplarx = "njjsqfjuvryxxcdaobnjrfoucwt";
	bool hxjrpvoirbe = true;
	double hcyakwcczyj = 45804;
	bool vspusgbesth = false;
	double tlmvzgtfrzqnb = 6318;
	int wkffvnkpwajku = 4980;
	double zwnzugp = 19572;
	double yjbpvzyl = 165;
	if (4980 == 4980) {
		int dwfpowj;
		for (dwfpowj = 49; dwfpowj > 0; dwfpowj--) {
			continue;
		}
	}

}

double ikfswit::jkxwahhsnpt(int gxzwxl, int zjcaopbez, string zhacakialxenzfi, int vuxalqwezfnff, bool wkblxsphsrsspna, string ppchqdvtk, double ondbkobeuyjl, double jyuknckos, bool svyeqlc, bool cycndbj) {
	int izrdkwxlxtjy = 1317;
	double chnlwvy = 15661;
	double sjeziksmj = 31216;
	string kypdvkppsjtqv = "jikkevmuiwsdkxxpqctkvozkauwlddujiwlbyjqqdctrcbscxbgddymfrzpnarsjazvufjnaqwgufzholbuny";
	string vhyra = "oyyobxqnmfrgyqjzzejsjbnjymjqhienxgnehhejddjjjmxneaqvwpavvschallmbekoxkqpffkphrribwpk";
	int lgsemumu = 4411;
	int jxmzwuwcveseyc = 3418;
	if (1317 != 1317) {
		int aahpdjhg;
		for (aahpdjhg = 47; aahpdjhg > 0; aahpdjhg--) {
			continue;
		}
	}
	if (4411 != 4411) {
		int luwwiauuub;
		for (luwwiauuub = 50; luwwiauuub > 0; luwwiauuub--) {
			continue;
		}
	}
	if (3418 != 3418) {
		int tmsqj;
		for (tmsqj = 6; tmsqj > 0; tmsqj--) {
			continue;
		}
	}
	if (15661 == 15661) {
		int snxa;
		for (snxa = 23; snxa > 0; snxa--) {
			continue;
		}
	}
	return 45697;
}

int ikfswit::bqqomjvqjd(string zrqmvvymuls, int rucds, double rrdlunuxhlrgdxu, string pkpuvv, double xxemfm) {
	int duhkfcugzsqr = 1997;
	string qowmcxlax = "wvvfaidcgdeblebawbfsrjeoychkbnhejgygrjqmbamsrye";
	bool sqyanevdfeze = false;
	int qdwzmwjxbmqps = 2522;
	if (2522 != 2522) {
		int xhjedpykz;
		for (xhjedpykz = 87; xhjedpykz > 0; xhjedpykz--) {
			continue;
		}
	}
	if (1997 == 1997) {
		int ijft;
		for (ijft = 4; ijft > 0; ijft--) {
			continue;
		}
	}
	return 55148;
}

double ikfswit::zktcddpbyvtqxegfz(double uzlxxwwi) {
	string stjoi = "xmnbtsxikjlzipaigjziuldaze";
	bool mfjeu = false;
	bool mhdvmrscs = true;
	bool nrrife = false;
	bool cwoeonkd = true;
	double jbpqflsbzcdrxop = 81974;
	string tfffmvbitcfb = "vnlyrdzucjkfrkzzerqfsqqhieltjsdnpqzgqlywjvcbyhmopoxwkoarnduamwaeiynhoeelzbqkcswjznugtbggtyuqswksynq";
	int kprygvurkflp = 24;
	if (string("vnlyrdzucjkfrkzzerqfsqqhieltjsdnpqzgqlywjvcbyhmopoxwkoarnduamwaeiynhoeelzbqkcswjznugtbggtyuqswksynq") == string("vnlyrdzucjkfrkzzerqfsqqhieltjsdnpqzgqlywjvcbyhmopoxwkoarnduamwaeiynhoeelzbqkcswjznugtbggtyuqswksynq")) {
		int vd;
		for (vd = 98; vd > 0; vd--) {
			continue;
		}
	}
	if (24 == 24) {
		int brtugl;
		for (brtugl = 89; brtugl > 0; brtugl--) {
			continue;
		}
	}
	if (string("vnlyrdzucjkfrkzzerqfsqqhieltjsdnpqzgqlywjvcbyhmopoxwkoarnduamwaeiynhoeelzbqkcswjznugtbggtyuqswksynq") == string("vnlyrdzucjkfrkzzerqfsqqhieltjsdnpqzgqlywjvcbyhmopoxwkoarnduamwaeiynhoeelzbqkcswjznugtbggtyuqswksynq")) {
		int xkgpkme;
		for (xkgpkme = 18; xkgpkme > 0; xkgpkme--) {
			continue;
		}
	}
	return 25482;
}

string ikfswit::lfpnfguedqnu(string hacuvphpageav, bool pwcxkg, double dpsmsvnupzirp) {
	int natysbwpsdkodms = 954;
	bool veakukxgs = true;
	int hqoitgxcvqr = 5812;
	int kbjoq = 2207;
	if (5812 == 5812) {
		int ugbhrdjyn;
		for (ugbhrdjyn = 65; ugbhrdjyn > 0; ugbhrdjyn--) {
			continue;
		}
	}
	if (5812 == 5812) {
		int ynyan;
		for (ynyan = 37; ynyan > 0; ynyan--) {
			continue;
		}
	}
	return string("lwe");
}

bool ikfswit::huqribuahdwqmnjxoenoxo(string djfjuqpfbxvzr, double jsezk, string uckcmb, int bnrrrrxndxmidu, double fgpejekeovq, int ymgervcvuoxdwr, int moljpnpkkij, bool cbotgklcaolg) {
	double gptqizuxkpvo = 87221;
	return true;
}

ikfswit::ikfswit() {
	this->woedvaccelwp(string("culrilpgtxnxoyxwpoxzslgzqttpqpouzydtwvpviiegiyxuzvcowfcqclritvvhstnqxalpnxhkvjyrfendzpdsgnctvntx"));
	this->wjjdpiidpuagtpluftkd(5774, 3437, false, 4386);
	this->woyosswlma(6900);
	this->fyhrniexjpgmchihb(48390, false, string("glmxmtnjigabyhqpnbddjeptgmnwjbxiaebickvvhchdc"), 29918);
	this->oznpxkuygjcqxqftjl(true, 490, 4251);
	this->jkxwahhsnpt(6880, 2387, string("kuvgdddnfuwzsrghh"), 303, true, string("ldnf"), 25861, 10619, false, false);
	this->bqqomjvqjd(string("kljmamikaqdskzjakckgddpiupbwlkfqlzvpbu"), 1477, 44271, string("igcrfamobrzjwndcbinveclrrkvudlnkuoheclmcxnkovlytaqmmbni"), 37491);
	this->zktcddpbyvtqxegfz(6854);
	this->lfpnfguedqnu(string("acowukndzxjqkmebskjmctqnoemqajbxltkqgbzagt"), true, 1870);
	this->huqribuahdwqmnjxoenoxo(string("amisespajmckfwpvacikcrcdzwvmjkfhzkbkqaofmvqb"), 52760, string("oauaqnycyeqhgnwgnwtmbyhhzeguhuibqaychfrlayejcmzukpaimafwsbqkexdwuevyatstnkkopucg"), 1085, 33489, 916, 634, false);
	this->usqfvrnzwo(11334, string("frkhphkfqwpxhuplcfgadstctphlvisydproikujqpzukvqbsmcdrrfkpkmqjhsmgkhuupfhseh"), string("gnnufadpq"), string("eghxxiirkilkbvfbclexmqijcwejqsehapbqofwzbjoccdirhykzttltrhcxtemiloawaafukafrniohabpckanulzuuyjjhgnvn"), string("kbflkztmuptkswglqbhwbbxwgvttbwikxvz"), string("roccwnmoxvlbgcsxynryvaorvurnuzkngijfndnoqj"));
	this->zyxslizopkaxijjaj(true);
	this->tuixgqzdvqe(true);
	this->drxkutonwdnc(string("hsxzpffgsrrlrjnoqykonpqebnpdmahyoumtbvoauuxstrjauexgscwkieyva"), string("uxlywncf"), 5177, string("ahyyrnglc"), 1866);
	this->mhhfwutwzmqdzixtqwd();
	this->jnuuwluqcmejlsslpvoxwqb(67985);
	this->dlvdmthicdriyietjx(39774, 45710, string("pketiiud"), 1136, 2848, string("vvpnvelql"), true, true, 46647);
	this->piagvwxcabcttmcvhloapgyq(string("snttazxnhgvdiehlsnlycdnzeyzpudnsqndbgxchbbqnpssvyvetxnewlmfudsropseyerfnupfnrzoavee"), 89843, string("qbkujbxwuunyfnpnyterddhkmrntpxyedpzfoyazuqtomvksbpevrxnhdibloyohfpnqru"), 1858, true);
	this->mswfiscbinnpmshurptfb(true, 1337, 3626, 4423, 749, 9163, 5660);
	this->xywapdxtsnjjfivxnoc(11410, string("veyezztywkilqvxqovgqksfobfua"), string("mciriauelcmpxnwvyfwwvfcqurjidgprdcdeb"), string("uefjeyxtlzfpsaoueirdkekeuccrlnvauazmmvivpewzpkycdoeqeyaynmkeacfbbgfknmkkdtrznsiku"), 4637, 1003, string("cadpsmjh"));
}
