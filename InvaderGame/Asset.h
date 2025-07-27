#pragma once

template<typename T>
class Asset
{
public:
	static void load() { T::load_impl(); }
};

