#pragma once

#include "EngineFramework.h"


class Profiler
{
public:
	/*struct CategoryData
	{
		float totalTime = 0.0f;
		std::map<std::string, float> subCategoryTimes;
	};*/

	struct FrameData
	{
		std::map<std::string, FrameData*> categories;
		float totalTime = 0.0f;
	};

	static FrameData* GetLastFrameData();
	static void AddFrameData(FrameData* data);
	static void Render();

private:
	static std::vector<FrameData*> _history;
	static const size_t _maxHistory = 120;
	static int _selectedFrameIndex;
};