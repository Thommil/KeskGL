/*
 * GlTExture.h
 *
 *  Created on: Apr 22, 2013
 *      Author: Thomas MILLET
 */
#include <GLES2/gl2.h>

using namespace std;

#ifndef GLTEXTURE_H_
#define GLTEXTURE_H_

typedef char byte;

/**
 * Abstraction class to define a texture source data
 */
class GlTexture {
public:
	/**
	 * Handle for unbound texture
	 */
	static const GLuint UNBIND_HANDLE = GL_ZERO;

	/**
	 * Texture is a standard 2D texture
	 */
	static const GLenum TARGET_TEXTURE_2D = GL_TEXTURE_2D;

	/**
	 * Texture format ALPHA
	 *
	 * Each element is a single alpha component.
	 *
	 * The GL converts it to floating point and assembles it into
	 * an RGBA element by attaching 0 for red, green, and blue.
	 * Each component is then clamped to the range [0,1].
	 */
	static const GLint FORMAT_ALPHA = GL_ALPHA;

	/**
	 * Texture format RGB
	 *
	 * Each element is an RGB triple.
	 *
	 * The GL converts it to floating point and assembles it into
	 * an RGBA element by attaching 1 for alpha.
	 * Each component is then clamped to the range [0,1].
	 */
	static const GLint FORMAT_RGB = GL_RGB;

	/**
	 * Texture format RGBA
	 *
	 * Each element contains all four components.
	 * The GL converts it to floating point.
	 * Each component is clamped to the range [0,1].
	 */
	static const GLint FORMAT_RGBA = GL_RGBA;

	/**
	 * Texture format LUMINANCE
	 *
	 * Each element is a single luminance value.
	 * The GL converts it to floating point, then assembles it into
	 * an RGBA element by replicating the luminance value three times
	 * for red, green, and blue and attaching 1 for alpha.
	 * Each component is then clamped to the range [0,1].
	 */
	static const GLint FORMAT_LUMINANCE = GL_LUMINANCE;

	/**
	 * Texture format LUMINANCE_ALPHA
	 *
	 * Each element is a luminance/alpha pair.
	 * The GL converts it to floating point, then assembles it into
	 * an RGBA element by replicating the luminance value three times
	 * for red, green, and blue.
	 * Each component is then clamped to the range [0,1].
	 */
	static const GLint FORMAT_LUMINANCE_ALPHA = GL_LUMINANCE_ALPHA;

	/**
	 * Indicates no compression type (other types are available in {@link GlTexture}
	 */
	static const GLenum COMP_FALSE = GL_FALSE;

	/**
	 * Compression format for ETC1
	 */
	static const GLenum COMP_ETC1 = 0x8d64;

	/**
	 * Compression format for ATITC for RGB textures
	 */
	static const GLenum COMP_ATC_RGB_AMD = 0x8C92;

	/**
	 * Compression format for ATITC for RGBA textures using explicit alpha encoding
	 */
	static const GLenum COMP_ATC_RGBA_EXPLICIT_ALPHA_AMD = 0x8C93;

	/**
	 * Compression format for ATITC for RGBA textures using interpolated alpha encoding
	 */
	static const GLenum COMP_RGBA_INTERPOLATED_ALPHA_AMD = 0x87EE;

	/**
	 * Compression format for PVRTC RGB 4bits
	 */
	static const GLenum COMP_RGB_PVRTC_4BPPV1_IMG = 0x8C00;

	/**
	 * Compression format for PVRTC RGB 2bits
	 */
	static const GLenum COMP_RGB_PVRTC_2BPPV1_IMG = 0x8C01;

	/**
	 * Compression format for PVRTC RGBA 4bits
	 */
	static const GLenum COMP_RGBA_PVRTC_4BPPV1_IMG = 0x8C02;

	/**
	 * Compression format for PVRTC RGBA 2bits
	 */
	static const GLenum COMP_RGBA_PVRTC_2BPPV1_IMG = 0x8C03;

	/**
	 * Compression format for S3TC DXT1 RGB
	 */
	static const GLenum COMP_RGB_S3TC_DXT1_EXT = 0x83F1;

	/**
	 * Compression format for S3TC DXT1 RGBA
	 */
	static const GLenum COMP_RGBA_S3TC_DXT1_EXT = 0x83F2;

	/**
	 * Compression format for S3TC DXT3
	 */
	static const GLenum COMP_RGBA_S3TC_DXT3_ANGLE = 0x83F3;

