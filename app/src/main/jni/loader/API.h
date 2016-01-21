/*
 * API.h
 *
 *  Created on: Apr 21, 2013
 *      Author: Thomas MILLET
 */
#include <GLES2/gl2.h>

using namespace std;

#ifndef API_H_
#define API_H_

class API {
public:
	/**
	 * Version
	 */
	const static float VERSION = 1.0f;

	/************************************************************************************************
	 * COMMON
	 ************************************************************************************************

	/**
	 * Indicates a null or empty entry
	 */
	const static GLenum EMPTY = 0x00000000;

	/**
	 * Indicates a false value
	 */
	const static GLenum FALSE = 0x00000000;

	/**
	 * Default value for unspecified entry
	 */
	const static GLenum UNSPECIFIED = 0xffffffff;

	/**
	 * Default value for error entry
	 */
	const static GLenum ERROR = 0xffffffff;

	/**
	 *  VAR_TYPE
	 */

	/**
	 * Indicates a type FLOAT
	 */
	const static GLenum FLOAT = 0x0000000a;

	/**
	 * Indicates a type SHORT
	 */
	const static GLenum SHORT = 0x0000000b;

	/**
	 * Indicates a type BOOLEAN
	 */
	const static GLenum BOOLEAN = 0x0000000c;

	/**
	 * Indicates a type INTEGER
	 */
	const static GLenum INTEGER = 0x0000000d;

	/**
	 * Indicates a type STRING
	 */
	const static GLenum STRING = 0x0000000e;

	/**
	 * Parameter of type float[2]
	 */
	const static GLenum FLOAT2 = 0x0000000f;

	/**
	 * Parameter of type float[3]
	 */
	const static GLenum FLOAT3 = 0x00000010;

	/**
	 * Parameter of type float[4]
	 */
	const static GLenum FLOAT4 = 0x00000011;

	/**
	 * Parameter of type surface
	 */
	const static GLenum SURFACE = 0x00000012;

	/**
	 * Parameter of type sampler
	 */
	const static GLenum SAMPLER = 0x00000013;

	/**
	 *  ATTRIBUTES
	 */

	/**
	 * ID attribute
	 */
	const static GLenum ID = 0x0000001a;

	/**
	 * TYPE attribute
	 */
	const static GLenum TYPE = 0x0000001b;


	/************************************************************************************************
	 * HEADER - Header
	 ************************************************************************************************

	  [HEADER][$VERSION]
	  [$GEOMETRY_COUNT][$LIGHT_COUNT][$CAMERA_COUNT][$IMAGERY_COUNT][$MATERIAL_COUNT][$SCENE_COUNT]

	 ************************************************************************************************/
	const static GLenum HEADER = 0x00000032;

	/************************************************************************************************
	 * GEOMETRY	- Describes the visual shapes and appearances of an object in a scene.
	 ************************************************************************************************

	  [GEOMETRY][$ID][$PRIMITIVES_COUNT]
		  [$PRIMITIVE_TYPE][$INPUT_COUNT][$VERTEX_COUNT]
		  [$SEMANTIC][$SET][$SIZE][$OFFSET]...[$SEMANTIC][$SET][$SIZE][$OFFSET]
		  [$VALUES]
		  ...
		  [$PRIMITIVE_TYPE][$INPUT_COUNT][$VERTEX_COUNT]
		  [$SEMANTIC][$SET][$SIZE][$OFFSET]...[$SEMANTIC][$SET][$SIZE][$OFFSET]
		  [$VALUES]
	  ...
	  [GEOMETRY][$ID][$PRIMITIVES_COUNT]

	 ************************************************************************************************/
	const static GLenum GEOMETRY = 0x00000064;

	/************************************************************************************************
	 * LIGHT	- Describes the light sources in the scene
	 ************************************************************************************************

	  [LIGHT][$ID][$TYPE]
		  case AMBIENT : [$COLOR_RGB]
		  case DIRECTIONAL : [$COLOR_RGB]
		  case POINT : [$COLOR_RGB][$KC][$KL][$KQ]
		  case SPOT : [$COLOR_RGB][$KC][$KL][$KQ][$FALLOFF_ANGLE][$FALLOFF_EXP]
	  ...
	  [LIGHT][$ID][$TYPE]

	 ************************************************************************************************/
	const static GLenum LIGHT = 0x000000C8;

	/************************************************************************************************
	 * CAMERA	- Describes the optics in the scene
	 ************************************************************************************************

	  [CAMERA][$ID][$CAMERA_TYPE]
		  case PERSPECTIVE : [$XFOV][$YFOV][$ASPECT_RATIO][$ZNEAR][$ZFAR][$UP_X][$UP_Y][$UP_Z]
		  case ORTHOGRAPHIC : [$XMAG][$YMAG][$ASPECT_RATIO][$ZNEAR][$ZFAR][$UP_X][$UP_Y][$UP_Z]
	  ...
	  [CAMERA][$ID]

	 ************************************************************************************************/
	const static GLenum CAMERA = 0x0000012C;

