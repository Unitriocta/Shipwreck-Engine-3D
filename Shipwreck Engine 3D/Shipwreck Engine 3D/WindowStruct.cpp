#include "WindowStruct.h"


#include "MathExtras.h"



void WindowStruct::FormatElements(WindowStruct elementStructure) {

	switch (elementStructure.formatType)
	{
		case TOP_TO_BOTTOM:
		{
			for (int i = 0; i < elementStructure.elements.size(); i++) {
				UIElement element = elementStructure.elements[i];

				startEngine.Gfx().DrawRect(Vec2(0.0f, 0.0f), element.size, 19, element.elementRoundness, startEngine.Math().NormalizedColor(0, 255, 0, 255), elementStructure.hWnd);
			}
		}
		break;

		case LEFT_TO_RIGHT:
		{

		}
		break;
	}
}