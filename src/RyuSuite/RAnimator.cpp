#include "RAnimator.h"

#include <imgui.h>
#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>

namespace RyuAnimator{

using namespace ImGui;
using json = nlohmann::json;

Editor::Editor()
{}

Editor::~Editor()
{}

void
Editor::parseJsonData()
{
// ...
    std::ifstream f("assets/spritesheets/ichi/ichi_spritesheet_level1.json");
    
    std::cout << "Open JSON...\n";
    if(f)
    {
        json data = json::parse(f);  
        std::cout << "Parsing JSON...\n";
        std::string jsonString = data.dump();
        std::cout << jsonString << "\n";
    }
}


void
Editor::createEditorWidgets(bool* p_open)
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    
    if(showAnimationEditor)
    {
        if(ImGui::Begin("Ryu Animation Editor",p_open, window_flags))
        {
            if(ImGui::BeginMenuBar())
            {
                if(ImGui::BeginMenu("File"))
                {
                    ImGui::MenuItem("Menu",NULL,false,false);
                    if (ImGui::MenuItem("Read JSON (Aseprite)")) {parseJsonData();}
                    if (ImGui::MenuItem("Close","Strg+W")) {*p_open = false;}
                    ImGui::EndMenu();
                }
                ImGui::EndMenuBar();
            }

          // Left
          static int selected = 0;
          ImGui::BeginChild("left_section",ImVec2(100,0),true);
          for(int i = 0; i < 20; i++)
          {
            char label[128];
            sprintf(label,"Ani_%d", i);
            if(ImGui::Selectable(label, selected == i)) { selected = i;}
          }
      
          ImGui::EndChild();
        }
        ImGui::End();
    }
}


} /// namespace RyuAnimator 