	/**
	 * Compression format for S3TC DXT5 RGB
	 */
	static const GLenum COMP_RGBA_S3TC_DXT5_ANGLE = 0x83F0;

	/**
	 * Compression format for 3DC X
	 */
	static const GLenum COMP_3DC_X_AMD = 0x87F9;

	/**
	 * Compression format for 3DC XY
	 */
	static const GLenum COMP_3DC_XY_AMD = 0x87FA;


	/**
	 * Texture color type for GL_UNSIGNED_BYTE
	 */
	static const GLenum TYPE_UNSIGNED_BYTE = GL_UNSIGNED_BYTE;

	/**
	 * Size of a pixel using type GL_UNSIGNED_BYTE
	 */
	static const GLshort SIZEOF_UNSIGNED_BYTE = 4;

	/**
	 * Texture color type for TYPE_UNSIGNED_SHORT_5_6_5
	 */
	static const GLenum TYPE_UNSIGNED_SHORT_5_6_5 = GL_UNSIGNED_SHORT_5_6_5;

	/**
	 * Size of a pixel using type GL_UNSIGNED_SHORT_5_6_5
	 */
	static const GLshort SIZEOF_UNSIGNED_SHORT_5_6_5 = 2;

	/**
	 * Texture color type for TYPE_UNSIGNED_SHORT_4_4_4_4
	 */
	static const GLenum TYPE_UNSIGNED_SHORT_4_4_4_4 = GL_UNSIGNED_SHORT_4_4_4_4;

	/**
	 * Size of a pixel using type GL_UNSIGNED_SHORT_4_4_4_4
	 */
	static const GLshort SIZEOF_UNSIGNED_SHORT_4_4_4_4 = 2;

	/**
	 * Texture color type for TYPE_UNSIGNED_SHORT_5_5_5_1
	 */
	static const GLenum TYPE_UNSIGNED_SHORT_5_5_5_1 = GL_UNSIGNED_SHORT_5_5_5_1;

	/**
	 * Size of a pixel using type GL_UNSIGNED_SHORT_5_5_5_1
	 */
	static const GLshort SIZEOF_UNSIGNED_SHORT_5_5_5_1 = 2;


	/**
	 * Indicate the wrap mode for the "s" axe
	 */
	static const GLenum WRAP_MODE_S = GL_TEXTURE_WRAP_S;

	/**
	 * Indicate the wrap mode for the "t" axe
	 */
	static const GLenum WRAP_MODE_T = GL_TEXTURE_WRAP_T;

	/**
	 * Wrapping mode : repeat the texture
	 */
	static const GLint WRAP_REPEAT = GL_REPEAT;

	/**
	 * Wrapping mode : clamp fetches to the edge of the texture
	 */
	static const GLint WRAP_CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE;

	/**
	 * Wrapping mode : repeat using mirrored image
	 */
	static const GLint WRAP_MIRRORED_REPEAT = GL_MIRRORED_REPEAT;


	/**
	 * Magnification settings for lowest quality
	 */
	static const GLint MAG_FILTER_LOW = GL_NEAREST;

	/**
	 * Magnification settings for highest quality
	 */
	static const GLint MAG_FILTER_HIGH = GL_LINEAR;


	/**
	 * Minification settings for lowest quality without mipmaping
	 */
	static const GLint MIN_FILTER_LOW = GL_NEAREST;

	/**
	 * Minification settings for highest quality without mipmaping
	 */
	static const GLint MIN_FILTER_HIGH = GL_LINEAR;

	/**
	 * Minification settings for lowest quality with mipmaping
	 */
	static const GLint MIN_FILTER_MIPMAP_LOW = GL_NEAREST_MIPMAP_NEAREST;

	/**
	 * Minification settings for medium quality with mipmaping
	 */
	static const GLint MIN_FILTER_MIPMAP_MEDIUM = GL_NEAREST_MIPMAP_LINEAR;

	/**
	 * Minification settings for high quality with mipmaping
	 */
	static const GLint MIN_FILTER_MIPMAP_BILINEAR = GL_LINEAR_MIPMAP_NEAREST;

	/**
	 * Minification settings for highest quality with mipmaping
	 */
	static const GLint MIN_FILTER_MIPMAP_TRILINEAR = GL_LINEAR_MIPMAP_LINEAR;