	/************************************************************************************************
	 * IMAGERY	- Describes the images in the scene
	 ************************************************************************************************

	  [IMAGE][$ID][$FORMAT][$INTERNAL_FORMAT][$TYPE][$WIDTH][$HEIGHT][$COMPRESS][$MAG][$MIN][$PATH]
	  ...
	  [IMAGE][$ID][$FORMAT][$INTERNAL_FORMAT][$TYPE][$WIDTH][$HEIGHT][$COMPRESS][$MAG][$MIN][$PATH]

	 ************************************************************************************************/
	const static GLenum IMAGERY = 0x00000190;


	/************************************************************************************************
	 * MATERIAL	- Describes the materials in the scene
	 ************************************************************************************************

	  [MATERIAL][$ID][$MATERIAL_TYPE]
		  [$EMISSION_VAR_TYPE][$EMISSION_VALUE]
		  [$REFLECTIVE_VAR_TYPE][$REFLECTIVE_VALUE]
		  [$REFLECTIVITY_VAR_TYPE][$REFLECTIVITY_VALUE]
		  [$TRANSPARENT_VAR_TYPE][$TRANSPARENT_VALUE]
		  [$TRANSPARENCY_VAR_TYPE][$TRANSPARENCY_VALUE]
		  [$REFRACTION_VAR_TYPE][$REFRACTION_VALUE]
		  [$AMBIENT_VAR_TYPE][$AMBIENT_VALUE]
		  [$DIFFUSE_VAR_TYPE][$DIFFUSE_VALUE]
		  [$SPECULAR_VAR_TYPE][$SPECULAR_VALUE]
		  [$SHININESS_VAR_TYPE][$SHININESS_VALUE]
	  ...
	  [MATERIAL][$ID][$MATERIAL_TYPE]

	  $*_VAR_TYPE :
		case FLOAT : [$FLOAT_VALUE]
		case FLOAT3 : [$FLOAT_VALUE_0][$FLOAT_VALUE_1][$FLOAT_VALUE_2]
		case FLOAT4 : [$FLOAT_VALUE_0][$FLOAT_VALUE_1][$FLOAT_VALUE_2][$FLOAT_VALUE_3]
		case SAMPLER : [$IMAGERY_ID][$SEMANTIC]
		case UNSPECIFIED : -

	 ************************************************************************************************/
	const static GLenum MATERIAL = 0x000001f4;

	/************************************************************************************************
	 * NODE - Describes the nodes element in the scene
	 ************************************************************************************************

	  [NODE][$ID][$NODE_TYPE][$ELEMENT_COUNT]
			if LOOKAT : [LOOKAT][$EYE_X][$EYE_Y][$EYE_Z][$INTEREST_X][$INTEREST_Y][$INTEREST_Z][$UP_X][$UP_Y][$UP_Z]
			if MATRIX : [MATRIX][$0_0]...[$3_3]
			if ROTATE : [ROTATE][$ROTATE_X][$ROTATE_Y][$ROTATE_Z][$ROTATE_DEGRE]
			if SCALE : [SCALE][$SCALE_X][$SCALE_Y][$SCALE_Z]
			if SKEW : [SKEW][$ROTATE_DEGRE][$ROTATE_X][$ROTATE_Y][$ROTATE_Z][$TRANSLATE_X][$TRANSLATE_Y][$TRANSLATE_Z]
			if TRANSLATE : [TRANSLATE][$TRANSLATE_X][$TRANSLATE_Y][$TRANSLATE_Z]
			if CAMERA : [CAMERA][$CAMERA_ID]
			if GEOMETRY : [GEOMETRY][$GEOMETRY_ID][$MATERIAL_COUNT]
				[$MATERIAL_ID][$MATERIAL_TARGET_ID][$BIND_COUNT]
					[$INPUT_SEMANTIC][$INPUT_SET][$MATERIAL_SEMANTIC]
					...
					[$INPUT_SEMANTIC][$INPUT_SET][$MATERIAL_SEMANTIC]
				...
				[$MATERIAL_ID][$MATERIAL_TARGET_ID][$BIND_COUNT]
			if LIGHT : [LIGHT][$LIGHT_ID]
			if NODE : [NODE][$NODE_ID]
	  ...
	  [NODE][$ID][$NODE_TYPE][$ELEMENT_COUNT]
	 ************************************************************************************************/
	const static GLenum NODE = 0x00000258;

