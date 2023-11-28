#include "FontCreation.h"

#include <sstream>
#include <windows.h>

//void FontCreation::LoadFont() {
//	FT_Library fontLibrary;
//	FT_Face    fontFace;
//
//	FT_Error error = FT_Init_FreeType(&fontLibrary);
//
//	if (error) {
//		//display error msg in MsgBox or smthn
//	}
//
//	error = FT_New_Face(fontLibrary,
//		"D:/Shipwreck Engine 3D/Shipwreck Engine 3D/Myfont-Regular.ttf",
//		0,
//		&fontFace);
//	if (error == FT_Err_Unknown_File_Format)
//	{
//		//the font file could be opened and read, but it appears
//		//that its font format is unsupported
//	}
//	else if (error)
//	{
//		//another error code means that the font file could not
//		//be opened or read, or that it is broken...
//	}
//
//	/*error = FT_New_Memory_Face( fontLibrary, buffer, fontSize, 0, &fontFace );*/
//
//	error = FT_Set_Char_Size(
//		fontFace, /* handle to face object         */
//		0,        /* char_width in 1/64 of points  */
//		16 * 64,  /* char_height in 1/64 of points */
//		300,      /* horizontal device resolution  */
//		300);     /* vertical device resolution    */
//
//}

FontCreation::SetupFace& FontCreation::LoadFont(const char* filename) {
	SetupFace setupFace(filename);
	return setupFace;
}

FT_Face FontCreation::SetupFace::LoadCharacter(FT_ULong charToLoad) {
	
	/*int charIndex = FT_Get_Char_Index(fontFace.face, charToLoad);

	int loadFlags = FT_LOAD_DEFAULT;
	FT_Error error = FT_Load_Glyph(fontFace.face, charIndex, loadFlags);

	FT_Render_Mode renderMode = FT_RENDER_MODE_NORMAL;
	error = FT_Render_Glyph(fontFace.face->glyph, renderMode);*/



	FT_Error error = FT_Load_Char(fontFace.face, charToLoad, FT_LOAD_RENDER);


	return fontFace.face;

	/*FT_Bitmap* charBitmap = &fontFace.face->glyph->bitmap;

	return charBitmap;*/
}