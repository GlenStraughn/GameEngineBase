//
//  Material.cpp
//  EngineBase
//
//  Created by STRAUGHN GLEN K on 9/17/14.
//  Copyright (c) 2014 dave. All rights reserved.
//

#include "Material.h"


void Material::bindMaterial(Transform &T, Camera &camera)
{
    // Send Colors to OpenGL
    glUseProgram(shaderProgram);
    
	// MATRICES FROM TRANSFORM
	GLint loc = glGetUniformLocation(shaderProgram, "uObjectWorldM");
	if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(T.transform));
	//
	loc = glGetUniformLocation(shaderProgram, "uObjectWorldInverseM");
	if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(T.invTransform));
	//
	glm::mat4x4 objectWorldViewPerspect = camera.worldViewProject * T.transform;
	loc = glGetUniformLocation(shaderProgram, "uObjectPerpsectM");
	if (loc != -1) glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(objectWorldViewPerspect));
    
	// MATERIAL COLORS
	for (int i = 0; i < (int) colors.size(); i++) {
		if (colors[i].id == -1) {
			loc = glGetUniformLocation(shaderProgram, colors[i].name.c_str());
			colors[i].id = loc;
		}
		if (colors[i].id >= 0) {
			glUniform4fv(colors[i].id, 1, &colors[i].val[0]);
		}
	}
    
	// MATERIAL TEXTURES
	for (int i = 0; i < (int) textures.size(); i++) {
		if (textures[i].id == -1) {
			loc = glGetUniformLocation(shaderProgram, textures[i].name.c_str());
			textures[i].id = loc;
		}
		if (textures[i].id >= 0) {
			//printf("\n%d %d\n", textures[i].id, textures[i].val->samplerId);
			glActiveTexture(GL_TEXTURE0 + i);
			glUniform1i(textures[i].id, i);
			glBindTexture(GL_TEXTURE_2D, textures[i].val->textureId);
			glBindSampler(textures[i].id, textures[i].val->samplerId);
		}
	}
}