	/**
	 * GL handle
	 */
	GLuint handle;

public:
	GlTexture(){
		this->handle = UNBIND_HANDLE;
	}
	virtual ~GlTexture() = 0;

	/**
	 * Bind the current texture to default GPU active texture GL_TEXTURE0
	 */
	void bind(){
		this->bind(GL_TEXTURE0);
	}

	/**
	 * Bind the current texture to GPU active texture activeTexture
	 *
	 */
	void bind(GLuint activeTexture){
		glActiveTexture(activeTexture);
		glBindTexture(GL_TEXTURE_2D, this->handle);
	}

	/**
	 * Unbind the current texture to default GPU active texture GL_TEXTURE0
	 */
	void unbind(){
		this->unbind(GL_TEXTURE0);
	}

	/**
	 * Unbind the current texture
	 *
	 */
	void unbind(GLuint activeTexture){
		glActiveTexture(activeTexture);
		glBindTexture(GL_TEXTURE_2D, UNBIND_HANDLE);
	}

	/**
	 * Get the bytes for this Texture
	 *
	 */
	virtual byte* getBytes() const = 0;

	/**
	 * Get the id linked to this provider for indexing
	 */
	virtual GLuint getId() const = 0;

	/**
	 * Get the source image height
	 */
	virtual GLsizei getHeight() const = 0;

	/**
	 * Get the source image width
	 */
	virtual GLsizei getWidth() const = 0;

	/**
	 * Get the GL target of texture
	 *
	 * Default implementation use TARGET_TEXTURE_2D
	 */
	GLenum getTarget() const{
		return TARGET_TEXTURE_2D;
	}

	/**
	 * Get the texture color format
	 *
	 * @return FORMAT_ALPHA, FORMAT_RGB, FORMAT_RGBA, FORMAT_LUMINANCE or FORMAT_LUMINANCE_ALPHA
	 */
	GLint getFormat() const{
		return FORMAT_RGBA;
	}

	/**
	 * Get the texture color type
	 *
	 * @return TYPE_UNSIGNED_BYTE, TYPE_UNSIGNED_SHORT_5_6_5, TYPE_UNSIGNED_SHORT_4_4_4_4 or TYPE_UNSIGNED_SHORT_5_5_5_1
	 */
	GLenum getType() const{
		return TYPE_UNSIGNED_BYTE;
	}

	/**
	 * Indicates if the current texture uses compression and
	 * which compression format
	 *
	 * @return The compression format, COMP_FALSE for no compression
	 */
	GLenum getCompressionFormat() const{
		return COMP_FALSE;
	}

	/**
	 * Get the wrapping mode of texture for the given axe
	 *
	 * Default implementation returns WRAP_REPEAT
	 *
	 * @return WRAP_REPEAT, WRAP_CLAMP_TO_EDGE or WRAP_MIRRORED_REPEAT
	 */
	GLint getWrapMode(int axeId) const{
		return WRAP_REPEAT;
	}

	/**
	 * Get the magnification filter setting
	 *
	 * Subclass should override this method to set filter
	 *
	 * Default implementation returns MAG_FILTER_LOW
	 *
	 * @return MAG_FILTER_LOW or MAG_FILTER_HIGH
	 */
	GLint getMagnificationFilter() const{
		return MAG_FILTER_LOW;
	}

	/**
	 * Get the magnification filter setting
	 *
	 * Subclass should override this method to set filter
	 *
	 * Default implementation returns MIN_FILTER_MIPMAP_MEDIUM
	 *
	 * @return MIN_FILTER_LOW, MIN_FILTER_HIGH, MIN_FILTER_MIPMAP_LOW,
	 * 		   MIN_FILTER_MIPMAP_MEDIUM, MIN_FILTER_MIPMAP_BILINEAR
	 * 		   or MIN_FILTER_MIPMAP_TRILINEAR
	 */
	GLint getMinificationFilter() const{
		return MIN_FILTER_MIPMAP_MEDIUM;
	}

	/**
	 * Get the current texture buffer size
	 *
	 * @return The texture buffer size in bytes
	 */
	virtual size_t getSize() const = 0;

	/**
	 * Implements this method to add specific parameter before uploading to GPU
	 */
	virtual void onUpload() = 0;

	/**
	 * Removes texture from GPU
	 */
	 void free(){
		glDeleteTextures(1,&this->handle);
	}
};

#endif /* GLTEXTURE_H_ */
