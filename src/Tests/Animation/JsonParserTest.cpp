#include <Ryu/Animation/JsonParser.h>

#include <fmt/format.h>
#include <fmt/core.h>
#include "gtest/gtest.h"
#include <fstream>

namespace RyuParser {

// The fixture for testing class Foo.
class JsonParserTest : public ::testing::Test {

 public:
  RyuParser::JsonAnimations jsonAnis;
  json jsonContent;
  std::string validJsonString;
  
 protected:
  // You can remove any or all of the following functions if their bodies would
  // be empty.

  JsonParserTest() {
     // You can do set-up work for each test here.
  }

  ~JsonParserTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
  }


  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).

     validJsonString = R"(
      {
        "Name" : "hannes",
        "Spritesheet" : "sheet_l1",
        "Animations" : [
          {"AnimationDirection":"forward","AnimationId":"None","FrameSize":{"height":96,"width":80},"Frames":[{"duration":100,"event":"None","height":96,"width":80,"x_sheet":0,"y_sheet":0}],"Name":"idle","Sheet_begin":0,"Sheet_end":0,"animationDuration":"100 ms","numFrames":1,"repeat":false}
        ]
      })";
  }

  void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
  }

  // Class members declared here can be used by all tests in the test suite
  // for Foo.
};

// Tests that the Foo::Bar() method does Abc.
TEST_F(JsonParserTest, CheckJsonFields) {
  
  std::string file("ichi.json"); //this/package/testdata/myinputfile.dat";
  JsonParser p;
  std::ifstream f(file);
  // jsonContent = json::parse(f);
  jsonContent = json::parse(validJsonString);

  p.getAnimationsFromJson(jsonContent, jsonAnis);
            
  for(auto& i : jsonAnis.animations)
  {
    fmt::print("ani: {} frames: {} dur: {} \n", i.name, i.numFrames, i.animationDuration.asMilliseconds());
  }
  // EXPECT_THROW("Can't parse",json::exception);
}

// Tests that Foo does Xyz.
// TEST_F(FooTest, DoesXyz) {
  // Exercises the Xyz feature of Foo.

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
