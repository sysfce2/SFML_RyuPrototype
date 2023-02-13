#include "RAnimator.h"
#include <Ryu/Animation/SpritesheetAnimation.h>

#include <bits/stdint-uintn.h>
#include <imgui.h>
#include <imgui-SFML.h>
#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

    void from_json(const json& j, Frame& frame) {
        j.at("frame").at("x").get_to(frame.x);
        j.at("frame").at("y").get_to(frame.y);
        j.at("frame").at("w").get_to(frame.width);
        j.at("frame").at("h").get_to(frame.height);
        j.at("duration").get_to(frame.duration);
    }
    // to convert to a json one need to implement the method "to_json(...)"
}


Editor::Editor():
     parsedSpritesheet(false)
    ,selectedSpritesheet()
    ,showAnimationEditor(true)
    ,guiCharTextureManager()
    ,aniIsPlaying(false)
    ,textureSet(false)
{
    initTextures();
}

Editor::~Editor()
{}


void
Editor::update(sf::Time dt)
{
    if(aniIsPlaying)
    {
        spritesheetAnimation.update(dt);
    }
}

/* \brief: At the moment we can only parse jsons generated with Aseprite, but it could be possible to extend to
*          other PixelArtTools.
*          Before exporting please make sure that every Animation has exactly one tag. It will be confusing
*          if animations occur in the list which according tags only were created to describe parent states.
*          e.g. "Climbing" as a parent tag, "startClimb", "loopClimb", "endClimb" as childrens.  
*          We only would be interested in the child animations !
*/
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
        // build-up metadata
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

            // just for testing the tabs a second spritesheet
            aniVector.resize(11);              
            animations.emplace("spritesheet2", aniVector);              
        }            
        selectedSpritesheet = spriteSheet;
        // read framespecific data due aseprite-json spec
        if (data.contains("frames"))
        {
            for(auto& ani : animations[selectedSpritesheet])
            {
                for(int i = ani.fromFrame;i<=ani.toFrame;i++)
                {
                    std::string framePosition = selectedSpritesheet+" "+std::to_string(i)+".aseprite";
                    AnimationTags::Frame frame{
                        .duration = data["frames"][framePosition]["duration"],
                        .height = data["frames"][framePosition]["frame"]["h"], 
                        .width = data["frames"][framePosition]["frame"]["w"],
                        .x = data["frames"][framePosition]["frame"]["x"],
                        .y = data["frames"][framePosition]["frame"]["y"]
                    };
                    ani.frames.push_back(frame);
                }
            }
        }
        
        std::cout << data["meta"]["frameTags"].dump() << "\n";
        parsedSpritesheet = true;
        
    }
    catch(json::exception e)
    {
        std::cout << e.id << ": " << e.what() << "\n";
        std::cout << "Can't parse file, probably filestream-error. Filename correct ?\n";
    }
    
}

void
Editor::initTextures()
{
    guiCharTextureManager.load(Textures::LevelID::Level1,"assets/spritesheets/ichi/ichi_spritesheet_level1.png");
}


void
Editor::createEditorWidgets(bool* p_open)
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar;
    
    if(showAnimationEditor)
    {
        if(ImGui::Begin("Ryu Animation Editor",p_open, window_flags))
        {

            static int selected = 0;
           
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
          ImGui::BeginChild("left_section",ImVec2(150,0),true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);

          int i = 0;
                                
          for(const auto& ani : animations[selectedSpritesheet])
          {
            char label[128];
            sprintf(label,"%d_%s", i,ani.name.c_str());
            if(ImGui::Selectable(label, selected == i)) 
            { 
                  selected = i;
            }
            i++;
          }
      
          ImGui::EndChild();
        

        ImGui::SameLine();

        // Right
        ImGui::BeginGroup();
        if(ImGui::BeginTabBar("SpriteSheets"))
        {
            for (const auto& sheet : animations)
            {
                 if(sheet.first == "") continue;
                 if(ImGui::BeginTabItem(sheet.first.c_str()))
                 {
                     selectedSpritesheet=sheet.first;
                     createAnimationDetails(selected, sheet);   
                     ImGui::EndTabItem();
                 }
            }
            ImGui::EndTabBar();
        }

        ImGui::EndGroup();

      }
      ImGui::End();
    }
}

