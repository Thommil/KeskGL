/*
 * GlAsset.h
 *
 *  Created on: Apr 14, 2013
 *      Author: Thomas MILLET
 */
#include <hash_map>
#include <GLES2/gl2.h>

#include "loader/API.h"
#include "texture/GlTextureSet.h"

using namespace std;

#ifndef GLASSET_H_
#define GLASSET_H_

/**
 * Class representation of assets :
 *  - list all objects and instances
 *  - one instance of GlAsset is held by GlContext
 *  - helpers to free resources
 *  - output inner format of BinaryLoader
 */
class GlAsset {
public:
	/**
	 * Geometry abstraction class
	 */
	class Geometry{
		public:
			/**
			 * Abstraction class for an element of a Geometry
			 */
			class Element{
			public:
				/**
				 * Index of SEMANTIC in input settings
				 */
				const static GLenum SEMANTIC = 0;

				/**
				 * Index of SET in input settings
				 */
				const static GLenum SET = 1;

				/**
				 * Index of SIZE in input settings
				 */
				const static GLenum SIZE = 2;

				/**
				 * Index of OFFSET in input settings
				 */
				const static GLenum OFFSET = 3;

				/**
				 * Types of elements
				 */

				/**
				 * Type for LINES geometry
				 */
				const static GLenum LINES = API::LINES;

				/**
				 * Type for LINESTRIPS geometry
				 */
				const static GLenum LINESTRIPS = API::LINESTRIPS;

				/**
				 * Type for POLYGONS geometry
				 */
				const static GLenum POLYGONS = API::POLYGONS;

				/**
				 * Type for POLYLIST geometry
				 */
				const static GLenum POLYLIST = API::POLYLIST;

				/**
				 * Type for TRIANGLES geometry
				 */
				const static GLenum TRIANGLES = API::TRIANGLES;

				/**
				 * Type for TRIFANS geometry
				 */
				const static GLenum TRIFANS = API::TRIFANS;

				/**
				 * Type for TRISTRIPS geometry
				 */
				const static GLenum TRISTRIPS = API::TRISTRIPS;

				/**
				 * Types of input semantic
				 */

				/**
				 * Geometric binormal (bitangent) vector
				 */
				const static GLenum BINORMAL = API::BINORMAL;

				/**
				 * Color coordinate vector
				 */
				const static GLenum COLOR = API::COLOR;

				/**
				 * Continuity constraGLuint at the control vertex (CV)
				 */
				const static GLenum CONTINUITY = API::CONTINUITY;

				/**
				 * Raster or MIP-level input
				 */
				const static GLenum IMAGE = API::IMAGE;

				/**
				 * Sampler input
				 */
				const static GLenum INPUT = API::INPUT;

				/**
				 * Tangent vector for preceding control poGLuint
				 */
				const static GLenum IN_TANGENT = API::IN_TANGENT;

				/**
				 * Sampler GLuinterpolation type
				 */
				const static GLenum INTERPOLATION = API::INTERPOLATION;

				/**
				 * Inverse of local-to-world matrix
				 */
				const static GLenum INV_BIND_MATRIX = API::INV_BIND_MATRIX;

				/**
				 * Skin influence identifier
				 */
				const static GLenum JOINT = API::JOINT;

				/**
				 * Number of piece-wise linear approximation steps to use
				 * for the spline segment that follows this CV
				 */
				const static GLenum LINEAR_STEPS = API::LINEAR_STEPS;

				/**
				 * Morph targets for mesh morphing
				 */
				const static GLenum MORPH_TARGET = API::MORPH_TARGET;

				/**
				 * Weights for mesh morphing
				 */
				const static GLenum MORPH_WEIGHT = API::MORPH_WEIGHT;

				/**
				 * Normal vector
				 */
				const static GLenum NORMAL = API::NORMAL;

				/**
				 * Sampler output
				 */
				const static GLenum OUTPUT = API::OUTPUT;

