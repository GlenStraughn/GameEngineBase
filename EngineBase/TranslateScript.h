//================================================================//
// TranslateScript
//	Script for translating a node
//================================================================//
// Haywire Studios
// Glen Straughn
// 11.26.2014
//================================================================//

#pragma once

#include "Script.h"
#include "ScriptTypeEnum.h"
#include <glm/glm.hpp>

class TranslateScript : public Script
{
public:
	TranslateScript();

	void run();

	void setFloatArray(string variableName, float array[]);
	void setFloatValue(string variableName, float value);
	float getFloatAttribute(string variableName);

protected:
	glm::vec3 velocity;
};