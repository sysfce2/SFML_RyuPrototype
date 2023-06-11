#include <RyuSuite/RAnimator.h>
#include <Ryu/Animation/SpritesheetAnimation.h>
#include <Ryu/Events/EventEnums.h>
#include <Ryu/Animation/EditorEnums.h>
#include <Ryu/Animation/JsonParser.h>

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <bits/stdint-intn.h>
#include <bits/stdint-uintn.h>
#include <cstddef>
#include <imgui.h>
#include <imgui-SFML.h>
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include <fmt/format.h>


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility> /// std::pair

namespace RyuAnimator{

using namespace ImGui;
using json = nlohmann::json;

// namespace RyuParser {
namespace AnimationSpec {
    void from_json(const json& j, Animation& ani) {
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


    void to_json(json& j, const Frame& frame){
        // json jEvent = frame.event;
        j = json{
            {"duration",frame.duration},
            {"height",frame.height},
            {"width",frame.width},
            {"x_sheet",frame.x},
            {"y_sheet",frame.x},
            {"event",frame.event._to_string()}
                
        };
    }
/*
    struct Animation
    {
      std::string name;
      int16_t fromFrame; /// Frame Startposition in spritesheet
      int16_t toFrame; /// Frame Endposition in spritresheet
      std::string direction;
      std::vector<Frame> frames;
      std::size_t numFrames; // == TaggedAnimation::toFrame - TaggedAnimation::fromFrame
      sf::Vector2i frameSize;
      sf::Time animationDuration;
      bool repeat;
      Textures::CharacterID animationId;
    };
    struct Spec{
      std::string specName;
      std::string spritesheetName;
      std::vector<Animation> animations; 
    }; 
*/
 
    void to_json(json& j, const Animation& ani) {
    // TODO: fill some fields with corect values / add somehoe lienbreaks to the json file ?
      std::string timeInMs{
          std::to_string(ani.animationDuration.asMilliseconds()) + " ms"};
    
      json jFrames = ani.frames; 
      j = json{{"Name", ani.name},
               {"Sheet_begin", ani.fromFrame},
               {"Sheet_end", ani.toFrame},
               {"AnimationDirection", ani.direction},
               {"numFrames", ani.numFrames},
               {"Frames", jFrames},
               {"FrameSize",
                {{"height", ani.frameSize.y}, {"width", ani.frameSize.x}}},
               {"animationDuration", timeInMs},
               {"repeat", ani.repeat},
               {"AnimationId", ani.animationId._to_string()}};
    }
} /// namespace AnimationSpec


// namespace RyuAnimator{

Editor::Editor():
     parsedSpritesheet(false)
    ,selectedSpritesheet()
    ,showAnimationEditor(true)
    ,guiCharTextureManager()
    ,guiTextureManager()
    ,aniIsPlaying(false)
    ,textureSet(false)
    ,preferences()
{
    initTextures();
    initData();
}

Editor::~Editor()
{}

constexpr std::pair<float,float> frameSize{20.f,35.f}; 
constexpr std::pair<float,float> frameAreaSize{705.f,180.f};
constexpr std::pair<float,float> aniAreaSize{250.f,400.f};
static bool frameDetailsVisible=true;
// content of the selected Frame
/* FrameDetails-Section */
static int intDuration;
static int selectedFrame;
static int currentEventItem = 0;
static int currentActiveFrame = 0;

// TODO: dynamically initialize array ? -> here elements needs to be iniatilized manually ^^
const char* eventItems[] = {"","","","",""};

static sf::Vector2i sheetPosition{};


void
Editor::initData()
{
    int i = 0;
    for (EEvent evt : EEvent::_values())
    {
        eventItems[i] = evt._to_string();
        ++i;
    }
}

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
            
            std::vector<AnimationSpec::Animation> aniVector;
            
            for(const auto& a : anis)
            {
                auto ani = a.get<AnimationSpec::Animation>();
                aniVector.emplace_back(ani);
            }
            
