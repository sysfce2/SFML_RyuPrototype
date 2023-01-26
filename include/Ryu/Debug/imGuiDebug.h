#pragma once 

#include <imgui.h>

#include <iostream>

namespace RyuDebug{

	// TODO: add the "states" to a heederfile (these are the only which are set somewhere else and used here
	bool showImGuiDemoWindow;
  bool activateRyuDebug;
	const char* characterState;
	bool characterIsFalling;

  void ShowWidgets()
  {
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("Ryu Debug Viewer\n" "(right-click to change position)");
    ImGui::Separator();
    if(ImGui::CollapsingHeader("Basic"))
    {
        if (ImGui::IsMousePosValid())
             ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
        else
             ImGui::Text("Mouse Position: <invalid>");
    }
    ImGui::Separator();
    if(ImGui::CollapsingHeader("Character"))
    {
				// ImGui::Text("CharacterState: %s ",characterState);	
				ImGui::TextColored(ImVec4(1.0f,0.0f,0.0f,1.0f),"State: %s",characterState);	
				ImGui::TextColored(ImVec4(1.0f,1.0f,0.0f,1.0f),"CharacterFalling: %s",characterIsFalling ? "true" : "false");	
        ImGui::Separator();
    }
    ImGui::Separator();
  	ImGui::Text("For ImGui-Demo-Window press '-' ");	
  }

  static void CreateDebugGui()
  {
		if(showImGuiDemoWindow) {ImGui::ShowDemoWindow(&showImGuiDemoWindow);}
		
		if(activateRyuDebug)
		{

		static int location = -1;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (location >= 0)
    {
        const float PAD = 10.0f;
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        ImVec2 work_size = viewport->WorkSize;
        ImVec2 window_pos, window_pos_pivot;
        window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
        window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
        window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
        window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    else if (location == -2)
    {
        // Center window                                                                                             
        ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        window_flags |= ImGuiWindowFlags_NoMove;
    }
    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
		if (ImGui::Begin("RyuDebugOverlay", &activateRyuDebug, window_flags))
    {
        ShowWidgets();
		    if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Custom",       NULL, location == -1)) location = -1;
            if (ImGui::MenuItem("Center",       NULL, location == -2)) location = -2;
            if (ImGui::MenuItem("Top-left",     NULL, location == 0)) location = 0;
            if (ImGui::MenuItem("Top-right",    NULL, location == 1)) location = 1;
            if (ImGui::MenuItem("Bottom-left",  NULL, location == 2)) location = 2;
            if (ImGui::MenuItem("Bottom-right", NULL, location == 3)) location = 3;
            if (activateRyuDebug && ImGui::MenuItem("Close")) activateRyuDebug = false;
            ImGui::EndPopup();
        }
    }
    ImGui::End();

		}
  }
}