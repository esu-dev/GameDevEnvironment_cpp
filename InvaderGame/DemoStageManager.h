#pragma once

#include "EngineBehaviour.h"

class DemoStageManager : public EngineBehaviour
{
public:
    NAME(DemoStageManager)
    SERIALIZE3(EngineBehaviour,
        SERIALIZE_FIELD3(_gameClearTextGO)
    )

    void Update() override;

private:
    GameObject* _gameClearTextGO;
};