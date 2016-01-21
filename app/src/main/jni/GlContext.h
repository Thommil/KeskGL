/*
 * GlContext.h
 *
 *  Created on: Apr 14, 2013
 *      Author: Thomas MILLET
 */
#include <string>
#include <GLES2/gl2.h>
#include "GlAsset.h"

using namespace std;

#ifndef GLCONTEXT_H_
#define GLCONTEXT_H_

/**
 * OpenGL context for Android 2.2+<br/>
 * <br/>
 * Based on OpenGLES 2.0
 */
class GlContext {
protected:
	static string extensions;

public:
	/**
	 * VIEW matrix for overall context
	 */
	GLfloat vMatrix[4][4];

	/**
	 * PROJECTION matrix for overall context
	 */
	GLfloat pMatrix[4][4];

	/**
	 * Assets bound to this context
	 */
	GlAsset assets;

public:
	GlContext();
	virtual ~GlContext();

	/**
	 * Returns the list of extensions for actual EGL
	 */
	static string* getExtensions();

	/**
	 *	Indicates if the extension is supported on current context
	 */
	static bool isExtensionSupported(const GLchar* name);

	/**
	 * Enable extension (in supported extensions list)
	 */
	static void enableExtension(const GLchar* name);

	/**
	 * Disable extension (in supported extensions list)
	 */
	static bool disableExtension(const GLchar* name);

	/**
	 * Get the OpenGL renderer
	 */
	static const GLchar* getRenderer(){ return (GLchar*)glGetString(GL_RENDERER); }

	/**
	 * Get the OpenGL vendor
	 */
	static const GLchar* getVendor(){ return (GLchar*)glGetString(GL_VENDOR); }

	/**
	 * Get the OpenGL version
	 */
	static const GLchar* getVersion(){ return (GLchar*)glGetString(GL_VERSION);}
};

#endif /* GLCONTEXT_H_ */
