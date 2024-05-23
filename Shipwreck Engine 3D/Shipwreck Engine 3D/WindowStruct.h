#pragma once
#include "UIElement.h"

#include "StartEngine.h"
using namespace EngineInstance;


/*  Format Types:
*  
*  TOP_TO_BOTTOM
*  LEFT_TO_RIGHT
*/

#define TOP_TO_BOTTOM 0
#define LEFT_TO_RIGHT 1


#include <memory>


class WindowStruct
{
public:
	void FormatElements(WindowStruct elementStructure);

	StartEngine& startEngine;

	HWND hWnd;

	std::unique_ptr<D3DGraphics> postGraphics;

	UINT formatType;
	std::vector<UIElement> elements;
};