				/**
				 * Tangent vector for succeeding control poGLuint
				 */
				const static GLenum OUT_TANGENT = API::OUT_TANGENT;

				/**
				 * Geometric coordinate vector
				 */
				const static GLenum POSITION = API::POSITION;

				/**
				 * Geometric tangent vector
				 */
				const static GLenum TANGENT = API::TANGENT;

				/**
				 * Texture binormal (bitangent) vector
				 */
				const static GLenum TEXBINORMAL = API::TEXBINORMAL;

				/**
				 * Texture coordinate vector
				 */
				const static GLenum TEXCOORD = API::TEXCOORD;

				/**
				 * Texture tangent vector
				 */
				const static GLenum TEXTANGENT = API::TEXTANGENT;

				/**
				 * Generic parameter vector
				 */
				const static GLenum UV = API::UV;

				/**
				 * Mesh vertex
				 */
				const static GLenum VERTEX = API::VERTEX;

				/**
				 * Skin influence weighting value
				 */
				const static GLenum WEIGHT = API::WEIGHT;

				/**
				 * The type of element (LINES, TRIANGLES ...)
				 */
				GLuint type;

				/**
				 * The number of vertices in element
				 */
				GLuint count;

				/**
				 * The bound material instance id
				 */
				GLuint materialId;

				/**
				 * The list of inputs based on Loader API format
				 */
				GLuint **inputs;

				/**
				 * The size of data in bytes
				 */
				size_t size;

				/**
				 * The stride of data in bytes
				 */
				GLuint stride;

				/**
				 * The values bound to inputs
				 */
				GLfloat **values;

				/**
				 * The VBO handle if available
				 */
				GLuint handle;

				public:
					Element();
					virtual ~Element();
			};

			/**
			 * The geometry ID
			 */
			GLuint id;

			/**
			 * The total of elements
			 */
			GLuint elementsCount;

			/**
			 * The list of elements
			 */
			Element *elements;

		public:
			Geometry();
			virtual ~Geometry();
	};

	/**
	 * Light abstraction class
	 */
	class Light{
	public:
		/**
		 * Index of KC in settings
		 */
		const static GLenum KC = 0;

		/**
		 * Index of KL in settings
		 */
		const static GLenum KL = 1;

		/**
		 * Index of KQ in settings
		 */
		const static GLenum KQ = 2;

		/**
		 * Index of FALLOFF_ANGLE in settings (spot only)
		 */
		const static GLenum FALLOFF_ANGLE = 3;

		/**
		 * Index of FALLOFF_EXP in settings (spot only)
		 */
		const static GLenum FALLOFF_EXP = 4;

		/**
		 * Ambient light source
		 */
		const static GLenum AMBIENT = API::AMBIENT;

		/**
		 * Directional light source
		 */
		const static GLenum DIRECTIONAL = API::DIRECTIONAL;

		/**
		 * POINT light source
		 */
		const static GLenum POINT = API::POINT;

		/**
		 * Spot light source
		 */
		const static GLenum SPOT = API::SPOT;

		/**
		 * The light ID
		 */
		 GLuint id;

		/**
		 * The light type
		 */
		GLuint type;

		/**
		 * The light color (vec4)
		 */
		GLfloat color[4];

		/**
		 * The light settings see {@link API}
		 */
		GLfloat settings[5];

	public:
		Light();
		virtual ~Light();
	};

	/**
	 * Camera abstraction class
	 */
	class Camera{
	public:
		/**
		 * Index of XFOV in settings (perspective)
		 */
		const static GLenum XFOV = 0;

		/**
		 * Index of XMAG in settings (orthographic)
		 */
		const static GLenum XMAG = 0;

		/**
		 * Index of YFOV in settings (perspective)
		 */
		const static GLenum YFOV = 1;

