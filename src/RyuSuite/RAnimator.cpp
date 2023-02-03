#include "RAnimator.h"

#include <imgui.h>
#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>

namespace RyuAnimator{

using namespace ImGui;
using json = nlohmann::json;

namespace AnimationTags {
    void from_json(const json& j, TaggedAnimation& ani) {
        j.at("name").get_to(ani.name);
        j.at("from").get_to(ani.fromFrame);
        j.at("to").get_to(ani.toFrame);
        j.at("direction").get_to(ani.direction);
    }
    // to convert to a json one need to implement the method "to_json(...)"
}


Editor::Editor():
     parsedSpritesheet(false)
    ,selectedSpritesheet()
{}

Editor::~Editor()
{}



void
Editor::parseJsonData()
{
    // TODO: later we select the path / from an openFiledialog / we can load multiple spritesheets
    std::string spriteSheet("ichi_spritesheet_level1");
    std::string path("assets/spritesheets/ichi/");
    std::string format("json");
    std::ifstream f(path+spriteSheet+"."+format);
    
    std::cout << "Open JSON...\n";
    try
    {
        json data = json::parse(f);  
        std::cout << "Parsing JSON...\n";
        std::string jsonString = data.dump();
        std::cout << jsonString << "\n";
        if (data.contains("meta"))
        {
            auto anis = data["meta"]["frameTags"];
            
            std::vector<AnimationTags::TaggedAnimation> aniVector;
            
            for(const auto& a : anis)
            {
                auto taggedAni = a.get<AnimationTags::TaggedAnimation>();
                aniVector.emplace_back(taggedAni);
            }
            
            animations.emplace(spriteSheet, aniVector);              
            
            std::cout << data["meta"]["frameTags"].dump() << "\n";
            parsedSpritesheet = true;
            selectedSpritesheet = spriteSheet;
        }
    }
    catch(json::exception e)
    {
        std::cout << e.id << ": " << e.what() << "\n";
        std::cout << "Can't parse file, probably filestream-error. Filename correct ?\n";
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
          ImGui::BeginChild("left_section",ImVec2(150,0),true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);

          int i = 0;
                                
          for(const auto& ani : animations[selectedSpritesheet])
          {
            i++;
            char label[128];
            sprintf(label,"%d_%s", i,ani.name.c_str());

            if(ImGui::Selectable(label, selected == i)) { selected = i;}
          }
      
          ImGui::EndChild();
        }

        ImGui::SameLine();

        ImGui::BeginGroup();
        if(ImGui::BeginTabBar("SpriteSheets"))
        {
            if(ImGui::BeginTabItem("Bla"))
            {
                ImGui::Text("jhkhkhsk");
                ImGui::EndTabItem();
            }
            if(ImGui::BeginTabItem("Blub"))
            {
                ImGui::Text("hsiggjbkkhkhsk");
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::EndGroup();

        
        ImGui::End();
    }
}


} /// namespace RyuAnimator 