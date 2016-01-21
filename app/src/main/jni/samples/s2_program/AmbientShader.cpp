/*
 * AmbientShader.cpp
 *
 *  Created on: Apr 24, 2013
 *      Author: Thomas MILLET
 */

#include "ContextManager.h"
#include "AmbientShader.h"

#include <nv_file/nv_file.h>
#include <nv_math/nv_matrix.h>

#include <android/log.h>
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "AmbientShader", __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "AmbientShader", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN,  "AmbientShader", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,  "AmbientShader", __VA_ARGS__))

AmbientShader::AmbientShader(ContextManager *contextManager) {
	LOGD("NEW");
	this->glContext = contextManager->getGLContext();
	NvFInit(contextManager->getUIContext()->activity->assetManager);
	this->u_mvpMatrixMat4Handle = 0;
	this->u_MaterialAmbientVec4Handle = 0;
	this->u_LightAmbientColorVec4Handle = 0;
	this->a_PositionVec4Handle = 0;
}

AmbientShader::~AmbientShader() {
	LOGD("DELETE");
	delete this->program;
}

void AmbientShader::compile(){
	LOGD("compile()");
	NvFile* file = NvFOpen("shaders/material/ambient/notex.vert");
	if(file){
		size_t fileSize = NvFSize(file);
		char *vcontent = new char[fileSize+1];
		NvFRead((char*)vcontent, 1, fileSize, file);
		vcontent[fileSize] = '\0';
		NvFClose(file);

		file = NvFOpen("shaders/material/ambient/notex.frag");
		if(file){
			fileSize = NvFSize(file);
			char *fcontent = new char[fileSize+1];
			NvFRead((char*)fcontent, 1, fileSize, file);
			fcontent[fileSize] = '\0';
			NvFClose(file);

			this->program = new GlProgram(vcontent,fcontent);

			if(this->program->programHandle){
				this->u_mvpMatrixMat4Handle = this->program->getUniformHandle("u_mvpMatrixMat4");
				this->u_MaterialAmbientVec4Handle = this->program->getUniformHandle("u_MaterialAmbientVec4");
				this->u_LightAmbientColorVec4Handle = this->program->getUniformHandle("u_LightAmbientColorVec4");
				this->a_PositionVec4Handle = this->program->getAttributeHandle("a_PositionVec4");

				GLfloat eyeVec3[3] = {0,0,1};
				GLfloat interestVec3[3] = {0,0,0};
				GLfloat upVec3[3] = {0,1,0};
				NvBuildLookatMatf(this->glContext->vMatrix, eyeVec3, interestVec3, upVec3);
				NvBuildOrthoMatf(this->glContext->pMatrix, -1, 1, -1, 1, 0.1, 2);
				NvBuildIdentityMatf(this->mMatrix);

				glUniform4f(this->u_MaterialAmbientVec4Handle, 1, 1, 1, 1);
				glUniform4f(this->u_LightAmbientColorVec4Handle, 1, 1, 1, 1);
			}

			delete[] vcontent;
			delete[] fcontent;
		}
		else{
			LOGE("Failed to open font.frag");
		}
	}
	else{
		LOGE("Failed to open font.vert");
	}
}

void AmbientShader::render(){
	if(this->program && this->program->programHandle){
		this->program->start();
		this->program->enableAttributes();

		NvMultRotZDegMatf(this->mMatrix,this->mMatrix,2);
		NvMultMatf(this->mvpMatrix,this->glContext->vMatrix,this->mMatrix);
		NvMultMatf(this->mvpMatrix,this->glContext->pMatrix,this->mvpMatrix);

		glUniformMatrix4fv(this->u_mvpMatrixMat4Handle,1,false,(const GLfloat *)this->mvpMatrix);

		GLfloat vertices[12] = {
				-0.5, 0.5, 0.0,
				-0.5, -0.5, 0.0,
				0.5, -0.5, 0.0,
				0.5, 0.5, 0.0
		};

		glVertexAttribPointer(this->a_PositionVec4Handle,3,GL_FLOAT,false,0,vertices);
		glDrawArrays(GL_TRIANGLE_FAN,0,4);

		this->program->disableAttributes();
		this->program->stop();
	}
}

void AmbientShader::free(){
	LOGD("free()");
	if(this->program){
		this->program->free();
	}
}
