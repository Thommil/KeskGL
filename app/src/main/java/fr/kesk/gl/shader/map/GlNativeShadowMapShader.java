package fr.kesk.gl.shader.map;

import java.io.IOException;
import java.io.InputStream;

import android.opengl.GLES20;
import android.opengl.GLException;
import android.opengl.Matrix;
import fr.kesk.gl.GlContext;
import fr.kesk.gl.GlAssets.Light;
import fr.kesk.gl.buffer.GlFrameBufferObject;
import fr.kesk.gl.buffer.GlFrameBufferObject.Attachment;
import fr.kesk.gl.shader.GlProgram;
import fr.kesk.gl.texture.GlTextureSet;

/**
 * Shader used to generate native ShadowMap texture for reuse
 * <br/>
 * <br/>
 * After rendering, results are stored in :<br/>
 * 	- shadowMap -> The depth texture in [depth]<br/>
 * 	- shadowMatrix -> The light BIAS_M_P matrix<br/>
 * <br/>
 * Use native depth texture if supported.
 * <br/>
 * These assets must be used in rendering shader to add shadows, see
 * fr.kesk.libgl.shader.gsgl.map.* for shader samples. 
 *
 * @author Thomas MILLET
 *
 */
public class GlNativeShadowMapShader extends GlAbstractShadowMapShader {

	/**
	 * TAG log
	 */
	@SuppressWarnings("unused")
	private final static String TAG = GlNativeShadowMapShader.class.getName();
	
	/**
	 * Location of ShadowMap vertex shader
	 */
	private final static String VERTEX_SHADER = "/fr/kesk/libgl/shader/gsgl/map/native_shadowmap.vtx";
	
	/**
	 * Location of ShadowMap fragment shader
	 */
	private final static String FRAGMENT_SHADER = "/fr/kesk/libgl/shader/gsgl/map/native_shadowmap.fgt";
		
	/**
	 * The extension used for this shader
	 */
	public final static String NATIVE_EXTENSION_NAME = "GL_OES_depth_texture";
	
	/* (non-Javadoc)
	 * @see fr.kesk.libgl.shader.map.GlAbstractShadowMapShader#compile()
	 */
	@Override
	public void compile() {
		if(GlContext.isExtensionSupported(NATIVE_EXTENSION_NAME)){
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
				this.program.enableAttributes();
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
				this.fbo.attach(this.currentShadowMap, Attachment.TYPE_DEPTH);
				
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
		else throw new GLException(GLES20.GL_INVALID_OPERATION,"GL_OES_depth_texture not supported");
	}
	
	/* (non-Javadoc)
	 * @see fr.kesk.libgl.shader.map.GlAbstractShadowMapShader#newShadowMap()
	 */
	@Override
	public GlShadowMapTexture newShadowMap() {
		//android.util.Log.d(TAG,"newShadowMap()");
		this.currentShadowMap = new GlNativeShadowMap(this.shadowTextures.size, this.quality, this.fbo); 
		this.shadowTextures.put(this.currentShadowMap);
		return this.currentShadowMap;
	}

	/* (non-Javadoc)
	 * @see fr.kesk.libgl.shader.map.GlAbstractShadowMapShader#getVertexShaderInputStream()
	 */
	@Override
	protected InputStream getVertexShaderInputStream() {
		return this.getClass().getResourceAsStream(VERTEX_SHADER);
	}


	/* (non-Javadoc)
	 * @see fr.kesk.libgl.shader.map.GlAbstractShadowMapShader#getFragmentShaderInputStream()
	 */
	@Override
	protected InputStream getFragmentShaderInputStream() {
		return this.getClass().getResourceAsStream(FRAGMENT_SHADER);
	}
	
	/**
	 * 
	 * Specific shadow map based on native depth texture
	 * 
	 * @author Thomas MILLET
	 *
	 */
	private final static class GlNativeShadowMap extends GlShadowMapTexture{
		
		/**
		 * Default constructor
		 * @param id The texture id
		 * @param size The texture border size
		 * @param fbo The fbo owning this texture
		 */
		public GlNativeShadowMap(int id, int size, GlFrameBufferObject fbo) {
			super(id, size, fbo);
		}

		/* (non-Javadoc)
		 * @see fr.kesk.libgl.shader.map.GlShadowMapTexture#getFormat()
		 */
		@Override
		public int getFormat() {
			return 0x1902;
		}

		/* (non-Javadoc)
		 * @see fr.kesk.libgl.shader.map.GlShadowMapTexture#getType()
		 */
		@Override
		public int getType() {
			return GLES20.GL_UNSIGNED_INT;
		}
	}
}
