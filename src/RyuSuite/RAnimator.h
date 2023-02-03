#pragma once

#include <map>
#include <vector>
#include <string>



namespace RyuAnimator
{

namespace AnimationTags {
  struct TaggedAnimation
  {
    std::string name;
    int fromFrame;
    int toFrame;
    std::string direction;
  };
} /// namespace AnimationTags

  class Editor
  {
    public:
      Editor();
      ~Editor();

    //private:
      void createEditorWidgets(bool* p_open);

      void parseJsonData();

      bool showAnimationEditor;

      // map with all spritesheets loaded and according animations
      // key: spritesheetname, value: vector of animations
      std::map<std::string, std::vector<AnimationTags::TaggedAnimation> > animations;
      bool parsedSpritesheet;
      std::string selectedSpritesheet;
  };
}