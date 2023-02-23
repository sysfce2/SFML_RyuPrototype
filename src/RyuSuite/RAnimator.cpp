#include "RAnimator.h"
#include <Ryu/Animation/SpritesheetAnimation.h>

#include <SFML/System/Time.hpp>
#include <bits/stdint-intn.h>
#include <bits/stdint-uintn.h>
#include <cstddef>
#include <imgui.h>
#include <imgui-SFML.h>
#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility> /// std::pair

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
    ,guiTextureManager()
    ,aniIsPlaying(false)
    ,textureSet(false)
{
    initTextures();
}

Editor::~Editor()
{}

constexpr std::pair<float,float> frameSize{20.f,35.f};
constexpr std::pair<float,float> frameAreaSize{860.f,180.f};
constexpr std::pair<float,float> aniAreaSize{250.f,400.f};
static bool frameDetailsVisible;

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
                    if (ImGui::MenuItem("Read JSON (Aseprite)")) 
                    {
                        // TODO: here later something like open a dialog window
                        parseJsonData();
                    }
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
                 if(sheet.first == "") continue; /// otherwise we create an empty tab
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
Editor::setFrameDetails()
{
    if(frameDetailsVisible)
    {
        static int intDuration;
    
        ImGui::InputInt("Duration",&intDuration );
    }
}

void 
Editor::createAnimationDetails(int selectedAni, const TaggedSheetAnimation& sheet )
{
    auto ani = sheet.second.at(selectedAni);
    ImGui::Text((std::to_string(selectedAni)+": "+ani.name+", ").c_str());
    ImGui::SameLine();
    ImGui::Text("Frames : %s, ", (std::to_string(ani.frames.size()).c_str()));
    size_t i = 0;
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
    
    // ImGui::BeginChild("Animation",ImVec2(frameWidth,frameHeight),true,ImGuiWindowFlags_NoScrollbar);
    ImGui::BeginChild("Animation",ImVec2(aniAreaSize.first,aniAreaSize.second),true,ImGuiWindowFlags_NoScrollbar);

    int16_t startX = ani.frames.at(0).x / frameWidth; // for the spritesheetAnimationDetails
    int16_t startY = ani.frames.at(0).y / frameHeight;// for the spritesheetAnimationDetails
    

    // TODO: here dynamic anis due selection from the right side
    setSpritesheetAnimationDetails({
                .frameSize={frameWidth,frameHeight}
               ,.startFrame={startX,startY}
               ,.numFrames=i
               ,.duration = sf::milliseconds(durationAni)
               ,.repeat = true ///  TODO from editor ui: entered by user
               ,.animationId = Textures::CharacterID::IchiIdleRun});///  TODO from editor ui: entered by user    
    ImGui::Image(spritesheetAnimation.getSprite()/*guiCharTextureManager.getResource(Textures::LevelID::Level1)*/);
    // ImGui::ShowMetricsWindow();
    
    setFrameDetails();

    ImGui::EndChild();

    
    ImGui::SameLine();

    // TODO: adjust values due Spritesheetsize and FrameSize
    ImGui::BeginChild("SpriteSheet", ImVec2(800,400),true,ImGuiWindowFlags_HorizontalScrollbar ); /// orig. 1040x960

    ImGui::Image(guiCharTextureManager.getResource(Textures::LevelID::Level1));
    ImGui::EndChild();
    ImGui::BeginChild("PlayButton");
    ImGuiComboFlags flags = 0;
    // TODO us real play/stop etc button .... tmp play button is a arrowbutton
    
    //if(ImGui::ArrowButton("Play",ImGuiDir_Right)) {aniIsPlaying = ! aniIsPlaying;}
    if(ImGui::ImageButton(guiTextureManager.getResource(Textures::GuiID::StartFrame)))
        {}
    ImGui::SameLine();
    if(ImGui::ImageButton(guiTextureManager.getResource(Textures::GuiID::BackwardFrame)))
        {}
    ImGui::SameLine();
    if(ImGui::ImageButton(guiTextureManager.getResource(Textures::GuiID::Play)))
    {
        aniIsPlaying = ! aniIsPlaying;         
    }
    ImGui::SameLine();
    if(ImGui::ImageButton(guiTextureManager.getResource(Textures::GuiID::ForwardFrame)))
        {}
    ImGui::SameLine();
    if(ImGui::ImageButton(guiTextureManager.getResource(Textures::GuiID::EndFrame)))
        {}

    ImGui::BeginChild("TimeLine",ImVec2(frameAreaSize.first,frameAreaSize.second),ImGuiWindowFlags_AlwaysVerticalScrollbar);
        size_t j = 1;
        float currentFrameAreaX = 0;
        for (auto& f : ani.frames)
        {
            //if(ImGui::ImageButton(guiTextureManager.getResource(Textures::GuiID::Frame)))
            if(ImGui::Button(std::to_string(j).c_str(),ImVec2(frameSize.first,frameSize.second)))
            {
                editFrame(i);
            }
            if(currentFrameAreaX < frameAreaSize.first)
            {
                currentFrameAreaX += (frameSize.first+10.f);
                std::cout << currentFrameAreaX << "\n";
                ImGui::SameLine();
            }
            else
            {
                currentFrameAreaX = 0;
            }
            ++j;
        }
    ImGui::EndChild();
    
    ImGui::EndChild();
}


void
Editor::editFrame(size_t frame)
{
    frameDetailsVisible =! frameDetailsVisible;
}

void
Editor::initTextures()
{
    // TODO: this could also be done when oping the spritesheet through a dialog ... whe the size of the spritesheets become bigger this will be a memory killer
    guiCharTextureManager.load(Textures::LevelID::Level1,"assets/spritesheets/ichi/ichi_spritesheet_level1.png");
    guiTextureManager.load(Textures::GuiID::ForwardFrame,"assets/gui/animator/06_nextFrame.jpeg");
    guiTextureManager.load(Textures::GuiID::BackwardFrame,"assets/gui/animator/03_previousFrame.jpeg");
    guiTextureManager.load(Textures::GuiID::Play,"assets/gui/animator/04_playAni.jpeg");
    guiTextureManager.load(Textures::GuiID::Stop,"assets/gui/animator/05_stopAni.jpeg");
    guiTextureManager.load(Textures::GuiID::StartFrame,"assets/gui/animator/02_startFrame.jpeg");
    guiTextureManager.load(Textures::GuiID::EndFrame,"assets/gui/animator/07_lastFrame.jpeg");
    guiTextureManager.load(Textures::GuiID::Frame,"assets/gui/animator/01_frame.jpeg");
    
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