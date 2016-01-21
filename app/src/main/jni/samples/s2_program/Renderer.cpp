/*
 * Renderer.cpp
 *
 *  Created on: Apr 14, 2013
 *      Author: Thomas MILLET
 */
#include <jni.h>

#include <android/input.h>

#include "GlAsset.h"
#include "shader/GlProgram.h"

#include "Renderer.h"
#include "AmbientShader.h"

#include <android/log.h>
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "Renderer", __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "Renderer", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN,  "Renderer", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,  "Renderer", __VA_ARGS__))

Renderer::Renderer() : GlElement() {
	LOGD("NEW");
	this->contextManager = NULL;
	this->shader = NULL;
}

Renderer::~Renderer(){
	LOGD("DELETE");
	delete this->shader;
}

void Renderer::onCreate(ContextManager *contextManager){
	LOGD("onCreate()");
	this->contextManager = contextManager;
	this->shader = new AmbientShader(contextManager);
	this->shader->compile();
	glClearColor(0,0,0,1);
}

void Renderer::onResize(int width, int height){
	LOGD("onResize(%d, %d)",width, height);
}

void Renderer::onLayout(long elapsedTime){
	//LOGD("onLayout(%ld)",elapsedTime);
}

void Renderer::onDraw(){
	//LOGD("onDraw()");
	glClear(GL_COLOR_BUFFER_BIT);
	this->shader->render();
}

int Renderer::onEvent(AInputEvent* event){
	//LOGD("handleInput(%d)",AKeyEvent_getAction(event));
	return 0;
}

void Renderer::onFree(){
	LOGD("onFree()");
	this->shader->free();
}

void Renderer::onFpsRefresh(int fps){
	LOGD("FPS : %d",fps);
}
