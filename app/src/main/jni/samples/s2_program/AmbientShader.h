/*
 * AmbientShader.h
 *
 *  Created on: Apr 24, 2013
 *      Author: Thomas MILLET
 */

#ifndef AMBIENTSHADER_H_
#define AMBIENTSHADER_H_

#include <GLES2/gl2.h>

#include "ContextManager.h"
#include "shader/GlProgram.h"
#include "shader/GlShader.h"

class AmbientShader: public GlShader {
protected:
	GLfloat mvpMatrix[4][4];
	GLfloat mMatrix[4][4];
	GlContext *glContext;
	GlProgram *program;
	GLint u_mvpMatrixMat4Handle;
	GLint u_MaterialAmbientVec4Handle;
	GLint u_LightAmbientColorVec4Handle;
	GLint a_PositionVec4Handle;

public:
	AmbientShader(ContextManager *contextManager);
	virtual ~AmbientShader();

	void setQuality(GLenum quality){};
	void render(GlAsset::Node *nodeInstance){
		this->render();
	}

	void compile();
	void render();
	void free();
};

#endif /* AMBIENTSHADER_H_ */