	/************************************************************************************************
	 * SCENE - Describes the scene elements
	 ************************************************************************************************

	  [SCENE][$ID][$NODE_COUNT]
	  [NODE_ID]
	  ...
	  [SCENE][$ID][$NODE_COUNT]
	 ************************************************************************************************/
	const static GLenum SCENE = 0x000002bc;

	/**
	 *  PRIMITIVE_TYPE
	 */

	/**
	 * Provides the information needed for a mesh to bind vertex attributes
	 * together and then organize those vertices GLenumo individual lines.
	 */
	const static GLenum LINES = 0x00000001;

	/**
	 * Provides the information needed to bind vertex attributes together
	 * and then organize those vertices GLenumo connected line-strips.
	 */
	const static GLenum LINESTRIPS = 0x00000003;

	/**
	 * Provides the information needed for a mesh to bind vertex attributes
	 * together and then organize those vertices GLenumo individual polygons.
	 */
	const static GLenum POLYGONS = 0x00000002;

	/**
	 * Provides the information needed for a mesh to bind vertex attributes
	 * together and then organize those vertices GLenumo individual polygons.
	 */
	const static GLenum POLYLIST = 0x00000004;

	/**
	 * Provides the information needed to for a mesh to bind vertex attributes
	 * together and then organize those vertices GLenumo individual triangles.
	 */
	const static GLenum TRIANGLES = 0x00000004;

	/**
	 * Provides the information needed for a mesh to bind vertex attributes
	 * together and then organize those vertices GLenumo connected triangles.
	 */
	const static GLenum TRIFANS = 0x00000006;

	/**
	 * Provides the information needed for a mesh to bind vertex attributes
	 * together and then organize those vertices GLenumo connected triangles.
	 */
	const static GLenum TRISTRIPS = 0x00000005;

	/**
	 *  INPUT SEMANTIC
	 */

	/**
	 * Geometric binormal (bitangent) vector
	 */
	const static GLenum BINORMAL = 0x000004B0;

	/**
	 * Color coordinate vector
	 */
	const static GLenum COLOR = 0x000004B1;

	/**
	 * Continuity constraGLenum at the control vertex (CV)
	 */
	const static GLenum CONTINUITY = 0x000004B2;

	/**
	 * Raster or MIP-level input
	 */
	const static GLenum IMAGE = 0x000004B3;

	/**
	 * Sampler input
	 */
	const static GLenum INPUT = 0x000004B4;

	/**
	 * Tangent vector for preceding control poGLenum
	 */
	const static GLenum IN_TANGENT = 0x000004B5;

	/**
	 * Sampler GLenumerpolation type
	 */
	const static GLenum INTERPOLATION = 0x000004B6;

	/**
	 * Inverse of local-to-world matrix
	 */
	const static GLenum INV_BIND_MATRIX = 0x000004B7;

	/**
	 * Skin influence identifier
	 */
	const static GLenum JOINT = 0x000004B8;

	/**
	 * Number of piece-wise linear approximation steps to use
	 * for the spline segment that follows this CV
	 */
	const static GLenum LINEAR_STEPS = 0x000004B9;

	/**
	 * Morph targets for mesh morphing
	 */
	const static GLenum MORPH_TARGET = 0x000004BA;

	/**
	 * Weights for mesh morphing
	 */
	const static GLenum MORPH_WEIGHT = 0x000004BB;

	/**
	 * Normal vector
	 */
	const static GLenum NORMAL = 0x000004BC;

	/**
	 * Sampler output
	 */
	const static GLenum OUTPUT = 0x000004BD;

	/**
	 * Tangent vector for succeeding control poGLenum
	 */
	const static GLenum OUT_TANGENT = 0x000004BE;

	/**
	 * Geometric coordinate vector
	 */
	const static GLenum POSITION = 0x000004BF;

	/**
	 * Geometric tangent vector
	 */
	const static GLenum TANGENT = 0x000004C0;

	/**
	 * Texture binormal (bitangent) vector
	 */
	const static GLenum TEXBINORMAL = 0x000004C1;

	/**
	 * Texture coordinate vector
	 */
	const static GLenum TEXCOORD = 0x000004C2;

	/**
	 * Texture tangent vector
	 */
	const static GLenum TEXTANGENT = 0x000004C3;

	/**
	 * Generic parameter vector
	 */
	const static GLenum UV = 0x000004C4;

	/**
	 * Mesh vertex
	 */
	const static GLenum VERTEX = 0x000004C5;

	/**
	 * Skin influence weighting value
	 */
	const static GLenum WEIGHT = 0x000004C6;

	/**
	 *  LIGHT_TYPE
	 */

	/**
	 * Ambient light source
	 */
	const static GLenum AMBIENT = 0x000007D0;

	/**
	 * Directional light source
	 */
	const static GLenum DIRECTIONAL = 0x000007D1;

	/**
	 * PoGLenum light source
	 */
	const static GLenum POINT = 0x000007D2;

