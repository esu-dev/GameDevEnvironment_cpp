#pragma once
class TimeController
{
public:
	static void Initialize();
	static void Update();

private:
	static bool _isEditorON;
};

