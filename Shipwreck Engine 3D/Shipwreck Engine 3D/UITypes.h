#pragma once




#include "MathExtras.h"
#include "FontCreation.h"



	struct Textbox {
	public:
		Textbox(const char* _textToLoad, FontCreation::SetupFace* _fontManager, Vec2 _textRect, Vec2 _textPos, float _fontSize, HWND _hWnd)
			: textToLoad(_textToLoad), fontManager(_fontManager), textRect(_textRect), textPos(_textPos), fontSize(_fontSize), hWnd(_hWnd)
		{}

		Textbox()
		{}

		const char* textToLoad;

		FontCreation::SetupFace* fontManager;


		void LoadText();

	public:
		Vec2 textRect;
		Vec2 textPos;

		struct TextGlyph {
			TextGlyph(Vec2 _pos, Vec2 _rect, FT_UInt _index, FT_Glyph _glyph)
			{
				pos = _pos;
				rect = _rect;
				index = _index; 
				glyph = _glyph;
			}
			TextGlyph()
			{
				pos = Vec2(0,0);
				rect = Vec2(0, 0);
				index = 0;
				glyph = NULL;
			}

			Vec2 pos;
			Vec2 rect;
			FT_UInt index;
			FT_Glyph glyph;
		};

		std::vector<TextGlyph> glyphs;


		float fontSize;

		HWND hWnd;
	};