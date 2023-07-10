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
  
  auto getAnimationDir = [this](int index){
    return jsonAnis.animations.at(index).direction;
  };

  
  validJsonString = R"(
      {
        "Name" : "hannes",
        "Spritesheet" : "sheet_level1",
        "Animations" : 
        [
          {"AnimationDirection":"forward","AnimationId":"CharacterSpeedChanged",
            "FrameSize": {"height":96,"width":80},
            "Frames":[{"duration":100,"event":"CharacterSpeedChanged","height":96,"width":80,"x_sheet":3,"y_sheet":2}],
            "Name":"idle","Sheet_begin":3,"Sheet_end":3,
            "animationDuration":"666 ms",
            "numFrames":1,"repeat":false},
          {"AnimationDirection":"backward","AnimationId":"None",
            "FrameSize": {"height":96,"width":80},
            "Frames":[{"duration":100,"event":"CharacterSpeedChanged","height":96,"width":80,"x_sheet":3,"y_sheet":2},{"duration":100,"event":"CharacterSpeedChanged","height":96,"width":80,"x_sheet":3,"y_sheet":2}],
            "Name":"walkBack","Sheet_begin":4,"Sheet_end":5,
            "animationDuration":"2 s",
            "numFrames":2,"repeat":false}
        ]
      })";
  
  JsonParser p;
  jsonContent = json::parse(validJsonString);

  p.getAnimationsFromJson(jsonContent, jsonAnis);


  // spritesheet name
  EXPECT_EQ("hannes",jsonAnis.jsonName);
  // spritesheet-fielname
  EXPECT_EQ("sheet_level1",jsonAnis.spritesheetName);
  // Animation count
  EXPECT_EQ(2,jsonAnis.animations.size());
  // animationName[0]
  EXPECT_EQ("idle",jsonAnis.animations.at(0).name);
  EXPECT_EQ("walkBack",jsonAnis.animations.at(1).name);
  // animationName[0].fromFrame
  EXPECT_EQ(3,jsonAnis.animations.at(0).fromFrame);
  // animationName[0].toFrame
  EXPECT_EQ(3,jsonAnis.animations.at(0).toFrame);
  // animation[0] direction 
  EXPECT_EQ("forward", getAnimationDir(0));
  // animation[0].frame[0].duration
  EXPECT_EQ(100,jsonAnis.animations.at(0).frames.at(0).duration);
  // animation[0].frame[0].height
  EXPECT_EQ(96,jsonAnis.animations.at(0).frames.at(0).height);
  // animation[0].frame[0].width
  EXPECT_EQ(80,jsonAnis.animations.at(0).frames.at(0).width);
  // animation[0].frame[0].x
  EXPECT_EQ(3,jsonAnis.animations.at(0).frames.at(0).x);
  // animation[0].frame[0].y
  EXPECT_EQ(2,jsonAnis.animations.at(0).frames.at(0).y);
  // animation[0].frame[0].event
  EXPECT_EQ(1,jsonAnis.animations.at(0).frames.at(0).event);
  // numFrames
  EXPECT_EQ(1,jsonAnis.animations.at(0).numFrames);
  // frameSize.x  
  EXPECT_EQ(96,jsonAnis.animations.at(0).frameSize.x);
  // frameSize.y  
  EXPECT_EQ(80,jsonAnis.animations.at(0).frameSize.y);
  // duration
  EXPECT_EQ(sf::milliseconds(666),jsonAnis.animations.at(0).animationDuration);
  EXPECT_EQ(sf::seconds(2),jsonAnis.animations.at(1).animationDuration);

  // repeat
  EXPECT_EQ(false,jsonAnis.animations.at(0).repeat);
  // EXPECT_EQ(1,jsonAnis.animations.at(0).numFrames);
            
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
