#pragma once

#include "framework.h"
#include "Record.h"

class RecordSample
{
private:
	struct Hoge
	{
		float _float;
	};

	Record<int> _int;
	Record<Hoge> _hoge;
	Record<Hoge*> _hogePtr;
	std::string _string;
};