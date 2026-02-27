#pragma once

#include <vector>
#include <string>
#include <map>


class Profiler
{
public:
	struct FrameData
	{
		std::map<std::string, float> categoryTimes;
		float totalFrameTime = 0.0f;
	};

	static void AddFrameData(const FrameData& data);
	static void Render();

private:
	static std::vector<FrameData> _history;
	static const size_t _maxHistory = 120;
	static int _selectedFrameIndex;
};