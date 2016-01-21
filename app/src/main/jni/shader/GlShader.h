/*
 * GlShader.h
 *
 *  Created on: Apr 22, 2013
 *      Author: Thomas MILLET
 */
#include <GLES2/gl2.h>
#include "GlAsset.h"

using namespace std;

#ifndef GLSHADER_H_
#define GLSHADER_H_

/**
 * Generic Shader API for drawing operations.
 */
class GlShader {
public:
	virtual ~GlShader(){};

	/**
	 * VERY_LOW quality settings
	 */
	const static GLenum QUALITY_VERY_LOW = 64;

	/**
	 * LOW quality settings
	 */
	const static GLenum QUALITY_LOW = 128;

	/**
	 * MEDIUM quality settings
	 */
	const static GLenum QUALITY_MEDIUM = 256;

	/**
	 * HIGH quality settings
	 */
	const static GLenum QUALITY_HIGH = 512;

	/**
	 * VERY_HIGH quality settings
	 */
	const static GLenum QUALITY_VERY_HIGH = 1024;

	/**
	 * Set the shader quality based on shader available quality modes
	 *
	 * @param quality The quality indice depending on implementation
	 */
	virtual void setQuality(GLenum quality) = 0;

	/**
	 * Compile the shader (implementation must authorized multiple calls)
	 */
	virtual void compile() = 0;

	/**
	 * Called on draw state to render current shader state
	 */
	virtual void render() = 0;

	/**
	 * Called on draw state to render a node and its children using current shader
	 *
	 * @param node The nodes instance to render
	 */
	virtual void render(GlAsset::Node *nodeInstance) = 0;

	/**
	 * Free resources allocated by this shader
	 */
	virtual void free() = 0;
};

#endif /* GLSHADER_H_ */
