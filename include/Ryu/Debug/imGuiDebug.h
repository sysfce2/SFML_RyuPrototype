#pragma once 

#include <imgui.h>

#include <iostream>

namespace RyuDebug{

	bool showImGuiDemoWindow;
  bool activateRyuDebug;
  static void CreateDebugGui()
  {
		if(showImGuiDemoWindow) {ImGui::ShowDemoWindow(&showImGuiDemoWindow);}
		
		if(activateRyuDebug)
		{
			//std::cout << "ImGui-Version: " << IMGUI_CHECKVERSION() <<"\n";
     	ImGui::Begin("Heldfsdslo, world!");
     	ImGui::Button("Look at this pretty button");
     	ImGui::End();
		}
  }
}