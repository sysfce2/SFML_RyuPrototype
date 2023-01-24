# RyuPrototypeSFML

Ryu prototype build finally with SFML .

But before starting with Ryu one need to learn SFML. Here we did with the SFML Game Development book.
See [License.txt]. in 02_SFML_GD_BOOK.

used tools & libraries

1. SFML v 2.5.1
    - see https://en.sfml-dev.org/
    - sudo apt install libsfml-dev
2. Box2D for physics v 2.4.1
    - clone it from: git@github.com:erincatto/box2d.git
    - ./build.sh
    - make install
3. Dear ImGui >=v1.8
    - see https://github.com/ocornut/imgui
    - clone git@github.com:ocornut/imgui.git    

4. ImGui-SFML >=v2.5
    - see https://github.com/eliasdaler/imgui-sfml
    - clone from: git@github.com:eliasdaler/imgui-sfml.git
    - cmake <ImGui-SFML repo folder>  -DBUILD_SHARED_LIBS=ON -DIMGUI_SFML_IMGUI_DEMO=ON -DIMGUI_DIR=<path with built SFML>
    - (sudo) cmake --build <ImGui-SFML repo folder>  --target install
5. - in Ryu project:
    - (mkdir build && cd build)
    - cmake ..
    - make
    