#include "EString.h"

#include "EngineFramework.h"

EString::EString(const char* data)
{
	_length = strlen(data);
	_data = new char[_length + 1];
	memcpy(_data, data, _length + 1);
}

EString::EString(const EString& other)
{
	_length = other._length;
	_data = new char[_length + 1];
	memcpy(_data, other._data, _length + 1);
}

EString::~EString()
{
	delete[] _data;
}

void EString::operator=(const EString& other)
{
	delete[] _data;
	_length = other._length;
	_data = new char[_length + 1];
	memcpy(_data, other._data, _length + 1);
}

const char* EString::c_str()
{
	return _data ? _data : "";
}