#pragma once

template<typename T1, typename T2>
class Asset
{
public:
	static T2 load() { return T1::load_impl(); }
};

