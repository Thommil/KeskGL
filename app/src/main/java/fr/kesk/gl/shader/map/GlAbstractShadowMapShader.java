package fr.kesk.gl.shader.map;

import java.io.IOException;
import java.io.InputStream;

import android.annotation.SuppressLint;
import android.opengl.GLES20;
import android.opengl.GLException;
import android.opengl.Matrix;
import fr.kesk.gl.GlAssets;
import fr.kesk.gl.GlContext;
import fr.kesk.gl.GlAssets.Camera;
import fr.kesk.gl.GlAssets.Geometry.Element;
import fr.kesk.gl.GlAssets.Light;
import fr.kesk.gl.GlAssets.Node;
import fr.kesk.gl.GlAssets.Node.GeometryInstance;
import fr.kesk.gl.buffer.GlBuffer;
import fr.kesk.gl.buffer.GlFrameBufferObject;
import fr.kesk.gl.buffer.GlRenderBufferObject;
import fr.kesk.gl.buffer.GlFrameBufferObject.Attachment;
import fr.kesk.gl.shader.GlProgram;
import fr.kesk.gl.shader.GlShader;
import fr.kesk.gl.texture.GlTextureSet;
import fr.kesk.gl.tools.BufferUtils;
import fr.kesk.gl.tools.GLES20Utils;
import fr.kesk.gl.tools.MatrixUtils;

/**
 * Shader used to generate ShadowMap texture for reuse, implementations
 * must at least provide gsgl streams from shaders. 
 * <br/>
 * <br/>
 * After rendering, results are stored in :<br/>
 * 	- shadowMap -> The depth texture which format is defined in shaders<br/>
 * 	- shadowMatrix -> The light BIAS_M_P matrix<br/>
 *
 * @author Thomas MILLET
 *
 */
public abstract class GlAbstractShadowMapShader implements GlShader {

	/**
	 * TAG log
	 */
	@SuppressWarnings("unused")
	private final static String TAG = GlAbstractShadowMapShader.class.getName();
	
	/**
	 * Texture border size matching quality
	 */
	protected int quality = QUALITY_MEDIUM;
	 
	/**
	 * Handle on light MVP matrix in shader
	 */
	protected int u_lightMvpMatrixMat4Handle;
	
	/**
	 * Handle on position in shader
	 */
	protected int a_PositionVec4Handle;
	
	/**
	 * The bound FBO
	 */
	protected GlFrameBufferObject fbo;
	
	/**
	 * The current shadow map
	 */
	protected GlShadowMapTexture currentShadowMap;
	
	/**
	 * The associated program for shadow generation
	 */
	protected GlProgram program;
	
	/**
	 * Stores the V, P and MVP matrixes based on light view
	 */
	public final float[] lightMvpCache = new float[64];
	
	/**
	 * Stores the light position and direction (4 last bytes used for storage)
	 */
	public final float[] lightModel = new float[12];
		
	/**
	 * Stores the original view port for restoration
	 */
	public float[] viewPort;
	
	/**
	 * Stores the original cull face mode
	 */
	public float[] cullfaceMode;
	
	/**
	 * The bias matrix for shadow matrix generation
	 */
	public static final float[]BIAS_MATRIX = new float[]{0.5f, 0.0f, 0.0f, 0.0f,
														0.0f, 0.5f, 0.0f, 0.0f,
														0.0f, 0.0f, 0.5f, 0.0f,
														0.5f, 0.5f, 0.5f, 1.0f};
	
	/**
	 * The associated list of sahdw textures
	 */
	protected GlTextureSet shadowTextures;
	
	/**
	 * Stores the light used used for shadow mapping
	 */
	public int lightType;
	
	/**
	 * Get a GSGL input stream from vertex shader
	 * 
	 * @return A GSGL stream on vertex shader source code
	 */
	protected abstract InputStream getVertexShaderInputStream();
	
	/**
	 * Get a GSGL input stream from fragment shader
	 * 
	 * @return A GSGL stream on fragment shader source code
	 */
	protected abstract InputStream getFragmentShaderInputStream();
	
