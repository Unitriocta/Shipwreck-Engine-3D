//This project uses The FreeType Project LICENSE found at FTL.txt




#include "TextRenderer.h"

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <math.h>

#include <d3d11.h>

#include <cmath>

#include FT_FREETYPE_H


#define WIDTH   640
#define HEIGHT  480

#define DEFAULT_FONT_SIZE 10



const char* _fileName = "D:\\Shipwreck Engine 3D\\Shipwreck Engine 3D\\Myfont - Regular.ttf";
FT_Face _face;
FT_Library p_library;


struct Vec2 {
    float x;
    float y;
};



//bool CreateShaderResourceView(ID3D11Device* p_device, FT_Bitmap* p_bitmap, ID3D11ShaderResourceView* shaderResourceView);
//bool LoadTTF(ID3D11Device* p_device);
//void RenderFont(ID3D11Device* p_device, const char* p_text, Vec2 p_position, ID3D11ShaderResourceView* shaderResourceView);
//
//
//bool LoadTTF(ID3D11Device* p_device)
//{
//    if (FT_Init_FreeType(&p_library))
//        return false;
//
//    if (FT_New_Face(p_library, _fileName, 0, &_face))
//        return false;
//
//    if (FT_Set_Pixel_Sizes(_face, 0, DEFAULT_FONT_SIZE))
//        return false;
//
//    return true;
//}
//
//void RenderFont(ID3D11Device* p_device, const char* p_text, Vec2 p_position, ID3D11ShaderResourceView* shaderResourceView)
//{
//    FT_GlyphSlot slot = _face->glyph;
//
//    FT_Vector pen;
//    pen.x = p_position.x;
//    pen.y = p_position.y;
//
//    FT_Matrix matrix;
//    float angle = (/*90*/0.0 / 360) * 3.14159 * 2;
//
//    matrix.xx = (FT_Fixed)(cos(angle) * 0x10000L);
//    matrix.xy = (FT_Fixed)(-sin(angle) * 0x10000L);
//    matrix.yx = (FT_Fixed)(sin(angle) * 0x10000L);
//    matrix.yy = (FT_Fixed)(cos(angle) * 0x10000L);
//
//    for (unsigned int i = 0; i < strlen(p_text); ++i)
//    {
//        FT_Set_Transform(_face, &matrix, &pen);
//
//        if (FT_Load_Char(_face, p_text[i], FT_LOAD_RENDER))
//            continue;
//
//        if (!CreateShaderResourceView(p_device, &slot->bitmap,shaderResourceView))
//            return;
//
//        // Increment pen position
//        pen.x += slot->advance.x >> 6;
//    }
//}
//
//bool CreateShaderResourceView(ID3D11Device* p_device, FT_Bitmap* p_bitmap, ID3D11ShaderResourceView* shaderResourceView)
//{
//    D3D11_TEXTURE2D_DESC textureDesc;
//    ZeroMemory(&textureDesc, sizeof(textureDesc));
//
//    textureDesc.Width = p_bitmap->width;
//    textureDesc.Height = p_bitmap->rows;
//    textureDesc.MipLevels = 1;
//    textureDesc.ArraySize = 1;
//    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
//    textureDesc.SampleDesc.Count = 1;
//    textureDesc.SampleDesc.Quality = 0;
//    textureDesc.Usage = D3D11_USAGE_DEFAULT;
//    textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
//    textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
//    textureDesc.MiscFlags = 0;
//
//    ID3D11Texture2D* texture2D;
//    ZeroMemory(&texture2D, sizeof(texture2D));
//
//    D3D11_SUBRESOURCE_DATA resourceData;
//
//    resourceData.pSysMem = p_bitmap->buffer;
//    resourceData.SysMemPitch = p_bitmap->pitch;
//    resourceData.SysMemSlicePitch = 0;
//
//    HRESULT res = p_device->CreateTexture2D(&textureDesc, &resourceData, &texture2D);
//    if (FAILED(res))
//        return false;
//
//    D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
//    shaderResourceViewDesc.Format = textureDesc.Format;
//    shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
//    shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
//    shaderResourceViewDesc.Texture2D.MipLevels = 1;
//
//    HRESULT result = p_device->CreateShaderResourceView(texture2D, &shaderResourceViewDesc, &shaderResourceView);
//    if (FAILED(result))
//        return false;
//
//    return true;
//}