#pragma once

#include "Ryu/Scene/SceneNode.h"
#include <Ryu/Events/EventEnums.h>
#include <Ryu/Statemachine/CharacterState.h>
#include <memory>

class CharacterStateJumpNormal : public CharacterState {
  public:
    CharacterStateJumpNormal();
    ~CharacterStateJumpNormal();
    std::unique_ptr<CharacterState> handleInput(CharacterBase &character,
                                                EInput input) override;
    void update(CharacterBase &character) override;

    void enter(CharacterBase &character) override;
    void exit(CharacterBase &character) override;
    void onNotify(CharacterBase &character, Ryu::EEvent event) override;
};