	/**
	 * Spot light source
	 */
	const static GLenum SPOT = 0x000007D3;

	/**
	 *  MATERIAL_TYPE and parameters
	 */

	/**
	 * Constant material (ambient)
	 */
	const static GLenum CONSTANT = 0x00001388;

	/**
	 * Diffuse material and parameter
	 */
	const static GLenum DIFFUSE = 0x00001389;

	/**
	 * Specular material and parameter
	 */
	const static GLenum SPECULAR = 0x0000138a;


	/**
	 *  CAMERA_TYPE
	 */

	/**
	 * Perspective camera
	 */
	const static GLenum PERSPECTIVE = 0x00000BB8;

	/**
	 * Orthographic camera
	 */
	const static GLenum ORTHOGRAPHIC = 0x00000BB9;

	/**
	 *  IMAGE_FORMAT
	 */

	/**
	 * Bitmap image format
	 */
	const static GLenum BMP = 0x00000FA0;

	/**
	 * JPG image format
	 */
	const static GLenum JPG = 0x00000FA1;

	/**
	 * PNG image format
	 */
	const static GLenum PNG = 0x00000FA2;

	/**
	 * GIF image format
	 */
	const static GLenum GIF = 0x00000FA3;

	/**
	 *  IMAGE INTERNAL_FORMAT
	 */

	/**
	 * Image GLenumernal format based on GL_ALPHA
	 */
	const static GLenum ALPHA = 0x1906;

	/**
	 * Image GLenumernal format based on GL_LUMINANCE
	 */
	const static GLenum LUMINANCE = 0x1909;

	/**
	 * Image GLenumernal format based on GL_LUMINANCE_ALPHA
	 */
	const static GLenum LUMINANCE_ALPHA = 0x190a;

	/**
	 * Image GLenumernal format based on GL_RGB
	 */
	const static GLenum RGB = 0x1907;

	/**
	 * Image GLenumernal format based on GL_RGBA
	 */
	const static GLenum RGBA = 0x1908;

	/**
	 *  IMAGE TYPE
	 */

	/**
	 * Image GLenumernal format based on GL_UNSIGNED_BYTE
	 */
	const static GLenum UNSIGNED_BYTE = 0x1401;

	/**
	 * Image GLenumernal format based on GL_UNSIGNED_SHORT_5_6_5
	 */
	const static GLenum UNSIGNED_SHORT_5_6_5 = 0x8363;

	/**
	 * Image GLenumernal format based on GL_UNSIGNED_SHORT_4_4_4_4
	 */
	const static GLenum UNSIGNED_SHORT_4_4_4_4 = 0x8033;

	/**
	 * Image GLenumernal format based on GL_UNSIGNED_SHORT_5_5_5_1
	 */
	const static GLenum UNSIGNED_SHORT_5_5_5_1 = 0x8034;

	/**
	 * IMAGE MIPMAP
	 */

	/**
	 * NO MIMAP MIN/MAG NEAREST
	 */
	const static GLenum NEAREST = 0x2600;

	/**
	 * NO MIMAP MIN/MAG LINEAR
	 */
	const static GLenum LINEAR = 0x2601;

	/**
	 * MIMAP MIN NEAREST
	 */
	const static GLenum NEAREST_MIPMAP_NEAREST = 0x2700;

	/**
	 * MIMAP MIN NEAREST/LINEAR
	 */
	const static GLenum NEAREST_MIPMAP_LINEAR = 0x2702;

	/**
	 * MIMAP MIN LINEAR/NEAREST
	 */
	const static GLenum LINEAR_MIPMAP_NEAREST = 0x2701;

	/**
	 * MIMAP MIN LINEAR/LINEAR
	 */
	const static GLenum LINEAR_MIPMAP_LINEAR = 0x2703;

	/**
	 * NODE_TYPE
	 */

	/**
	 * NODE node
	 */
	const static GLenum NODE_TYPE = 0x1770;

	/**
	 * JOINT node
	 */
	const static GLenum JOINT_TYPE = 0x1771;

	/**
	 * TRANSFORMATIONS
	 */

	/**
	 * Lookat tranformation
	 */
	const static GLenum LOOKAT = 0x1772;

	/**
	 * Matrix tranformation
	 */
	const static GLenum MATRIX = 0x1773;

	/**
	 * Rotate tranformation
	 */
	const static GLenum ROTATE = 0x1774;

	/**
	 * Scale tranformation
	 */
	const static GLenum SCALE = 0x1775;

	/**
	 * Skew tranformation
	 */
	const static GLenum SKEW = 0x1776;

	/**
	 * Translate tranformation
	 */
	const static GLenum TRANSLATE = 0x1777;
/**
 * Interface to be shared between minifier and loader.
 */
private:
	virtual ~API() = 0;
};

#endif /* API_H_ */
