#pragma once
#include "Interface.h"

void RenderDefault()
{
	ImGuiStyle& style = ImGui::GetStyle();


	style.Alpha = 1.00f;               // Global alpha applies to everything in ImGui
	style.WindowPadding = ImVec2(10, 10);     // Padding within a window
	style.WindowMinSize = ImVec2(200, 200);   // Minimum window size
	style.WindowRounding = 0.0f;               // Radius of window corners rounding. Set to 0.0f to have rectangular windows
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f); // wysrodkowanie textu tytulu
	style.ChildWindowRounding = 0.0f;               // Radius of child window corners rounding. Set to 0.0f to have rectangular child windows
	style.FramePadding = ImVec2(4, 3);       // Padding within a framed rectangle (used by most widgets)
	style.FrameRounding = 0.0f;               // Radius of frame corners rounding. Set to 0.0f to have rectangular frames (used by most widgets).
	style.ItemSpacing = ImVec2(5, 5);       // Horizontal and vertical spacing between widgets/lines
	style.ItemInnerSpacing = ImVec2(4, 4);       // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label)
	style.TouchExtraPadding = ImVec2(0, 0);       // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
	style.IndentSpacing = 21.0f;              // Horizontal spacing when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
	style.ColumnsMinSpacing = 6.0f;               // Minimum horizontal spacing between two columns
	style.ScrollbarSize = 16.0f;              // Width of the vertical scrollbar, Height of the horizontal scrollbar
	style.ScrollbarRounding = 9.0f;               // Radius of grab corners rounding for scrollbar
	style.GrabMinSize = 10.0f;              // Minimum width/height of a grab box for slider/scrollbar
	style.GrabRounding = 8.0f;               // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f); // Alignment of button text when button is larger than text.
	style.DisplayWindowPadding = ImVec2(22, 22);     // Window positions are clamped to be visible within the display area by at least this amount. Only covers regular windows.
	style.DisplaySafeAreaPadding = ImVec2(4, 4);       // If you cannot see the edge of your screen (e.g. on a TV) increase the safe area padding. Covers popups/tooltips as well regular windows.
	style.AntiAliasedLines = true;               // Enable anti-aliasing on lines/borders. Disable if you are really short on CPU/GPU.
	style.AntiAliasedShapes = true;               // Enable anti-aliasing on filled shapes (rounded rectangles, circles, etc.)
	style.CurveTessellationTol = 1.25f;              // Tessellation tolerance. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.
















	G::extra_flags = 0;
	style.WindowPadding = ImVec2(10, 10);
	style.WindowRounding = 0.0f;
	style.FramePadding = ImVec2(4, 4);
	style.FrameRounding = 0.00f;
	style.ItemSpacing = ImVec2(5, 5);
	style.ItemInnerSpacing = ImVec2(4, 4);
	style.IndentSpacing = 25.0f;
	style.ScrollbarSize = 9.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 5.0f;
	style.GrabRounding = 0.0f;

	style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.00f);    //ImVec4(0.10196078431f, 0.10196078431f, 0.10196078431f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.94901960784f, 0.29411764705f, 0.7294117647f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.09803921568f, 0.09803921568f, 0.09803921568f, 0.97f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.09803921568f, 0.09803921568f, 0.09803921568f, 0.97f);  // bg listy
																										   //style.Colors[ImGuiCol_PopupBg] = ImVec4(0.06274509803f, 0.08235294117f, 0.15686274509f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.18823529411f, 0.18823529411f, 0.18823529411f, 1.00f);   // border linia to co nizej
	//style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.0f, 1.0f, 1.0f, 0.59f);  // shadow takiej lini obramowej
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.09803921568f, 0.09803921568f, 0.09803921568f, 1.00f);  // t這 pasek fov i przyciskow
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);  // kolor chekboxa po najechaniu
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.27450980392f, 0.27450980392f, 0.27450980392f, 1.00f); // checkbox po przytrzymaniu
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.09411764705f, 0.09411764705f, 0.09411764705f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.12549019607f, 0.12549019607f, 0.12549019607f, 0.98f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.12549019607f, 0.12549019607f, 0.12549019607f, 0.98f); // pasek g鏎ny tam gdzie jest chujowa nazwa
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.12549019607f, 0.12549019607f, 0.12549019607f, 0.98f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.11764705882f, 0.11764705882f, 0.11764705882f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.27450980392f, 0.27450980392f, 0.27450980392f, 1.00f);   //  scroll
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.11764705882f, 0.11764705882f, 0.11764705882f, 1.00f);  ///
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.04705882352f, 0.04705882352f, 0.04705882352f, 1.00f);   // t這 listy
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.18823529411f, 0.18823529411f, 0.18823529411f, 1.00f); // przycisk
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.18823529411f, 0.18823529411f, 0.18823529411f, 1.00f);   // pasek fov 
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.18823529411f, 0.18823529411f, 0.18823529411f, 1.00f); // aktywny pasek fov
	style.Colors[ImGuiCol_Button] = ImVec4(0.19215686274f, 0.19215686274f, 0.19215686274f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);  // kolor button jak sie na niego najedzie
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.27450980392f, 0.27450980392f, 0.27450980392f, 1.00f);    // aktywna lista
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);  // po najechaniu
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.28450980392f, 0.28450980392f, 0.28450980392f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.09803921568f, 0.09803921568f, 0.09803921568f, 0.97f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.09803921568f, 0.09803921568f, 0.09803921568f, 0.97f);  // idk cos z lista
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.27450980392f, 0.27450980392f, 0.27450980392f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 0.00f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.03f, 0.03f, 0.03f, 1.00f);   //
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.03f, 0.03f, 0.03f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.03f, 0.03f, 0.03f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.89f, 0.76f, 0.38f, 0.63f);  // idk
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.09803921568f, 0.09803921568f, 0.09803921568f, 0.97f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);  // idk
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.09803921568f, 0.09803921568f, 0.09803921568f, 0.97f);  // idk
}

