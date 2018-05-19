

#pragma once

#include "Render.h"





// We don't use these anywhere else, no reason for them to be
// available anywhere else
enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

// Initialises the rendering system, setting up fonts etc
void Render::SetupFonts()
{
	font.Default = 0x1D; // MainMenu Font from vgui_spew_fonts 
    font.ESP = g_Surface->FontCreate();
    font.Guns = g_Surface->FontCreate();
    font.Defuse = g_Surface->FontCreate();
    font.Time = g_Surface->FontCreate();
	font.DroppedGunsicon = g_Surface->FontCreate();
    font.DroppedGuns = g_Surface->FontCreate();
	font.Watermark = g_Surface->FontCreate();



	g_Surface->SetFontGlyphSet(font.ESP, "Tahoma", 11, 700, 0, 0, FONTFLAG_DROPSHADOW);
	g_Surface->SetFontGlyphSet(font.Defuse, "Tahoma", 15, 700, 0, 0, FONTFLAG_DROPSHADOW);
	g_Surface->SetFontGlyphSet(font.Guns, "Tahoma", 10, 700, 0, 0, FONTFLAG_DROPSHADOW);
	g_Surface->SetFontGlyphSet(font.Time, "Arial", 13, 550, 0, 0, FONTFLAG_OUTLINE);
	//g_Surface->SetFontGlyphSet(font.DroppedGunsicon, "astriumwep", 20, 700, 0, 0, FONTFLAG_ANTIALIAS);
	//g_Surface->SetFontGlyphSet(font.DroppedGuns, "Tahoma", 8, 700, 0, 0, FONTFLAG_DROPSHADOW);
	g_Surface->SetFontGlyphSet(font.DroppedGuns, "Tahoma", 10, 700, 0, 0, FONTFLAG_DROPSHADOW);
	g_Surface->SetFontGlyphSet(font.DroppedGunsicon, "astriumwep", 20, 700, 0, 0, FONTFLAG_ANTIALIAS);
	g_Surface->SetFontGlyphSet(font.Watermark, "Tahoma", 11, 700, 0, 0, FONTFLAG_OUTLINE);

}

RECT Render::GetViewport()
{
	RECT Viewport = { 0, 0, 0, 0 };
	int w, h;
	g_Engine->GetScreenSize(w, h);
	Viewport.right = w; Viewport.bottom = h;
	return Viewport;
}

void Render::Clear(int x, int y, int w, int h, Color color)
{
	g_Surface->DrawSetColor(color);
	g_Surface->DrawFilledRect(x, y, x + w, y + h);
}

void Render::DrawOutlinedRect(int x, int y, int w, int h, Color col)
{
	g_Surface->DrawSetColor(col);
	g_Surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void Render::Outline(int x, int y, int w, int h, Color color)
{
	g_Surface->DrawSetColor(color);
	g_Surface->DrawOutlinedRect(x, y, x + w, y + h);
}

void Render::DrawString2(DWORD font, int x, int y, Color color, DWORD alignment, const char* msg, ...)
{
	va_list va_alist;
	char buf[1024];
	va_start(va_alist, msg);
	_vsnprintf(buf, sizeof(buf), msg, va_alist);
	va_end(va_alist);
	wchar_t wbuf[1024];
	MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

	int r = 255, g = 255, b = 255, a = 255;
	color.GetColor(r, g, b, a);

	int width, height;
	g_Surface->GetTextSize(font, wbuf, width, height);

	if (alignment & FONT_RIGHT)
		x -= width;
	if (alignment & FONT_CENTER)
		x -= width / 2;

	g_Surface->DrawSetTextFont(font);
	g_Surface->DrawSetTextColor(r, g, b, a);
	g_Surface->DrawSetTextPos(x, y - height / 2);
	g_Surface->DrawPrintText(wbuf, wcslen(wbuf));
}


void Render::OutlineRainbow(int x, int y, int width, int height, float flSpeed, float &flRainbow)
{
	Color colColor(0, 0, 0);

	flRainbow += flSpeed;
	if (flRainbow > 1.f) flRainbow = 0.f;

	for (int i = 0; i < width; i++)
	{
		float hue = (1.f / (float)width) * i;
		hue -= flRainbow;
		if (hue < 0.f) hue += 1.f;

		Color colRainbow = colColor.FromHSB(hue, 1.f, 1.f);
		Outline(x + i, y, 1, height, colRainbow);
	}
}

void Render::Pixel(int x, int y, Color col)
{
	g_Surface->DrawSetColor(col);
	g_Surface->DrawFilledRect(x, y, x + 1, y + 1);
}

void Render::Line(int x, int y, int x2, int y2, Color color)
{
	g_Surface->DrawSetColor(color);
	g_Surface->DrawLine(x, y, x2, y2);
}

void Render::PolyLine(int *x, int *y, int count, Color color)
{
	g_Surface->DrawSetColor(color);
	g_Surface->DrawPolyLine(x, y, count);
}

bool Render::WorldToScreen(Vector &in, Vector &out)
{
	const matrix3x4& worldToScreen = g_Engine->WorldToScreenMatrix(); //Grab the world to screen matrix from CEngineClient::WorldToScreenMatrix

	float w = worldToScreen[3][0] * in[0] + worldToScreen[3][1] * in[1] + worldToScreen[3][2] * in[2] + worldToScreen[3][3]; //Calculate the angle in compareson to the player's camera.
	out.z = 0; //Screen doesn't have a 3rd dimension.

	if (w > 0.001) //If the object is within view.
	{
		RECT ScreenSize = GetViewport();
		float fl1DBw = 1 / w; //Divide 1 by the angle.
		out.x = (ScreenSize.right / 2) + (0.5f * ((worldToScreen[0][0] * in[0] + worldToScreen[0][1] * in[1] + worldToScreen[0][2] * in[2] + worldToScreen[0][3]) * fl1DBw) * ScreenSize.right + 0.5f); //Get the X dimension and push it in to the Vector.
		out.y = (ScreenSize.bottom / 2) - (0.5f * ((worldToScreen[1][0] * in[0] + worldToScreen[1][1] * in[1] + worldToScreen[1][2] * in[2] + worldToScreen[1][3]) * fl1DBw) * ScreenSize.bottom + 0.5f); //Get the Y dimension and push it in to the Vector.
		return true;
	}

	return false;
}

void Render::Text(int x, int y, Color color, DWORD font, const char* text, ...)
{
	size_t origsize = strlen(text) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

	g_Surface->DrawSetTextFont(font);

	g_Surface->DrawSetTextColor(color);
	g_Surface->DrawSetTextPos(x, y);
	g_Surface->DrawPrintText(wcstring, wcslen(wcstring));
}

void Render::Text(int x, int y, Color color, DWORD font, const wchar_t* text)
{
	g_Surface->DrawSetTextFont(font);
	g_Surface->DrawSetTextColor(color);
	g_Surface->DrawSetTextPos(x, y);
	g_Surface->DrawPrintText(text, wcslen(text));
}

void Render::Text(int x, int y, DWORD font, const wchar_t* text)
{
	g_Surface->DrawSetTextFont(font);
	g_Surface->DrawSetTextPos(x, y);
	g_Surface->DrawPrintText(text, wcslen(text));
}

void Render::Textf(int x, int y, Color color, DWORD font, const char* fmt, ...)
{
	if (!fmt) return; //if the passed string is null return
	if (strlen(fmt) < 2) return;

	//Set up va_list and buffer to hold the params 
	va_list va_alist;
	char logBuf[256] = { 0 };

	//Do sprintf with the parameters
	va_start(va_alist, fmt);
	_vsnprintf_s(logBuf + strlen(logBuf), 256 - strlen(logBuf), sizeof(logBuf) - strlen(logBuf), fmt, va_alist);
	va_end(va_alist);

	Text(x, y, color, font, logBuf);
}

RECT Render::GetTextSize(DWORD font, const char* text)
{
	size_t origsize = strlen(text) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, text, _TRUNCATE);

	RECT rect; int x, y;
	g_Surface->GetTextSize(font, wcstring, x, y);
	rect.left = x; rect.bottom = y;
	rect.right = x;
	return rect;
}

void Render::GradientV(int x, int y, int w, int h, Color c1, Color c2)
{
	Clear(x, y, w, h, c1);
	BYTE first = c2.r();
	BYTE second = c2.g();
	BYTE third = c2.b();
	for (int i = 0; i < h; i++)
	{
		float fi = float(i), fh = float(h);
		float a = float(fi / fh);
		DWORD ia = DWORD(a * 255);
		Clear(x, y + i, w, 1, Color(first, second, third, ia));
	}
}

void Render::GradientH(int x, int y, int w, int h, Color c1, Color c2)
{
	Clear(x, y, w, h, c1);
	BYTE first = c2.r();
	BYTE second = c2.g();
	BYTE third = c2.b();
	for (int i = 0; i < w; i++)
	{
		float fi = float(i), fw = float(w);
		float a = float(fi / fw);
		DWORD ia = DWORD(a * 255);
		Clear(x + i, y, 1, h, Color(first, second, third, ia));
	}
}

void Render::Polygon(int count, Vertex_t* Vertexs, Color color)
{
	static int Texture = g_Surface->CreateNewTextureID(true); //need to make a texture with procedural true
	unsigned char buffer[4] = { 255, 255, 255, 255 };//{ color.r(), color.g(), color.b(), color.a() };

	g_Surface->DrawSetTextureRGBA(Texture, buffer, 1, 1); //Texture, char array of texture, width, height
	g_Surface->DrawSetColor(color); // keep this full color and opacity use the RGBA @top to set values.
	g_Surface->DrawSetTexture(Texture); // bind texture

	g_Surface->DrawTexturedPolygon(count, Vertexs);
}

void Render::PolygonOutline(int count, Vertex_t* Vertexs, Color color, Color colorLine)
{
	static int x[128];
	static int y[128];

	Polygon(count, Vertexs, color);

	for (int i = 0; i < count; i++)
	{
		x[i] = int(Vertexs[i].m_Position.x);
		y[i] = int(Vertexs[i].m_Position.y);
	}

	PolyLine(x, y, count, colorLine);
}

void Render::PolyLine(int count, Vertex_t* Vertexs, Color colorLine)
{
	static int x[128];
	static int y[128];

	for (int i = 0; i < count; i++)
	{
		x[i] = int(Vertexs[i].m_Position.x);
		y[i] = int(Vertexs[i].m_Position.y);
	}

	PolyLine(x, y, count, colorLine);
}

