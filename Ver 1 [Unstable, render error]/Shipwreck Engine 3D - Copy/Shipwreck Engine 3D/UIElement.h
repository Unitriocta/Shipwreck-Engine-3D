#pragma once

#include <string>

#include <Windows.h>
#include "Graphics.h"


#include "MathExtras.h"


/*
  Element Format types; How an element is formatted within a window within the WindowStruct class:
    * LABEL_BEFORE
    * LABEL_AFTER
	* NO_LABEL
*/

#define LABEL_BEFORE  1   constexpr;
#define LABEL_AFTER   2   constexpr;

/*
  Dropdown types; How a dropdown element responds to user input:
    * MULTIPLE_CHOICE
    * SINGLE_CHOICE
*/

#define MULTIPLE_CHOICE 1   constexpr;
#define SINGLE_CHOICE   2   constexpr;



/*TODO::
        * Element size, [,/]
		* Position, [,/]
		* Roundness [,/]
		* Color
*/
class UIElement
{
public:
	enum class ElementType // To access a type in here, use UIElement::ElementType::<Element Type>
	{
		Button,
		Checkbox,
		Scrollbar,
		Slider,
		InputField,
		Label, //a string displayed, just displays the elementName, potentially centered?
		Dropdown,
		Image //A logo that can be displayed, you just need to give it a path
	};

	ElementType elementType;



	int labelType; // Uses the defined variables to specify label location

	std::string label; // Displays based on the labelType; before element, after element, or no label

	std::string elementName; // id / name of the element




	float elementRoundness; //applies to all elements

	float dropDownRoundness; //applies to all dropdowns

	Vec2 size;


	float pastSpacing; //spacing between the previous element and this one, this moves THIS element further away from the previous the higher the number is
	//^^^^^^^^^^^^^^^^^ This can change if the WindowStruct format is LEFT_TO_RIGHT, rather than TOP_TO_BOTTOM

	float nextSpacing; //spacing between the previous element and this one, this moves the NEXT element further away the higher the number is
	//^^^^^^^^^^^^^^^^^ This can change if the WindowStruct format is LEFT_TO_RIGHT, rather than TOP_TO_BOTTOM



	bool isActive;      //                        applies to buttons and checkboxes

	float sliderValue;  //                        applies to sliders and scrollbars

	std::string text;   //                        applies to input fields and labels


	int dropdownType;   //                        applies to dropdowns
	std::vector<std::string> options;          //   applies to dropdowns
	std::vector<std::string> optionsSelected;  //   applies to dropdowns


	std::string imagePath;  //                    applies to images

};