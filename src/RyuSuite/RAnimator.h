#pragma once

#include <map>
#include <vector>
#include <string>



namespace RyuAnimator
{

namespace AnimationTags {
  struct Frame
  {
    int x; /// x-position in spritesheet
    int y; /// y-position in spritesheet
    int width;
    int height;
    int duration;
  };

  struct TaggedAnimation
  {
    std::string name;
    int fromFrame; /// Frame Startposition in spritesheet
    int toFrame; /// Frame Endposition in spritresheet
    std::string direction;
    std::vector<Frame> frames;
  };
} /// namespace AnimationTags


using TaggedSheetAnimation = std::pair<std::string, std::vector<AnimationTags::TaggedAnimation>>  ; 

  class Editor
  {
    public:
      Editor();
      ~Editor();

    //private:
      void createEditorWidgets(bool* p_open);

      void parseJsonData();

      bool showAnimationEditor;
  
      void createAnimationDetails(int selectedAni,const TaggedSheetAnimation& sheet );

      // map with all spritesheets loaded and according animations
      // key: spritesheetname, value: vector of animations
      std::map<std::string, std::vector<AnimationTags::TaggedAnimation> > animations;
      bool parsedSpritesheet;
      std::string selectedSpritesheet;
  };
}