            animations.emplace(spriteSheet, aniVector);

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
                    AnimationSpec::Frame frame{
                        .duration = data["frames"][framePosition]["duration"],
                        .height = data["frames"][framePosition]["frame"]["h"], 
                        .width = data["frames"][framePosition]["frame"]["w"],
                        .x = data["frames"][framePosition]["frame"]["x"],
                        .y = data["frames"][framePosition]["frame"]["y"],
                        .event = EEvent::None,
                    };
                    ani.frames.push_back(frame);
                }
            }
            
            setAnimationPreferences(selectedSpritesheet);
        
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
Editor::parseJsonFile()
{
    RyuParser::JsonParser jParser;
    std::string file("ichi.json");
    jParser.getAnimationsFromJson(file);
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

          if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
              ImGui::MenuItem("Menu", NULL, false, false);
              if (ImGui::MenuItem("Read JSON (Aseprite)")) {
                // TODO: here later something like open a dialog window
                parseJsonData();
              }
              if (ImGui::MenuItem("Open Json ...")){
                parseJsonFile();
              }
                    
              if (ImGui::MenuItem("Close", "Strg+W")) {
                *p_open = false;
              }
              ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
          }

          // Left
          ImGui::BeginChild("left_section",ImVec2(150,0),true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_AlwaysVerticalScrollbar);

          int i = 0;
                                
          for(auto& ani : animations[selectedSpritesheet])
          {
            char label[128];
            sprintf(label,"%d_%s", i,ani.name.c_str());
            if(ImGui::Selectable(label, selected == i)) 
            { 
                  selected = i;
                  // intDuration = 0;
                  selectedFrame = 1;
                  // currentEventItem = 0;
                  editFrame(ani,selectedFrame);
            }
            i++;
          }
      
          ImGui::EndChild();
        

        ImGui::SameLine();

        // Right
        ImGui::BeginGroup();
        if(ImGui::BeginTabBar("SpriteSheets"))
        {
            for (auto& sheet : animations)
            {
                 if(sheet.first == "") continue; /// otherwise we create an empty tab
                 if(ImGui::BeginTabItem(sheet.first.c_str()))
                 {
                     selectedSpritesheet=sheet.first;
                     setAnimationPreferences(sheet.first);
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
Editor::setTooltipText(const char * tooltip="- not implemented -")
{
    if(ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(tooltip);
    }
}

// we need the animation duration before we click on a animation (showing details) bc it can/will be that 
// we instant want to export the data to json -
void
Editor::setAnimationPreferences(std::string sheetName)
{
    if(preferences["animationDurationSet_"+sheetName])
    {
        return;
    }    

    preferences["animationDurationSet_"+sheetName] = true;
    
    std::cout << "SHEET: " << sheetName << "\n";
    // std::cout << sheetName << ": " << aniName << ": duration " << std::to_string(duration) << "\n";
    for(auto& ani : animations[sheetName])
    {
        size_t i = 0;
        uint16_t durationAni = 0;
        for(const auto& frame : ani.frames)
        {
            i++;
            durationAni+= frame.duration;
        }
        std::cout << "Ani: " << ani.name << ": " << std::to_string(durationAni) << "ms \n";
        ani.animationDuration = sf::milliseconds(durationAni);
        ani.numFrames = i;
        ani.frameSize.y = ani.frames.at(0).height;
        ani.frameSize.x = ani.frames.at(0).width;

        // TODO: set Anidirection / AniId / repeat
    }
    
}

void
Editor::calculateAnimationDuration(AnimationSpec::Animation& ani)
{
    int32_t overallDuration = 0;
    for(const auto frame : ani.frames)
    {
        overallDuration+=frame.duration;
    }

    if(intDuration != overallDuration)
    {
        ani.animationDuration = sf::milliseconds(overallDuration);
    }
}


// Note: when editinmg timing in the animation please note that changing the time in frame 1 leads to a weird overall timing, dont know why :/
void 
Editor::createAnimationDetails(int selectedAni, TaggedSheetAnimation& sheet)
{
    auto ani = sheet.second.at(selectedAni);
    ImGui::Text((std::to_string(selectedAni)+": "+ani.name+", ").c_str());
    ImGui::SameLine();
    ImGui::Text("Frames : %s, ", (std::to_string(ani.frames.size()).c_str()));
    ImGui::SameLine();
    calculateAnimationDuration(ani);
    ImGui::Text("Ani-Duration: %d ms", ani.animationDuration.asMilliseconds());    
    ImGui::SameLine();
    ImGui::Text("CurrentFrame: %s / %s", std::to_string(currentActiveFrame).c_str(),std::to_string(ani.frames.size()).c_str());
    
    uint16_t frameWidth = ani.frames.at(0).width;
    uint16_t frameHeight = ani.frames.at(0).height;
    
    ImGui::BeginChild("Animation",ImVec2(aniAreaSize.first,aniAreaSize.second),true,ImGuiWindowFlags_NoScrollbar);

    int16_t startX = ani.frames.at(0).x / frameWidth; // for the spritesheetAnimationDetails
    int16_t startY = ani.frames.at(0).y / frameHeight;// for the spritesheetAnimationDetails

    // TODO: here dynamic anis due selection from the right side
    setSpritesheetAnimationDetails({
                .frameSize={frameWidth,frameHeight}
               ,.startFrame={startX,startY}
               ,.numFrames= ani.numFrames
               ,.duration = ani.animationDuration
               ,.repeat = true ///  TODO from editor ui: entered by user
               ,.animationId = Textures::CharacterID::IchiIdleRun}, ani.animationDuration,  ani.frames);///  TODO from editor ui: entered by user    
    ImGui::Image(spritesheetAnimation.getSprite()/*guiCharTextureManager.getResource(Textures::LevelID::Level1)*/);
    currentActiveFrame =  spritesheetAnimation.getCurrentFrame()+1;
    // std::cout << currentActiveFrame << " set AniPrefs(dur in ms):" << spritesheetAnimation.getDuration().asMilliseconds() << "\n" ;
    // ImGui::ShowMetricsWindow();
    
    setFrameDetails(selectedAni,sheet,selectedFrame,sheet.second.at(selectedAni));

    ImGui::EndChild();

    
    ImGui::SameLine();

    // TODO: adjust values due Spritesheetsize and FrameSize
    ImGui::BeginChild("SpriteSheet", ImVec2(650,400),true,ImGuiWindowFlags_HorizontalScrollbar ); /// orig. 1040x960

    // sf::Texture* texture = &(guiTextureManager.getResource(Textures::GuiID::FrameBorder));
    ImVec2 pos = ImGui::GetCursorScreenPos();//ImVec2(100,200);//

    // std::cout << "Pos_ori: " << pos.x << "|" << pos.y << "\n";

    if(currentActiveFrame-1 <= ani.frames.size())
    {
        pos.x = pos.x + ani.frames.at(currentActiveFrame-1).x;
        pos.y = pos.y + ani.frames.at(currentActiveFrame-1).y;
    }

    
    // std::cout << "Pos_new: " << pos.x << "|" << pos.y << "\n";
    
    // ImGui::GetWindowDrawList()->AddImage((void*)texture, pos,ImVec2(pos.x+80,pos.y+96)/* ImVec2(800, 600),ImVec2(880, 696)*/, ImVec2(0,0), ImVec2(1,1),IM_COL32_A_MASK);
    ImGui::GetWindowDrawList()->AddRectFilled(pos,ImVec2(pos.x+80,pos.y+96),IM_COL32(127,0,0,255));//L32_WHITE);

    ImGui::Image(guiCharTextureManager.getResource(Textures::LevelID::Level1));
    // hm how to add image oveer another with SFML?
    
     // std::cout << "whygetcursoscreenpos???\n";
    ImGui::EndChild();
    ImGui::BeginChild("PlayButton");
    ImGuiComboFlags flags = 0;
    // TODO us real play/stop etc button .... tmp play button is a arrowbutton
    
    //if(ImGui::ArrowButton("Play",ImGuiDir_Right)) {aniIsPlaying = ! aniIsPlaying;}
    if(ImGui::ImageButton(guiTextureManager.getResource(Textures::GuiID::StartFrame)))
    {
    }
    setTooltipText();
    ImGui::SameLine();
    if(ImGui::ImageButton(guiTextureManager.getResource(Textures::GuiID::BackwardFrame)))
    {}
    setTooltipText();
    ImGui::SameLine();
    if(!aniIsPlaying)
    {
        if(ImGui::ImageButton(guiTextureManager.getResource(Textures::GuiID::Play)))
        {
            aniIsPlaying = true;         
        }
    }
    else{
        if(ImGui::ImageButton(guiTextureManager.getResource(Textures::GuiID::Stop)))
        {
            aniIsPlaying = false;         
        }
    }
    ImGui::SameLine();
    if(ImGui::ImageButton(guiTextureManager.getResource(Textures::GuiID::ForwardFrame)))
    {}
    setTooltipText();
    ImGui::SameLine();
    if(ImGui::ImageButton(guiTextureManager.getResource(Textures::GuiID::EndFrame)))
    {
    }
    setTooltipText();

    // show frames as a timeline
    ImGui::BeginChild("TimeLine",ImVec2(frameAreaSize.first,frameAreaSize.second),ImGuiWindowFlags_AlwaysVerticalScrollbar);
        size_t j = 1;
        float currentFrameAreaX = 0;
        for (auto& f : ani.frames)
        {
            //if(ImGui::ImageButton(guiTextureManager.getResource(Textures::GuiID::Frame)))
            if(ImGui::Button(std::to_string(j).c_str(),ImVec2(frameSize.first,frameSize.second)))
            {
                selectedFrame = j;
                editFrame(ani,selectedFrame);
            }
            setTooltipText("click for details");
            if(currentFrameAreaX < frameAreaSize.first)
            {
                currentFrameAreaX += (frameSize.first+10.f);
                //std::cout << currentFrameAreaX << "\n";
                ImGui::SameLine();
            }
            else
            {
                currentFrameAreaX = 0;
            }
            ++j;
        }
    ImGui::EndChild();
    
    // Save Json-Data
    ImGui::SameLine();
    if(ImGui::BeginChild("Preferences", ImVec2(0,0), false))
    {
        // to use with std::string or own datatype we need a wrapper for InputTextXXX)
        static char JsonFilename[128] = "";
        // TODO: how to add automatically ?in C-style
        // static char extension[6] = ".json";
        ImGui::InputTextWithHint("Filename","put Json filename here",JsonFilename,IM_ARRAYSIZE(JsonFilename));
        if(ImGui::Button("Save Json"))
        {
            exportAnimationDetailsToFile(JsonFilename);                
        }
     }
     ImGui::EndChild();
    
    ImGui::EndChild();
}

void
Editor::setFrameDetails(int selectedAni, TaggedSheetAnimation& sheet, int frameNumber, AnimationSpec::Animation& ani)
{
    if(frameDetailsVisible)
    {
        //auto ani = sheet.second.at(selectedAni);
        ImGui::Text("Frame: %d", frameNumber);    
        if(ImGui::InputInt("Duration",&intDuration))
        {
            if(selectedFrame != 0)
            {
                std::cout << "DurationPress: " << intDuration << "\n";    
                ani.frames.at(frameNumber-1).duration = intDuration ;
            }
        }
        setTooltipText("set duration in ms");
        if(ImGui::Combo("Event",&currentEventItem, eventItems, IM_ARRAYSIZE(eventItems))) //;
        {
            if(selectedFrame != 0)
            {
               std::cout << "Event : " << std::to_string(currentEventItem) << "(" << EEvent::_from_integral(currentEventItem)._to_string()  << ") saved to Frame: " << std::to_string(frameNumber) <<" \n";
               //auto ani = sheet.second.at(selectedAni);
               ani.frames.at(frameNumber-1).event = EEvent::_from_integral(currentEventItem);
               std::cout << "From ani-map: " << ani.frames.at(frameNumber-1).event._to_string() << "\n";  
            }
        }

        //ImGui::Combo("Event",&currentEventItem, EEvent::_names(),3 );
        ImGui::Separator();
    }
}

void
Editor::editFrame(AnimationSpec::Animation& ani, size_t frame )
{
    std::cout << ani.name << "," << std::to_string(frame) << "\n";
    intDuration = ani.frames.at(frame-1).duration;
    currentEventItem = (ani.frames.at(frame-1).event)._to_integral(); 
    std::cout << "FrameDetail(event): " << currentEventItem << ", ani-map: " << ani.frames.at(frame-1).event._to_string() << "\n";
    //frameDetailsVisible =! frameDetailsVisible;
    ani.frameSize.x = (ani.frames.at(frame-1)).height;
    ani.frameSize.y = (ani.frames.at(frame-1)).width;
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
    guiTextureManager.load(Textures::GuiID::FrameBorder,"assets/gui/animator/08_border_frame.jpeg");

    
}

void
Editor::setSpritesheetAnimationDetails(const AnimationConfig& config, sf::Time aniDuration, std::vector<AnimationSpec::Frame>& frames)
{   
    // std::cout << "OverallDuration: " << aniDuration.asMilliseconds() << " ms, config-dur: " << config.duration.asMilliseconds() << "ms\n";
    spritesheetAnimation.setFrameSize(config.frameSize);
    spritesheetAnimation.setStartFrame({config.frameSize.x * config.startFrame.x, config.frameSize.y * config.startFrame.y});
    spritesheetAnimation.setNumFrames(aniDuration, frames);
    spritesheetAnimation.setDuration(config.duration);
    spritesheetAnimation.setRepeating(config.repeat);

    // TODO: check if texture is set for animation ...
    spritesheetAnimation.setTexture(guiCharTextureManager.getResource(Textures::LevelID::Level1/*config.spritesheetId*/));
    textureSet = true;
    // set origin of texture to center
    sf::FloatRect bounds = spritesheetAnimation.getSprite().getLocalBounds();
    spritesheetAnimation.getSprite().setOrigin(bounds.width / 2.f, bounds.height / 2.f);
 
}

void
Editor::exportAnimationDetailsToFile(char* JsonFilename)
{
    std::vector<AnimationSpec::Animation> aniSpecs;
    // TODO: no filename entered ? what then -> standard name but: HOW 
    std::ofstream oJson(sizeof(JsonFilename) == 0 ? "output.txt" : JsonFilename);
    
    for(auto& ani : animations[selectedSpritesheet])
    {
          aniSpecs.push_back(ani);
    }

    // TODO: use variables for charcter name etc.
    oJson << "{\n" << R"(  "Name" : "ichi",)" << "\n";
    oJson << R"(  "Spritesheet" : ")" << selectedSpritesheet << R"(",)" << "\n";
    oJson << R"(  "Animations" : [)" << "\n    ";

    int lineNr = 0;
    for(const auto& spec : aniSpecs)
    {
        lineNr++;
        json j = spec;
        oJson << j;
        if (lineNr != aniSpecs.size())
        {
            oJson << R"(,)";
        }
        oJson << "\n    " ;
    }

    oJson << R"(])" << "\n}";

    fmt::print("saved to {}\n", (sizeof(JsonFilename) == 0 ? "output.txt" : JsonFilename));

    // json j()
    /* 
    Output json: example ichi.json
    {
        "Name" : "Ichi",
        "Spritesheet" : "level1.png"
        "Animations" :
        [
            {"name": "idle", 
            "sheetStart" : {"x":1, "y":1}, 
            "frameSize" : {"width" : 80, "height" : 96},
            "numFrames" : 4,
            "duration_ms" : 800,
            "frames" : 
                [
                    {"number" : 1, "duration_ms" : 200, "event" : "None"},
                    {"number" : 2, "duration_ms" : 120, "event" : "None"},
                    {"number" : 3, "duration_ms" : 80, "event" : "None"},
                    {"number" : 4, "duration_ms" : 400, "event" : "FootstepSFX"},
                ],
            "repeat" : true,
            "animationId" : "Textures::CharacterID::IchiIdleRun"        
            },
        ]
    }
    // built asserts if total duration = sumFrames 
    */
    // atm we need to save:
    /*
   .frameSize={frameWidth,frameHeight}
   .startFrame={startX,startY} // reg. the spritesheet
   .numFrames=i
   .duration = sf::milliseconds(durationAni)// in ms
   .repeat = true ///  TODO from editor ui: entered by user
   .animationId = Textures::CharacterID::IchiIdleRun});///  TODO from editor ui: entered by user    
   ^*/
}


} /// namespace RyuAnimator 