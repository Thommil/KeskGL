/*
 * GlContext.cpp
 *
 *  Created on: Apr 14, 2013
 *      Author: Thomas MILLET
 */
#include <string>
#include <GLES2/gl2.h>

#include "GlContext.h"

#include <nv_math/nv_matrix.h>

#include <android/log.h>
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "GlContext", __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "GlContext", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN,  "GlContext", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,  "GlContext", __VA_ARGS__))

string GlContext::extensions;

GlContext::GlContext() {
	//LOGD("NEW");
	NvBuildIdentityMatf(this->vMatrix);
	NvBuildIdentityMatf(this->pMatrix);
}

GlContext::~GlContext() {
	//LOGD("DELETE");
}

string* GlContext::getExtensions(){
	if(GlContext::extensions.empty()){
		GlContext::extensions = (char*)glGetString(GL_EXTENSIONS);
	}
	return &GlContext::extensions;
}

bool GlContext::isExtensionSupported(const GLchar* name){
	//LOGD("isExtensionSupported : %s",name.data());
	return GlContext::getExtensions()->find(name) != string::npos;
}

void GlContext::enableExtension(const GLchar* name){
	//LOGD("disableExtension : %s",name.data());
	size_t pos = GlContext::getExtensions()->find(name);
	if(pos != string::npos){
		GlContext::getExtensions()->append(name);
	}
}

bool GlContext::disableExtension(const GLchar* name){
	//LOGD("disableExtension : %s",name.data());
	size_t pos = GlContext::getExtensions()->find(name);
	if(pos != string::npos){
		GlContext::getExtensions()->erase(pos, strlen(name));
		return true;
	}
	else return false;
}
