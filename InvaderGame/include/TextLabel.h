#pragma once

#include "DirectX.h"
#include "Component.h"

class TextCharacter;

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
	std::vector<TextCharacter> _textCharacterVector;

	void MakeShaderResourceViewOf(wchar_t code, ComPtr<ID3D11ShaderResourceView> *srv);
	void Render(const Vector3& cameraPosition);
};

class TextCharacter
{
public:
	ComPtr<ID3D11ShaderResourceView> ShaderResourceView;
	int fontSize;

	TextCharacter() : fontSize(0) {}
};