void 
Editor::createAnimationDetails(int selectedAni, const TaggedSheetAnimation& sheet )
{
    auto ani = sheet.second.at(selectedAni);
    ImGui::Text((std::to_string(selectedAni)+": "+ani.name+", ").c_str());
    ImGui::SameLine();
    ImGui::Text("Frames : %s, ", (std::to_string(ani.frames.size()).c_str()));
    int i = 1;
    uint16_t durationAni = 0;

    

    // TODO: details as mouseoverhint / Frame !
    for (const auto& frame : ani.frames)
    {
        durationAni+= frame.duration;   
        /* MouseOver for the induvidual frames 
        ImGui::Text("Duration(%d): %s ms",i, (std::to_string(frame.duration).c_str()));
        ImGui::Text("height(%d): %s",i, (std::to_string(frame.height).c_str()));
        ImGui::Text("width(%d): %s",i, (std::to_string(frame.width).c_str()));
        ImGui::Text("x-sheet(%d): %s",i, (std::to_string(frame.x).c_str()));
        ImGui::Text("y-sheet(%d): %s",i, (std::to_string(frame.y).c_str()));
        */
        i++;
        // ImGui::Separator();
    }
    ImGui::SameLine();
    
    ImGui::Text("Ani-Duration: %d ms", durationAni);    
    uint16_t frameWidth = ani.frames.at(0).width;
    uint16_t frameHeight = ani.frames.at(0).height;
    
    ImGui::BeginChild("Animation",ImVec2(frameWidth,frameHeight),true,ImGuiWindowFlags_NoScrollbar);
        // loading an image is not so straight forward: see here: https://github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples

    // TODO: here dynamic anis due selection from the right side
    setSpritesheetAnimationDetails({
                .frameSize={80,96}
               ,.startFrame={1,0}
               ,.numFrames=8
               ,.duration = sf::seconds(1)
               ,.repeat = true
               ,.animationId = Textures::CharacterID::IchiIdleRun});
    ImGui::Image(spritesheetAnimation.getSprite()/*guiCharTextureManager.getResource(Textures::LevelID::Level1)*/);
    // ImGui::ShowMetricsWindow();
    ImGui::EndChild();
    ImGui::SameLine();

    // TODO: adjust values due Spritesheetsize and FrameSize
    ImGui::BeginChild("SpriteSheet", ImVec2(400,384),true,ImGuiWindowFlags_HorizontalScrollbar ); /// orig. 1040x960

    ImGui::Image(guiCharTextureManager.getResource(Textures::LevelID::Level1));
    ImGui::EndChild();
    ImGui::BeginChild("PlayButton");
    ImGuiComboFlags flags = 0;
    // TODO us real play/stop etc button .... tmp play button is a arrowbutton
    if(ImGui::ArrowButton("Play",ImGuiDir_Right)) {aniIsPlaying = ! aniIsPlaying;}
   
    ImGui::EndChild();
}




void
Editor::setSpritesheetAnimationDetails(const AnimationConfig& config)
{   
    
    spritesheetAnimation.setFrameSize(config.frameSize);
    spritesheetAnimation.setStartFrame({config.frameSize.x * config.startFrame.x, config.frameSize.y * config.startFrame.y});
    spritesheetAnimation.setNumFrames(config.numFrames);
    spritesheetAnimation.setDuration(config.duration);
    spritesheetAnimation.setRepeating(config.repeat);

    // TODO: check if texture is set for animation ...
    spritesheetAnimation.setTexture(guiCharTextureManager.getResource(Textures::LevelID::Level1/*config.spritesheetId*/));
    textureSet = true;
    // set origin of texture to center
    sf::FloatRect bounds = spritesheetAnimation.getSprite().getLocalBounds();
    spritesheetAnimation.getSprite().setOrigin(bounds.width / 2.f, bounds.height / 2.f);

}


} /// namespace RyuAnimator 