void Render::OutlineCircle(int x, int y, int r, int seg, Color color)
{
	g_Surface->DrawSetColor(0, 0, 0, 255);
	g_Surface->DrawOutlinedCircle(x, y, r - 1, seg);
	g_Surface->DrawOutlinedCircle(x, y, r + 1, seg);
	g_Surface->DrawSetColor(color);
	g_Surface->DrawOutlinedCircle(x, y, r, seg);
}

Render* g_Render = new(Render);


namespace junk7569074 {
	void junk2451157() {
		return;
		float gfpw4;
		float acbw3;
		float t8rbvu;
		float w8vck;
		float b070tm;
		float wj7v3b;
		float aya5yg;
		float d58v86;
		float kh23j;
		float ciexoj;
		if (d58v86 == 8570581.3025)
			d58v86 = 7282692.1540;
		kh23j = 1131266.3056;
		if (wj7v3b == 5223760.3979)
			wj7v3b = 5002760.5909;
		acbw3 = 4701488.6622;
		if (aya5yg == 2986647.4036)
			aya5yg = 2448086.3196;
		wj7v3b = 4105449.1369;
		for (int xc4gv6 = 0; xc4gv6 > 100; xc4gv6++)
		{
			gfpw4 = 847191.1021;
		}
		ciexoj = 7265039.4475;
		for (int rxij36 = 0; rxij36 > 100; rxij36++)
		{
			t8rbvu = 9504950.8488;
		}
		d58v86 = 4050905.6847;
		for (int q74h8 = 0; q74h8 > 100; q74h8++)
		{
			wj7v3b = 9267461.7671;
		}
		ciexoj = 3486393.0500;
		b070tm = 988854.6253;
		while (ciexoj == 3544217.4023)
		{
			ciexoj = 5372546.1819;
		}
		kh23j = 10192102.5351;
		for (int j90nf = 0; j90nf > 100; j90nf++)
		{
			aya5yg = 2392351.6572;
		}
		kh23j = 7849020.1728;
		while (gfpw4 == 8150150.9337)
		{
			gfpw4 = 6992403.1000;
		}
		kh23j = 7262357.7116;
	}
	void junk6709678() {
		return;
		float m8agqm;
		float ohn5z;
		float acnzu;
		float asy94g;
		float e0fdobr;
		float rp698q;
		float ocyxic;
		float ikydms;
		float h39wu;
		float p9lt0u;
		for (int cojig = 0; cojig > 100; cojig++)
		{
			ohn5z = 1556829.0636;
		}
		ocyxic = 3347442.7230;
		if (ocyxic == 4555977.5746)
			ocyxic = 1200661.9330;
		acnzu = 9800075.7492;
		m8agqm = 4712722.3455;
		for (int iotiqk = 0; iotiqk > 100; iotiqk++)
		{
			ocyxic = 1744632.3312;
		}
		rp698q = 8098856.8169;
		for (int x5x61 = 0; x5x61 > 100; x5x61++)
		{
			ocyxic = 3953578.9096;
		}
		h39wu = 8985863.6504;
		if (rp698q == 4306024.4320)
			rp698q = 4685250.0791;
		ohn5z = 3106839.7183;
		if (p9lt0u == 6770970.5643)
			p9lt0u = 97562.9606;
		rp698q = 2382654.4007;
		for (int rp66mu = 0; rp66mu > 100; rp66mu++)
		{
			rp698q = 7586167.3998;
		}
		acnzu = 1177550.6293;
		while (asy94g == 5786836.3783)
		{
			asy94g = 4801520.8798;
		}
		h39wu = 1714509.2712;
		asy94g = 9256730.4063;
	}
	void junk7357722() {
		return;
		float khme8;
		float konvtq;
		float hzafu;
		float knhwyx;
		float zce4op;
		float od2pmd;
		float f5kiwn;
		float ih9wfj;
		float kx83fm;
		float d89zg9;
		ih9wfj = 7596015.0398;
		for (int yyw7af = 0; yyw7af > 100; yyw7af++)
		{
			knhwyx = 5441125.8474;
		}
		od2pmd = 511296.4369;
		ih9wfj = 3794949.8947;
		konvtq = 7839975.2888;
		kx83fm = 6353297.7947;
		for (int csljq = 0; csljq > 100; csljq++)
		{
			zce4op = 6209525.2952;
		}
		d89zg9 = 6046057.5502;
		if (khme8 == 2967497.7069)
			khme8 = 293615.2145;
		kx83fm = 2917568.6979;
		for (int gyqnc = 0; gyqnc > 100; gyqnc++)
		{
			konvtq = 3884918.2834;
		}
		zce4op = 9552678.5842;
		if (zce4op == 9717438.7177)
			zce4op = 4139751.0514;
		khme8 = 1604078.1305;
		for (int mwpw75 = 0; mwpw75 > 100; mwpw75++)
		{
			hzafu = 2068918.9011;
		}
		f5kiwn = 313816.7432;
	}
	void junk4430274() {
		return;
		float padoaj;
		float bh0mw;
		float oau7sj;
		float o5rxur;
		float pzmvoq;
		float o6i1tm;
		float jb59m;
		float bc3uyc;
		float h2sin;
		float weo0x;
		jb59m = 6442834.6473;
		for (int r07yag = 0; r07yag > 100; r07yag++)
		{
			oau7sj = 5091247.2828;
		}
		bh0mw = 6705102.8160;
		if (o6i1tm == 7025079.8820)
			o6i1tm = 7679842.2234;
		o5rxur = 4168925.9562;
		o5rxur = 889830.4486;
		while (pzmvoq == 653190.9998)
		{
			pzmvoq = 4472080.5921;
		}
		h2sin = 5545511.9311;
		if (weo0x == 1257359.6328)
			weo0x = 4049885.0862;
		weo0x = 2572442.2037;
		for (int e0oj1 = 0; e0oj1 > 100; e0oj1++)
		{
			bc3uyc = 9749616.4661;
		}
		bc3uyc = 2784322.9527;
		jb59m = 1232682.9938;
		o5rxur = 8818000.9066;
		while (o6i1tm == 5951418.7049)
		{
			o6i1tm = 7514089.6665;
		}
		padoaj = 399264.6047;
	}
	void junk6045266() {
		return;
		float d3yq3r;
		float flkq7k;
		float bxmid;
		float nsvv4r;
		float n4rwwt;
		float x7vdp;
		float vnobmi;
		float b937wp;
		float w5rep;
		float c27k1;
		while (x7vdp == 2894488.8763)
		{
			x7vdp = 383200.5690;
		}
		d3yq3r = 2012899.0783;
		w5rep = 5537027.4794;
		flkq7k = 4051857.0291;
		if (d3yq3r == 4490785.3093)
			d3yq3r = 4058928.2838;
		vnobmi = 1794879.0823;
		while (bxmid == 8125877.0710)
		{
			bxmid = 7872826.7265;
		}
		c27k1 = 8457123.0247;
		w5rep = 9846100.2403;
		if (d3yq3r == 10229674.3856)
			d3yq3r = 7282046.6407;
		w5rep = 1207957.3054;
		for (int j62z7c = 0; j62z7c > 100; j62z7c++)
		{
			c27k1 = 2587196.7744;
		}
		n4rwwt = 6024792.3894;
		nsvv4r = 4078325.2117;
		while (vnobmi == 30680.3240)
		{
			vnobmi = 9574810.0095;
		}
		n4rwwt = 2623492.1371;
	}
	void junk1393373() {
		return;
		float qogxvi;
		float p3w3rm;
		float gh7wx5;
		float n239xt;
		float q4bo37;
		float glfsiu;
		float zr85so;
		float edjrc;
		float kqt0s;
		float o4uan4;
		for (int ignyn4 = 0; ignyn4 > 100; ignyn4++)
		{
			n239xt = 2674215.9676;
		}
		q4bo37 = 10390178.8312;
		p3w3rm = 5390644.2126;
		o4uan4 = 3246371.4825;
		for (int zsud = 0; zsud > 100; zsud++)
		{
			gh7wx5 = 3513528.5246;
		}
		glfsiu = 7104992.8132;
		for (int ivgzmc = 0; ivgzmc > 100; ivgzmc++)
		{
			zr85so = 4746391.9490;
		}
		qogxvi = 2589654.8260;
		if (n239xt == 8690188.6740)
			n239xt = 328648.3854;
		edjrc = 8938996.0753;
		p3w3rm = 10363754.4344;
		while (edjrc == 4686042.2881)
		{
			edjrc = 4257930.0012;
		}
		n239xt = 528536.8998;
		zr85so = 1958469.8460;
		while (p3w3rm == 5766702.5852)
		{
			p3w3rm = 5326040.3886;
		}
		edjrc = 6684592.2461;
	}
	void junk8302770() {
		return;
		float m0ps3ro;
		float gz3lol;
		float ttsuq;
		float v87kz;
		float ff0mn;
		float z1ddml;
		float qkqcu8;
		float lld70d;
		float gxdpg;
		float rc1dtb;
		for (int hemz37 = 0; hemz37 > 100; hemz37++)
		{
			v87kz = 3146522.2566;
		}
		gxdpg = 7598157.2926;
		rc1dtb = 3635807.7789;
		for (int k8p8w = 0; k8p8w > 100; k8p8w++)
		{
			v87kz = 8114650.5883;
		}
		v87kz = 6686414.4389;
		while (rc1dtb == 7351306.1113)
		{
			rc1dtb = 8049712.3285;
		}
		rc1dtb = 4491266.8968;
		if (qkqcu8 == 10545479.2306)
			qkqcu8 = 1981562.7217;
		z1ddml = 4482688.3173;
		for (int xpv35e = 0; xpv35e > 100; xpv35e++)
		{
			lld70d = 9905438.1248;
		}
		m0ps3ro = 768025.3403;
		gz3lol = 6439778.8577;
		ttsuq = 7600083.3262;
		z1ddml = 8764362.1244;
		while (rc1dtb == 3071740.5574)
		{
			rc1dtb = 3926816.0685;
		}
		ff0mn = 10321735.7447;
	}
	void junk6231670() {
		return;
		float gnj3x;
		float wvyf8a;
		float xpdr0ad;
		float m37dy;
		float fsm9pn;
		float qey2uc;
		float xo98bo;
		float x6ma2;
		float cz2kes;
		float scvyp;
		for (int y2av7i = 0; y2av7i > 100; y2av7i++)
		{
			scvyp = 6446451.8410;
		}
		m37dy = 2645677.6345;
		while (m37dy == 9909196.2723)
		{
			m37dy = 7360282.3295;
		}
		cz2kes = 6877430.6142;
		for (int exy49 = 0; exy49 > 100; exy49++)
		{
			xo98bo = 5945279.1460;
		}
		fsm9pn = 5917345.4035;
		while (cz2kes == 1882262.7543)
		{
			cz2kes = 6163058.9566;
		}
		scvyp = 3610389.8262;
		fsm9pn = 1099771.8147;
		if (x6ma2 == 3958946.2832)
			x6ma2 = 7108833.7773;
		wvyf8a = 7861447.4166;
		while (x6ma2 == 4856097.9230)
		{
			x6ma2 = 2194712.7622;
		}
		gnj3x = 7926402.9154;
		m37dy = 8085931.6153;
		if (x6ma2 == 9962598.1398)
			x6ma2 = 8868896.9533;
		cz2kes = 9624651.1906;
		if (xpdr0ad == 10436880.5642)
			xpdr0ad = 7496720.4201;
		xpdr0ad = 764730.8538;
	}
	void junk1248561() {
		return;
		float wvp62m;
		float j2qpnv;
		float qqh6pg;
		float jldhal;
		float sexhh5;
		float mq3w1p;
		float vkeqvu;
		float rxohaq;
		float zgsgtd;
		float z05r2p;
		if (zgsgtd == 6236523.6096)
			zgsgtd = 5323953.5179;
		vkeqvu = 7977680.2780;
		for (int fj58b = 0; fj58b > 100; fj58b++)
		{
			z05r2p = 5763604.7445;
		}
		qqh6pg = 206415.5909;
		for (int fxu1h = 0; fxu1h > 100; fxu1h++)
		{
			rxohaq = 1916465.5578;
		}
		qqh6pg = 5954785.3381;
		zgsgtd = 6092276.7298;
		while (sexhh5 == 424857.5252)
		{
			sexhh5 = 3800696.2871;
		}
		sexhh5 = 4293675.3031;
		vkeqvu = 6807955.2284;
		if (zgsgtd == 4014593.7865)
			zgsgtd = 9341238.4140;
		mq3w1p = 2273447.8373;
		while (z05r2p == 417072.2671)
		{
			z05r2p = 4738663.7966;
		}
		mq3w1p = 9932819.3385;
		for (int ne7b3v = 0; ne7b3v > 100; ne7b3v++)
		{
			j2qpnv = 4599433.4958;
		}
		zgsgtd = 2265506.0977;
		for (int lqwvjk = 0; lqwvjk > 100; lqwvjk++)
		{
			j2qpnv = 6576518.8744;
		}
		j2qpnv = 3325283.8538;
	}
	void junk916914() {
		return;
		float sbm478;
		float jcm3l;
		float mmugh8;
		float x2tno5;
		float hna72k;
		float pe6rjh;
		float sesy1;
		float jn0k8i;
		float y49uqe;
		float p93b39;
		while (pe6rjh == 2603190.8445)
		{
			pe6rjh = 2813700.9631;
		}
		p93b39 = 2432233.3979;
		if (jcm3l == 2489288.1416)
			jcm3l = 10081144.5278;
		hna72k = 6971777.6239;
		jn0k8i = 7718773.4272;
		if (hna72k == 10279460.2764)
			hna72k = 5571019.7837;
		jn0k8i = 6634923.9742;
		if (hna72k == 4615883.6754)
			hna72k = 1255696.6992;
		y49uqe = 3989413.8647;
		while (jn0k8i == 5097445.2267)
		{
			jn0k8i = 7764648.8067;
		}
		p93b39 = 1996822.3554;
		while (p93b39 == 3800006.3834)
		{
			p93b39 = 14757.4644;
		}
		mmugh8 = 6799915.4346;
		if (p93b39 == 3215240.5627)
			p93b39 = 2152192.2760;
		sbm478 = 555452.9037;
		if (mmugh8 == 7019169.3559)
			mmugh8 = 6446228.6812;
		jn0k8i = 9374789.5201;
		while (mmugh8 == 738197.0798)
		{
			mmugh8 = 2687230.8903;
		}
		jn0k8i = 2013633.3817;
	}
	void doJunk() {
		junk7569074::junk2451157();
		junk7569074::junk6709678();
		junk7569074::junk7357722();
		junk7569074::junk4430274();
		junk7569074::junk6045266();
		junk7569074::junk1393373();
		junk7569074::junk8302770();
		junk7569074::junk6231670();
		junk7569074::junk1248561();
		junk7569074::junk916914();
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class caybrvg {
public:
	bool vxpilsqkcaqe;
	double twbzw;
	caybrvg();
	void pknjbjydefqcphb(int vhjgzvnvip, int fnvbyc, int gicazoijlck, string sktznssdtqbxmym, double fozwqtqsiywn, int cigemirgwo, bool ypaqnhazdnlexo, int abrxdylne, string pvoyarjxqeqtikn, bool bzeoljnlh);
	string disfgkkoyeeiuksqohwcfl(bool uakynd, bool euwbomajmtrzo, string pdbur, int sqqhptctbdh);
	string iczzxtizxcbdrwpeeqy(string emdjmkxqnd, int xcotzwyxt, double rmdopuwlkxd, bool fszfis, string soynekg);
	string ukejvdrmtlydomhmt(double lxlwxvskffrwd, double cldrel, bool ragekhscgnyyrgs, int wnowxwkz, bool dmoyuyrf);
	void oeiozoodla(double ohtxdeviavdt);
	string xnipambvianyanohrogzswye(int zrhgrpygg, double hqasbkbfpczo, bool ujjpmaku, double buvcx, string agyeyo, string teoztutuijhzq, int spvwdldnbviljog);
	double furnqgbluowfla(int orsldmwxbe, bool jyzcyhkxmlhs, bool fjjmzriqbca, int bhrkgogy, double xuwsomsl, string pptmhi);
	double adeumumkrxiubr(bool cxsbygwvxg, bool omgznrtslpl, double zshqgdlajj, double xdtemlxnaswcaot, double xpfjr, bool bnrxxht, double kqdgtwzbdhv, double ipssudby, bool yrignqbqs);

protected:
	double pkuqzfxnvzvrgyl;
	string xddceqryo;
	double yxmsxytap;
	int lgcluoyyfb;
	int awjpfbklfpr;

	int mgmgobpsdqyzxbnfrxgwwr(string yxhihydqmgzchr, int nghxt, string zikmyejoi, int nhygmmnmzakde, bool bublrpch);
	void xbgubiterxcga(int wmqxmwgsha, string zuhqqugwe, string pqfxxb, bool ctriechjepyfstw, string yzzslzbykwpe, string ehbhlqdaw, bool qdacb, string hsizaavayyb, bool gqykvoqqsmdywl, int wveyafohoo);
	void bihxbtjhvttzrtnpwveqqo(bool dtxbexlt, string nrrmdvk, int vgbwxngcflqjpz);
	string omsjpblmzfgbkoepcgw(double zcbezoitt, double omxvlnicfbkf, bool edaobkkadfdzb, string pbismhqbgquna);
	string nnrstxbzbhjjr(int jrlzbcysieio, string jdleldeprz, double tnrfszd, string ptvghlgp, string ushux, bool ruxkgmlrcnhonlr, double gnhopwoi, int xcbxqicvc, int lnbme);
	double qhjnghvqjhcekrp(string czyczesu);
	bool ljihtjgmaq();
	void cchxwrawjvwwsjzspehbkp(double zludediqvyfcc, bool qigekweno, int frbtqpfnv, double cprweywbcgbpa, string ndnpblx, bool kbudd, bool epsngxzbrhzlwu);

private:
	double ybggn;

	void cdguinavboy(double dlmnfztpianaamf, string midlfjytgtxf, double iwlcg, int ajafghlqpaxyfr, double eqjisw, bool lmihodaswao, string hiwzhg, int qlabtcy, double djmnwhi);

};


void caybrvg::cdguinavboy(double dlmnfztpianaamf, string midlfjytgtxf, double iwlcg, int ajafghlqpaxyfr, double eqjisw, bool lmihodaswao, string hiwzhg, int qlabtcy, double djmnwhi) {

}

int caybrvg::mgmgobpsdqyzxbnfrxgwwr(string yxhihydqmgzchr, int nghxt, string zikmyejoi, int nhygmmnmzakde, bool bublrpch) {
	bool koxghpgigld = true;
	double xhpvhjplfckwj = 33754;
	string ylrezpyqwdyet = "slbkvrqwjawaraqqibtbrqyvzaquhmhenismtertrbvzzmvnwtanqajeenribvyxefzdnkbfzhfnuqhfqp";
	int fbagm = 3819;
	double baxesignxq = 8226;
	bool dwngwvrvdkavy = true;
	string eydgzm = "rswjzszqwzgd";
	if (8226 != 8226) {
		int dtjciap;
		for (dtjciap = 46; dtjciap > 0; dtjciap--) {
			continue;
		}
	}
	if (string("rswjzszqwzgd") != string("rswjzszqwzgd")) {
		int rtlrokytzs;
		for (rtlrokytzs = 6; rtlrokytzs > 0; rtlrokytzs--) {
			continue;
		}
	}
	if (3819 != 3819) {
		int sbwwxxwt;
		for (sbwwxxwt = 94; sbwwxxwt > 0; sbwwxxwt--) {
			continue;
		}
	}
	if (true != true) {
		int sdk;
		for (sdk = 28; sdk > 0; sdk--) {
			continue;
		}
	}
	return 29545;
}

void caybrvg::xbgubiterxcga(int wmqxmwgsha, string zuhqqugwe, string pqfxxb, bool ctriechjepyfstw, string yzzslzbykwpe, string ehbhlqdaw, bool qdacb, string hsizaavayyb, bool gqykvoqqsmdywl, int wveyafohoo) {
	int fhwkqhuqfzmot = 4476;
	string iqwijtrncwdc = "ullncnapghpvmrmttccokbqpxpcmrmabpudbrhjjtbwgqzbcypuxgdjdhhdthxejoebxyxcymxreaofplzic";
	string ohjwvzry = "jeorppuxmsjorllvlvpihkoagxeuqenwuffjxzavjrusmqnenycyvpzqzmcaytnmynihbwdwhsdigwmfhbwvqujmnzzrckucmluw";
	double nwwlxlmowhzptxi = 11882;
	string jzogcm = "wniumwqibhbouhvfcixyryrzxnmtrelxur";
	bool vljjuctjujllqr = true;
	bool xmnfpoayznjsmhr = false;
	string xkzzpijmmrbyk = "yqqhzovwmfnikqyxidrfvshesoxzjrdfuf";
	double cnbebwcybo = 33204;
	int wtexn = 2166;
	if (string("jeorppuxmsjorllvlvpihkoagxeuqenwuffjxzavjrusmqnenycyvpzqzmcaytnmynihbwdwhsdigwmfhbwvqujmnzzrckucmluw") != string("jeorppuxmsjorllvlvpihkoagxeuqenwuffjxzavjrusmqnenycyvpzqzmcaytnmynihbwdwhsdigwmfhbwvqujmnzzrckucmluw")) {
		int bro;
		for (bro = 63; bro > 0; bro--) {
			continue;
		}
	}
	if (4476 == 4476) {
		int bjyhmxnsx;
		for (bjyhmxnsx = 23; bjyhmxnsx > 0; bjyhmxnsx--) {
			continue;
		}
	}

}

void caybrvg::bihxbtjhvttzrtnpwveqqo(bool dtxbexlt, string nrrmdvk, int vgbwxngcflqjpz) {
	double jhluscdfaqhvi = 9026;
	bool aodilqihhhbh = true;
	int pphqku = 1319;
	if (9026 == 9026) {
		int vzzctwu;
		for (vzzctwu = 37; vzzctwu > 0; vzzctwu--) {
			continue;
		}
	}
	if (9026 == 9026) {
		int ka;
		for (ka = 26; ka > 0; ka--) {
			continue;
		}
	}
	if (1319 != 1319) {
		int xfxnneus;
		for (xfxnneus = 77; xfxnneus > 0; xfxnneus--) {
			continue;
		}
	}
	if (9026 != 9026) {
		int bfzoagdf;
		for (bfzoagdf = 81; bfzoagdf > 0; bfzoagdf--) {
			continue;
		}
	}
	if (1319 == 1319) {
		int debbmoz;
		for (debbmoz = 17; debbmoz > 0; debbmoz--) {
			continue;
		}
	}

}

string caybrvg::omsjpblmzfgbkoepcgw(double zcbezoitt, double omxvlnicfbkf, bool edaobkkadfdzb, string pbismhqbgquna) {
	bool ancbs = true;
	double ebwkejkbsxzqbl = 17562;
	string epvxfnrlxqepfp = "gyivydznbvfbmjhptbleanwhfqlsgpakiytshqpkvuczzfjpejstj";
	bool eeyxncjqhet = true;
	double ryyqofpzm = 35364;
	if (true == true) {
		int fw;
		for (fw = 25; fw > 0; fw--) {
			continue;
		}
	}
	if (35364 == 35364) {
		int mvfajscum;
		for (mvfajscum = 15; mvfajscum > 0; mvfajscum--) {
			continue;
		}
	}
	return string("lohvhiwdxbhxrrgiuvfk");
}

string caybrvg::nnrstxbzbhjjr(int jrlzbcysieio, string jdleldeprz, double tnrfszd, string ptvghlgp, string ushux, bool ruxkgmlrcnhonlr, double gnhopwoi, int xcbxqicvc, int lnbme) {
	int nkihgjbampldao = 2123;
	string qstrxazrprw = "puwjsvbwllfdso";
	string oxdhgaoakgjpze = "lfjhzefqh";
	string rkgtkeqpxo = "ybkmauyiglldyegwza";
	double lggystkhqeh = 53227;
	if (string("puwjsvbwllfdso") != string("puwjsvbwllfdso")) {
		int kqzmzjgxne;
		for (kqzmzjgxne = 58; kqzmzjgxne > 0; kqzmzjgxne--) {
			continue;
		}
	}
	if (53227 == 53227) {
		int auaqqbbf;
		for (auaqqbbf = 77; auaqqbbf > 0; auaqqbbf--) {
			continue;
		}
	}
	if (2123 == 2123) {
		int bxyjrhhoyb;
		for (bxyjrhhoyb = 65; bxyjrhhoyb > 0; bxyjrhhoyb--) {
			continue;
		}
	}
	return string("pbvzlbkb");
}

double caybrvg::qhjnghvqjhcekrp(string czyczesu) {
	double utjhadtgdnjnqem = 16078;
	int pfhvfcsgyhyoc = 2731;
	int tmscuwikmso = 4079;
	double arvkijejke = 37852;
	bool ltldbeppsxaakzr = true;
	bool ucypulndbbjz = true;
	int mbrgravgbvwf = 2651;
	if (37852 == 37852) {
		int samotvcl;
		for (samotvcl = 72; samotvcl > 0; samotvcl--) {
			continue;
		}
	}
	if (37852 != 37852) {
		int txebacnzy;
		for (txebacnzy = 76; txebacnzy > 0; txebacnzy--) {
			continue;
		}
	}
	return 81524;
}

bool caybrvg::ljihtjgmaq() {
	bool eglaribluonjy = true;
	bool acjugesawwau = true;
	int ehmwdj = 3520;
	double jakyc = 1621;
	if (true != true) {
		int pnlnny;
		for (pnlnny = 72; pnlnny > 0; pnlnny--) {
			continue;
		}
	}
	return false;
}

void caybrvg::cchxwrawjvwwsjzspehbkp(double zludediqvyfcc, bool qigekweno, int frbtqpfnv, double cprweywbcgbpa, string ndnpblx, bool kbudd, bool epsngxzbrhzlwu) {
	int eahqccx = 149;
	bool kpkhxom = false;
	string ecxqrtzktr = "mrgvxrkdjfxuksmqvmunbhqfcdazmvfmukwpve";
	string grlcrylcjg = "bdmoafdnvtsrndavmbobuzcdjkycrteistwtwktgmvtrnfyultmpvpasmwtkwedkjdebrptavlxwhoyocptctnhieamokva";

}

void caybrvg::pknjbjydefqcphb(int vhjgzvnvip, int fnvbyc, int gicazoijlck, string sktznssdtqbxmym, double fozwqtqsiywn, int cigemirgwo, bool ypaqnhazdnlexo, int abrxdylne, string pvoyarjxqeqtikn, bool bzeoljnlh) {
	int vcelbqgqqrjyhci = 4240;
	string zxzybxnxtslcf = "fbdmskglmpwvodxtpshqztohgmdbewxcxajxsenumkneeskskanzyqbtvgfptvktsoltfcirypnhgaamqahdrjpjkpbyussrf";
	string gtqlfb = "xpvnolsg";
	string vnzyarxfkhh = "cgjsyzgspnulrtpppvuetxjckueeevlsxbpfjzetuvxjwolhqgmjmrnrgzkbgvyanenckgpfkkneperidpoyzdrvzvgmzagni";
	int komqyojfh = 672;
	bool sbhruebemasqybp = false;
	if (string("fbdmskglmpwvodxtpshqztohgmdbewxcxajxsenumkneeskskanzyqbtvgfptvktsoltfcirypnhgaamqahdrjpjkpbyussrf") == string("fbdmskglmpwvodxtpshqztohgmdbewxcxajxsenumkneeskskanzyqbtvgfptvktsoltfcirypnhgaamqahdrjpjkpbyussrf")) {
		int fn;
		for (fn = 38; fn > 0; fn--) {
			continue;
		}
	}
	if (string("cgjsyzgspnulrtpppvuetxjckueeevlsxbpfjzetuvxjwolhqgmjmrnrgzkbgvyanenckgpfkkneperidpoyzdrvzvgmzagni") == string("cgjsyzgspnulrtpppvuetxjckueeevlsxbpfjzetuvxjwolhqgmjmrnrgzkbgvyanenckgpfkkneperidpoyzdrvzvgmzagni")) {
		int ykhbtjers;
		for (ykhbtjers = 24; ykhbtjers > 0; ykhbtjers--) {
			continue;
		}
	}
	if (false != false) {
		int msjq;
		for (msjq = 86; msjq > 0; msjq--) {
			continue;
		}
	}
	if (string("xpvnolsg") != string("xpvnolsg")) {
		int juadrz;
		for (juadrz = 74; juadrz > 0; juadrz--) {
			continue;
		}
	}

}

string caybrvg::disfgkkoyeeiuksqohwcfl(bool uakynd, bool euwbomajmtrzo, string pdbur, int sqqhptctbdh) {
	string qbmugz = "oxqarg";
	double ibjgfzcl = 10936;
	bool wiabmcswt = true;
	string kiembpcbzthvj = "ftmnsghokbsykgdsdolgkjknfojwogioftgddp";
	string gftdzqpsrzkc = "bpbwhvetdmrxarumgohmzyjcxewhouazvravgemojnmxwjvzdhiaovtwatanujzosmvrqjujuojofvraixuhchkzrvefs";
	bool uhxwhousyxvtmpy = false;
	bool omtnoss = true;
	if (10936 == 10936) {
		int pe;
		for (pe = 92; pe > 0; pe--) {
			continue;
		}
	}
	if (string("ftmnsghokbsykgdsdolgkjknfojwogioftgddp") != string("ftmnsghokbsykgdsdolgkjknfojwogioftgddp")) {
		int tptte;
		for (tptte = 32; tptte > 0; tptte--) {
			continue;
		}
	}
	if (true != true) {
		int yr;
		for (yr = 37; yr > 0; yr--) {
			continue;
		}
	}
	if (string("oxqarg") == string("oxqarg")) {
		int ubgj;
		for (ubgj = 76; ubgj > 0; ubgj--) {
			continue;
		}
	}
	return string("uyd");
}

string caybrvg::iczzxtizxcbdrwpeeqy(string emdjmkxqnd, int xcotzwyxt, double rmdopuwlkxd, bool fszfis, string soynekg) {
	int cbldwnsdnomxjx = 6782;
	bool wtaturoutgb = true;
	int lzknbxxywyhzmbi = 3317;
	string zxabydujasas = "eoiodi";
	if (string("eoiodi") != string("eoiodi")) {
		int wkl;
		for (wkl = 97; wkl > 0; wkl--) {
			continue;
		}
	}
	if (3317 != 3317) {
		int kkqltx;
		for (kkqltx = 77; kkqltx > 0; kkqltx--) {
			continue;
		}
	}
	if (true != true) {
		int giqidbhds;
		for (giqidbhds = 47; giqidbhds > 0; giqidbhds--) {
			continue;
		}
	}
	return string("qvtm");
}

string caybrvg::ukejvdrmtlydomhmt(double lxlwxvskffrwd, double cldrel, bool ragekhscgnyyrgs, int wnowxwkz, bool dmoyuyrf) {
	bool epudv = true;
	int cwchf = 402;
	if (true != true) {
		int eofnldl;
		for (eofnldl = 78; eofnldl > 0; eofnldl--) {
			continue;
		}
	}
	if (true != true) {
		int jrfdudhrn;
		for (jrfdudhrn = 26; jrfdudhrn > 0; jrfdudhrn--) {
			continue;
		}
	}
	return string("baegwdpd");
}

void caybrvg::oeiozoodla(double ohtxdeviavdt) {
	double amjzukgbkutco = 76413;
	string xcowcqkpnuh = "fswaodulqopjyuccwyfxvxgmrwruobeyopwhtktslixmnwwiimbekqvhcjdzyybszfnbcanc";
	double ovfaenbaybewmos = 19085;
	string lfzieoifx = "bwojgktpyhrgmaslwkqzthvdwiuqbjxnghpeiboggaqtapwkuxwxybmijhjix";
	string pprutzyqv = "hujxsazjwgqgxhfknizltevkfgakgjxsbgggkwjvfqiuenawgyuudgbwekneisop";
	bool cafmrpp = true;
	double rmzqxxyfanfst = 4065;
	double rachklknhtmn = 5829;
	string ohubntycan = "hlzctpmaxcuv";
	if (string("hlzctpmaxcuv") != string("hlzctpmaxcuv")) {
		int gohrdrzchv;
		for (gohrdrzchv = 17; gohrdrzchv > 0; gohrdrzchv--) {
			continue;
		}
	}
	if (string("bwojgktpyhrgmaslwkqzthvdwiuqbjxnghpeiboggaqtapwkuxwxybmijhjix") == string("bwojgktpyhrgmaslwkqzthvdwiuqbjxnghpeiboggaqtapwkuxwxybmijhjix")) {
		int hwiqie;
		for (hwiqie = 10; hwiqie > 0; hwiqie--) {
			continue;
		}
	}
	if (5829 == 5829) {
		int hyrs;
		for (hyrs = 100; hyrs > 0; hyrs--) {
			continue;
		}
	}
	if (4065 == 4065) {
		int oypq;
		for (oypq = 75; oypq > 0; oypq--) {
			continue;
		}
	}

}

string caybrvg::xnipambvianyanohrogzswye(int zrhgrpygg, double hqasbkbfpczo, bool ujjpmaku, double buvcx, string agyeyo, string teoztutuijhzq, int spvwdldnbviljog) {
	bool qeivfmoww = false;
	int vcrxuobjegogx = 70;
	bool njfaootpyw = false;
	string krgnqwovonhgtic = "kudsecyiveyamfddbesupzmmbkqwszfrhapyybtnpicitocjqkvavtdvypbodloypmhfahlewlzraqazxkuicfxqjdv";
	string qmtqny = "ktuttamgcxetlqhrqmfamdqvsxwtqkbufwlpxijewlsysdyzezgffzpfwznbyipirps";
	int tqgvykszicnh = 3498;
	bool zsbaellbbecyyw = false;
	string pnrddyqekqfys = "twpqsibiegxqnvvzsvuqwmmgtmntqbivlkp";
	if (string("twpqsibiegxqnvvzsvuqwmmgtmntqbivlkp") == string("twpqsibiegxqnvvzsvuqwmmgtmntqbivlkp")) {
		int mvrdpefry;
		for (mvrdpefry = 54; mvrdpefry > 0; mvrdpefry--) {
			continue;
		}
	}
	if (false != false) {
		int wpztui;
		for (wpztui = 15; wpztui > 0; wpztui--) {
			continue;
		}
	}
	if (3498 == 3498) {
		int gxtyssph;
		for (gxtyssph = 77; gxtyssph > 0; gxtyssph--) {
			continue;
		}
	}
	return string("vagv");
}

double caybrvg::furnqgbluowfla(int orsldmwxbe, bool jyzcyhkxmlhs, bool fjjmzriqbca, int bhrkgogy, double xuwsomsl, string pptmhi) {
	double nvsar = 24585;
	string gltbaeee = "sd";
	string ifwhkp = "rjlgevotnfofhgmqpojbfrupmqgpzjldbukveuokasqxkljydobeducdrukohrseofraorennjhqoyid";
	int hmsvckymsiog = 454;
	if (454 != 454) {
		int ou;
		for (ou = 86; ou > 0; ou--) {
			continue;
		}
	}
	if (string("rjlgevotnfofhgmqpojbfrupmqgpzjldbukveuokasqxkljydobeducdrukohrseofraorennjhqoyid") == string("rjlgevotnfofhgmqpojbfrupmqgpzjldbukveuokasqxkljydobeducdrukohrseofraorennjhqoyid")) {
		int bvn;
		for (bvn = 42; bvn > 0; bvn--) {
			continue;
		}
	}
	if (string("sd") != string("sd")) {
		int tmgiexxp;
		for (tmgiexxp = 3; tmgiexxp > 0; tmgiexxp--) {
			continue;
		}
	}
	if (string("sd") == string("sd")) {
		int sisqwrsxi;
		for (sisqwrsxi = 90; sisqwrsxi > 0; sisqwrsxi--) {
			continue;
		}
	}
	if (24585 == 24585) {
		int orozknvw;
		for (orozknvw = 28; orozknvw > 0; orozknvw--) {
			continue;
		}
	}
	return 31940;
}

double caybrvg::adeumumkrxiubr(bool cxsbygwvxg, bool omgznrtslpl, double zshqgdlajj, double xdtemlxnaswcaot, double xpfjr, bool bnrxxht, double kqdgtwzbdhv, double ipssudby, bool yrignqbqs) {
	double gegdphxlorbigl = 20880;
	string fgvzqcpat = "nhqcnbhxxtjrgeewmlpeeydhljuiwffaxqknjqbunnnbrpybzjafsyeovhribwposgeau";
	string vojgkudzct = "arjowavxtuppwqwebmnpyqfiqucmwjunfvgekwfdcqejmfeihfzeadiutbxilthbuvzcuuwpehgdqhfcwbhrptbpztfeeabsjell";
	double jthqcykzyrbf = 55764;
	double sscwjwnkremnqk = 40288;
	string kblfddfpte = "uyuvwfnqxjfedwjwettxhyqialkfwcxcqardqr";
	string oifpqfsyqht = "sfceaghaevkscdhvgcocgyoilzcbbqzmthylplmjjcqwrlvdugeobsughnqaihfzgstqonjssnnvzirznyevi";
	int kscpotfyeidvr = 5669;
	string hdzurxmyjqjaqj = "sqlsymitfxsbrlrgczgdo";
	if (string("nhqcnbhxxtjrgeewmlpeeydhljuiwffaxqknjqbunnnbrpybzjafsyeovhribwposgeau") == string("nhqcnbhxxtjrgeewmlpeeydhljuiwffaxqknjqbunnnbrpybzjafsyeovhribwposgeau")) {
		int spvbwzp;
		for (spvbwzp = 49; spvbwzp > 0; spvbwzp--) {
			continue;
		}
	}
	return 30093;
}

caybrvg::caybrvg() {
	this->pknjbjydefqcphb(3616, 1177, 2511, string("mptxrzrlbctoamrnbiexobecbrfgqzkqhfrltnmzohdovjkanmdupijgczvmbrusmduukrresy"), 72866, 1992, true, 5210, string("mkkamrtzsdfgkjeokouvepmspnvkoxsigykkxfnchtwiynxlpifcunbpjafxtrvkdywetnlxsbbxfsvmjz"), false);
	this->disfgkkoyeeiuksqohwcfl(true, true, string("zbeprgdflr"), 3732);
	this->iczzxtizxcbdrwpeeqy(string("gorbxmvvlbx"), 6975, 39040, false, string("eugaimywikwfnxpeqpwvkidjdbhjxnovgdpygqqvuhdcxglbywr"));
	this->ukejvdrmtlydomhmt(15583, 26463, true, 776, false);
	this->oeiozoodla(20906);
	this->xnipambvianyanohrogzswye(943, 3372, true, 10363, string("qqpxjucyylxorfuhghikrnbjqfmwbjpgdqvdusyszoorrvluprtrpecjpsvnnlxi"), string("jzoghidvusqmwzjp"), 909);
	this->furnqgbluowfla(5711, false, false, 1306, 20659, string("hgvtbdoaswskbmauydddqexfijbmszyycaqsftmbakawyxznsdaqvwcowwpgawpzwyiaaozyuphsitdcmjztxrgggw"));
	this->adeumumkrxiubr(false, true, 783, 31705, 48916, false, 17497, 4626, true);
	this->mgmgobpsdqyzxbnfrxgwwr(string("gksradqgkiqgaknopxllbelakvrtvkaerevcfborfbriraptrlkmscmjihiyvhvubuvyj"), 3765, string("fwwmrmfzuzzfeilfzjzazwqbgysdlmbciusmaoghxcvrxseavtjpgelvxbscphexsqes"), 2995, false);
	this->xbgubiterxcga(1022, string("rikyetpjtexchioenjeegxboxytqmkddgipwhbguwsmourzccbzawbuamrlfsxomcyughyxnrzocoxnlg"), string("manniekbafixjdioclanvykanenmeyrymdnsbinerdnvsymlgoyowxsoqvfxe"), false, string("ainr"), string("wuyejxcienavhsgjvqweleaaxesgraavclaqnootrmmfhyrjslnwxyhfcajsegwqfqetsgineldkqjljtspmuzvx"), true, string("likwofeuqecxkbnsyoosnutnmexlkkagzwkpzuqzjbxlazzehiusugzijietghtxubfpbrwqmlhsosjswoxzal"), true, 2210);
	this->bihxbtjhvttzrtnpwveqqo(false, string("kuourrahhxsksbbrdujncacgprhaqvqavuqenpvusbgcngrqehpzqcslebaffluuatsytkvzboboqefom"), 1722);
	this->omsjpblmzfgbkoepcgw(16360, 38618, true, string("uuren"));
	this->nnrstxbzbhjjr(3902, string("qulavyiihtk"), 22960, string("jecokawlhqjnatrfgqeeacfqbjmlruegebivcenzfjvydpxvhaoxoemm"), string("yaellzhdwiswtfpbutvagphjcxspouvvfjnslkqskgqnxitavhbetmiipybcjvjikpzmhcyhzjkxyvecsq"), true, 59292, 1589, 5484);
	this->qhjnghvqjhcekrp(string("vyauuwjtk"));
	this->ljihtjgmaq();
	this->cchxwrawjvwwsjzspehbkp(15714, false, 2786, 23373, string("oxxtbhipjnjuemmvfevdozwzmjrkxiehzfil"), true, false);
	this->cdguinavboy(26338, string("uhnbzgcidyuqtlzzekxdojtsfhbjwatmugodbkjtqngzjsxgphluwuxlqa"), 47924, 2448, 10997, true, string("cerdccdmndtmjcfyukiznpqypgjrwrnbqbsfmolfiooihbhqfzsdiqhvfmgpfsyl"), 23, 28445);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class woijrie {
public:
	double lobgqb;
	string vqfhxye;
	int bsnpddjrq;
	woijrie();
	double swborthqmeptphsjckfabwh(double eexwx, bool goseahochlhp, double jhreifge);
	double jrhcpvhxtd();
	bool gyuovjwdmbslhscqsypbzsu(string fsigtluicz, string qnqfh, bool pubbdwmcposut, string imgwlpddj, int hlbxfrpuuy, bool zueptrw, bool tlxinjglq, bool gxqtiiptngahoa, double ncziipbekharqtf);
	int nrrorolytm(string pmivkph, bool gqvlsguebgguw, int pdnhpjw, double eylozvyhwqd, double gpexfbifzlpczdy, string tddqevyte, double vcrseifpugmwhgv);
	bool upkhydxirpsrqgnzredbqmw(bool tueygyeswlojml, bool ruvsgco, int zfongpfkfoa, string sowxqtq);
	double dztiytnjaubxrki(string hesrhzmxtenlex, string xnuypz, int kqnazjthlfzi, int hgzxzfkfdsvxs, string amcsol, double hoqftbvejsgs, double jndmbggxdhqzdgk, int cziwxecpg, int nyddt);
	void psfbvifeijxyngeycnbui(double rzjwacvr, int evxzybyddaifbg, bool cpuvrdvozus, double dsgpksoiukrruff, double rhhmis, int bovvmeusxilruu);

protected:
	bool rmynlzbsmudaqtw;
	double cgtofwoqwirsyyh;
	bool tkresjh;

	string ylqcqkfttgrtuheltdd(bool slgurbaegyhzhmp, bool ofaynsitueb, bool qzpcaubcwzkhevy, int aarrfaubr, bool vawfujgthmhukh, string owpjubbj, string hywutkxlprmrdgz, int balmdxv, int rcibwgxttgozw, double twovksiw);
	void lrcamcxnwcpm(bool wipuxusegswvryv, string zitjnbukr);
	int ghohfxchetfcawofanuxwyctp(double fawykslicejef, double heyyrqolcugnux, bool rcrgcx, string xqwxtnrgtwwbjqy, bool mvaqzltqrl);
	int tusetmijibhlplynnyzxax(bool jipbpjgpsrzcc, int vugkvzoegyuadq, string mjtbnzmpujrcf, int afxmyaeof, double haimljmltwqw);
	double bhsvbxpfguvdyoaq(string olnyxont, double zjkuvt, double eyfyset, int fapgvc, int ovkaqrfcrc);
	void mdvizlixdhwqkuwusizduvfva(double sxfopei, double ixvogiykhnnamov, int edtqqmwhxlkiuug, string wbggueod, bool mrllemaahhephgj, double xlhvuve, string sbwav, string odtslmdlfhx);
	string xyhmbaavqqwj(string zrdbpaeeyhq);
	bool ihniwjqbke(int rzpgn, int wsxlbcuxxoqsk, string bvdjmqsxynfc, int toyjraurihykxit, double aafnbj, double umuqtqfcii, bool rvlkwtgfxqqjo, double cprzfbyjpz, double xuahywrhmov);

private:
	int fkknrugzjob;

	void oveivsbentloeplnplt(bool ucyyegdshxx, int hlyrvjyesft, bool bsobodk, bool wanmrale, string eunrdsyiybs, string sxefqientjumald, string utnvuttrjporgvk, string ygsvvq, bool sdqfawvaaq, int qqowbsrxxuhgggd);
	int qqfrpshfpaxrobnujypztke(int obxqfizloznrqor, bool mzayyaizan, double edboq, int pkkkkpscuz, bool mszfnlxslvmwp, bool tjpwjmjplj, bool mabcpkyshtbm, bool rszvynqwyrtsbpn);
	string clbpupctlw(bool burmfilzbaeofr, string sctlqpnaeeqjlg, int qwjcdse);
	double npckfvfkogvbngiyqfppt(string hhqyfqpshtgtv, string jdyqijo);
	double ubuwsaubwlcxxaceabbn(int krgskkvi, string jzzuh, int cdpslgjvwzsprq, int lsrcklfjquywoai, string bbxerzml, string gclcsbefqzsy, bool bnscqskxwsfpvgu, string nlihsekms);
	bool nvkqzskkwpgzudzsogllrz(bool acqpfcqqvgik, bool bavcfo, int swiatpep, string sklslmsgxdrio, bool aofxldoltn, string wdcldfswqbfhpig, bool qerdqvf);
	bool npsdbrjsidmmptj();
	int daofqjtzayn(bool lhqahvterq, int zinvtzij, string pmntypmjswwe, string kyhhib, double obbnifa, string ofehqcmmtrrl);
	string ntdyyyuimocysbwpu(bool ylwoxb, bool ltcziv, double hspxynxxdvvmhxl, int bagqafawbbyyuxx, double eudmdqq, bool nkdlnwb, string hrttqiwp);

};


void woijrie::oveivsbentloeplnplt(bool ucyyegdshxx, int hlyrvjyesft, bool bsobodk, bool wanmrale, string eunrdsyiybs, string sxefqientjumald, string utnvuttrjporgvk, string ygsvvq, bool sdqfawvaaq, int qqowbsrxxuhgggd) {
	double xqzpcm = 14840;
	string csutpm = "nnzcjdftsrgrlltknsuhfbgddnzonnmrhpujrv";
	double zmpkvhssgbgd = 48690;
	if (string("nnzcjdftsrgrlltknsuhfbgddnzonnmrhpujrv") == string("nnzcjdftsrgrlltknsuhfbgddnzonnmrhpujrv")) {
		int kzgsf;
		for (kzgsf = 20; kzgsf > 0; kzgsf--) {
			continue;
		}
	}
	if (string("nnzcjdftsrgrlltknsuhfbgddnzonnmrhpujrv") != string("nnzcjdftsrgrlltknsuhfbgddnzonnmrhpujrv")) {
		int exuqxxtfl;
		for (exuqxxtfl = 68; exuqxxtfl > 0; exuqxxtfl--) {
			continue;
		}
	}
	if (48690 == 48690) {
		int njkfhowno;
		for (njkfhowno = 12; njkfhowno > 0; njkfhowno--) {
			continue;
		}
	}
	if (string("nnzcjdftsrgrlltknsuhfbgddnzonnmrhpujrv") == string("nnzcjdftsrgrlltknsuhfbgddnzonnmrhpujrv")) {
		int wmagpxsbl;
		for (wmagpxsbl = 47; wmagpxsbl > 0; wmagpxsbl--) {
			continue;
		}
	}
	if (14840 != 14840) {
		int fqs;
		for (fqs = 2; fqs > 0; fqs--) {
			continue;
		}
	}

}

int woijrie::qqfrpshfpaxrobnujypztke(int obxqfizloznrqor, bool mzayyaizan, double edboq, int pkkkkpscuz, bool mszfnlxslvmwp, bool tjpwjmjplj, bool mabcpkyshtbm, bool rszvynqwyrtsbpn) {
	string qyndyvi = "mhkkvgnjiqcrkflg";
	double vcybhfktcp = 19254;
	string fmtfkfewv = "iiayjzjexjmddzlqvesgfhhnqyjsbxbxluikifyhxvdsrjoyhunxtjqbsqag";
	if (19254 != 19254) {
		int omjjebqd;
		for (omjjebqd = 79; omjjebqd > 0; omjjebqd--) {
			continue;
		}
	}
	return 28782;
}

string woijrie::clbpupctlw(bool burmfilzbaeofr, string sctlqpnaeeqjlg, int qwjcdse) {
	bool owwivd = false;
	string mrjncsmpdlfqux = "bdmqrlxcqyil";
	double wkvogjvptkmzml = 11790;
	double jfgrgl = 24703;
	double qjzulgqtoqaowgn = 26153;
	bool xthknxlpdcdscf = false;
	return string("wuzhvbatwomxypmqe");
}

double woijrie::npckfvfkogvbngiyqfppt(string hhqyfqpshtgtv, string jdyqijo) {
	bool ituav = true;
	double furolibhpcr = 2089;
	string rvbkexwgkn = "ccnhhxeavlftzowyubkvaessljrrscpfqgbwydxhszbkgxagkmxykflqpnljxlvlcfxpekzn";
	if (true == true) {
		int fpchzla;
		for (fpchzla = 91; fpchzla > 0; fpchzla--) {
			continue;
		}
	}
	if (true != true) {
		int tzyn;
		for (tzyn = 18; tzyn > 0; tzyn--) {
			continue;
		}
	}
	if (string("ccnhhxeavlftzowyubkvaessljrrscpfqgbwydxhszbkgxagkmxykflqpnljxlvlcfxpekzn") == string("ccnhhxeavlftzowyubkvaessljrrscpfqgbwydxhszbkgxagkmxykflqpnljxlvlcfxpekzn")) {
		int ifpu;
		for (ifpu = 6; ifpu > 0; ifpu--) {
			continue;
		}
	}
	if (2089 == 2089) {
		int wcqeqt;
		for (wcqeqt = 13; wcqeqt > 0; wcqeqt--) {
			continue;
		}
	}
	return 61397;
}

double woijrie::ubuwsaubwlcxxaceabbn(int krgskkvi, string jzzuh, int cdpslgjvwzsprq, int lsrcklfjquywoai, string bbxerzml, string gclcsbefqzsy, bool bnscqskxwsfpvgu, string nlihsekms) {
	double sxfjrqwtlh = 10752;
	string ssxmhrgyi = "ucafqoreqovv";
	string cfhhoqkag = "ysgwmrefqjuqapgrflrqrmlgojldrsxwqapwenocreekgivhkypqrxaizhvyoncraojzdnxcqisti";
	bool tehpna = false;
	int zhfcingkq = 5451;
	string daswmkoplmvjsjc = "xwyugwlimfratavtjsvfpizoeunxrfqgdgaftxrfxxjklbvefclusmviiq";
	if (false == false) {
		int nhclj;
		for (nhclj = 28; nhclj > 0; nhclj--) {
			continue;
		}
	}
	if (string("ysgwmrefqjuqapgrflrqrmlgojldrsxwqapwenocreekgivhkypqrxaizhvyoncraojzdnxcqisti") == string("ysgwmrefqjuqapgrflrqrmlgojldrsxwqapwenocreekgivhkypqrxaizhvyoncraojzdnxcqisti")) {
		int hyltk;
		for (hyltk = 9; hyltk > 0; hyltk--) {
			continue;
		}
	}
	if (10752 == 10752) {
		int idfiokxbc;
		for (idfiokxbc = 6; idfiokxbc > 0; idfiokxbc--) {
			continue;
		}
	}
	return 18790;
}

bool woijrie::nvkqzskkwpgzudzsogllrz(bool acqpfcqqvgik, bool bavcfo, int swiatpep, string sklslmsgxdrio, bool aofxldoltn, string wdcldfswqbfhpig, bool qerdqvf) {
	bool fchaewogafqbwn = true;
	string rsjwyccy = "huusooslwzvchqrttyobhdogjfozcsehfhkpntcosckjbbbwnjqsmxawcjfhdmpabdmbuislpccjqjywvmqmu";
	bool dvrzc = true;
	int acuaiuclwgqa = 1968;
	if (true == true) {
		int vixvdt;
		for (vixvdt = 81; vixvdt > 0; vixvdt--) {
			continue;
		}
	}
	if (string("huusooslwzvchqrttyobhdogjfozcsehfhkpntcosckjbbbwnjqsmxawcjfhdmpabdmbuislpccjqjywvmqmu") == string("huusooslwzvchqrttyobhdogjfozcsehfhkpntcosckjbbbwnjqsmxawcjfhdmpabdmbuislpccjqjywvmqmu")) {
		int rbd;
		for (rbd = 88; rbd > 0; rbd--) {
			continue;
		}
	}
	if (true != true) {
		int jjrcvsu;
		for (jjrcvsu = 100; jjrcvsu > 0; jjrcvsu--) {
			continue;
		}
	}
	if (1968 == 1968) {
		int tm;
		for (tm = 56; tm > 0; tm--) {
			continue;
		}
	}
	if (true == true) {
		int wwmfcxcby;
		for (wwmfcxcby = 97; wwmfcxcby > 0; wwmfcxcby--) {
			continue;
		}
	}
	return false;
}

bool woijrie::npsdbrjsidmmptj() {
	double aiobgfs = 83229;
	int crizwsayoifqipk = 458;
	bool bbnkforikp = true;
	string jarhuucf = "rqwjtzmokgowfczsoddouwyvhvwm";
	bool llofw = false;
	if (true == true) {
		int mermob;
		for (mermob = 77; mermob > 0; mermob--) {
			continue;
		}
	}
	if (true == true) {
		int owu;
		for (owu = 55; owu > 0; owu--) {
			continue;
		}
	}
	if (true != true) {
		int mudjzemoi;
		for (mudjzemoi = 78; mudjzemoi > 0; mudjzemoi--) {
			continue;
		}
	}
	if (83229 == 83229) {
		int irckorw;
		for (irckorw = 59; irckorw > 0; irckorw--) {
			continue;
		}
	}
	return false;
}

int woijrie::daofqjtzayn(bool lhqahvterq, int zinvtzij, string pmntypmjswwe, string kyhhib, double obbnifa, string ofehqcmmtrrl) {
	return 80739;
}

string woijrie::ntdyyyuimocysbwpu(bool ylwoxb, bool ltcziv, double hspxynxxdvvmhxl, int bagqafawbbyyuxx, double eudmdqq, bool nkdlnwb, string hrttqiwp) {
	bool pcsyyipzopft = true;
	bool xkmbmswgvo = false;
	bool hzppleqzzf = true;
	bool ddkqaanoytjs = false;
	int kslblkbnqndinoq = 1987;
	return string("g");
}

string woijrie::ylqcqkfttgrtuheltdd(bool slgurbaegyhzhmp, bool ofaynsitueb, bool qzpcaubcwzkhevy, int aarrfaubr, bool vawfujgthmhukh, string owpjubbj, string hywutkxlprmrdgz, int balmdxv, int rcibwgxttgozw, double twovksiw) {
	return string("czoplq");
}

void woijrie::lrcamcxnwcpm(bool wipuxusegswvryv, string zitjnbukr) {

}

int woijrie::ghohfxchetfcawofanuxwyctp(double fawykslicejef, double heyyrqolcugnux, bool rcrgcx, string xqwxtnrgtwwbjqy, bool mvaqzltqrl) {
	int xmeeut = 374;
	string esntdjgpvnwzygi = "heubstfdrdsddpchilyhimdptdozvlixfrkumonodvwbiitzoaqihesrswelabisorgbzwzysrp";
	int znvsbuad = 4592;
	int blozvckk = 878;
	string nuljfshpuoe = "bjsnubuyzwxbufdlcckfmkvtlqnaqvgqgtiuzfteyselmtlelic";
	if (string("heubstfdrdsddpchilyhimdptdozvlixfrkumonodvwbiitzoaqihesrswelabisorgbzwzysrp") != string("heubstfdrdsddpchilyhimdptdozvlixfrkumonodvwbiitzoaqihesrswelabisorgbzwzysrp")) {
		int syiudgs;
		for (syiudgs = 49; syiudgs > 0; syiudgs--) {
			continue;
		}
	}
	if (4592 == 4592) {
		int didslc;
		for (didslc = 28; didslc > 0; didslc--) {
			continue;
		}
	}
	if (374 == 374) {
		int bjegknfuft;
		for (bjegknfuft = 56; bjegknfuft > 0; bjegknfuft--) {
			continue;
		}
	}
	if (string("bjsnubuyzwxbufdlcckfmkvtlqnaqvgqgtiuzfteyselmtlelic") != string("bjsnubuyzwxbufdlcckfmkvtlqnaqvgqgtiuzfteyselmtlelic")) {
		int nm;
		for (nm = 34; nm > 0; nm--) {
			continue;
		}
	}
	return 24979;
}

int woijrie::tusetmijibhlplynnyzxax(bool jipbpjgpsrzcc, int vugkvzoegyuadq, string mjtbnzmpujrcf, int afxmyaeof, double haimljmltwqw) {
	string llcsf = "spntpoehdgpshwsoggmuhqlmtiouvjzhiifebzhruxgxuqrixcorcbuhitczslebbpvtdemcqslvjfqatzxxeijiofmnhlnaymz";
	bool uhhwwvy = true;
	int oxkdljilpjiuh = 1637;
	if (1637 != 1637) {
		int xd;
		for (xd = 82; xd > 0; xd--) {
			continue;
		}
	}
	if (true != true) {
		int fujzi;
		for (fujzi = 13; fujzi > 0; fujzi--) {
			continue;
		}
	}
	if (string("spntpoehdgpshwsoggmuhqlmtiouvjzhiifebzhruxgxuqrixcorcbuhitczslebbpvtdemcqslvjfqatzxxeijiofmnhlnaymz") != string("spntpoehdgpshwsoggmuhqlmtiouvjzhiifebzhruxgxuqrixcorcbuhitczslebbpvtdemcqslvjfqatzxxeijiofmnhlnaymz")) {
		int rpogirk;
		for (rpogirk = 92; rpogirk > 0; rpogirk--) {
			continue;
		}
	}
	return 31189;
}

double woijrie::bhsvbxpfguvdyoaq(string olnyxont, double zjkuvt, double eyfyset, int fapgvc, int ovkaqrfcrc) {
	int djaot = 3037;
	string umdtwbtxwvu = "aodadghnxwtppvhdalhmkdrcbjglnrd";
	string qqxpdnykqh = "oepfniknrnb";
	bool sdujjpupugecia = true;
	double nhfon = 36401;
	double yshihjvzb = 15150;
	string wmdfmonwtubixe = "wbqyjbkavpeqncjvqwtqttzhpilylngfovtqiifig";
	bool yeoaoyttmb = false;
	double ybsiip = 41271;
	if (false == false) {
		int kfpiehr;
		for (kfpiehr = 95; kfpiehr > 0; kfpiehr--) {
			continue;
		}
	}
	if (41271 == 41271) {
		int udgepst;
		for (udgepst = 82; udgepst > 0; udgepst--) {
			continue;
		}
	}
	if (string("wbqyjbkavpeqncjvqwtqttzhpilylngfovtqiifig") == string("wbqyjbkavpeqncjvqwtqttzhpilylngfovtqiifig")) {
		int uhsqtd;
		for (uhsqtd = 30; uhsqtd > 0; uhsqtd--) {
			continue;
		}
	}
	if (false == false) {
		int bwnbj;
		for (bwnbj = 8; bwnbj > 0; bwnbj--) {
			continue;
		}
	}
	if (15150 == 15150) {
		int juyy;
		for (juyy = 74; juyy > 0; juyy--) {
			continue;
		}
	}
	return 7942;
}

void woijrie::mdvizlixdhwqkuwusizduvfva(double sxfopei, double ixvogiykhnnamov, int edtqqmwhxlkiuug, string wbggueod, bool mrllemaahhephgj, double xlhvuve, string sbwav, string odtslmdlfhx) {
	string ukixpjwksm = "zjleirvjuaqtaetqavvjmdarqldrmalloovnbfqiayvlrmnuzzuzjvgbbwuxyrwztzl";
	bool hwlppvkwmki = false;
	string sqtnjogsfa = "zwkvyuziamyindoikwretlewveimtdwjncxraudcuajhoyrjnenyfpeoqhsbvxpj";
	double afnjkzp = 30572;
	double nnqxyt = 8066;
	string teokldwlfm = "";
	if (string("zwkvyuziamyindoikwretlewveimtdwjncxraudcuajhoyrjnenyfpeoqhsbvxpj") == string("zwkvyuziamyindoikwretlewveimtdwjncxraudcuajhoyrjnenyfpeoqhsbvxpj")) {
		int px;
		for (px = 21; px > 0; px--) {
			continue;
		}
	}
	if (30572 == 30572) {
		int trfetikdv;
		for (trfetikdv = 6; trfetikdv > 0; trfetikdv--) {
			continue;
		}
	}

}

string woijrie::xyhmbaavqqwj(string zrdbpaeeyhq) {
	string lefinqy = "oyultnjvdjavxwdrizrdfztdowisgdwpuohyygbttikkhvahkhzxxvpsmyfmpitpifnxqpihbpftbsbnyfc";
	string quzjpufjmvg = "ngvhtkxdwfyvzknvxctebifzesvun";
	bool prwnol = false;
	int ncnuuyhs = 467;
	double onexyvauqovfund = 41720;
	bool eivcefxylzmiotx = true;
	if (true == true) {
		int koam;
		for (koam = 75; koam > 0; koam--) {
			continue;
		}
	}
	if (false == false) {
		int ycicsrpgir;
		for (ycicsrpgir = 31; ycicsrpgir > 0; ycicsrpgir--) {
			continue;
		}
	}
	return string("obvufkjvmvztynq");
}

bool woijrie::ihniwjqbke(int rzpgn, int wsxlbcuxxoqsk, string bvdjmqsxynfc, int toyjraurihykxit, double aafnbj, double umuqtqfcii, bool rvlkwtgfxqqjo, double cprzfbyjpz, double xuahywrhmov) {
	double qigysepde = 17576;
	string wnfkfq = "iprhwztyiwmczzdnmvjoklwulmulzaneqbeacsaxhjhvmyesqpibacbkwtfz";
	int ycvnyij = 4819;
	double akztcjh = 15751;
	double hrfwl = 38188;
	return false;
}

double woijrie::swborthqmeptphsjckfabwh(double eexwx, bool goseahochlhp, double jhreifge) {
	double nrcrwyoh = 13646;
	string byrlh = "cklohlfmbtarvuhccbpqohzsbylnnirbzlpsuvgkyqgramghcgxcohcqdqxfqrrhjeyagynpbstwwhcxgzjdbzdtoguk";
	double nrjvyfqji = 79995;
	if (79995 == 79995) {
		int vxsedozwhc;
		for (vxsedozwhc = 96; vxsedozwhc > 0; vxsedozwhc--) {
			continue;
		}
	}
	return 24322;
}

double woijrie::jrhcpvhxtd() {
	int gptrsnjerhzrwl = 1617;
	string yqgnaoplcqhzmx = "wytafcazdvfoytkacqicgxjuwjyxxuklkbklitsnvojalwbtcpbqmabholfgeaghmwzsolkecbwboyhtphmmrvlcsiyhhs";
	string uaxjhyjkfkrkkk = "ozacoqowcsdyndzkxkfnhkppvjnpmvohpulosqjbcujpdmfecoqhbwoyucswqgfbvxvmpmspgjjeenkgigv";
	bool byxpybvxwi = false;
	bool gctzwp = true;
	int rejkha = 448;
	double zigyqiw = 5873;
	bool ulbgrms = true;
	if (448 != 448) {
		int dfzju;
		for (dfzju = 3; dfzju > 0; dfzju--) {
			continue;
		}
	}
	if (string("wytafcazdvfoytkacqicgxjuwjyxxuklkbklitsnvojalwbtcpbqmabholfgeaghmwzsolkecbwboyhtphmmrvlcsiyhhs") == string("wytafcazdvfoytkacqicgxjuwjyxxuklkbklitsnvojalwbtcpbqmabholfgeaghmwzsolkecbwboyhtphmmrvlcsiyhhs")) {
		int ffxbebqxs;
		for (ffxbebqxs = 79; ffxbebqxs > 0; ffxbebqxs--) {
			continue;
		}
	}
	return 27681;
}

bool woijrie::gyuovjwdmbslhscqsypbzsu(string fsigtluicz, string qnqfh, bool pubbdwmcposut, string imgwlpddj, int hlbxfrpuuy, bool zueptrw, bool tlxinjglq, bool gxqtiiptngahoa, double ncziipbekharqtf) {
	return true;
}

int woijrie::nrrorolytm(string pmivkph, bool gqvlsguebgguw, int pdnhpjw, double eylozvyhwqd, double gpexfbifzlpczdy, string tddqevyte, double vcrseifpugmwhgv) {
	int zdgxkfzsb = 714;
	string lgkxgg = "rvfdhyqljatrqvjofxrreffwqiqmyrzpchp";
	int ddjbmrpymsade = 1041;
	string rqldaamid = "zebrvnxwcbutmwwnbwwwxd";
	int bywaokgib = 6487;
	bool mjgxyn = false;
	int mxgxsmerirk = 2592;
	int qcpsnicvw = 1526;
	if (false != false) {
		int qgjz;
		for (qgjz = 61; qgjz > 0; qgjz--) {
			continue;
		}
	}
	if (string("rvfdhyqljatrqvjofxrreffwqiqmyrzpchp") == string("rvfdhyqljatrqvjofxrreffwqiqmyrzpchp")) {
		int nyyjub;
		for (nyyjub = 83; nyyjub > 0; nyyjub--) {
			continue;
		}
	}
	if (1041 != 1041) {
		int kmrpwe;
		for (kmrpwe = 73; kmrpwe > 0; kmrpwe--) {
			continue;
		}
	}
	return 56989;
}

bool woijrie::upkhydxirpsrqgnzredbqmw(bool tueygyeswlojml, bool ruvsgco, int zfongpfkfoa, string sowxqtq) {
	double yvmxuzqhcerr = 3591;
	bool kxbjrybihhg = true;
	int hktoxbcdaa = 693;
	bool blyhhvykajaf = true;
	bool zkwxslglrkfv = false;
	if (true == true) {
		int tlcd;
		for (tlcd = 43; tlcd > 0; tlcd--) {
			continue;
		}
	}
	if (true == true) {
		int cmt;
		for (cmt = 95; cmt > 0; cmt--) {
			continue;
		}
	}
	return true;
}

double woijrie::dztiytnjaubxrki(string hesrhzmxtenlex, string xnuypz, int kqnazjthlfzi, int hgzxzfkfdsvxs, string amcsol, double hoqftbvejsgs, double jndmbggxdhqzdgk, int cziwxecpg, int nyddt) {
	string eqrvniryuujfuvo = "jdxzekzoppkuzoiwoknnckhpiobmlzxjwozmadfcygmvfqbacdeienisd";
	string rdzcjmh = "lkfcwdscpjdbxmkgzovwxcsxubqayftalvrnwd";
	double azxapjdwlxgz = 58044;
	bool jtsrqqflwnkmf = true;
	bool ajdmiwuvvsagpyh = false;
	bool zjugceba = true;
	return 11710;
}

void woijrie::psfbvifeijxyngeycnbui(double rzjwacvr, int evxzybyddaifbg, bool cpuvrdvozus, double dsgpksoiukrruff, double rhhmis, int bovvmeusxilruu) {

}

woijrie::woijrie() {
	this->swborthqmeptphsjckfabwh(34678, false, 34341);
	this->jrhcpvhxtd();
	this->gyuovjwdmbslhscqsypbzsu(string("hbvetrivlrqszwpcmqjmsdvclnnmhpncojehmrccdyczvfkfvopzkpyfemdtvsawrcdmnxft"), string("sxkdxe"), true, string("hoglocqgmmdlezcpfnmjexnlobjuzbniyfmakmuvbfkfqollpcujutahoqxupczcjj"), 2507, false, false, false, 4945);
	this->nrrorolytm(string("spiwqiwrnqftkjdqluqchbalfalcqcwedykbpphuy"), true, 5132, 10526, 7849, string("idgmdyqncpjjffkyaxactonptsbhrdvtvxx"), 42189);
	this->upkhydxirpsrqgnzredbqmw(true, true, 2975, string("znxalhuoynynnttqdyzdmsgfoywnvrjteryaqjeilshazgiqopygw"));
	this->dztiytnjaubxrki(string("mndkysgmmjpxobxiclnsgstucsxzegsrqavkgcymlw"), string("bbswvaeybbtjbbsrrxydcsjwqikjphfmldlrgclodgnauumqwxax"), 2706, 2345, string("kispqllilrguaqcbzdtaoxiiikzouejcrsjiqddfunifwndwlswpwjfnpzikhaykgohpolv"), 26985, 7094, 4622, 557);
	this->psfbvifeijxyngeycnbui(16792, 6469, false, 75707, 75213, 361);
	this->ylqcqkfttgrtuheltdd(false, false, true, 2716, true, string("htsiuxeanhhjxbvnebegrktvpvzblzkiopeezpwtbuxfxpjkluffmqytzprzxdvcwijhqsbqrdvwklxpselbyj"), string("phgaxscwwhybszzwquclalsxxjroegbglvozzysodlknbjfmsyztscqlcgtmwlazzuabsiiirzricawp"), 3772, 3381, 28815);
	this->lrcamcxnwcpm(true, string("jgeqeesszkjltcpkkbtbolxzcyaogd"));
	this->ghohfxchetfcawofanuxwyctp(5073, 34505, true, string("znhnkaqfkolfancnltzvsxqcrblmwekpfchbcxlpwzpn"), false);
	this->tusetmijibhlplynnyzxax(true, 3642, string("lcfjuuzxzpptfpinjydkmgitavee"), 6471, 47454);
	this->bhsvbxpfguvdyoaq(string("ig"), 26908, 22703, 1673, 2009);
	this->mdvizlixdhwqkuwusizduvfva(6946, 15505, 1035, string("nvjyxwblyzzztclvlqbzilhhijrd"), true, 45335, string("fyopvoxgrqcnkmmbjzsjxufyudbxvrshmvalhqbgbehbswhueykkholglbnwzbnejp"), string("eflaaxbjplubcnlxypykpmliwptfpztimvvpuqtaylxlhkwmwssaox"));
	this->xyhmbaavqqwj(string("rdylcadsxhl"));
	this->ihniwjqbke(2725, 77, string("vjgeyy"), 1301, 21992, 53895, false, 16990, 9080);
	this->oveivsbentloeplnplt(true, 2577, false, false, string("endmodubccwprxaspniuunslaafmfzlgohthrrkfkgsazdwdliokhcdambnauhzq"), string("fmglwqm"), string("sc"), string("hrdtpuphvckmwerhndqjkskjqtnbhykowqyokticduhxraiiobbsftefpdxlozhqjmaoqozclabnffukveuiuznzdjrvc"), true, 1639);
	this->qqfrpshfpaxrobnujypztke(2115, true, 91847, 691, true, true, false, false);
	this->clbpupctlw(false, string("tvbxm"), 2964);
	this->npckfvfkogvbngiyqfppt(string("gthsvqmwahstoqhbljswtdeuvjnpujjcgqksmneyteaveodjfbfssqqyhovfevalrcueua"), string("ohgwedgophhxkvmxtubxgnouriektuacgpthlnwuvpudcdchydufckngacnnre"));
	this->ubuwsaubwlcxxaceabbn(6991, string("pksweeemtobduadyvrxefasqfeoqumiuohzqshxrxctsqrhxisybdwzqfzxdmjtwibeudthw"), 4542, 4346, string("ooxuufjzwgdsyncyhwjfvoiojr"), string("ycajczexopjypoomzzaagpkpgcvqpjuodppzxwvovneloxsgbgvsuexzrbtohxzkvmwwpqu"), true, string("krevjdxubsnbybiorphdxbsuw"));
	this->nvkqzskkwpgzudzsogllrz(false, true, 5594, string("ixyrputwvoldsbcpmsuliplgfbtnxepnetlgubxyaacnshnomhbcavkmozglhfxvkusoublhzlbkrqfketh"), false, string("qadocwmsmxfzbsuctlardfeokzbmoqwiz"), true);
	this->npsdbrjsidmmptj();
	this->daofqjtzayn(true, 7811, string("kftgwpiquujebamdbqicvjcdedjztceixjdjdsnldwjglnjsuydnrzpxnnocyjkkpcwnqzwsvqiwnrbebpqvrgugxoexzui"), string("awzl"), 8042, string("vfdmgrvpnxaytjzkikceothbzpwjmnqxfnalbjkouohvbcnfvmcmlqmrxstjfyfniakxmegnoadyjuazmlqfkidcnhntjaimhloa"));
	this->ntdyyyuimocysbwpu(false, false, 53976, 2475, 31559, false, string("jhubycwinrrrselmjvcy"));
}
