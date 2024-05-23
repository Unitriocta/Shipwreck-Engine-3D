#pragma once
#include "UITypes.h"
#include <iostream>
#include <vector>



namespace Console
{
	class ConsoleInfo {
	public:
		ConsoleInfo() {
			fontSize = 12.0f;
			consoleRect = Vec2(400.0f, 400.0f);
			consoleTopLeftPos = Vec2(-200.0f, -40.0f);
			//lineSpacing = 50.0f;
			//fontSize = 12.0f;
		}

		std::vector<Textbox> lines;


		FontCreation::SetupFace* fontManager;
		Vec2 consoleRect = Vec2(400.0f, 400.0f);
		Vec2 consoleTopLeftPos = Vec2(-400.0f, -40.0f);
		float lineSpacing = 50.0f;
		float fontSize = 12.0f;
		HWND hWnd;
	};

	extern ConsoleInfo console;
	

	inline void ConsoleSetup(FontCreation::SetupFace* _fontManager, HWND hWnd) {
		console.fontManager = _fontManager;
		console.hWnd = hWnd;
	}

	inline ConsoleInfo* GetConsole() {
		return &console;
	}

	inline void ConsolePrint(std::string& newText) {

		const char* consoleCharText = newText.c_str();

		int totalRows = 0;

		for (int i = 0; i < console.lines.size(); i++) {
			totalRows += console.lines[i].rows;
		}

		Textbox printTextbox = Textbox((char*)consoleCharText, console.fontManager, console.consoleRect, Vec2(console.consoleTopLeftPos.x, console.consoleTopLeftPos.y - (console.lineSpacing * totalRows)), console.fontSize, console.hWnd);
		printTextbox.LoadText();
		console.lines.push_back(printTextbox);
		
		if (console.lines.size() >= 9) {
			console.lines.erase(console.lines.begin());
		}
	}

}