/*
 * GlAsset.cpp
 *
 *  Created on: Apr 14, 2013
 *      Author: Thomas MILLET
 */
#include "GlAsset.h"

#include <GLES2/gl2.h>

#include <android/log.h>
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, "GlAsset", __VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "GlAsset", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN,  "GlAsset", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR,  "GlAsset", __VA_ARGS__))

GlAsset::GlAsset() {
	//LOGD("NEW");
}

GlAsset::~GlAsset() {
	//LOGD("DELETE");
	this->free();
}

GlAsset::Geometry::Geometry(){
	//LOGD("NEW Geometry");
	this->id = GL_NONE;
	this->elements = NULL;
	this->elementsCount = GL_NONE;
}

GlAsset::Geometry::~Geometry(){
	//LOGD("NEW Geometry");
	for(int i=0; i < this->elementsCount; i++){
		delete[] this->elements;
	}
}

GlAsset::Geometry::Element::Element(){
	//LOGD("NEW Geometry::Element");
	this->type = TRIANGLES;
	this->count = GL_NONE;
	this->size = GL_NONE;
	this->materialId = GL_NONE;
	this->inputs = NULL;
	this->values = NULL;
	this->handle = GL_NONE;
	this->stride = GL_NONE;
}

GlAsset::Geometry::Element::~Element(){
	//LOGD("DELETE Geometry::Element");
	if(this->handle){
		glDeleteBuffers(1, &this->handle);
	}
	for(int i=0; i < this->count ; i++){
		delete[] ((this->inputs) + i);
		if(this->values) delete[] ((this->values) + i);
	}
}

GlAsset::Light::Light(){
	//LOGD("NEW Light");
	this->id = GL_NONE;
	this->type = GL_NONE;
	this->color[0] = 1.0;
	this->color[1] = 1.0;
	this->color[2] = 1.0;
	this->color[3] = 1.0;
	this->settings[0] = GL_NONE;
	this->settings[1] = GL_NONE;
	this->settings[2] = GL_NONE;
	this->settings[3] = GL_NONE;
	this->settings[4] = GL_NONE;
}

GlAsset::Light::~Light(){
	//LOGD("DELETE Light");
}

GlAsset::Camera::Camera(){
	//LOGD("NEW Camera");
	this->id = GL_NONE;
	this->type = GL_NONE;
	this->settings[0] = GL_NONE;
	this->settings[1] = GL_NONE;
	this->settings[2] = GL_NONE;
	this->settings[3] = GL_NONE;
	this->settings[4] = GL_NONE;
	this->up[0] = 0.0;
	this->up[1] = 0.0;
	this->up[2] = 1.0;
}

GlAsset::Camera::~Camera(){
	//LOGD("DELETE Camera");
}

GlAsset::Material::Material(){
	//LOGD("NEW Material");
	this->id = GL_NONE;
	this->type = GL_NONE;
	this->emissionColor = NULL;
	this->emissionBinding = NULL;
	this->reflectiveColor = NULL;
	this->reflectiveBinding = NULL;
	this->reflectivity = GlAsset::Material::NOT_SET;
	this->transparentColor = NULL;
	this->transparentBinding = NULL;
	this->transparency = GlAsset::Material::NOT_SET;
	this->refraction = NULL;
	this->ambientColor = NULL;
	this->ambientBinding = NULL;
	this->diffuseColor = NULL;
	this->diffuseBinding = NULL;
	this->specularColor = NULL;
	this->specularBinding = NULL;
	this->shininess = GlAsset::Material::NOT_SET;
}

GlAsset::Material::~Material(){
	//LOGD("DELETE Material");
	if(this->emissionColor != NULL) delete[] this->emissionColor;
	if(this->emissionBinding != NULL) delete[] this->emissionBinding;
	if(this->reflectiveColor != NULL) delete[] this->reflectiveColor;
	if(this->reflectiveBinding != NULL) delete[] this->reflectiveBinding;
	if(this->transparentColor != NULL) delete[] this->transparentColor;
	if(this->transparentBinding != NULL) delete[] this->transparentBinding;
	if(this->refraction != NULL) delete[] this->refraction;
	if(this->ambientColor != NULL) delete[] this->ambientColor;
	if(this->ambientBinding != NULL) delete[] this->ambientBinding;
	if(this->diffuseColor != NULL) delete[] this->diffuseColor;
	if(this->diffuseBinding != NULL) delete[] this->diffuseBinding;
	if(this->specularColor != NULL) delete[] this->specularColor;
	if(this->specularBinding != NULL) delete[] this->specularBinding;
}

GlAsset::Node::Node(){
	//LOGD("NEW Node");
	this->id = GL_NONE;
	this->type = GL_NONE;
	this->cameraInstances = NULL;
	this->cameraInstancesCount = GL_NONE;
	this->lightInstances = NULL;
	this->lightInstancesCount = GL_NONE;
	this->nodeInstances = NULL;
	this->nodeInstancesCount = GL_NONE;
	this->geometryInstances = NULL;
	this->geometryInstancesCount = GL_NONE;
}

GlAsset::Node::~Node(){
	//LOGD("DELETE Node");
	delete[] this->geometryInstances;
}

GlAsset::Node::GeometryInstance::GeometryInstance(){
	//LOGD("NEW GeometryInstance");
	this->geometry = NULL;
	this->materials = NULL;
	this->materialsCount = GL_NONE;
}

GlAsset::Node::GeometryInstance::~GeometryInstance(){
	//LOGD("DELETE GeometryInstance");
	delete[] this->materials;
}

GlAsset::Node::GeometryInstance::MaterialInstance::MaterialInstance(){
	//LOGD("NEW MaterialInstance");
	this->id = GL_NONE;
	this->target = NULL;
	this->bindings = NULL;
	this->bindingsCount = GL_NONE;
}

GlAsset::Node::GeometryInstance::MaterialInstance::~MaterialInstance(){
	//LOGD("DELETE MaterialInstance");
	for(int i=0; i < this->bindingsCount ; i++){
		delete[] ((this->bindings) + i);
	}
}

GlAsset::Scene::Scene(){
	//LOGD("NEW Scene");
	this->id = GL_NONE;
	this->nodeInstances = NULL;
	this->nodeInstancesCount = 0;
}

GlAsset::Scene::~Scene(){
	//LOGD("NEW Scene");
	delete[] this->nodeInstances;
}

void GlAsset::free(){
	this->freeLights();
	this->freeCameras();
	this->freeGeometries();
	this->freeMaterials();
	this->freeNodes();
	this->freeScenes();
}

void GlAsset::freeGeometries(){
	this->geometries.clear();
}

void GlAsset::freeGeometry(GLuint id){
	this->geometries.erase(id);
}

void GlAsset::freeLights(){
	this->lights.clear();
}

void GlAsset::freeLight(GLuint id){
	this->lights.erase(id);
}

void GlAsset::freeCameras(){
	this->cameras.clear();
}

void GlAsset::freeCamera(GLuint id){
	this->cameras.erase(id);
}

void GlAsset::freeTextures(){

}

void GlAsset::freeTexture(GLuint id){

}

void GlAsset::freeMaterials(){
	this->materials.clear();
}

void GlAsset::freeMaterial(GLuint id){
	this->materials.erase(id);
}

void GlAsset::freeNodes(){
	this->nodes.clear();
}

void GlAsset::freeNode(GLuint id){
	this->nodes.erase(id);
}

void GlAsset::freeScenes(){
	this->scenes.clear();
}

void GlAsset::freeScene(GLuint id){
	this->scenes.erase(id);
}


