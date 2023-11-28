#include "UITypes.h"





void Textbox::LoadText() {

	FT_UInt prevGlyph;

	FT_Vector kerning;


	FT_Vector pen;
	pen.x = textPos.x/*300 * 64*/;
	pen.y = textPos.y/*(480 - 200) * 64*/;



	RECT rect;

	GetWindowRect(hWnd, &rect);


	int winWidth = (rect.right - rect.left);
	int winHeight = (rect.bottom - rect.top);


	glyphs.resize(strlen(textToLoad));
	for (int i = 0; i < strlen(textToLoad); i++) {

		//FT_Set_Transform(fontManager->fontFace.face, &matrix, &pen);

		FT_GlyphSlot slot = fontManager->LoadCharacter(textToLoad[i])->glyph;
		FT_Bitmap* textBitmap = &slot->bitmap;

		//row increments:
		if (pen.x > textPos.x + textRect.x) {
			pen.x = textPos.x;
			pen.y += (FT_UInt)(slot->face->height * 1.15f) >> 6/*55*/;
		}

		if (textToLoad[i] == ' ') {
			pen.x += slot->face->glyph->metrics.horiAdvance / 700;
		}
		else {
			if (i >= 1 && FT_HAS_KERNING(slot->face)) {
				FT_Get_Kerning(slot->face, prevGlyph, slot->glyph_index, FT_KERNING_DEFAULT, &kerning);
				pen.x += kerning.x >> 6;
			}



			float advance = (slot->advance.x / 5) >> 6;

			float xOffset = (advance - ((float)textBitmap->width / 9));
			float yOffset = (slot->face->bbox.yMax) - (slot->face->glyph->metrics.horiBearingY) - (slot->face->glyph->metrics.vertBearingY);



			float height = slot->face->height;
			float letHeight = textBitmap->rows / height;

			Vec2 letPos = Vec2(pen.x + xOffset, pen.y + yOffset / 330);

			Vec2 letPosConverted = Vec2(
				(/*To make it centered: */  ((winWidth / 2) + letPos.x - (winWidth / 2)) / (winWidth / 2)), // + 1 and - 1 make it centered>>
				-(/*To make it centered: */ ((winHeight / 2) + letPos.y - (winHeight / 2)) / (winHeight / 2)));
			//WIDTH: (coord + 1) * (windowWidth / 2), HEIGHT: abs((coord - 1) * (windowWidth / 2))
			
			

			FT_UInt index = FT_Get_Char_Index(slot->face, textToLoad[i]);
			FT_Load_Glyph(fontManager->fontFace.face, index,FT_LOAD_DEFAULT);
			FT_Get_Glyph(slot,&glyphs[i].glyph);

			glyphs[i] =
				TextGlyph(letPosConverted, 
				Vec2( ((float)textBitmap->width / textBitmap->rows) * letHeight, /*1 */ letHeight ), 
				index, 
				glyphs[i].glyph);


			prevGlyph = index;


			pen.x += advance;
			pen.y += slot->advance.y;
		}
	}
}