	/* (non-Javadoc)
	 * @see fr.kesk.gl.shader.GlShader#compile()
	 */
	@Override
	public void compile(){
		//android.util.Log.d(TAG,"initialize()");
		//Build program if needed
		if(this.program == null){
			InputStream vertIn = null;
			InputStream fragIn = null;
			try{
				vertIn = this.getVertexShaderInputStream();
				fragIn = this.getFragmentShaderInputStream();
				this.program = new GlProgram(vertIn, fragIn);
				this.program.start();
				
				//Context
				this.u_lightMvpMatrixMat4Handle = this.program.getUniformHandle("u_mvpMatrixMat4");
				//Attributes
				this.a_PositionVec4Handle = this.program.getAttributeHandle("a_PositionVec4");
				
			}finally{
				if(vertIn != null){
					try{
						vertIn.close();
					}catch(IOException ioe){}
				}
				if(fragIn != null){
					try{
						fragIn.close();
					}catch(IOException ioe){}
				}
			}
		}
	
		if(this.shadowTextures == null){
			this.shadowTextures = new GlTextureSet();
		}
		
		if(this.currentShadowMap == null){
			this.newShadowMap();
		}
		
		//Create FBO or reinitialize
		if(this.fbo == null){
			this.fbo = new GlFrameBufferObject();
			
			//Build shadow map		
			this.fbo.attach(this.currentShadowMap, Attachment.TYPE_COLOR);
			this.fbo.attach(new GlRenderBufferObject(GLES20.GL_DEPTH_COMPONENT16, this.quality, this.quality), Attachment.TYPE_DEPTH);
			
			if(this.fbo.getStatus() != GlFrameBufferObject.STATUS_COMPLETE){
				throw new GLException(0, "Failed to initialize FBO (status "+this.fbo.getStatus()+")");
			}
		}
		
		//Setup a default camera
		Matrix.orthoM(lightMvpCache, 16, -10, 10, -10, 10, 0.1f, 100f);
		
		//Default Light
		this.lightType = Light.DIRECTIONAL;
		this.lightModel[0] = this.lightModel[1] = this.lightModel[2] = 10f;
		this.lightModel[3] = 1f;
		this.lightModel[4] = this.lightModel[5] = this.lightModel[7] = 0f;
		this.lightModel[6] = -1f;		
	}	
	
	/* (non-Javadoc)
	 * @see fr.kesk.gl.shader.GlShader#setQuality(int)
	 */
	@Override
	public void setQuality(final int quality) {
		//android.util.Log.d(TAG,"setQuality("+quality+")");
		this.quality = quality;
	}
	
	/**
	 * Configure the light source used for shadow mapping
	 * 
	 * @param lightNodeInstance The node containing the light instance
	 * @param lightIndex The index of the light in light instances of the node
	 */
	public void setLight(final Node lightNodeInstance, final int lightIndex){
		//android.util.Log.d(TAG,"setLight("+lightNodeInstance.id+","+lightIndex+")");
		this.lightType = lightNodeInstance.lightInstances[lightIndex].type;
		switch(this.lightType){
			case Light.DIRECTIONAL :
				this.lightModel[8] = this.lightModel[8] = this.lightModel[9] = this.lightModel[11] = 0f;
				this.lightModel[10] = -1f;
				MatrixUtils.multiplyMV(this.lightModel, 8, lightNodeInstance.model, 0, this.lightModel, 8);
				this.lightModel[0] = this.lightModel[1] = this.lightModel[2] = 0f;
				this.lightModel[3] = 1f;
				MatrixUtils.multiplyMV(this.lightModel, 0, lightNodeInstance.model, 0, this.lightModel, 0);
				final float length = Matrix.length(this.lightModel[0], this.lightModel[1], this.lightModel[2]);
				this.lightModel[8] *= length;
				this.lightModel[9] *= length;
				this.lightModel[10] *= length;
				break;
			case Light.POINT :
				this.lightModel[0] = this.lightModel[1] = this.lightModel[2] = 0f;
				this.lightModel[3] = 1f;
				MatrixUtils.multiplyMV(this.lightModel, 0, lightNodeInstance.model, 0, this.lightModel, 0);
				break;
			case Light.SPOT :
				this.lightModel[0] = this.lightModel[1] = this.lightModel[2] = 0f;
				this.lightModel[3] = 1f;
				MatrixUtils.multiplyMV(this.lightModel, 0, lightNodeInstance.model, 0, this.lightModel, 0);
				this.lightModel[8] = this.lightModel[8] = this.lightModel[9] = this.lightModel[11] = 0f;
				this.lightModel[10] = -1f;
				MatrixUtils.multiplyMV(this.lightModel, 8, lightNodeInstance.model, 0, this.lightModel, 8);
				this.lightModel[4] = this.lightModel[0]+this.lightModel[8];
				this.lightModel[5] = this.lightModel[1]+this.lightModel[9];
				this.lightModel[6] = this.lightModel[2]+this.lightModel[10];
				this.lightModel[7] = 1f;
				break;
		}
	}
	