void RenderOld()
{
	ImGuiStyle& style = ImGui::GetStyle();


	style.Alpha = 1.00f;               // Global alpha applies to everything in ImGui
	style.WindowPadding = ImVec2(10, 10);     // Padding within a window
	style.WindowMinSize = ImVec2(200, 200);   // Minimum window size
	style.WindowRounding = 5.0f;               // Radius of window corners rounding. Set to 0.0f to have rectangular windows
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f); // wysrodkowanie textu tytulu
	style.ChildWindowRounding = 1.0f;               // Radius of child window corners rounding. Set to 0.0f to have rectangular child windows
	style.FramePadding = ImVec2(4, 3);       // Padding within a framed rectangle (used by most widgets)
	style.FrameRounding = 2.0f;               // Radius of frame corners rounding. Set to 0.0f to have rectangular frames (used by most widgets).
	style.ItemSpacing = ImVec2(5, 5);       // Horizontal and vertical spacing between widgets/lines
	style.ItemInnerSpacing = ImVec2(4, 4);       // Horizontal and vertical spacing between within elements of a composed widget (e.g. a slider and its label)
	style.TouchExtraPadding = ImVec2(0, 0);       // Expand reactive bounding box for touch-based system where touch position is not accurate enough. Unfortunately we don't sort widgets so priority on overlap will always be given to the first widget. So don't grow this too much!
	style.IndentSpacing = 21.0f;              // Horizontal spacing when e.g. entering a tree node. Generally == (FontSize + FramePadding.x*2).
	style.ColumnsMinSpacing = 6.0f;               // Minimum horizontal spacing between two columns
	style.ScrollbarSize = 16.0f;              // Width of the vertical scrollbar, Height of the horizontal scrollbar
	style.ScrollbarRounding = 9.0f;               // Radius of grab corners rounding for scrollbar
	style.GrabMinSize = 10.0f;              // Minimum width/height of a grab box for slider/scrollbar
	style.GrabRounding = 8.0f;               // Radius of grabs corners rounding. Set to 0.0f to have rectangular slider grabs.
	style.ButtonTextAlign = ImVec2(0.5f, 0.5f); // Alignment of button text when button is larger than text.
	style.DisplayWindowPadding = ImVec2(22, 22);     // Window positions are clamped to be visible within the display area by at least this amount. Only covers regular windows.
	style.DisplaySafeAreaPadding = ImVec2(4, 4);       // If you cannot see the edge of your screen (e.g. on a TV) increase the safe area padding. Covers popups/tooltips as well regular windows.
	style.AntiAliasedLines = true;               // Enable anti-aliasing on lines/borders. Disable if you are really short on CPU/GPU.
	style.AntiAliasedShapes = true;               // Enable anti-aliasing on filled shapes (rounded rectangles, circles, etc.)
	style.CurveTessellationTol = 1.25f;              // Tessellation tolerance. Decrease for highly tessellated curves (higher quality, more polygons), increase to reduce quality.
















	G::extra_flags = 0;
	style.WindowPadding = ImVec2(10, 10);
	style.WindowRounding = 15.0f;
	style.FramePadding = ImVec2(4, 4);
	style.FrameRounding = 6.00f;
	style.ItemSpacing = ImVec2(9, 5);
	style.ItemInnerSpacing = ImVec2(8, 4);
	style.IndentSpacing = 25.0f;
	style.ScrollbarSize = 0.0f;
	style.ScrollbarRounding = 0.0f;
	style.GrabMinSize = 9.0f;
	style.GrabRounding = 5.0f;

	style.Colors[ImGuiCol_Text] = ImVec4(0.5477f, 0.214543453454f, 0.0053424f, 1.00f);    //ImVec4(0.10196078431f, 0.10196078431f, 0.10196078431f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.8777777657f, 0.876576f, 0.465464567867f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(1.0f, 0.06546456456456f, 0.56078431372f, 0.99f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.654666666664705882f, 0.8968882f, 0.6768768764705882f, 1.00f);  // bg listy
																										   //style.Colors[ImGuiCol_PopupBg] = ImVec4(0.06274509803f, 0.08235294117f, 0.15686274509f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.27654, 0.679692f, 0.68767868792f, 1.00f);   // border linia to co nizej
																									 //style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.27450980392f, 0.27450980392f, 0.27450980392f, 1.00f);  // shadow takiej lini obramowej
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.11764705882f, 0.11764705882f, 0.11764705882f, 0.97f);   // t這 pasek fov i przyciskow
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.68767678687879f, 0.58735469f, 0.69f, 1.00f);  // kolor chekboxa po najechaniu
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.687676877450980392f, 0.35467827450980392f, 0.27450980392f, 1.00f); // checkbox po przytrzymaniu
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.6876876873f, 0.876876876f, 0.03f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.03f, 0.03f, 0.03f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.03f, 0.03f, 0.03f, 1.00f); // pasek g鏎ny tam gdzie jest chujowa nazwa
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.06274509803f, 0.08235294117f, 0.15686274509f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.11764705882f, 0.11764705882f, 0.11764705882f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.27450980392f, 0.27450980392f, 0.27450980392f, 1.00f);   //  scroll
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.11764705882f, 0.11764705882f, 0.11764705882f, 1.00f);  ///
	style.Colors[ImGuiCol_ComboBg] = ImVec4(0.98882f, 0.654705882f, 0.687705882f, 1.00f);   // t這 listy
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.9792f, 0.6542f, 0.27450980392f, 1.00f);  // przycisk
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.27450980392f, 0.27450980392f, 0.27450980392f, 1.00f);   // pasek fov 
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.27450980392f, 0.27450980392f, 0.27450980392f, 1.00f);  // aktywny pasek fov
	style.Colors[ImGuiCol_Button] = ImVec4(0.8773137f, 0.645843137f, 0.19879783137f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);  // kolor button jak sie na niego najedzie
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.27450980392f, 0.27450980392f, 0.27450980392f, 1.00f);    // aktywna lista
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.69f, 0.69f, 0.69f, 1.00f);  // po najechaniu
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.28450980392f, 0.28450980392f, 0.28450980392f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.5325235506274509803f, 0.999908235294117f, 0.999915686274509f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.432432427f, 0.43242342f, 0.532523f, 1.00f);  // idk cos z lista
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.27450980392f, 0.27450980392f, 0.27450980392f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 0.00f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
	style.Colors[ImGuiCol_CloseButton] = ImVec4(0.03f, 0.03f, 0.03f, 1.00f);   //
	style.Colors[ImGuiCol_CloseButtonHovered] = ImVec4(0.03f, 0.03f, 0.03f, 1.00f);
	style.Colors[ImGuiCol_CloseButtonActive] = ImVec4(0.03f, 0.03f, 0.03f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.89f, 0.76f, 0.38f, 0.63f);  // idk
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);  // idk
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.52f, 0.83429f, 0.5321316f, 1.00f);  // idk
}