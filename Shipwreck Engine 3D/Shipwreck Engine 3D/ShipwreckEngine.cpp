#include "ShipwreckEngine.h"


ActualGameScript actualGameScript;


namespace GameScripting {
	std::map<std::string, GameScript*> nameToScript =
	{ {"ActualGameScript", &actualGameScript}};

	inline void AddScriptToGameObject(Container* scriptObject, std::string scriptName) {
		scriptObject->scripts.push_back(nameToScript[scriptName]);
		actualGameScript.scriptObjects.push_back(scriptObject->name);
	}
}