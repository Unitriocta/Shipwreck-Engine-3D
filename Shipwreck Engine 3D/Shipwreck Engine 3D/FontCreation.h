#pragma once


#include <freetype/ftglyph.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <d3d11.h>

#include "MathExtras.h"


class FontCreation
{
public:
	// variables
	struct SetupFace {
	public:
		SetupFace(const char* filename) :
			fontFace(fontLib, filename)
		{}

	public:
		//Structs:
		FT_Face LoadCharacter(FT_ULong charToLoad);

		struct FontLib {
			FontLib() {
				FT_Error error = FT_Init_FreeType(&lib);

				if (error) {
					//display error msg in MsgBox or smthn
				}
			}

			FT_Library lib;
		};
		//"D:/Shipwreck Engine 3D/Shipwreck Engine 3D/Myfont-Regular.ttf"
		struct FontFace {
			FontFace(const FontLib& _lib, const char* filename) {
				FT_Error error = FT_New_Face(_lib.lib,
					filename,
					0,
					&face);
				if (error == FT_Err_Unknown_File_Format)
				{
					//the font file could be opened and read, but it appears
					//that its font format is unsupported
				}
				else if (error)
				{
					//another error code means that the font file could not
					//be opened or read, or that it is broken...
				}
				//FT_Set_Char_Size(face, 16*64, 16*32, 1920, 1080);
				//FT_Set_Char_Size(face, 50*64, 0, 100, 0);
				FT_Set_Pixel_Sizes(face, 4 * 64, 4 * 64);
				//16 * 64 is a 16 pt font
			}

			FT_Face face;
		};

	public:

		FontLib  fontLib;

		FontFace fontFace;
	};
public:
	// classes / functions:

	SetupFace& LoadFont(const char* filename);
};