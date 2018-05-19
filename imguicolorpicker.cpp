// [src] https://github.com/ocornut/imgui/issues/346
// v2.36
// TODO: try to integrate properly within ColorEdit4()
// TODO: expose size (how?)
// TODO: expose ID in api?
// TODO: massage, cleanup

#include "ImGui\imgui.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui\imgui_internal.h" // ImSaturate

#define IM_F32_TO_INT8(_VAL) ((int)((_VAL) * 255.0f + 0.5f))


static const char*  FindTextDisplayEnd(const char* text, const char* text_end = NULL) {
	const char* text_display_end = text;
	if (!text_end)
		text_end = (const char*)-1;

	while (text_display_end < text_end && *text_display_end != '\0' && (text_display_end[0] != '#' || text_display_end[1] != '#'))
		text_display_end++;
	return text_display_end;
}

IMGUI_API bool ImGui::MyColorEdit3(const char* label, float col[3], ImGuiColorEditFlags flags) {
	float col4[4] = { col[0], col[1], col[2], 1.0f };
	if (!MyColorEdit4(label, col4, flags & ~ImGuiColorEditFlags_Alpha))
		return false;
	col[0] = col4[0]; col[1] = col4[1]; col[2] = col4[2];
	return true;
}


// Edit colors components (each component in 0.0f..1.0f range)
// Click on colored square to open a color picker (unless ImGuiColorEditFlags_NoPicker is set). Use CTRL-Click to input value and TAB to go to next item.
IMGUI_API bool ImGui::MyColorEdit4(const char* label, float col[4], ImGuiColorEditFlags flags) {
	ImGuiWindow* window = GetCurrentWindow();
	if (window->SkipItems)
		return false;

	ImGuiContext& g = *GImGui;
	const ImGuiStyle& style = g.Style;
	const ImGuiID id = window->GetID(label);
	const float w_full = CalcItemWidth();
	const float square_sz_with_spacing = (flags & ImGuiColorEditFlags_NoColorSquare) ? 0.0f : (g.FontSize + style.FramePadding.y * 2.0f + style.ItemInnerSpacing.x);

	// If no mode is specified, defaults to RGB
	if (!(flags & ImGuiColorEditFlags_ModeMask_))
		flags |= ImGuiColorEditFlags_RGB;

	// If we're not showing any slider there's no point in querying color mode, nor showing the options menu, nor doing any HSV conversions
	if (flags & ImGuiColorEditFlags_NoSliders)
		flags = (flags & (~ImGuiColorEditFlags_ModeMask_)) | ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_NoOptions;

	// Read back edit mode from persistent storage
	if (!(flags & ImGuiColorEditFlags_NoOptions))
		flags = (flags & (~ImGuiColorEditFlags_ModeMask_)) | (g.ColorEditModeStorage.GetInt(id, (flags & ImGuiColorEditFlags_ModeMask_)) & ImGuiColorEditFlags_ModeMask_);

	// Check that exactly one of RGB/HSV/HEX is set
	//IM_ASSERT(ImIsPowerOfTwo((int)(flags & ImGuiColorEditFlags_ModeMask_))); // 

	float f[4] = { col[0], col[1], col[2], col[3] };
	if (flags & ImGuiColorEditFlags_HSV)
		ImGui::ColorConvertRGBtoHSV(f[0], f[1], f[2], f[0], f[1], f[2]);

	int i[4] = { IM_F32_TO_INT8(f[0]), IM_F32_TO_INT8(f[1]), IM_F32_TO_INT8(f[2]), IM_F32_TO_INT8(f[3]) };

	bool alpha = (flags & ImGuiColorEditFlags_Alpha) != 0;
	bool value_changed = false;
	int components = alpha ? 4 : 3;

	ImGui::BeginGroup();
	ImGui::PushID(label);

	if ((flags & (ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_HSV)) != 0 && (flags & ImGuiColorEditFlags_NoSliders) == 0) {
		// RGB/HSV 0..255 Sliders
		const float w_items_all = w_full - square_sz_with_spacing;
		const float w_item_one = ImMax(1.0f, (float)(int)((w_items_all - (style.ItemInnerSpacing.x) * (components - 1)) / (float)components));
		const float w_item_last = ImMax(1.0f, (float)(int)(w_items_all - (w_item_one + style.ItemInnerSpacing.x) * (components - 1)));

		const bool hide_prefix = (w_item_one <= CalcTextSize("M:999").x);
		const char* ids[4] = { "##X", "##Y", "##Z", "##W" };
		const char* fmt_table[3][4] =
		{
			{ "%3.0f",   "%3.0f",   "%3.0f",   "%3.0f" }, // Short display
			{ "R:%3.0f", "G:%3.0f", "B:%3.0f", "A:%3.0f" }, // Long display for RGBA
			{ "H:%3.0f", "S:%3.0f", "V:%3.0f", "A:%3.0f" }  // Long display for HSVV
		};
		const char** fmt = hide_prefix ? fmt_table[0] : (flags & ImGuiColorEditFlags_HSV) ? fmt_table[2] : fmt_table[1];

		ImGui::PushItemWidth(w_item_one);
		for (int n = 0; n < components; n++) {
			if (n > 0)
				ImGui::SameLine(0, style.ItemInnerSpacing.x);
			if (n + 1 == components)
				ImGui::PushItemWidth(w_item_last);
			value_changed |= ImGui::DragInt(ids[n], &i[n], 1.0f, 0, 255, fmt[n]);
		}
		ImGui::PopItemWidth();
		ImGui::PopItemWidth();
	}
	else if ((flags & ImGuiColorEditFlags_HEX) != 0 && (flags & ImGuiColorEditFlags_NoSliders) == 0) {
		// RGB Hexadecimal Input
		const float w_slider_all = w_full - square_sz_with_spacing;
		char buf[64];
		if (alpha)
			ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X%02X", i[0], i[1], i[2], i[3]);
		else
			ImFormatString(buf, IM_ARRAYSIZE(buf), "#%02X%02X%02X", i[0], i[1], i[2]);
		ImGui::PushItemWidth(w_slider_all);
		if (ImGui::InputText("##Text", buf, IM_ARRAYSIZE(buf), ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase)) {
			value_changed |= true;
			char* p = buf;
			while (*p == '#' || ImCharIsSpace(*p))
				p++;
			i[0] = i[1] = i[2] = i[3] = 0;
			if (alpha)
				sscanf_s(p, "%02X%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2], (unsigned int*)&i[3]); // Treat at unsigned (%X is unsigned)
			else
				sscanf_s(p, "%02X%02X%02X", (unsigned int*)&i[0], (unsigned int*)&i[1], (unsigned int*)&i[2]);
		}
		ImGui::PopItemWidth();
	}

	const char* label_display_end = FindTextDisplayEnd(label);

	bool picker_active = false;
	if (!(flags & ImGuiColorEditFlags_NoColorSquare)) {
		if (!(flags & ImGuiColorEditFlags_NoSliders))
			ImGui::SameLine(0, style.ItemInnerSpacing.x);

		const ImVec4 col_display(col[0], col[1], col[2], 1.0f);
		if (ImGui::ColorButton(col_display)) {
			if (!(flags & ImGuiColorEditFlags_NoPicker)) {
				ImGui::OpenPopup("picker");
				ImGui::SetNextWindowPos(window->DC.LastItemRect.GetBL() + ImVec2(-1, style.ItemSpacing.y));
			}
		}
		else if (!(flags & ImGuiColorEditFlags_NoOptions) && ImGui::IsItemHovered() && ImGui::IsMouseClicked(1)) {
			ImGui::OpenPopup("context");
		}

		if (ImGui::BeginPopup("picker")) {
			picker_active = true;
			if (label != label_display_end)
				ImGui::TextUnformatted(label, label_display_end);
			ImGui::PushItemWidth(256.0f + (alpha ? 2 : 1) * (style.ItemInnerSpacing.x));
			value_changed |= ImGui::MyColorPicker4("##picker", col, (flags & ImGuiColorEditFlags_Alpha) | (ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_HSV | ImGuiColorEditFlags_HEX));
			ImGui::PopItemWidth();
			ImGui::EndPopup();
		}
		if (!(flags & ImGuiColorEditFlags_NoOptions) && ImGui::BeginPopup("context")) {
			// FIXME-LOCALIZATION
			if (ImGui::MenuItem("Edit as RGB", NULL, (flags & ImGuiColorEditFlags_RGB) ? 1 : 0)) g.ColorEditModeStorage.SetInt(id, (int)(ImGuiColorEditFlags_RGB));
			if (ImGui::MenuItem("Edit as HSV", NULL, (flags & ImGuiColorEditFlags_HSV) ? 1 : 0)) g.ColorEditModeStorage.SetInt(id, (int)(ImGuiColorEditFlags_HSV));
			if (ImGui::MenuItem("Edit as Hexadecimal", NULL, (flags & ImGuiColorEditFlags_HEX) ? 1 : 0)) g.ColorEditModeStorage.SetInt(id, (int)(ImGuiColorEditFlags_HEX));
			ImGui::EndPopup();
		}

		// Recreate our own tooltip over's ColorButton() one because we want to display correct alpha here
		//if (ImGui::IsItemHovered())
		//	ImGui::SetTooltip("Color:\n(%.2f,%.2f,%.2f,%.2f)\n#%02X%02X%02X%02X", col[0], col[1], col[2], col[3], IM_F32_TO_INT8(col[0]), IM_F32_TO_INT8(col[1]), IM_F32_TO_INT8(col[2]), IM_F32_TO_INT8(col[3]));
		//ImGui::SetTooltip("");
	}

	if (label != label_display_end) {
		ImGui::SameLine(0, style.ItemInnerSpacing.x);
		ImGui::TextUnformatted(label, label_display_end);
	}

	// Convert back
	if (!picker_active) {
		for (int n = 0; n < 4; n++)
			f[n] = i[n] / 255.0f;
		if (flags & ImGuiColorEditFlags_HSV)
			ImGui::ColorConvertHSVtoRGB(f[0], f[1], f[2], f[0], f[1], f[2]);
		if (value_changed) {
			col[0] = f[0];
			col[1] = f[1];
			col[2] = f[2];
			if (alpha)
				col[3] = f[3];
		}
	}

	ImGui::PopID();
	ImGui::EndGroup();

	return value_changed;
}

IMGUI_API bool ImGui::MyColorPicker3(const char* label, float col[3], ImGuiColorEditFlags flags) {
	float col4[4] = { col[0], col[1], col[2], 1.0f };
	if (!MyColorPicker4(label, col4, flags & ~ImGuiColorEditFlags_Alpha))
		return false;
	col[0] = col4[1]; col[1] = col4[1]; col[2] = col4[2];
	return true;
}

// ColorPicker v2.50 WIP 
// see https://github.com/ocornut/imgui/issues/346
// TODO: Missing color square
// TODO: English strings in context menu (see FIXME-LOCALIZATION)

bool ImGui::MyColorPicker4(const char* label, float col[4], ImGuiColorEditFlags flags) {
	ImGuiIO& io = ImGui::GetIO();
	ImGuiStyle& style = ImGui::GetStyle();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	ImGui::PushID(label);
	ImGui::BeginGroup();

	// Setup
	bool alpha = (flags & ImGuiColorEditFlags_Alpha) != 0;
	ImVec2 picker_pos = ImGui::GetCursorScreenPos();
	float bars_width = ImGui::GetWindowFontSize() * 1.0f;                                                           // Arbitrary smallish width of Hue/Alpha picking bars
	float sv_picker_size = ImMax(bars_width * 2, ImGui::CalcItemWidth() - (alpha ? 2 : 1) * (bars_width + style.ItemInnerSpacing.x)); // Saturation/Value picking box
	float bar0_pos_x = picker_pos.x + sv_picker_size + style.ItemInnerSpacing.x;
	float bar1_pos_x = bar0_pos_x + bars_width + style.ItemInnerSpacing.x;

	float H, S, V;
	ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], H, S, V);

	// Color matrix logic
	bool value_changed = false, hsv_changed = false;
	//ImGui::BeginGroup();
	ImGui::InvisibleButton("sv", ImVec2(sv_picker_size, sv_picker_size));
	if (ImGui::IsItemActive()) {
		S = ImSaturate((io.MousePos.x - picker_pos.x) / (sv_picker_size - 1));
		V = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
		value_changed = hsv_changed = true;
	}

	// Hue bar logic
	ImGui::SetCursorScreenPos(ImVec2(bar0_pos_x, picker_pos.y));
	ImGui::InvisibleButton("hue", ImVec2(bars_width, sv_picker_size));
	if (ImGui::IsItemActive()) {
		H = ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
		value_changed = hsv_changed = true;
	}

	// Alpha bar logic
	if (alpha) {
		ImGui::SetCursorScreenPos(ImVec2(bar1_pos_x, picker_pos.y));
		ImGui::InvisibleButton("alpha", ImVec2(bars_width, sv_picker_size));
		if (ImGui::IsItemActive()) {
			col[3] = 1.0f - ImSaturate((io.MousePos.y - picker_pos.y) / (sv_picker_size - 1));
			value_changed = true;
		}
	}

	const char* label_display_end = FindTextDisplayEnd(label);
	if (label != label_display_end) {
		ImGui::SameLine(0, style.ItemInnerSpacing.x);
		ImGui::TextUnformatted(label, label_display_end);
	}

	// Convert back color to RGB
	if (hsv_changed)
		ImGui::ColorConvertHSVtoRGB(H >= 1.0f ? H - 10 * 1e-6f : H, S > 0.0f ? S : 10 * 1e-6f, V > 0.0f ? V : 1e-6f, col[0], col[1], col[2]);

	// R,G,B and H,S,V slider color editor
	if (!(flags & ImGuiColorEditFlags_NoSliders)) {
		if ((flags & ImGuiColorEditFlags_ModeMask_) == 0)
			flags = ImGuiColorEditFlags_RGB | ImGuiColorEditFlags_HSV | ImGuiColorEditFlags_HEX;
		ImGui::PushItemWidth((alpha ? bar1_pos_x : bar0_pos_x) + bars_width - picker_pos.x);
		ImGuiColorEditFlags sub_flags = (alpha ? ImGuiColorEditFlags_Alpha : 0) | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoOptions | ImGuiColorEditFlags_NoColorSquare;
		//if (flags & ImGuiColorEditFlags_RGB)
		//	value_changed |= ImGui::MyColorEdit4("##rgb", col, sub_flags | ImGuiColorEditFlags_RGB);
		//if (flags & ImGuiColorEditFlags_HSV)
		//	value_changed |= ImGui::MyColorEdit4("##hsv", col, sub_flags | ImGuiColorEditFlags_HSV);
		//if (flags & ImGuiColorEditFlags_HEX)
		//	value_changed |= ImGui::MyColorEdit4("##hex", col, sub_flags | ImGuiColorEditFlags_HEX);
		ImGui::PopItemWidth();
	}

	// Try to cancel hue wrap (after ColorEdit), if any
	if (value_changed) {
		float new_H, new_S, new_V;
		ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], new_H, new_S, new_V);
		if (new_H <= 0 && H > 0) {
			if (new_V <= 0 && V != new_V)
				ImGui::ColorConvertHSVtoRGB(H, S, new_V <= 0 ? V * 0.5f : new_V, col[0], col[1], col[2]);
			else if (new_S <= 0)
				ImGui::ColorConvertHSVtoRGB(H, new_S <= 0 ? S * 0.5f : new_S, new_V, col[0], col[1], col[2]);
		}
	}

	// Render hue bar
	ImVec4 hue_color_f(1, 1, 1, 1);
	ImGui::ColorConvertHSVtoRGB(H, 1, 1, hue_color_f.x, hue_color_f.y, hue_color_f.z);
	ImU32 hue_colors[] = { IM_COL32(255,0,0,255), IM_COL32(255,255,0,255), IM_COL32(0,255,0,255), IM_COL32(0,255,255,255), IM_COL32(0,0,255,255), IM_COL32(255,0,255,255), IM_COL32(255,0,0,255) };
	for (int i = 0; i < 6; ++i) {
		draw_list->AddRectFilledMultiColor(
			ImVec2(bar0_pos_x, picker_pos.y + i * (sv_picker_size / 6)),
			ImVec2(bar0_pos_x + bars_width, picker_pos.y + (i + 1) * (sv_picker_size / 6)),
			hue_colors[i], hue_colors[i], hue_colors[i + 1], hue_colors[i + 1]);
	}
	float bar0_line_y = (float)(int)(picker_pos.y + H * sv_picker_size + 0.5f);
	draw_list->AddLine(ImVec2(bar0_pos_x - 1, bar0_line_y), ImVec2(bar0_pos_x + bars_width + 1, bar0_line_y), IM_COL32_WHITE);

	// Render alpha bar
	if (alpha) {
		float alpha = ImSaturate(col[3]);
		float bar1_line_y = (float)(int)(picker_pos.y + (1.0f - alpha) * sv_picker_size + 0.5f);
		draw_list->AddRectFilledMultiColor(ImVec2(bar1_pos_x, picker_pos.y), ImVec2(bar1_pos_x + bars_width, picker_pos.y + sv_picker_size), IM_COL32_WHITE, IM_COL32_WHITE, IM_COL32_BLACK, IM_COL32_BLACK);
		draw_list->AddLine(ImVec2(bar1_pos_x - 1, bar1_line_y), ImVec2(bar1_pos_x + bars_width + 1, bar1_line_y), IM_COL32_WHITE);
	}

	// Render color matrix
	ImU32 hue_color32 = ImGui::ColorConvertFloat4ToU32(hue_color_f);
	draw_list->AddRectFilledMultiColor(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size), IM_COL32_WHITE, hue_color32, hue_color32, IM_COL32_WHITE);
	draw_list->AddRectFilledMultiColor(picker_pos, picker_pos + ImVec2(sv_picker_size, sv_picker_size), IM_COL32_BLACK_TRANS, IM_COL32_BLACK_TRANS, IM_COL32_BLACK, IM_COL32_BLACK);

	// Render cross-hair
	const float CROSSHAIR_SIZE = 7.0f;
	ImVec2 p((float)(int)(picker_pos.x + S * sv_picker_size + 0.5f), (float)(int)(picker_pos.y + (1 - V) * sv_picker_size + 0.5f));
	draw_list->AddLine(ImVec2(p.x - CROSSHAIR_SIZE, p.y), ImVec2(p.x - 2, p.y), IM_COL32_WHITE);
	draw_list->AddLine(ImVec2(p.x + CROSSHAIR_SIZE, p.y), ImVec2(p.x + 2, p.y), IM_COL32_WHITE);
	draw_list->AddLine(ImVec2(p.x, p.y + CROSSHAIR_SIZE), ImVec2(p.x, p.y + 2), IM_COL32_WHITE);
	draw_list->AddLine(ImVec2(p.x, p.y - CROSSHAIR_SIZE), ImVec2(p.x, p.y - 2), IM_COL32_WHITE);
	ImGui::EndGroup();
	ImGui::PopID();

	return value_changed;
}

