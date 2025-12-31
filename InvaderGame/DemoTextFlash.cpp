#include "DemoTextFlash.h"

#include "EngineTime.h"
#include "TextLabel.h"

void DemoTextFlash::Update()
{
	static float visibleCounter = 0;
	static float invisibleCounter = 0;

	TextLabel* textLabel = this->GetComponent<TextLabel>();

	// •\Ž¦ó‘Ô
	if (textLabel->enabled)
	{
		if (visibleCounter >= _visibleTime)
		{
			textLabel->enabled = false;
			invisibleCounter = 0;
		}

		visibleCounter += EngineTime::GetDeltaTime();
	}
	// ”ñ•\Ž¦ó‘Ô
	else
	{
		if (invisibleCounter >= _invisibleTime)
		{
			textLabel->enabled = true;
			visibleCounter = 0;
		}

		invisibleCounter += EngineTime::GetDeltaTime();
	}
};