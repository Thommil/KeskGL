/*
 * GlProgram.cpp
 *
 *  Created on: Apr 22, 2013
 *      Author: Thomas MILLET
 */
#include <GLES2/gl2.h>
#include <string>

#include <hash_map>

#include "GlProgram.h"

#include <android/log.h>
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "GlProgram", __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "GlProgram", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN,  "GlProgram", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,  "GlProgram", __VA_ARGS__))

GlProgram::GlProgram(const GLchar* vertexShaderSrc, const GLchar* fragmentShaderSrc) {
	//LOGD("NEW");
	this->programHandle = UNBIND_HANDLE;
	this->vertexShaderHandle = this->loadShader(GL_VERTEX_SHADER, vertexShaderSrc);
	this->fragmentShaderHandle = this->loadShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
	if(this->vertexShaderHandle != UNBIND_HANDLE && this->fragmentShaderHandle != UNBIND_HANDLE){
		this->createAndLinkProgram();
	}
}

GLuint GlProgram::loadShader(GLenum type, const GLchar* src){
	//LOGD("loadShader()");
	GLuint shaderHandle = glCreateShader(type);

	glShaderSource(shaderHandle, 1, &src, NULL);
	glCompileShader(shaderHandle);

	GLint compileStatus;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS,&compileStatus);

	if(compileStatus == GL_FALSE) {
		glDeleteShader(shaderHandle);
		LOGE("Failed to compile %s",((type == GL_VERTEX_SHADER)? "vertex":"fragment"));
		return UNBIND_HANDLE;
	}

	//Parse shader to store attributes and uniform handles
	string srcString(src);
	size_t mainStart = srcString.find("void main()");
	bool goOn = true;
	size_t pos = 0;
	while(goOn){
		pos = srcString.find("attribute ", pos);
		if(pos < mainStart && pos != string::npos){
			int start = pos + 9;
			while(srcString.at(++start) == ' ');
			while(srcString.at(++start) != ' ');
			while(srcString.at(++start) == ' ');
			int stop = start;
			while(srcString.at(stop) != ' ' && srcString.at(stop) != ';') stop++;
			GLchar *attributeName = new GLchar[(stop-start)+1];
			srcString.copy(attributeName, stop-start,start);
			attributeName[stop-start] = '\0';
			if(this->attributeHandles.count(attributeName) == 0){
				this->attributeHandles[attributeName] = UNBIND_HANDLE;
			}
			pos = stop;
		}
		else{
			goOn = false;
		}
	}

	goOn = true;
	pos = 0;
	while(goOn){
		pos = srcString.find("uniform ", pos);
		if(pos < mainStart && pos != string::npos){
			int start = pos + 7;
			while(srcString.at(++start) == ' ');
			while(srcString.at(++start) != ' ');
			while(srcString.at(++start) == ' ');
			int stop = start;
			while(srcString.at(stop) != ' ' && srcString.at(stop) != ';') stop++;
			GLchar *uniformName = new GLchar[(stop-start)+1];
			srcString.copy(uniformName, stop-start,start);
			uniformName[stop-start] = '\0';
			if(this->uniformHandles.count(uniformName) == 0){
				this->uniformHandles[uniformName] = UNBIND_HANDLE;
			}
			pos = stop;
		}
		else{
			goOn = false;
		}
	}

	return shaderHandle;
}

void GlProgram::createAndLinkProgram(){
	//LOGD("createAndLinkProgram()");
	this->programHandle = glCreateProgram();

	if (this->programHandle != UNBIND_HANDLE) {
		// Bind the vertex shader to the program.
		glAttachShader(this->programHandle, this->vertexShaderHandle);

		// Bind the fragment shader to the program.
		glAttachShader(this->programHandle, this->fragmentShaderHandle);

		// Link the two shaders together into a program.
		glLinkProgram(this->programHandle);

		// Get the link status.
		int linkStatus;
		glGetProgramiv(this->programHandle, GL_LINK_STATUS, &linkStatus);

		if (linkStatus == GL_FALSE) {
			char errorMsg[256];
			int errorMsgLength = 0;
			glGetProgramInfoLog(this->programHandle, 256, &errorMsgLength, errorMsg);
			glDeleteProgram(this->programHandle);
			LOGE("Failed to link program : %s",errorMsg);
			return;
		}

		this->start();

		//Update handles and fill the attributes cache
		hash_map<const GLchar *, GLint>::iterator it = this->attributeHandles.begin();
		while(it != this->attributeHandles.end()){
			it->second = glGetAttribLocation(this->programHandle, it->first);
			it++;
		}

		it = this->uniformHandles.begin();
		while(it != this->uniformHandles.end()){
			it->second = glGetUniformLocation(this->programHandle, it->first);
			it++;
		}
	}
}

void GlProgram::start(){
	//LOGD("start()");
	if(!this->isEnabled){
		glUseProgram(this->programHandle);
		this->isEnabled = true;
	}
}

void GlProgram::stop(){
	//LOGD("stop()");
	if(this->isEnabled){
		glUseProgram(UNBIND_HANDLE);
		this->isEnabled = false;
	}
}

GLint GlProgram::getAttributeHandle(const GLchar* name){
	if(this->attributeHandles.count(name)){
		return this->attributeHandles[name];
	}
	return UNBIND_HANDLE;
}


GLint GlProgram::getUniformHandle(const GLchar* name){
	if(this->uniformHandles.count(name)){
		return this->uniformHandles[name];
	}
	return UNBIND_HANDLE;
}

void GlProgram::enableAttributes(){
	//LOGD("enableAttributes()");
	hash_map<const GLchar *, GLint>::iterator it = this->attributeHandles.begin();
	while(it != this->attributeHandles.end()){
		glEnableVertexAttribArray(it->second);
		it++;
	}
}

void GlProgram::disableAttributes(){
	//LOGD("disableAttributes()");
	hash_map<const GLchar *, GLint>::iterator it = this->attributeHandles.begin();
	while(it != this->attributeHandles.end()){
		glDisableVertexAttribArray(it->second);
		it++;
	}
}

void GlProgram::free(){
	//LOGD("free()");
	if(this->programHandle != UNBIND_HANDLE) {
		glDeleteProgram(this->programHandle);
	}
	if(this->vertexShaderHandle != UNBIND_HANDLE) {
		glDeleteShader(this->vertexShaderHandle);
	}
	if(this->fragmentShaderHandle != UNBIND_HANDLE) {
		glDeleteShader(this->fragmentShaderHandle);
	}
}

GlProgram::~GlProgram() {
	//LOGD("DELETE");
	this->attributeHandles.clear();
	this->uniformHandles.clear();
}