		/**
		 * Index of YMAG in settings (orthographic)
		 */
		const static GLenum YMAG = 1;

		/**
		 * Index of ASPECT_RATIO in settings
		 */
		const static GLenum ASPECT_RATIO = 2;

		/**
		 * Index of ASPECT_RATIO in settings
		 */
		const static GLenum ZNEAR = 3;

		/**
		 * Index of ZFAR in settings
		 */
		const static GLenum ZFAR = 4;

		/**
		 * Perspective camera
		 */
		const static GLenum PERSPECTIVE = API::PERSPECTIVE;

		/**
		 * Orthographic camera
		 */
		const static GLenum ORTHOGRAPHIC = API::ORTHOGRAPHIC;

		/**
		 * The light ID
		 */
		GLuint id;

		/**
		 * The light type
		 */
		GLuint type;

		/**
		 * The camera settings, see {@link API}
		 */
		GLfloat settings[5];

		/**
		 * The camera up vectors (vec3)
		 */
		GLfloat up[3];

	public:
		Camera();
		virtual ~Camera();
	};

	/**
	 * Material abstraction class
	 */
	class Material{
	public:
		/**
		 * Index in bindings to indicate HANDLE
		 */
		const static GLenum HANDLE = 0;

		/**
		 * Index in bindings to indicate SEMANTIC
		 */
		const static GLenum SEMANTIC = 1;

		/**
		 * Constant material (ambient)
		 */
		const static GLenum CONSTANT = API::CONSTANT;

		/**
		 * Diffuse material and parameter
		 */
		const static GLenum DIFFUSE = API::DIFFUSE;

		/**
		 * Specular material and parameter
		 */
		const static GLenum SPECULAR = API::SPECULAR;

		/**
		*	Indicates a not set value
		**/
		const static GLenum NOT_SET = API::UNSPECIFIED;

		/**
		 * The emission color in Vec4
		 */
		GLfloat* emissionColor;

		/**
		 * The emission bind handle
		 */
		GLuint* emissionBinding;

		/**
		 * The reflective color in Vec4
		 */
		GLfloat* reflectiveColor;

		/**
		 * The reflective bind handle
		 */
		GLuint* reflectiveBinding;

		/**
		 * The reflectivity value
		 */
		GLfloat reflectivity;

		/**
		 * The transparent color in Vec4
		 */
		GLfloat* transparentColor;

		/**
		 * The transparent bind handle
		 */
		GLuint* transparentBinding;

		/**
		 * The transparency value
		 */
		GLfloat transparency;

		/**
		 * The refraction value
		 */
		GLfloat* refraction;

		/**
		 * The ambient color in Vec4
		 */
		GLfloat* ambientColor;

		/**
		 * The ambient bind handle
		 */
		GLuint* ambientBinding;

		/**
		 * The diffuse color in Vec4
		 */
		GLfloat* diffuseColor;

		/**
		 * The diffuse bind handle
		 */
		GLuint* diffuseBinding;

		/**
		 * The specular color in Vec4
		 */
		GLfloat* specularColor;

		/**
		 * The specular bind handle
		 */
		GLuint* specularBinding;

		/**
		 * The shininess value
		 */
		GLfloat shininess;

		/**
		 * The material ID
		 */
		GLuint id;

		/**
		 * The material type
		 */
		GLuint type;

	public:
		Material();
		virtual ~Material();

	};

	/**
	 * Node abstraction class
	 */
	class Node{
	public:
		/**
		 * Geometry instance abstraction class
		 */
		class GeometryInstance{
		public:
			/**
			 * Material instance abstraction class
			 */
			class MaterialInstance{
			public:
				/**
				 * Index of INPUT_SEMANTIC in bindings
				 */
				const static GLenum INPUT_SEMANTIC = 0;

				/**
				 * Index of INPUT_SET in bindings
				 */
				const static GLenum INPUT_SET = 1;

