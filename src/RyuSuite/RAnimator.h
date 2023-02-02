#pragma once

// #include <imgui.h>

namespace RyuAnimator
{

  class Editor
  {
    public:
      Editor();
      ~Editor();

    //private:
      void createEditorWidgets(bool* p_open);

      void parseJsonData();

      bool showAnimationEditor;
  };
}