#pragma once

// とりあえずはSSOなしの実装となっているため、最適化されていない。
// コピーコンストラクタとムーブコンストラクタもなし
class EString
{
public:
	EString(const char* data);
	EString(const EString& other);
	~EString();
	void operator=(const EString& other);
	const char* c_str();

private:
	char* _data = nullptr;
	size_t _length = 0;
};