	/**
	 * Fine settings used to set the light view and projection matrixes. This method
	 * is very important as it allows to improve greatly quality without CPU additional
	 * charge. Try to set zFar and zNear as best as you can. A good practice is to bind a
	 * camera instance to each node instance used as root node for shadow map generation.  
	 * 
	 * @param cameraNodeInstance The node owning the camera
	 * @param cameraIndex The indew of the camera in the node instance
	 */
	@SuppressLint("NewApi")
	public void setCamera(final Node cameraNodeInstance, final  int cameraIndex){
		//android.util.Log.d(TAG,"setCamera("+cameraNodeInstance.id+","+cameraIndex+")");
		if(cameraNodeInstance.cameraInstances[cameraIndex].type == Camera.ORTHOGRAPHIC){
			Matrix.orthoM(lightMvpCache, 16, -cameraNodeInstance.cameraInstances[cameraIndex].settings[Camera.XMAG], cameraNodeInstance.cameraInstances[cameraIndex].settings[Camera.XMAG], -cameraNodeInstance.cameraInstances[cameraIndex].settings[Camera.YMAG], cameraNodeInstance.cameraInstances[cameraIndex].settings[Camera.YMAG], cameraNodeInstance.cameraInstances[cameraIndex].settings[Camera.ZNEAR], cameraNodeInstance.cameraInstances[cameraIndex].settings[Camera.ZFAR]);
		}
		else{
			MatrixUtils.perspectiveM(this.lightMvpCache, 16, cameraNodeInstance.cameraInstances[cameraIndex].settings[Camera.YFOV], 1, cameraNodeInstance.cameraInstances[cameraIndex].settings[Camera.ZNEAR], cameraNodeInstance.cameraInstances[cameraIndex].settings[Camera.ZFAR]);
		}
	}

	/**
	 * Get the current generated shadow map in a GlShadowMapTexture instance
	 * 
	 * @return The shadow map in a GlShadowMapTexture instance
	 */
	public GlShadowMapTexture getShadowMap(){
		return this.currentShadowMap;
	}
	
	/**
	 * Get the full texture set with key == index
	 * 
	 * @return The texture set
	 */
	public GlTextureSet getTextureSet(){
		return this.shadowTextures;
	}
	
	/**
	 * Create a new shadow map and return it 
	 * (old ones are reachable using getTextureSet()).
	 * 
	 * @return The new created shadow map which becomes the current shadow map
	 * 			used in FBO
	 */
	public GlShadowMapTexture newShadowMap(){
		//android.util.Log.d(TAG,"newShadowMap()");
		this.currentShadowMap = new GlShadowMapTexture(this.shadowTextures.size, this.quality, this.fbo);
		this.shadowTextures.put(this.currentShadowMap);
		return this.currentShadowMap;
	}
	
	/* (non-Javadoc)
	 * @see fr.kesk.gl.shader.GlShader#render()
	 */
	@Override
	public void render() {
		//NOP
	}

