// blub
#pragma once

#include <Ryu/Core/AssetManager.h> 
#include <Ryu/Core/AssetIdentifiers.h>
#include <Ryu/Animation/SpritesheetAnimation.h>
#include <Ryu/Animation/EditorEnums.h>

#include <imgui.h>
#include <Thirdparty/imgui-filebrowser/imfilebrowser.h>

#include <bits/stdint-intn.h>
#include <cstdint>
#include <map>
#include <vector>
#include <string>

#include <SFML/Graphics.hpp>

class SpritesheetAnimation;

using GuiCharTextureManager = AssetManager<sf::Texture, Textures::LevelID>;
using GuiTextureManager = AssetManager<sf::Texture, Textures::GuiID>;

namespace RyuAnimator
{

// fields important for every Animation 
// TODO (if Editor in use: delete struct in CharacterBase.h)
// it'll also included in AnimationSpec::Animation
struct AnimationConfig
{
    sf::Vector2i frameSize;
    sf::Vector2i startFrame;
    std::size_t numFrames;
    sf::Time duration;
    bool repeat;
    Textures::CharacterID animationId;
};

enum class EFileBrowserState
{
  None,
  SpriteSheetJson,
  AnimationConfigJson
};

using TaggedSheetAnimation = std::pair<const std::string, std::vector<AnimationSpec::Animation>>  ;

  class Editor
  {
    public:
      Editor();
      ~Editor();

    //private:

      void update(sf::Time dt);

      void initData();
  
      void calculateAnimationDuration(AnimationSpec::Animation& ani);
  
      void createEditorWidgets(bool* p_open);

      void parseJsonData(std::string path);
      void parseJsonFile();
      bool showAnimationEditor;
  
      void exportAnimationDetailsToFile(char* JsonFilename);
      
      void setFrameDetails(int selectedAni, TaggedSheetAnimation& sheet, int frameNumber, AnimationSpec::Animation& ani);
      
      void setAnimationPreferences(std::string sheetName);
  
      void createAnimationDetails(int selectedAni, TaggedSheetAnimation& sheet );
      void setSpritesheetAnimationDetails(const AnimationConfig& config, sf::Time aniDuration, std::vector<AnimationSpec::Frame>& frames);
      // map with all spritesheets loaded and according animations
      // key: spritesheetname, value: vector of animations
      std::map<std::string, std::vector<AnimationSpec::Animation> > animations;
      bool parsedSpritesheet;
      bool textureSet;
      std::string selectedSpritesheet;
      std::string selectedSpritesheetPath;
      std::string selectedSpritesheetId;
      char selectedSpriteSheetPath[128] = "assets/spritesheets/ichi/ichi_spritesheet_level1.png";
      EFileBrowserState fileBrowserState;

  private:

      void setTooltipText(const char * tooltip);
      void initTextures();
      void editFrame(AnimationSpec::Animation& ani, size_t frame);
      GuiCharTextureManager guiCharTextureManager;
      GuiTextureManager guiTextureManager;
      SpritesheetAnimation spritesheetAnimation;

      std::vector<AnimationSpec::Animation> animationSpecs;

      std::map<std::string, bool> preferences;

      std::unique_ptr<ImGui::FileBrowser> _fileBrowser;
      bool aniIsPlaying;
  };
}
