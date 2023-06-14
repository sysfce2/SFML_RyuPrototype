set (SRC_TEMPLATE 
      "JsonParserTest.cpp"
    )

find_package(GTest CONFIG REQUIRED)
#[[]  
set(HEADER
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Debug/b2DrawSFML.hpp"
     "${RyuPrototype_SOURCE_DIR}/include/Ryu/Debug/imGuiDebug.h"
)
]]
enable_testing()

add_executable(
  JsonParser_test
	"${SRC_TEMPLATE}"
)

target_link_libraries(
  JsonParser_test
  GTest::gtest_main
	ryuanimationlib
  fmt::fmt
	${SFML_LIBRARIES} 
	${SFML_DEPENDENCIES} 
	ImGui-SFML::ImGui-SFML 
	nlohmann_json::nlohmann_json 
)

include(GoogleTest)
gtest_discover_tests(JsonParser_test)

target_include_directories(JsonParser_test PUBLIC "${RyuPrototype_SOURCE_DIR}/include") 