	/* (non-Javadoc)
	 * @see fr.kesk.gl.shader.GlShader#render(fr.kesk.gl.GlAssets.Node)
	 */
	@Override
	public void render(Node nodeInstance) {
		//android.util.Log.d(TAG,"render("+nodeInstance.id+")");
		if(this.lightType == Light.DIRECTIONAL){
			this.lightModel[4] = this.lightModel[5] = this.lightModel[6] = 0f;
			this.lightModel[7] = 1f;
			MatrixUtils.multiplyMV(this.lightModel, 4, nodeInstance.model, 0, this.lightModel, 4);
			this.lightModel[0] = this.lightModel[4]-this.lightModel[8];
			this.lightModel[1] = this.lightModel[5]-this.lightModel[9];
			this.lightModel[2] = this.lightModel[6]-this.lightModel[10];
			this.lightModel[3] = 1f;
			Matrix.setLookAtM(this.lightMvpCache, 0, this.lightModel[0], this.lightModel[1], this.lightModel[2], this.lightModel[4], this.lightModel[5], this.lightModel[6], 0f, 1f, 0f);
			MatrixUtils.multiplyMM(this.lightMvpCache, 32, this.lightMvpCache, 16, this.lightMvpCache, 0);
			MatrixUtils.multiplyMM(this.currentShadowMap.shadowMatrix, 0, GlFastShadowMapShader.BIAS_MATRIX, 0, this.lightMvpCache, 32);
		}
		else if(this.lightType == Light.POINT){
			this.lightModel[4] = this.lightModel[5] = this.lightModel[6] = 0f;
			this.lightModel[7] = 1f;
			MatrixUtils.multiplyMV(this.lightModel, 4, nodeInstance.model, 0, this.lightModel, 4);
			Matrix.setLookAtM(this.lightMvpCache, 0, this.lightModel[0], this.lightModel[1], this.lightModel[2], this.lightModel[4], this.lightModel[5], this.lightModel[6], 0f, 1f, 0f);
			MatrixUtils.multiplyMM(this.lightMvpCache, 32, this.lightMvpCache, 16, this.lightMvpCache, 0);
			MatrixUtils.multiplyMM(this.currentShadowMap.shadowMatrix, 0, GlFastShadowMapShader.BIAS_MATRIX, 0, this.lightMvpCache, 32);
		}
		

		//Render the scene in the FBO
		this.fbo.bind();
		this.program.start();
		this.cullfaceMode = GlContext.glGetState(GLES20.GL_CULL_FACE_MODE);
		this.viewPort = GlContext.glGetState(GLES20.GL_VIEWPORT);
		GLES20.glCullFace(GLES20.GL_FRONT);
		GLES20.glViewport(0, 0, this.quality, this.quality);
		GLES20.glClear(GLES20.GL_DEPTH_BUFFER_BIT | GLES20.GL_COLOR_BUFFER_BIT);
		GLES20.glEnableVertexAttribArray(this.a_PositionVec4Handle);
		this.drawNode(nodeInstance);
		GLES20.glDisableVertexAttribArray(this.a_PositionVec4Handle);
		this.fbo.unbind();
		GLES20.glCullFace((int)this.cullfaceMode[0]);
		GLES20.glViewport((int)this.viewPort[0],(int)this.viewPort[1],(int)this.viewPort[2],(int)this.viewPort[3]);
	}
	
	/**
	 * Simple recursive method to draw a node and its subnodes
	 * 
	 * @param nodeInstance The node instance
	 */
	private void drawNode(Node nodeInstance){
		MatrixUtils.multiplyMM(this.lightMvpCache, 48, this.lightMvpCache, 32, nodeInstance.model, 0);
		GLES20.glUniformMatrix4fv(this.u_lightMvpMatrixMat4Handle, 1, false, this.lightMvpCache, 48);
		
		if(nodeInstance.geometryInstances != null){
			for(GeometryInstance geometryInstance : nodeInstance.geometryInstances){
				for(Element element : geometryInstance.geometry.elements){
					if(element.handle == GlBuffer.UNBIND_HANDLE){
						GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, GlBuffer.UNBIND_HANDLE);
						final GlBuffer<float[]> elementBuffer = BufferUtils.elementToGlBuffer(element);
						elementBuffer.position(elementBuffer.chunks[0]);
						GLES20.glVertexAttribPointer(this.a_PositionVec4Handle, elementBuffer.chunks[0].components, elementBuffer.datatype, false, elementBuffer.stride, elementBuffer.data);
						GLES20.glDrawArrays(element.type, 0, element.count);
						elementBuffer.free();
					}
					else{
						GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, element.handle);
						GLES20Utils.glVertexAttribPointer(this.a_PositionVec4Handle, element.inputs[0][GlAssets.Geometry.Element.SIZE], GLES20.GL_FLOAT, false, element.stride, element.inputs[0][GlAssets.Geometry.Element.OFFSET]);
						GLES20.glDrawArrays(element.type, 0, element.count);
						GLES20.glBindBuffer(GLES20.GL_ARRAY_BUFFER, GlBuffer.UNBIND_HANDLE);
					}
				}
			}
		}
		if(nodeInstance.nodeInstances != null){
			for(Node childNode : nodeInstance.nodeInstances){
				this.drawNode(childNode);
			}
		}
	}

	/* (non-Javadoc)
	 * @see fr.kesk.gl.shader.GlShader#free()
	 */
	@Override
	public void free() {
		//android.util.Log.d(TAG,"free()");
		if(this.fbo != null){
			this.fbo.free();
			this.fbo = null;
		}
		if(this.program != null){
			this.program.free();
			this.program = null;
		}
		if(this.shadowTextures != null){
			this.shadowTextures.free();
			this.shadowTextures = null;
		}
	}

}
