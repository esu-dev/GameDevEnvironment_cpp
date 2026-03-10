#include "GameState.h"

#include "EngineFramework.h"
#include "imgui_impl_dx11.h"
#include "EngineTime.h"

void GameState::Update()
{
	static int counter = 0;
	static float totalDeltaTime = 0;
	static float fps = 0;

	// fpsŒvŽZ
	counter++;
	totalDeltaTime += EngineTime::GetDeltaTime();
	if (counter >= 10)
	{
		fps = counter / totalDeltaTime;

		counter = 0;
		totalDeltaTime = 0;
	}

	ImGui::SetNextWindowSize(ImVec2(200, 100));
	ImGui::Begin("Game State");
	ImGui::Text(("fps: " + std::to_string(fps)).c_str());
	ImGui::End();
}