#pragma once

#include "DirectX.h"
#include "Component.h"

class TextCharacter;

class TextLabel : public Component
{
public:
	TextLabel();
	TextLabel(std::string text);
	void SetText(std::string text);
	void SetFontSize(int fontSize);
	void Update() override;

private:
	int FontSize = 16;
	std::string _text = "new_text";
	std::vector<TextCharacter> _textCharacterVector;

	void MakeShaderResourceViewOf(wchar_t code, ComPtr<ID3D11ShaderResourceView> *srv);
};

class TextCharacter
{
public:
	ComPtr<ID3D11ShaderResourceView> ShaderResourceView;
	int fontSize;

	TextCharacter() : fontSize(0) {}
};