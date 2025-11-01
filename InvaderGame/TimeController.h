#pragma once
class TimeController
{
public:
	static void Initialize();
	static void Update();

private:
	static float _time;
	static bool _isEditorON;
};

