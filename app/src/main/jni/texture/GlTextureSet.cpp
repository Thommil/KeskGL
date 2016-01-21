/*
 * GlTextureSet.cpp
 *
 *  Created on: Apr 21, 2013
 *      Author: Thomas MILLET
 */
#include "GlTextureSet.h"


#include <android/log.h>
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "GlTextureSet", __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "GlTextureSet", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN,  "GlTextureSet", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,  "GlTextureSet", __VA_ARGS__))

GlTextureSet::GlTextureSet() {
	//LOGD("NEW");

}

GlTextureSet::~GlTextureSet() {
	//LOGD("DELETE");
}

