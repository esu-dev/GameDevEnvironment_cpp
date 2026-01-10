#pragma once

#include "DirectX.h"
#include "Component.h"

class TextLabel : public Component
{
public:
	NAME(TextLabel)
	SERIALIZE3(Component,
		SERIALIZE_FIELD3(_canMove),
		SERIALIZE_FIELD3(FontSize),
		SERIALIZE_FIELD3(_text)
	)

	enum TextAlign
	{
		Left,
		Center,
		Right
	};

	TextLabel();
	void SetCanMove(bool canMove);
	void SetText(std::string text);
	void SetFontSize(int fontSize);
	void SetTextAlign(TextAlign textAlign);
	void Update() override;
	void EditorUpdate() override;


private:
	bool _canMove = false;
	int FontSize = 16;
	int _resolution = 20;
	TextAlign _textAlign = TextAlign::Center;
	std::string _text = "new_text";
	void MakeShaderResourceViewOf(wchar_t c, ComPtr<ID3D11ShaderResourceView>* srv);
	void Render(const Vector3& cameraPosition);

	struct CacheKey
	{
		wchar_t character;
		int fontSize;

		bool operator<(const CacheKey& other) const
		{
			if (character != other.character) return character < other.character;
			return fontSize < other.fontSize;
		}
	};
	static std::map<CacheKey, ComPtr<ID3D11ShaderResourceView>> _srvCache;
};