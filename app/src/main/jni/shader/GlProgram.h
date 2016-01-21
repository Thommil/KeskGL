/*
 * GlProgram.h
 *
 *  Created on: Apr 22, 2013
 *      Author: Thomas MILLET
 */
#include <GLES2/gl2.h>
#include <string>
#include <hash_map>

using namespace std;

#ifndef GLPROGRAM_H_
#define GLPROGRAM_H_


/**
 * Helper class to get shaders source code and link program
 *
 * This approach allows automatic linking based on parsing.
 */
class GlProgram {
private:
	/**
	 * Indicates if current program is enabled on pipeline
	 */
	bool isEnabled;

	/**
	 * Comparison function for hash_maps
	 */
	struct eqstr{
	  bool operator()(const GLchar* s1, const GLchar* s2) const{
	    return strcmp(s1, s2) == 0;
	  }
	};

	/**
	 *  Handles on GSGL attributes
	 */
	hash_map<const GLchar*, GLint, hash<const GLchar*>, eqstr> attributeHandles;

	/**
	 *  Handles on GSGL uniforms
	 */
	hash_map<const GLchar*, GLint, hash<const GLchar*>, eqstr> uniformHandles;

public:
	/**
	 * Handle for unbound program
	 */
	static const int UNBIND_HANDLE = GL_ZERO;

	/**
	 * Handle on current program
	 */
	GLuint programHandle;

	/**
	 * Handle of the vertex shader
	 */
	GLuint vertexShaderHandle;

	/**
	 *  Handle of the fragment shader
	 */
	GLuint fragmentShaderHandle;

protected:
	GLuint loadShader(GLenum type, const GLchar *src);
	void createAndLinkProgram();

public:
	GlProgram(const GLchar *vertexShaderSrc, const GLchar *fragmentShaderSrc);
	virtual ~GlProgram();

	/**
	 * Enabled the program on pipeline.<br/>
	 *
	 * Caution : only calls to start/stop are taken into account for current state
	 */
	void start();

	/**
	 * Disable the program on pipeline.<br/>
	 *
	 * Caution : only calls to start/stop are taken into account for current state
	 */
	void stop();

	/**
	 * Get the handle of a specified attribute
	 */
	GLint getAttributeHandle(const GLchar *name);

	/**
	 * Get the handle of a specified uniform
	 */
	GLint getUniformHandle(const GLchar *name);

	/**
	 * Enable all attributes
	 */
	void enableAttributes();

	/**
	 * Disable all attributes
	 */
	void disableAttributes();

	/**
	 * Free resources
	 */
	void free();
};

#endif /* GLPROGRAM_H_ */
