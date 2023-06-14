# could take a lok and test: https://github.com/friendlyanon/cmake-init

cmake_minimum_required(VERSION 3.7...3.26)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(RyuPrototype VERSION 1.0
        DESCRIPTION "SFML prototype of RYU - a combat based cinematic puzzle platformer"
        LANGUAGES CXX)

set(MAIN "src/Main.cpp")
set(INCLUDE_FOLDER "src/include")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake/;${CMAKE_MODULE_PATH}")

find_package(SFML 2.5.1 COMPONENTS audio graphics window system network)
find_package(box2d CONFIG REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(ImGui-SFML REQUIRED)
find_package(fmt)
	
if(ImGui-SFML_FOUND)
	message("\n-> ImGui-SFML directory found.")
else()
	message("\n-> ImGui-SFML directory not found.")
endif()

# If found, include and link; otherwise output error message
if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
else()
	set(SFML_ROOT "" CACHE PATH "SFML top-level directory")
	message("\n-> SFML directory not found. Set SFML_ROOT to SFML's top-level path (containing \"include\" and \"lib\" directories).")
	message("-> Make sure the SFML libraries with the same configuration (Release/Debug, Static/Dynamic) exist.\n")
endif()

# first add global find packages !!! -> then add sub CMakeLists -> the sub CMakes have all the find packages
add_subdirectory("src") 
add_executable(ryupt "${MAIN}")

target_include_directories(ryupt PUBLIC "${INCLUDE_FOLDER}") #HOW >TO ADD SUBFOLDErS ?
target_compile_features(ryupt PUBLIC cxx_std_17)

target_link_libraries(ryupt PUBLIC
	ryucorelib 
  ryudebuglib
	ryuscenelib	
  ryueventslib
	ryucontrollib
	ryuphysicslib
	ryusuitelib
	ryuanimationlib
	ryucharacterlib
	ryustateslib
	box2d	
	fmt::fmt
	${SFML_LIBRARIES} 
	${SFML_DEPENDENCIES} 
	ImGui-SFML::ImGui-SFML 
	nlohmann_json::nlohmann_json 
)# b2DebugDraw)

add_subdirectory("src/Tests")
