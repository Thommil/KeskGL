/*
 * Renderer.cpp
 *
 *  Created on: Apr 14, 2013
 *      Author: Thomas MILLET
 */
#include <jni.h>

#include <android/input.h>

#include "GlAsset.h"

#include "Renderer.h"

#include <android/log.h>
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "Renderer", __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "Renderer", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN,  "Renderer", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,  "Renderer", __VA_ARGS__))

Renderer::Renderer() : GlElement() {
	LOGD("NEW");
	this->contextManager = NULL;
}

Renderer::~Renderer(){
	LOGD("DELETE");
}

void Renderer::onCreate(ContextManager *contextManager){
	LOGD("onCreate()");
	this->contextManager = contextManager;
	glClearColor(0,0,0,1);
	LOGD("GL Renderer : %s", GlContext::getRenderer());
	LOGD("GL Vendor : %s", GlContext::getVendor());
	LOGD("GL Version : %s", GlContext::getVersion());
	LOGD("GL Extensions : %s", GlContext::getExtensions()->c_str());

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
}

int Renderer::onEvent(AInputEvent* event){
	LOGD("handleInput(%d)",AKeyEvent_getAction(event));
	if(AKeyEvent_getAction(event) == 0){
		this->contextManager->runOnGLTread(new RunnableTest("GL ACTION"));
	}
	else{
		this->contextManager->runOnUITread(new RunnableTest("UI ACTION"));
	}

	return 0;
}

void Renderer::onFree(){
	LOGD("onFree()");
}

void Renderer::onFpsRefresh(int fps){
	LOGD("FPS : %d",fps);
}

void Renderer::RunnableTest::run(ContextManager *contextManager){
	LOGD(this->text);
	delete this;
}