				/**
				 * Index of MATERIAL_SEMANTIC in bindings
				 */
				const static GLenum MATERIAL_SEMANTIC = 2;

				/**
				 * The material instance ID/Symbol
				 */
				GLuint id;

				/**
				 * The bound material
				 */
				GlAsset::Material *target;

				/**
				 * The list of material instance inputs bindings
				 */
				GLuint** bindings;

				/**
				 * Total of bindings
				 */
				GLuint bindingsCount;

			public:
				MaterialInstance();
				virtual ~MaterialInstance();
			};
		public:
			/**
			 * The geometry
			 */
			GlAsset::Geometry *geometry;

			/**
			 * The material instances
			 */
			MaterialInstance *materials;

			/**
			 * Amount of material instance
			 */
			GLuint materialsCount;

		public:
			GeometryInstance();
			virtual ~GeometryInstance();

		};
	public:

		/**
		 * NODE node
		 */
		const static GLenum NODE = API::NODE_TYPE;

		/**
		 * JOGLuint node
		 */
		const static GLenum JOGLuint = API::JOINT_TYPE;

		/**
		 * The node ID
		 */
		GLuint id;

		/**
		 * The node type
		 */
		GLuint type;

		/**
		 * The associated model matrix
		 */
		GLfloat model[4][4];

		/**
		 * The list of cameras instances
		 */
		GlAsset::Camera *cameraInstances;

		/**
		 * Total of cameras
		 */
		GLuint cameraInstancesCount;

		/**
		 * The list of lights instances
		 */
		GlAsset::Light *lightInstances;

		/**
		 * Total of lights
		 */
		GLuint lightInstancesCount;

		/**
		 * The list of nodes instances
		 */
		GlAsset::Node *nodeInstances;

		/**
		 * Total of nodes
		 */
		GLuint nodeInstancesCount;

		/**
		 * The list of geometries
		 */
		GlAsset::Node::GeometryInstance *geometryInstances;

		/**
		 * Total of geometries
		 */
		GLuint geometryInstancesCount;

	public:
		Node();
		virtual ~Node();

	};

	/**
	 * Scene abstraction class
	 */
	class Scene{
	public:
		/**
		 * The scene ID
		 */
		GLuint id;

		/**
		 * The total of nodes
		 */
		GLuint nodeInstancesCount;

		/**
		 * The list of nodes instances ID in the scene
		 */
		GlAsset::Node* nodeInstances;

	public:
		Scene();
		virtual ~Scene();

	};

public:
	/**
	 * Contains the list of geometries by their IDs
	 */
	hash_map<GLuint, GlAsset::Geometry*> geometries;

	/**
	 * Contains the list of lights by their IDs
	 */
	hash_map<GLuint, GlAsset::Light*> lights;

	/**
	 * Contains the list of cameras by their IDs
	 */
	hash_map<GLuint, GlAsset::Camera*> cameras;

	/**
	 * Contains the list of textures by their IDs
	 */
	GlTextureSet textures;

	/**
	 * Contains the list of materials by their IDs
	 */
	hash_map<GLuint, GlAsset::Material*> materials;

	/**
	 * Contains the list of nodes by their IDs
	 */
	hash_map<GLuint, GlAsset::Node*> nodes;

	/**
	 * Contains the list of scenes by their IDs
	 */
	hash_map<GLuint, GlAsset::Scene*> scenes;

public:
	GlAsset();
	virtual ~GlAsset();

	void free();
	void freeGeometries();
	void freeGeometry(GLuint id);
	void freeLights();
	void freeLight(GLuint id);
	void freeCameras();
	void freeCamera(GLuint id);
	void freeTextures();
	void freeTexture(GLuint id);
	void freeMaterials();
	void freeMaterial(GLuint id);
	void freeNodes();
	void freeNode(GLuint id);
	void freeScenes();
	void freeScene(GLuint id);


};

#endif /* GLASSET_H_ */