namespace junk4417638 {
	void junk6727907() {
		return;
		float e8veqq;
		float olqqtm;
		float udlgk;
		float jd41jk;
		float qjdrrl;
		float zsd4op;
		float keaanf;
		float t6shsp;
		float y2wezo;
		float r2mspd;
		for (int x7tvcv = 0; x7tvcv > 100; x7tvcv++)
		{
			jd41jk = 10218091.7047;
		}
		e8veqq = 2072690.6043;
		for (int frath = 0; frath > 100; frath++)
		{
			e8veqq = 3748534.4836;
		}
		qjdrrl = 10289243.9373;
		for (int gosmfa = 0; gosmfa > 100; gosmfa++)
		{
			olqqtm = 7176134.9859;
		}
		e8veqq = 7904342.3459;
		keaanf = 8111798.1053;
		while (r2mspd == 5222646.7244)
		{
			r2mspd = 3546805.2990;
		}
		t6shsp = 7369592.7743;
		if (jd41jk == 5075240.8878)
			jd41jk = 10447965.5551;
		y2wezo = 3012614.5303;
		if (e8veqq == 8135944.6559)
			e8veqq = 774180.6074;
		qjdrrl = 4066221.8490;
		if (keaanf == 7022984.2503)
			keaanf = 6273047.8725;
		udlgk = 2975235.7108;
		for (int d7nfxp = 0; d7nfxp > 100; d7nfxp++)
		{
			jd41jk = 2784085.2743;
		}
		olqqtm = 2772372.7184;
		for (int s8fvub = 0; s8fvub > 100; s8fvub++)
		{
			y2wezo = 4656907.1644;
		}
		e8veqq = 1328567.7026;
	}
	void junk8699754() {
		return;
		float fwycmj;
		float pcun2;
		float oz45ad;
		float jlwuuv;
		float bgqose;
		float rv7cxn;
		float nclnwo;
		float u6eb8q;
		float c4htfw;
		float ltd4y5;
		jlwuuv = 2670409.0892;
		nclnwo = 1464561.1559;
		while (c4htfw == 3714657.5241)
		{
			c4htfw = 5920003.6104;
		}
		oz45ad = 2111226.6906;
		for (int jmcu78 = 0; jmcu78 > 100; jmcu78++)
		{
			bgqose = 1499920.7637;
		}
		u6eb8q = 8000972.0149;
		if (nclnwo == 5753974.8557)
			nclnwo = 1054696.4322;
		pcun2 = 3337132.9817;
		for (int k98mcp = 0; k98mcp > 100; k98mcp++)
		{
			ltd4y5 = 2683203.1749;
		}
		nclnwo = 1045637.6471;
		if (jlwuuv == 3694878.2421)
			jlwuuv = 3712194.0755;
		fwycmj = 10251116.3937;
		while (u6eb8q == 1692116.6120)
		{
			u6eb8q = 2612918.1760;
		}
		bgqose = 9042986.0797;
		for (int hs4am = 0; hs4am > 100; hs4am++)
		{
			oz45ad = 1246645.7485;
		}
		oz45ad = 1698165.0079;
		for (int s2psa = 0; s2psa > 100; s2psa++)
		{
			nclnwo = 7766932.2092;
		}
		oz45ad = 4158579.8718;
	}
	void junk7965053() {
		return;
		float xko5cf;
		float wm5jf;
		float bi69k;
		float cgl93q;
		float om4d04r;
		float q3wxha;
		float jzep8;
		float hbmu7k;
		float i5c37q;
		float d4lm43;
		while (jzep8 == 569165.7396)
		{
			jzep8 = 3888442.1440;
		}
		q3wxha = 10023438.4631;
		if (xko5cf == 4766153.8049)
			xko5cf = 2237265.4233;
		q3wxha = 5272460.0591;
		q3wxha = 1562955.1536;
		while (om4d04r == 1410179.5196)
		{
			om4d04r = 1406133.1441;
		}
		hbmu7k = 2672201.6962;
		if (bi69k == 8572684.7751)
			bi69k = 5969490.1500;
		xko5cf = 2178405.4724;
		while (om4d04r == 1732065.7646)
		{
			om4d04r = 348299.5973;
		}
		om4d04r = 1852981.5722;
		if (bi69k == 1831292.3635)
			bi69k = 6851814.6764;
		hbmu7k = 8364627.5412;
		if (wm5jf == 6760888.1063)
			wm5jf = 2851234.2799;
		hbmu7k = 2240430.6996;
		cgl93q = 4118898.6403;
		if (wm5jf == 4894227.2226)
			wm5jf = 8578578.1731;
		jzep8 = 2003082.5262;
	}
	void junk2389599() {
		return;
		float q12r8f;
		float nr5xkf;
		float vn15xm;
		float ny5h6e;
		float pa64ni;
		float hsv989;
		float q46vrc;
		float xuzqwk;
		float ozp4r8;
		float q5j07c;
		pa64ni = 9332481.4464;
		if (ozp4r8 == 9741285.8732)
			ozp4r8 = 7192105.9777;
		q46vrc = 7778449.0415;
		for (int wocgd = 0; wocgd > 100; wocgd++)
		{
			xuzqwk = 8153858.8793;
		}
		pa64ni = 891592.2498;
		for (int i8jl6d = 0; i8jl6d > 100; i8jl6d++)
		{
			nr5xkf = 10270881.9086;
		}
		q5j07c = 3547288.2122;
		while (ozp4r8 == 4343900.4462)
		{
			ozp4r8 = 246455.8668;
		}
		pa64ni = 723355.0664;
		while (q12r8f == 1216951.7590)
		{
			q12r8f = 5263819.1175;
		}
		xuzqwk = 1164565.7786;
		q46vrc = 9042814.3871;
		if (pa64ni == 1470252.8346)
			pa64ni = 628911.1675;
		nr5xkf = 10320148.0252;
		pa64ni = 6057711.4825;
		if (ozp4r8 == 9175026.5932)
			ozp4r8 = 3037403.8743;
		pa64ni = 99162.9401;
	}
	void junk5035038() {
		return;
		float h5zoqi;
		float ytzcd7;
		float qxg408;
		float doib4;
		float g1fr2;
		float pq1kqe;
		float nxo7w;
		float srur4m;
		float ed00rr;
		float gtddkq;
		h5zoqi = 8752506.8605;
		srur4m = 754649.8561;
		h5zoqi = 9215201.6111;
		if (h5zoqi == 4014856.8401)
			h5zoqi = 9238795.1746;
		pq1kqe = 8731766.2835;
		for (int c5rgj = 0; c5rgj > 100; c5rgj++)
		{
			doib4 = 6330815.6156;
		}
		ed00rr = 9011815.1368;
		doib4 = 3816832.8149;
		for (int edl9z = 0; edl9z > 100; edl9z++)
		{
			ed00rr = 3132194.2374;
		}
		doib4 = 8625628.1242;
		pq1kqe = 7631045.4376;
		g1fr2 = 2070676.8645;
		while (nxo7w == 4026695.8164)
		{
			nxo7w = 9968433.6948;
		}
		g1fr2 = 9967996.4613;
	}
	void junk2784666() {
		return;
		float ptztq7;
		float tna4c4;
		float flm27;
		float w2z0wx;
		float qk2gei;
		float kxtbz6;
		float g2wlqw;
		float gal2p;
		float jld7o;
		float qvo61i;
		if (gal2p == 3459429.8566)
			gal2p = 6849653.8587;
		qvo61i = 9574354.4569;
		g2wlqw = 3067896.0711;
		for (int ri6ij = 0; ri6ij > 100; ri6ij++)
		{
			flm27 = 5861530.5623;
		}
		g2wlqw = 1432467.1392;
		qk2gei = 4683224.6733;
		while (qk2gei == 3045049.2639)
		{
			qk2gei = 3734879.8686;
		}
		jld7o = 1636489.6154;
		qvo61i = 4443262.8799;
		ptztq7 = 7543594.7024;
		for (int db4ldp = 0; db4ldp > 100; db4ldp++)
		{
			gal2p = 1332681.6031;
		}
		g2wlqw = 1335799.1358;
		qk2gei = 2602635.1202;
		for (int tiqv9v = 0; tiqv9v > 100; tiqv9v++)
		{
			kxtbz6 = 3046014.3291;
		}
		kxtbz6 = 1741350.7166;
	}
	void junk5543375() {
		return;
		float jyrskt;
		float e3dh2;
		float r4ewrw;
		float g3htw4;
		float l1ddts;
		float l6mydi;
		float h5c5um;
		float bk8ci;
		float iwmmj;
		float y6zy2k;
		while (bk8ci == 9707563.1199)
		{
			bk8ci = 7888068.0873;
		}
		bk8ci = 5550283.7954;
		if (y6zy2k == 4134529.2169)
			y6zy2k = 8883906.2721;
		jyrskt = 4562189.7730;
		while (r4ewrw == 8541230.5157)
		{
			r4ewrw = 6779492.3349;
		}
		r4ewrw = 2308254.9385;
		for (int dl59rl = 0; dl59rl > 100; dl59rl++)
		{
			h5c5um = 2025248.7288;
		}
		h5c5um = 9186385.3651;
		if (g3htw4 == 9615736.3934)
			g3htw4 = 600195.5469;
		l1ddts = 8110091.4388;
		for (int csy4s = 0; csy4s > 100; csy4s++)
		{
			y6zy2k = 7021957.9994;
		}
		l1ddts = 4061399.1931;
		jyrskt = 1487739.7281;
		for (int un5gjj = 0; un5gjj > 100; un5gjj++)
		{
			y6zy2k = 8373894.2930;
		}
		iwmmj = 7425300.2331;
		for (int ag48ps = 0; ag48ps > 100; ag48ps++)
		{
			l1ddts = 4486931.3336;
		}
		l6mydi = 8861092.5736;
		bk8ci = 995218.8583;
	}
	void junk2384451() {
		return;
		float myy6i9;
		float frdkju;
		float yjmfhu;
		float be11p5;
		float xgwr6p;
		float z37gcr;
		float mfhat7;
		float xh6knw;
		float eu7ofk;
		float qyf10n;
		for (int tc2h5e = 0; tc2h5e > 100; tc2h5e++)
		{
			z37gcr = 188112.5780;
		}
		be11p5 = 8321412.8473;
		if (yjmfhu == 6467416.8341)
			yjmfhu = 230852.8523;
		xh6knw = 5310530.8842;
		for (int gfhqpd = 0; gfhqpd > 100; gfhqpd++)
		{
			xgwr6p = 7664238.6366;
		}
		mfhat7 = 9198687.4556;
		if (xgwr6p == 2679036.6494)
			xgwr6p = 7470256.0828;
		eu7ofk = 325339.9519;
		for (int yfvne = 0; yfvne > 100; yfvne++)
		{
			mfhat7 = 1370687.2598;
		}
		z37gcr = 6717684.1983;
		if (xh6knw == 1776608.9580)
			xh6knw = 2542652.9197;
		frdkju = 3821076.5211;
		for (int p9uex = 0; p9uex > 100; p9uex++)
		{
			mfhat7 = 5730910.5454;
		}
		xh6knw = 6246751.9868;
		z37gcr = 6888700.6012;
		for (int visvlo = 0; visvlo > 100; visvlo++)
		{
			qyf10n = 5908984.8329;
		}
		qyf10n = 8343870.4033;
		myy6i9 = 9391032.7044;
	}
	void junk195148() {
		return;
		float mvub4r;
		float lpkvjd;
		float glbvop;
		float a86rl;
		float c4oy6u;
		float ena87k;
		float h8747j;
		float e3ju3p;
		float ozb14q;
		float gcl2ue;
		while (gcl2ue == 3440360.8587)
		{
			gcl2ue = 3377045.7117;
		}
		h8747j = 269401.9832;
		lpkvjd = 2119081.1103;
		for (int lh34p5 = 0; lh34p5 > 100; lh34p5++)
		{
			mvub4r = 3735487.9258;
		}
		a86rl = 9800587.3478;
		while (ozb14q == 6933147.1514)
		{
			ozb14q = 5386049.8418;
		}
		c4oy6u = 6190879.2417;
		if (glbvop == 5989552.1813)
			glbvop = 7458522.8003;
		e3ju3p = 2125104.8241;
		if (lpkvjd == 5016665.5299)
			lpkvjd = 3994753.8528;
		a86rl = 9177742.9861;
		h8747j = 6176938.4327;
		if (lpkvjd == 3540628.8035)
			lpkvjd = 9614479.6228;
		h8747j = 9114718.2921;
		if (ena87k == 413680.4020)
			ena87k = 8929422.9137;
		ena87k = 10020699.7390;
		c4oy6u = 3797231.9006;
	}
	void junk2076188() {
		return;
		float hmtzvj;
		float faa00ym;
		float lfeziv;
		float t3uwk;
		float l8ua;
		float ker;
		float cnsnwt;
		float znvpyq;
		float p8idw;
		float l2j0ce;
		for (int l5fbtc = 0; l5fbtc > 100; l5fbtc++)
		{
			t3uwk = 10452662.3462;
		}
		cnsnwt = 6646435.0037;
		while (t3uwk == 804610.9379)
		{
			t3uwk = 8410023.1384;
		}
		t3uwk = 7781345.6915;
		for (int g8i2e = 0; g8i2e > 100; g8i2e++)
		{
			cnsnwt = 6230841.9043;
		}
		p8idw = 4965309.1732;
		while (p8idw == 7867591.9622)
		{
			p8idw = 2884848.8169;
		}
		ker = 10063290.0808;
		if (p8idw == 9751173.4386)
			p8idw = 7260441.0943;
		lfeziv = 3760403.8642;
		for (int f509g = 0; f509g > 100; f509g++)
		{
			lfeziv = 8662921.9749;
		}
		p8idw = 10410253.8012;
		if (faa00ym == 6839850.0052)
			faa00ym = 5479024.7237;
		p8idw = 10501976.9027;
		for (int j5tzi = 0; j5tzi > 100; j5tzi++)
		{
			l8ua = 3093843.7693;
		}
		hmtzvj = 4060033.9260;
		if (faa00ym == 5359575.8147)
			faa00ym = 3702064.6553;
		ker = 863923.5074;
		if (cnsnwt == 7298457.0374)
			cnsnwt = 1652900.9073;
		faa00ym = 8580005.4183;
	}
	void doJunk() {
		junk4417638::junk6727907();
		junk4417638::junk8699754();
		junk4417638::junk7965053();
		junk4417638::junk2389599();
		junk4417638::junk5035038();
		junk4417638::junk2784666();
		junk4417638::junk5543375();
		junk4417638::junk2384451();
		junk4417638::junk195148();
		junk4417638::junk2076188();
	}
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class thixqcg {
public:
	double hdsoghsrt;
	string hloihsynyk;
	int orctwrxw;
	string exefhor;
	thixqcg();
	int yejlspmwtoxikeeld(bool phtyyzirj, double nilagyyfmlciu, int xgewwfw, string thogqjs, bool hfhpylymo, string ttgguugtahwjqfw);
	int bcpkpyawgtwcziy(bool hbnfvtv, bool oiwawiuwxnhlsde, int sacrohuqmymembm, bool ybmreojly, int qajcabufpzqa, string hownbqfdaxxdxst, int hbxrp, string vlznxtkmvsety, bool xhydfhbenyvldq);
	void jzyqdjokqq();
	int djvsfdeicx(int iipxsvshfbnpon, string zlqitncqs, string xabtkbbvchzkurq, double ohacmrffyn, string irlbxhqesj, double vycuiryiss, bool gtclcnqwcfq, int chxpfojwxse, string cuofvgzhzsh);
	bool yqnawlgvsucjzett();
	string yrkkqvvmlexws(double nodqepngfqb, bool osowntcxmdkybie, double owooscyzog, int iafchejxg, string dvzdea, bool kshzdqpevtcqinw, bool jfqzlocsstom, string rqkxkptzn, double ajtpuwarteco);
	string kgacvqxyqbtfkwydkvlxgjnz(string ksftdyjnzzrvpp, int yzsnnxfy, double aphuswno, string qqhamovhnaim, double aypnugus, string dxrortyrbfebdd);
	void gamesjxuelctsqbmu(int exfcspzqfwm, double itnfaafszkrlga, bool lvzzn, string gbtyatna, double stsolmgchtb, double oxyyjbaryftob, double wnioyqcqpb);
	int etuzqvpzvgmuszwy();

protected:
	double bazoscofjhqb;

	double timcbkxiyvwg();
	bool fbrthvecyaqcbvydc(string iiumxsqo, double tuqqdxaqyhnvq, double wliodagobnuiu, string vamiaxr, string mxgkztyk, bool kgpyzyt);
	bool gejcgdyfqjwimpbdlhqwogbov(double urooxegjk, double aotwlfid, int dvxnukecsi, string bquspgjlriwkpp, bool jnavyzogrwrgo, double clipta, double sbpnl, bool goggegmnfkgzl);
	double aovyhasweglntzmwfhry(int kxfjtiixi, string dndpmguioxzxqf, double iwkcouunotkj);
	string gbscjgsrutcnuzrxucaatcud(int ssvzbl);
	bool qzlxzdukfqsxohxayugl(string npsnywav, double vephgglhtix, bool hrdpodojkzept, bool apmujnnpizaew, bool ahjaykhymfzd, double qncnvfns, int qahnkaqp, double wveskkdb);
	double dsgnrjdwdcg(string aqrsfcmiocw, bool cnxicczhxzjzvm);

private:
	bool hukjpmkphnjgzer;
	double mzpygkj;
	string rzjpbsu;
	bool tdafw;

	bool eiwejcqlekiljdcd(bool yqswybz, double oavinzr, string wnfblywqnwnr, double wrmndl, int vjqzjycpti, string gxhvrbnytey, double clyhocci);
	int mhxgswueeiaojnzuxnt(int vtcai, int rlsakmhsba, double qprijtqzphurgy, double jqwthicsbyuyr, bool gpttivyteisi, string yztuet, double ykdlakwmqv, int hbfkgxzyjrdxa, double vxodlkjjlrlkqsr, string ncbgcombtmgsuh);
	int cozqrleovepdcvors(int yakfzmwcsp, double whgkslqekkr, double piknl, bool wznvkchhyuzgi);
	string bslpekrutjcuo(double ncziedtso, double mnuovvshyiu, int atfhnwz, double auwedtmlvfp, bool khaqtrnzsy);

};


bool thixqcg::eiwejcqlekiljdcd(bool yqswybz, double oavinzr, string wnfblywqnwnr, double wrmndl, int vjqzjycpti, string gxhvrbnytey, double clyhocci) {
	int owkichsxmubfv = 3203;
	double ttpuvaxh = 2714;
	string ziulgmbuj = "hzogalelmvrgbsvgoabhpn";
	bool yfqwhh = false;
	bool dmrmjqoap = true;
	if (false == false) {
		int be;
		for (be = 58; be > 0; be--) {
			continue;
		}
	}
	return false;
}

int thixqcg::mhxgswueeiaojnzuxnt(int vtcai, int rlsakmhsba, double qprijtqzphurgy, double jqwthicsbyuyr, bool gpttivyteisi, string yztuet, double ykdlakwmqv, int hbfkgxzyjrdxa, double vxodlkjjlrlkqsr, string ncbgcombtmgsuh) {
	string ugvezjixyqrndym = "whgqe";
	bool bydjlexgxsya = true;
	bool oxpuuaqxduly = true;
	bool zuotcevdxj = false;
	double rufrtjvtlhq = 21504;
	string ugiau = "hqfyptnknwjkztwzpbapjgbkj";
	string wgwngmuszr = "rgudgpofqysykszginpfillxxojgqgjcuhdhrfojnbhiqrujyegcdtwyiiugakkehtuocfn";
	int seqfsmqmrcezdw = 6098;
	bool tpshq = true;
	double ydogtopsr = 33870;
	if (true == true) {
		int voogmvbv;
		for (voogmvbv = 73; voogmvbv > 0; voogmvbv--) {
			continue;
		}
	}
	if (6098 == 6098) {
		int uyujecp;
		for (uyujecp = 0; uyujecp > 0; uyujecp--) {
			continue;
		}
	}
	if (true == true) {
		int rho;
		for (rho = 58; rho > 0; rho--) {
			continue;
		}
	}
	return 75666;
}

int thixqcg::cozqrleovepdcvors(int yakfzmwcsp, double whgkslqekkr, double piknl, bool wznvkchhyuzgi) {
	bool punpjdi = false;
	double waewhk = 68390;
	bool wcysrgrloh = true;
	string hnewxu = "weiqntzucrjzgargfjpsyhrnozomk";
	double vjdkl = 7426;
	if (true != true) {
		int yqqsmrvcd;
		for (yqqsmrvcd = 94; yqqsmrvcd > 0; yqqsmrvcd--) {
			continue;
		}
	}
	return 73192;
}

string thixqcg::bslpekrutjcuo(double ncziedtso, double mnuovvshyiu, int atfhnwz, double auwedtmlvfp, bool khaqtrnzsy) {
	bool kpwtrl = true;
	double fsfabd = 69105;
	int jztsgrsbly = 8791;
	bool ilfhyjfjifye = false;
	double wzkutqxlwvmir = 8158;
	double rojrazwlblahzfy = 831;
	double mpabdn = 71787;
	int kjaxrcbfcnyzk = 4830;
	if (8791 == 8791) {
		int ohuolp;
		for (ohuolp = 94; ohuolp > 0; ohuolp--) {
			continue;
		}
	}
	if (831 == 831) {
		int spwnkbxjh;
		for (spwnkbxjh = 53; spwnkbxjh > 0; spwnkbxjh--) {
			continue;
		}
	}
	if (false != false) {
		int diuerwueh;
		for (diuerwueh = 57; diuerwueh > 0; diuerwueh--) {
			continue;
		}
	}
	if (69105 != 69105) {
		int cqykclkw;
		for (cqykclkw = 72; cqykclkw > 0; cqykclkw--) {
			continue;
		}
	}
	return string("jsrhcmopvspxkr");
}

double thixqcg::timcbkxiyvwg() {
	double ftsmozxe = 48336;
	double qpfighkn = 23582;
	if (48336 != 48336) {
		int nuko;
		for (nuko = 80; nuko > 0; nuko--) {
			continue;
		}
	}
	return 15750;
}

bool thixqcg::fbrthvecyaqcbvydc(string iiumxsqo, double tuqqdxaqyhnvq, double wliodagobnuiu, string vamiaxr, string mxgkztyk, bool kgpyzyt) {
	double gskrkpxtaxrw = 3782;
	string vvoqnptlyv = "sfwspumpzftwrkbsqircgfgufdxxlivqerlpbecignetwzckjjjqebfwvymhczwygyaacjuxsblujdegnrgtzh";
	int saocoluld = 3534;
	double vewgockrfugik = 22328;
	double mvhumnc = 13899;
	int yerisjrnvii = 6354;
	string yjjkth = "eapnuoabdgvkdklugxbeahlmydnpxpcbwjlnhvzawpsnvyhktpyruddndhxdnhdwpdcvtyndnaoqilpudn";
	string fsllqtr = "tbxwjawqkpshhxzoz";
	double ovgczdubrwmgwto = 33343;
	double ukiuykxsov = 4591;
	if (3782 != 3782) {
		int plss;
		for (plss = 87; plss > 0; plss--) {
			continue;
		}
	}
	if (3782 != 3782) {
		int ixlfa;
		for (ixlfa = 42; ixlfa > 0; ixlfa--) {
			continue;
		}
	}
	if (3782 == 3782) {
		int lz;
		for (lz = 25; lz > 0; lz--) {
			continue;
		}
	}
	return true;
}

bool thixqcg::gejcgdyfqjwimpbdlhqwogbov(double urooxegjk, double aotwlfid, int dvxnukecsi, string bquspgjlriwkpp, bool jnavyzogrwrgo, double clipta, double sbpnl, bool goggegmnfkgzl) {
	string rxoeu = "fvkz";
	double idwdabvsnlhrap = 1467;
	bool llbbonisr = false;
	string jroxvkg = "opftbpqqzrucsnlaypjsgksgygaseojcjifanaisuusmqhcjunobrisldr";
	bool emnisrfbsjfs = true;
	if (true == true) {
		int myp;
		for (myp = 38; myp > 0; myp--) {
			continue;
		}
	}
	if (string("opftbpqqzrucsnlaypjsgksgygaseojcjifanaisuusmqhcjunobrisldr") == string("opftbpqqzrucsnlaypjsgksgygaseojcjifanaisuusmqhcjunobrisldr")) {
		int vbcwaainij;
		for (vbcwaainij = 40; vbcwaainij > 0; vbcwaainij--) {
			continue;
		}
	}
	if (true == true) {
		int hvbwyoilm;
		for (hvbwyoilm = 37; hvbwyoilm > 0; hvbwyoilm--) {
			continue;
		}
	}
	if (false == false) {
		int beuilnzv;
		for (beuilnzv = 8; beuilnzv > 0; beuilnzv--) {
			continue;
		}
	}
	if (false == false) {
		int ry;
		for (ry = 61; ry > 0; ry--) {
			continue;
		}
	}
	return false;
}

double thixqcg::aovyhasweglntzmwfhry(int kxfjtiixi, string dndpmguioxzxqf, double iwkcouunotkj) {
	int fwnmmbhhkgpyk = 7809;
	bool ztlcimkd = true;
	int rkvlquhbtcq = 1744;
	double axzvguoaketwvrt = 55929;
	string xakgk = "vqyxyvicwagltccubftgggrpfyfkalwjcpsemeuvymtuzhcjksskedlb";
	bool ybpfewdamcnfvm = false;
	double nnwxvqclngbrfq = 1850;
	double gtjcxa = 68594;
	string joxgedgqufz = "kccvetyroohnmjmmmejbmbcqpkagslsdnrwugaxiytydfhtffcpalsfzhvqun";
	return 98903;
}

string thixqcg::gbscjgsrutcnuzrxucaatcud(int ssvzbl) {
	double bgonzigdepxg = 27959;
	double tfkkuqnpuyldg = 22599;
	string mbgetxiimxs = "esuayaldpdzvfmjbmogxlncxpxrvztfuxojjtmzdtifwutusqwfsfwdkwkdeixlexragylozvmkervarcurmexeppvspeoqbia";
	int orsjhqvsme = 3703;
	if (22599 != 22599) {
		int vycdlz;
		for (vycdlz = 16; vycdlz > 0; vycdlz--) {
			continue;
		}
	}
	return string("jwwyirvaqcyvlk");
}

bool thixqcg::qzlxzdukfqsxohxayugl(string npsnywav, double vephgglhtix, bool hrdpodojkzept, bool apmujnnpizaew, bool ahjaykhymfzd, double qncnvfns, int qahnkaqp, double wveskkdb) {
	bool gnfecnesjaox = true;
	double rbuxklnaj = 35366;
	int xnxuvxcmf = 818;
	double dootqzsdatbl = 25049;
	double ffnmtqfnkpfsn = 92145;
	double tvmblbyim = 24834;
	double ogucjpolts = 49;
	return false;
}

double thixqcg::dsgnrjdwdcg(string aqrsfcmiocw, bool cnxicczhxzjzvm) {
	int vqnocolxajxg = 409;
	if (409 != 409) {
		int galt;
		for (galt = 91; galt > 0; galt--) {
			continue;
		}
	}
	if (409 == 409) {
		int dbmazsuj;
		for (dbmazsuj = 81; dbmazsuj > 0; dbmazsuj--) {
			continue;
		}
	}
	if (409 == 409) {
		int sw;
		for (sw = 18; sw > 0; sw--) {
			continue;
		}
	}
	if (409 != 409) {
		int vdmfpz;
		for (vdmfpz = 39; vdmfpz > 0; vdmfpz--) {
			continue;
		}
	}
	if (409 != 409) {
		int eqlo;
		for (eqlo = 88; eqlo > 0; eqlo--) {
			continue;
		}
	}
	return 12031;
}

int thixqcg::yejlspmwtoxikeeld(bool phtyyzirj, double nilagyyfmlciu, int xgewwfw, string thogqjs, bool hfhpylymo, string ttgguugtahwjqfw) {
	double wdtgtdqwbha = 25465;
	string rbyohc = "uhoijlroumopaxettznbgkgudfaxeetdhepdsbcsavvvauvhcmubvszmgirmikaklyxdiceugdmrsyytolqqrjzs";
	int gbvbspnomgnug = 16;
	double wsopnqfgceimqvf = 11314;
	string kdkcshughnfe = "nokfpdfdqfcqcbcinhuzqtwbzwvxhxhrivxuxdnnfqeofwvsarrbafzxajdhk";
	if (11314 != 11314) {
		int cyzuxwytla;
		for (cyzuxwytla = 11; cyzuxwytla > 0; cyzuxwytla--) {
			continue;
		}
	}
	return 1154;
}

int thixqcg::bcpkpyawgtwcziy(bool hbnfvtv, bool oiwawiuwxnhlsde, int sacrohuqmymembm, bool ybmreojly, int qajcabufpzqa, string hownbqfdaxxdxst, int hbxrp, string vlznxtkmvsety, bool xhydfhbenyvldq) {
	int kynxgmad = 1214;
	string encxomjpztz = "hzulbbtdhbfzgyjnmapfyqafdzhxf";
	bool zwrsc = false;
	double cevzlnqkoo = 10577;
	bool yqrbetnkrqjm = false;
	bool nfirkf = true;
	if (false == false) {
		int ez;
		for (ez = 16; ez > 0; ez--) {
			continue;
		}
	}
	if (10577 == 10577) {
		int bsxmuryhf;
		for (bsxmuryhf = 79; bsxmuryhf > 0; bsxmuryhf--) {
			continue;
		}
	}
	if (true != true) {
		int zeaaigm;
		for (zeaaigm = 88; zeaaigm > 0; zeaaigm--) {
			continue;
		}
	}
	if (false == false) {
		int lxiutawt;
		for (lxiutawt = 38; lxiutawt > 0; lxiutawt--) {
			continue;
		}
	}
	return 16058;
}

void thixqcg::jzyqdjokqq() {
	string pgjhiosmujwclb = "soxerldzarkqmdcmevhfdipvsmkzjvhkcfsleuowgledtzdulpsbezlaqtvgqslhcstoqretngifwongaxtopbbhpthxlspyq";
	if (string("soxerldzarkqmdcmevhfdipvsmkzjvhkcfsleuowgledtzdulpsbezlaqtvgqslhcstoqretngifwongaxtopbbhpthxlspyq") == string("soxerldzarkqmdcmevhfdipvsmkzjvhkcfsleuowgledtzdulpsbezlaqtvgqslhcstoqretngifwongaxtopbbhpthxlspyq")) {
		int fo;
		for (fo = 9; fo > 0; fo--) {
			continue;
		}
	}
	if (string("soxerldzarkqmdcmevhfdipvsmkzjvhkcfsleuowgledtzdulpsbezlaqtvgqslhcstoqretngifwongaxtopbbhpthxlspyq") == string("soxerldzarkqmdcmevhfdipvsmkzjvhkcfsleuowgledtzdulpsbezlaqtvgqslhcstoqretngifwongaxtopbbhpthxlspyq")) {
		int eiscvbmudv;
		for (eiscvbmudv = 6; eiscvbmudv > 0; eiscvbmudv--) {
			continue;
		}
	}
	if (string("soxerldzarkqmdcmevhfdipvsmkzjvhkcfsleuowgledtzdulpsbezlaqtvgqslhcstoqretngifwongaxtopbbhpthxlspyq") != string("soxerldzarkqmdcmevhfdipvsmkzjvhkcfsleuowgledtzdulpsbezlaqtvgqslhcstoqretngifwongaxtopbbhpthxlspyq")) {
		int kbcbn;
		for (kbcbn = 57; kbcbn > 0; kbcbn--) {
			continue;
		}
	}

}

int thixqcg::djvsfdeicx(int iipxsvshfbnpon, string zlqitncqs, string xabtkbbvchzkurq, double ohacmrffyn, string irlbxhqesj, double vycuiryiss, bool gtclcnqwcfq, int chxpfojwxse, string cuofvgzhzsh) {
	double ibzuivhzf = 67082;
	string hjpjj = "xmgyks";
	int goeujrko = 1067;
	double zgspeh = 6150;
	if (1067 != 1067) {
		int ubegonvvic;
		for (ubegonvvic = 14; ubegonvvic > 0; ubegonvvic--) {
			continue;
		}
	}
	if (6150 == 6150) {
		int vejqfcolae;
		for (vejqfcolae = 53; vejqfcolae > 0; vejqfcolae--) {
			continue;
		}
	}
	if (1067 != 1067) {
		int jdhhll;
		for (jdhhll = 17; jdhhll > 0; jdhhll--) {
			continue;
		}
	}
	return 12536;
}

bool thixqcg::yqnawlgvsucjzett() {
	double ipnma = 63335;
	bool jlilyyizx = true;
	string wyjvoqtey = "";
	if (63335 == 63335) {
		int aqdvxgnp;
		for (aqdvxgnp = 29; aqdvxgnp > 0; aqdvxgnp--) {
			continue;
		}
	}
	if (string("") != string("")) {
		int jgwxka;
		for (jgwxka = 17; jgwxka > 0; jgwxka--) {
			continue;
		}
	}
	if (true == true) {
		int mudybd;
		for (mudybd = 93; mudybd > 0; mudybd--) {
			continue;
		}
	}
	return false;
}

string thixqcg::yrkkqvvmlexws(double nodqepngfqb, bool osowntcxmdkybie, double owooscyzog, int iafchejxg, string dvzdea, bool kshzdqpevtcqinw, bool jfqzlocsstom, string rqkxkptzn, double ajtpuwarteco) {
	int cuwtvb = 8225;
	string qlezhhytufxgsg = "rcpgngfpowkdzgpkiqhingmzezdfljfordgrqzqcamlgdpstlkhluiumkmnwcocgvvgklgmqghrdthuvhzhavgpalxbxembfp";
	int bqktw = 788;
	if (8225 != 8225) {
		int vishrkifk;
		for (vishrkifk = 29; vishrkifk > 0; vishrkifk--) {
			continue;
		}
	}
	if (8225 != 8225) {
		int at;
		for (at = 66; at > 0; at--) {
			continue;
		}
	}
	if (string("rcpgngfpowkdzgpkiqhingmzezdfljfordgrqzqcamlgdpstlkhluiumkmnwcocgvvgklgmqghrdthuvhzhavgpalxbxembfp") != string("rcpgngfpowkdzgpkiqhingmzezdfljfordgrqzqcamlgdpstlkhluiumkmnwcocgvvgklgmqghrdthuvhzhavgpalxbxembfp")) {
		int jh;
		for (jh = 35; jh > 0; jh--) {
			continue;
		}
	}
	if (string("rcpgngfpowkdzgpkiqhingmzezdfljfordgrqzqcamlgdpstlkhluiumkmnwcocgvvgklgmqghrdthuvhzhavgpalxbxembfp") == string("rcpgngfpowkdzgpkiqhingmzezdfljfordgrqzqcamlgdpstlkhluiumkmnwcocgvvgklgmqghrdthuvhzhavgpalxbxembfp")) {
		int moiixgmv;
		for (moiixgmv = 41; moiixgmv > 0; moiixgmv--) {
			continue;
		}
	}
	if (string("rcpgngfpowkdzgpkiqhingmzezdfljfordgrqzqcamlgdpstlkhluiumkmnwcocgvvgklgmqghrdthuvhzhavgpalxbxembfp") == string("rcpgngfpowkdzgpkiqhingmzezdfljfordgrqzqcamlgdpstlkhluiumkmnwcocgvvgklgmqghrdthuvhzhavgpalxbxembfp")) {
		int ovfqggg;
		for (ovfqggg = 29; ovfqggg > 0; ovfqggg--) {
			continue;
		}
	}
	return string("astawezrpztxje");
}

string thixqcg::kgacvqxyqbtfkwydkvlxgjnz(string ksftdyjnzzrvpp, int yzsnnxfy, double aphuswno, string qqhamovhnaim, double aypnugus, string dxrortyrbfebdd) {
	double izlkp = 13600;
	string ehhsghe = "ygxhmhqgocmstlqnzuomhiczjgudvoktppgstjjjcctvxxiusyqpdwhwpoodgxsmghhrdzopmgmalmskedlpsbznlc";
	int ebuetfxoec = 2536;
	string tghzailxnovowpr = "togmjaedkoqphtnorswgqmaopxiodwfbywwjqcpfewithffsdxrqbkgkjtrqfrezlmsz";
	string kerzqxczbmeujw = "rhfavtfdndzvxmswzcjkouofrcnoxozeowawckgfefsqzemiesxpqayqirgenewahdpxbyalvyhuerhydlbiksap";
	bool lhmlgozmmz = true;
	if (string("togmjaedkoqphtnorswgqmaopxiodwfbywwjqcpfewithffsdxrqbkgkjtrqfrezlmsz") != string("togmjaedkoqphtnorswgqmaopxiodwfbywwjqcpfewithffsdxrqbkgkjtrqfrezlmsz")) {
		int kcljcvb;
		for (kcljcvb = 35; kcljcvb > 0; kcljcvb--) {
			continue;
		}
	}
	if (13600 == 13600) {
		int mdive;
		for (mdive = 41; mdive > 0; mdive--) {
			continue;
		}
	}
	if (13600 == 13600) {
		int fqn;
		for (fqn = 97; fqn > 0; fqn--) {
			continue;
		}
	}
	if (string("rhfavtfdndzvxmswzcjkouofrcnoxozeowawckgfefsqzemiesxpqayqirgenewahdpxbyalvyhuerhydlbiksap") != string("rhfavtfdndzvxmswzcjkouofrcnoxozeowawckgfefsqzemiesxpqayqirgenewahdpxbyalvyhuerhydlbiksap")) {
		int ggaazbo;
		for (ggaazbo = 90; ggaazbo > 0; ggaazbo--) {
			continue;
		}
	}
	if (2536 == 2536) {
		int qgc;
		for (qgc = 85; qgc > 0; qgc--) {
			continue;
		}
	}
	return string("tgbmwhfcpczlh");
}

void thixqcg::gamesjxuelctsqbmu(int exfcspzqfwm, double itnfaafszkrlga, bool lvzzn, string gbtyatna, double stsolmgchtb, double oxyyjbaryftob, double wnioyqcqpb) {
	bool yvvyveyqlnguzbk = true;
	bool yqtbiqwr = true;
	if (true == true) {
		int uadg;
		for (uadg = 43; uadg > 0; uadg--) {
			continue;
		}
	}
	if (true == true) {
		int vhf;
		for (vhf = 17; vhf > 0; vhf--) {
			continue;
		}
	}
	if (true != true) {
		int luaygewq;
		for (luaygewq = 86; luaygewq > 0; luaygewq--) {
			continue;
		}
	}
	if (true == true) {
		int ygaymk;
		for (ygaymk = 90; ygaymk > 0; ygaymk--) {
			continue;
		}
	}
	if (true != true) {
		int fnhsjl;
		for (fnhsjl = 32; fnhsjl > 0; fnhsjl--) {
			continue;
		}
	}

}

int thixqcg::etuzqvpzvgmuszwy() {
	string mrwttsrmxjs = "kkfukxzujsmkomxnddzzizeslttuyspmlyclnazqhljixgkaytptqcvrrekjkkyu";
	if (string("kkfukxzujsmkomxnddzzizeslttuyspmlyclnazqhljixgkaytptqcvrrekjkkyu") == string("kkfukxzujsmkomxnddzzizeslttuyspmlyclnazqhljixgkaytptqcvrrekjkkyu")) {
		int atfmkacw;
		for (atfmkacw = 43; atfmkacw > 0; atfmkacw--) {
			continue;
		}
	}
	return 66990;
}

thixqcg::thixqcg() {
	this->yejlspmwtoxikeeld(true, 62842, 4530, string("adjc"), false, string("m"));
	this->bcpkpyawgtwcziy(true, false, 2661, false, 6210, string("jkawsayixleddrtthddlwbtttsiwpabxervnlxzgqvjodteruibpecvoylhpeppgodrahf"), 6463, string("zymnbiiklbmkqggnmkgyfoqxdoonwoqmwzjahlavw"), true);
	this->jzyqdjokqq();
	this->djvsfdeicx(3002, string("lhykisegnpovefrrqdnyteqi"), string("hqjkmhtniqnxcjbckslsewdbbiikjxgnfhgrumnhixrd"), 43190, string("fctfmbnlqjfglzlguydlgixitscnndvitmwbleseodobzfpruhlnhywol"), 45359, true, 2860, string("afjgklkmdgtnxnfvsfphxybknsatwzkffkpdoalbuddsgjxexgkiqvsizofyjckbongeewejjybt"));
	this->yqnawlgvsucjzett();
	this->yrkkqvvmlexws(10565, true, 3329, 5241, string("qsdmxggloskcthquydyusxzgeyxjbnsmpbxesxojxbfcgzyvmntqvgzyzkflxkzfrbkmknff"), false, false, string("blwussmikzinhofgcbnuudjwnegmjntyzqjespkushrdiwfpseqrchmidpumegrebixhlzelfptlpupwjzrsbclmzoghiofzo"), 16506);
	this->kgacvqxyqbtfkwydkvlxgjnz(string("gxlkkbxfyfhkkqniszusonhtgitjxjbjbipbyqtoneettfuqawczmaixydliuljmiekgepcqdncunrlfaemlgbk"), 2947, 10679, string("kgqvlnzypfdoivtwcsnejqurorwhvxyzvahvwlwdfaxvyhgqtavaaxbkhmrtweesqzribadubbhbekhlqygu"), 83004, string("djptbumarzobfskeptrppckrzjcdpxyfujrcqfitiabqaavvehnmahcyynfcausxhxamygnedmbbhetvsdntpsrlebuwswv"));
	this->gamesjxuelctsqbmu(185, 2301, false, string("atovvajyurcegfffsvuhobycufbqzjovecsdxydwsqwedyrwohnatugkbrantdjyiksvlucnufpuaqxkiqnlnohqdntvnlkyzosa"), 44934, 22650, 46486);
	this->etuzqvpzvgmuszwy();
	this->timcbkxiyvwg();
	this->fbrthvecyaqcbvydc(string("ayerxkwvjknrtpslkwuswsaofrnbrajdwhuarncxjhapes"), 81773, 18256, string("msvlackpmngs"), string("hzpdewtdxx"), true);
	this->gejcgdyfqjwimpbdlhqwogbov(80596, 69788, 4236, string("dzkfermlfwtgfwrdnvpiqlupzbnkgbmhmelkubk"), true, 18129, 25345, false);
	this->aovyhasweglntzmwfhry(5865, string("ujfqqgkqplmseqvchtkkfqkgpcfcmbjxhkgbkjhtngtkubknplbvj"), 24426);
	this->gbscjgsrutcnuzrxucaatcud(3443);
	this->qzlxzdukfqsxohxayugl(string("tssnmupguzjxrvakefsziedyqcvkegnpassjlpravklbooludlrehsovfyydciwlr"), 3123, false, false, false, 9778, 244, 40244);
	this->dsgnrjdwdcg(string("odajjsodtkvydwaftkcjmqljtjr"), false);
	this->eiwejcqlekiljdcd(false, 10986, string("ghcbdntzqq"), 7196, 1855, string("ipuypoqdblyswglzhmediibmdop"), 73896);
	this->mhxgswueeiaojnzuxnt(1907, 160, 3051, 19265, false, string("porepfxfdhjgfjdxjvdrmtwgfnj"), 44340, 1338, 24709, string("wkmdrpydgnsqxhbawujkltymrwwtookjdolemmzqkjjqblcargvvgqkwphvnk"));
	this->cozqrleovepdcvors(371, 14872, 40259, false);
	this->bslpekrutjcuo(4060, 11204, 1561, 47214, true);
}

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

class ulggvec {
public:
	int ftpqvmsztvm;
	bool njaxb;
	ulggvec();
	bool pwkeowgdthklzozfmjr(string jqlgtojqltjo, bool zrxtejybnrb, bool vludllnjwrs, bool cvjodwjcds, bool rolhsatpgn, string oowsvfvvijv, bool mnrmeaf, bool kmprbpq, bool ikybrhrfeqmdpf);
	void vsobltymwhhrlfhjgznqpuzk(bool swweoyat, string ldofkebja);
	bool dyvsnymlauxvtav(int beioe, double prsxnfkptcsvse, bool bliedld, bool rdmanldyh, bool fqgeazmukscjrhy, double bsjramwonzvy, int bbttj, int loifkoljnad);

protected:
	double wyciqw;

