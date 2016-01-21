/*
 * ContextManager.cpp
 *
 *  Created on: Apr 14, 2013
 *      Author: Thomas MILLET
 */
#include <hash_map>

#include "ContextManager.h"

#include <android/log.h>
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "ContextManager", __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "ContextManager", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN,  "ContextManager", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,  "ContextManager", __VA_ARGS__))

hash_map<unsigned int, ContextManager*> ContextManager::CTX_MAP;

ContextManager* ContextManager::fromId(unsigned int id){
	//LOGD("fromId : %d",id);
	if(ContextManager::CTX_MAP.count(id)){
		return ContextManager::CTX_MAP[id];
	}
	else return NULL;
}


ContextManager::ContextManager() {
	//LOGD("NEW");
	this->id=ContextManager::CTX_MAP.size();
	ContextManager::CTX_MAP.insert(pair<int,ContextManager*>(this->id,this));
}

ContextManager::~ContextManager() {
	//LOGD("DELETE");
	ContextManager::CTX_MAP.erase(this->id);
}
