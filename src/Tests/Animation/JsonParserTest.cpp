#include "Ryu/Core/AssetIdentifiers.h"
#include "Ryu/Events/EventEnums.h"
#include <Ryu/Animation/JsonParser.h>

#include <SFML/System/Time.hpp>
#include <fmt/format.h>
#include <fmt/core.h>
#include "gtest/gtest.h"
#include <fstream>
#include <variant>

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

TEST_F(JsonParserTest, InvalidInput) {
  // t.b.d fill with content
}

// TODO: add case for Type: Scene ...

// Tests that the Foo::Bar() method does Abc.
TEST_F(JsonParserTest, CheckJsonFieldsCharacter) {

  using CharId = Textures::CharacterID;
  auto getAnimationDir = [this](CharId id){
    return jsonAnis.animations.at(id).direction;
  };


  validJsonString = R"(
      {
        "Name" : "hannes",
        "Spritesheet" : "Level1",
        "Path" : "assets/spritesheets/ichi/ichi_spritesheet_level1.png",
        "Animations" : 
        [
          {"AnimationDirection":"forward","AnimationId":"IchiDuckIdle","AnimationType": "Character", "Type" : "Character",
            "FrameSize": {"height":96,"width":80},
            "Frames":[{"duration":100,"event":"CharacterSpeedChanged","height":96,"width":80,"x_sheet":3,"y_sheet":2}],
            "Name":"idle","Sheet_begin":3,"Sheet_end":3,
            "animationDuration":"666 ms",
            "numFrames":1,"repeat":false},
          {"AnimationDirection":"backward","AnimationId":"IchiDuckWalk", "AnimationType": "Character", "Type" : "Character",
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
  Textures::LevelID sId = Textures::LevelID::Level1;
  EXPECT_EQ(sId._to_integral(),jsonAnis.spritesheetId);
  // spritesheetPath
  EXPECT_EQ("assets/spritesheets/ichi/ichi_spritesheet_level1.png",jsonAnis.spritesheetPath);
  // Animation count
  EXPECT_EQ(2,jsonAnis.animations.size());
  // animationName[0]
  EXPECT_EQ("idle",jsonAnis.animations.at(CharId::IchiDuckIdle).name);
  EXPECT_EQ("walkBack",jsonAnis.animations.at(CharId::IchiDuckWalk).name);
  // animationName[0].fromFrame
  EXPECT_EQ(3,jsonAnis.animations.at(CharId::IchiDuckIdle).fromFrame);
  // animationName[0].toFrame
  EXPECT_EQ(3,jsonAnis.animations.at(CharId::IchiDuckIdle).toFrame);
  // animation[0] direction 
  EXPECT_EQ("forward", getAnimationDir(CharId::IchiDuckIdle));
  // animation[0].frame[0].duration
  EXPECT_EQ(100,jsonAnis.animations.at(CharId::IchiDuckIdle).frames.at(0).duration);
  // animation[0].frame[0].height
  EXPECT_EQ(96,jsonAnis.animations.at(CharId::IchiDuckIdle).frames.at(0).height);
  // animation[0].frame[0].width
  EXPECT_EQ(80,jsonAnis.animations.at(CharId::IchiDuckIdle).frames.at(0).width);
  // animation[0].frame[0].x
  EXPECT_EQ(3,jsonAnis.animations.at(CharId::IchiDuckIdle).frames.at(0).x);
  // animation[0].frame[0].y
  EXPECT_EQ(2,jsonAnis.animations.at(CharId::IchiDuckIdle).frames.at(0).y);
  // animation[0].frame[0].event, Better Enum need first an variable with the value
  Ryu::EEvent evt = Ryu::EEvent::CharacterSpeedChanged;
  EXPECT_EQ(evt._to_integral(),jsonAnis.animations.at(CharId::IchiDuckIdle).frames.at(0).event);
  // numFrames
  EXPECT_EQ(1,jsonAnis.animations.at(CharId::IchiDuckIdle).numFrames);
  // frameSize.x  
  EXPECT_EQ(96,jsonAnis.animations.at(CharId::IchiDuckIdle).frameSize.x);
  // frameSize.y  
  EXPECT_EQ(80,jsonAnis.animations.at(CharId::IchiDuckIdle).frameSize.y);
  // duration
  EXPECT_EQ(sf::milliseconds(666),jsonAnis.animations.at(CharId::IchiDuckIdle).animationDuration);
  EXPECT_EQ(sf::seconds(2),jsonAnis.animations.at(CharId::IchiDuckWalk).animationDuration);
  // EXPECT_EQ(sf::microseconds(2),jsonAnis.animations.at(1).animationDuration);
  // repeat
  EXPECT_EQ(false,jsonAnis.animations.at(CharId::IchiDuckIdle).repeat);
  // animationID
  CharId aniId = CharId::IchiDuckIdle;
  auto animationId = jsonAnis.animations.at(CharId::IchiDuckIdle).animationId;
  EXPECT_EQ(aniId._to_integral(),std::get<CharId>(animationId));

  // animationType
  Textures::AnimationType aniType = Textures::AnimationType::Character;
  auto animationType = jsonAnis.animations.at(CharId::IchiDuckIdle).animationType;
  EXPECT_EQ(aniType/*._to_integral()*/, animationType);
  /* Debug
  for(auto& i : jsonAnis.animations)
  {
    fmt::print("ani: {} frames: {} dur: {} \n", i.name, i.numFrames, i.animationDuration.asMilliseconds());
  }
  */
  // EXPECT_THROW("Can't parse",json::exception);
}
}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