	double sototsrijcrnfozttpf(string bgtuvncaboudcem, double gxqmmoyrfhidszu);
	void ttvjgqugxnmp(double zrknljqhbmprnla, double ocgdjxvvi, string yoikfkfftthnz, double gnsqyuj);
	int bxlntlpjuqimy(string yvuneby, int qfljabnxwxa, int reocjuhvrbbell, int birgwepuardzu, int unzxajzdw, double erpglnet, bool xlvverirx);

private:
	int ekbpacxidwn;
	double jgttr;

	bool xbsoaemoboseqhbdcfk(bool zlbtmijfmcbwhk, bool hmluvwrbv, bool mkkhdas, int exbldpd, bool gihpkgnmphqx, bool pdivzmycrvou, string vaziinvgwzgjx, string dyxia, bool ivvuglnoqa, int lyodnxiat);
	bool unvefzbycodrhinixwof(double njqwv, int dmcgoisoxlbn, bool fugvuqiibgy, bool lyibnbrczc);

};


bool ulggvec::xbsoaemoboseqhbdcfk(bool zlbtmijfmcbwhk, bool hmluvwrbv, bool mkkhdas, int exbldpd, bool gihpkgnmphqx, bool pdivzmycrvou, string vaziinvgwzgjx, string dyxia, bool ivvuglnoqa, int lyodnxiat) {
	int pxbiinpczpo = 7250;
	double dajjlcnbrsnoesx = 70906;
	bool mjaokbs = true;
	string fpteyxxzqwg = "otuuaajahggznporvxz";
	bool hlmff = true;
	if (true != true) {
		int wghgo;
		for (wghgo = 87; wghgo > 0; wghgo--) {
			continue;
		}
	}
	if (string("otuuaajahggznporvxz") != string("otuuaajahggznporvxz")) {
		int oqkefy;
		for (oqkefy = 53; oqkefy > 0; oqkefy--) {
			continue;
		}
	}
	return false;
}

bool ulggvec::unvefzbycodrhinixwof(double njqwv, int dmcgoisoxlbn, bool fugvuqiibgy, bool lyibnbrczc) {
	bool djipmgtm = true;
	bool usgpj = false;
	int gmifb = 372;
	bool jqvbiptia = false;
	bool eaztufjsf = true;
	string nlkeizgfphxi = "axaazpmiaeiavvumhbxdixxcqotfojkulnquntevlwhladdbznqftsjkovrstgyl";
	double vmjjmkmao = 28906;
	int umyuxuqisti = 755;
	int xjgmfyyjfqy = 3401;
	string mlywqcxtyz = "dtwkmezhkkrqvfzxfssadktinrojdjiynlqbsjylqxhtynuwihhdflkifxbol";
	if (true != true) {
		int zd;
		for (zd = 77; zd > 0; zd--) {
			continue;
		}
	}
	if (true == true) {
		int dwgk;
		for (dwgk = 16; dwgk > 0; dwgk--) {
			continue;
		}
	}
	return false;
}

double ulggvec::sototsrijcrnfozttpf(string bgtuvncaboudcem, double gxqmmoyrfhidszu) {
	string dhofhzu = "wmexidwrerosbzptetuwevbacpgjwkvknjnwbdumlgxadnldajmubvdzvjfnzhxwlsmkbqcmvcundgduop";
	double woubfqkv = 10226;
	double qlvur = 9051;
	string urrctkeanlgm = "djixsguwdsbjcafyuoutla";
	double ghcecmt = 27187;
	string sduicxso = "vhcsgboxvelpltco";
	string gmkvnuljucijx = "cojydjvodkmdlefmygsuexjcecsvtnsaowgurlxbuujdrinoqpxyxaebagmugsqisyplnmjokuaxlvjuyna";
	int xezdskyfmksh = 1023;
	double uafsbstanxf = 23598;
	if (1023 == 1023) {
		int gidjlzjez;
		for (gidjlzjez = 4; gidjlzjez > 0; gidjlzjez--) {
			continue;
		}
	}
	if (9051 == 9051) {
		int yarwnqmmh;
		for (yarwnqmmh = 71; yarwnqmmh > 0; yarwnqmmh--) {
			continue;
		}
	}
	if (string("wmexidwrerosbzptetuwevbacpgjwkvknjnwbdumlgxadnldajmubvdzvjfnzhxwlsmkbqcmvcundgduop") != string("wmexidwrerosbzptetuwevbacpgjwkvknjnwbdumlgxadnldajmubvdzvjfnzhxwlsmkbqcmvcundgduop")) {
		int dindq;
		for (dindq = 70; dindq > 0; dindq--) {
			continue;
		}
	}
	return 47483;
}

void ulggvec::ttvjgqugxnmp(double zrknljqhbmprnla, double ocgdjxvvi, string yoikfkfftthnz, double gnsqyuj) {
	int ayddmgapuuytk = 2859;
	double dsvllwuhkd = 8785;
	double vsrnkjfj = 53356;
	if (8785 != 8785) {
		int bdr;
		for (bdr = 3; bdr > 0; bdr--) {
			continue;
		}
	}
	if (2859 != 2859) {
		int fbygr;
		for (fbygr = 84; fbygr > 0; fbygr--) {
			continue;
		}
	}

}

int ulggvec::bxlntlpjuqimy(string yvuneby, int qfljabnxwxa, int reocjuhvrbbell, int birgwepuardzu, int unzxajzdw, double erpglnet, bool xlvverirx) {
	bool taocuqisbawnd = true;
	if (true != true) {
		int mx;
		for (mx = 84; mx > 0; mx--) {
			continue;
		}
	}
	return 69767;
}

bool ulggvec::pwkeowgdthklzozfmjr(string jqlgtojqltjo, bool zrxtejybnrb, bool vludllnjwrs, bool cvjodwjcds, bool rolhsatpgn, string oowsvfvvijv, bool mnrmeaf, bool kmprbpq, bool ikybrhrfeqmdpf) {
	double pdgvalwycvn = 26218;
	int paqtwrjorpntnc = 794;
	double hhvlbmmphfjndpj = 28199;
	return false;
}

void ulggvec::vsobltymwhhrlfhjgznqpuzk(bool swweoyat, string ldofkebja) {
	double xlaiwfdayy = 64812;
	int gznewqpqla = 972;
	if (972 == 972) {
		int qcnxidkn;
		for (qcnxidkn = 2; qcnxidkn > 0; qcnxidkn--) {
			continue;
		}
	}
	if (64812 != 64812) {
		int jut;
		for (jut = 89; jut > 0; jut--) {
			continue;
		}
	}
	if (972 == 972) {
		int htt;
		for (htt = 44; htt > 0; htt--) {
			continue;
		}
	}
	if (64812 != 64812) {
		int otxygugsuh;
		for (otxygugsuh = 55; otxygugsuh > 0; otxygugsuh--) {
			continue;
		}
	}
	if (64812 != 64812) {
		int zlynec;
		for (zlynec = 81; zlynec > 0; zlynec--) {
			continue;
		}
	}

}

bool ulggvec::dyvsnymlauxvtav(int beioe, double prsxnfkptcsvse, bool bliedld, bool rdmanldyh, bool fqgeazmukscjrhy, double bsjramwonzvy, int bbttj, int loifkoljnad) {
	return false;
}

ulggvec::ulggvec() {
	this->pwkeowgdthklzozfmjr(string("nhaigvxkqpsylgsiskweggyvsbltkrayakpwdybftffknktdcdclgfrumz"), false, false, true, true, string("jdvgzrnspzktcunulkfdmkskabyeg"), true, false, true);
	this->vsobltymwhhrlfhjgznqpuzk(true, string("zbjhfcaoufmgtmwcvyswufmrykgjktyhomxbashcscdiksdv"));
	this->dyvsnymlauxvtav(1268, 2176, true, false, false, 11759, 515, 6393);
	this->sototsrijcrnfozttpf(string("ydqty"), 4377);
	this->ttvjgqugxnmp(49023, 49722, string("qecobjwnpvllzamauwrvtazk"), 22080);
	this->bxlntlpjuqimy(string("qzbumrkheojvyhcobmmruipmxzfivycwyuhbjferp"), 1645, 2601, 9419, 4415, 28583, false);
	this->xbsoaemoboseqhbdcfk(false, false, true, 38, true, true, string("rpnhk"), string("iowsygikfmfhbjkdhdaqkukubvikggfastkprvbkqpdvqoeyylarrvovp"), false, 4198);
	this->unvefzbycodrhinixwof(46517, 1945, true, false);
}
