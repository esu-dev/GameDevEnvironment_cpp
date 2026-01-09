#include "DemoStageManager.h"
#include "GameObject.h"

void DemoStageManager::Update()
{
    if (_gameClearTextGO == nullptr)
    {
        return;
    }

    if (_gameClearTextGO->ActiveSelf())
    {
        return;
    }

    auto crystals = GameObject::FindGameObjectsWithTag("Crystal");
    if (crystals.empty())
    {
        _gameClearTextGO->SetActive(true);
    }
}
