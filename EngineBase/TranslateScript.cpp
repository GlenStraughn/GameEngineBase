//================================================================//
// TranslateScript
//	Script for translating a node
//================================================================//
// Haywire Studios
// Glen Straughn
// 11.26.2014
//================================================================//

#include "TranslateScript.h"

TranslateScript::TranslateScript()
{
	type = TRANSLATE;
	velocity = glm::vec3(0, 0, 0);
}

void TranslateScript::run()
{
	Transform* t = node->getTransform();
	node->setTranslation(t->translation + velocity);
}

void TranslateScript::setFloatArray(string variableName, float value[])
{
	if(variableName == "velocity")
	{
		velocity.x = value[0];
		velocity.y = value[1];
		velocity.z = value[2];
	}
}

void TranslateScript::setFloatValue(string variableName, float value)
{
	if(variableName == "x" || variableName == "xVel" || variableName == "xVelocity")  // I like giving people options...
	{
		velocity.x = value;
	}
	else if(variableName == "y" || variableName == "yVel" || variableName == "yVelocity")
	{
		velocity.y = value;
	}
	else if(variableName == "z" || variableName == "zVel" || variableName == "zVelocity")
	{
		velocity.z = value;
	}
}

float TranslateScript::getFloatAttribute(string variableName)
{
	if(variableName == "x" || variableName == "xVel" || variableName == "xVelocity")  // I like giving people options...
	{
		return velocity.x;
	}
	else if(variableName == "y" || variableName == "yVel" || variableName == "yVelocity")  // I like giving people options...
	{
		return velocity.y;
	}
	else if(variableName == "x" || variableName == "xVel" || variableName == "xVelocity")  // I like giving people options...
	{
		return velocity.z;
	}
}