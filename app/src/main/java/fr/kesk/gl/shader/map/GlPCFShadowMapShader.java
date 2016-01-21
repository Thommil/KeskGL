package fr.kesk.gl.shader.map;

import java.io.InputStream;

import android.opengl.GLES20;
import android.opengl.GLException;

import fr.kesk.gl.GlContext;

/**
 * Shader used to generate PCF ShadowMap texture for reuse
 * <br/>
 * <br/>
 * After rendering, results are stored in :<br/>
 * 	- shadowMap -> The depth texture in vec4 for shadowmap2D extension<br/>
 * 	- shadowMatrix -> The light BIAS_M_P matrix<br/>
 * <br/>
 * Based on PCF implementation, simple and powerful shader if supported by device.
 * See http://www.khronos.org/registry/gles/extensions/EXT/EXT_shadow_samplers.txt for
 * details on use, compare modes and equations. 
 * <br/>
 * These assets must be used in rendering shader to add shadows, see
 * fr.kesk.libgl.shader.gsgl.map.* for shader samples. 
 *
 * @author Thomas MILLET
 *
 */
public class GlPCFShadowMapShader extends GlAbstractShadowMapShader {

	/**
	 * TAG log
	 */
	@SuppressWarnings("unused")
	private final static String TAG = GlPCFShadowMapShader.class.getName();
	
	/**
	 * Location of ShadowMap vertex shader
	 */
	private final static String VERTEX_SHADER = "/fr/kesk/libgl/shader/gsgl/map/pcf_shadowmap.vtx";
	
	/**
	 * Location of ShadowMap fragment shader
	 */
	private final static String FRAGMENT_SHADER = "/fr/kesk/libgl/shader/gsgl/map/pcf_shadowmap.fgt";
		
	/**
	 * The extension used for this shader
	 */
	public final static String PCF_EXTENSION_NAME = "GL_EXT_shadow_samplers";
	
	/* (non-Javadoc)
	 * @see fr.kesk.libgl.shader.map.GlAbstractShadowMapShader#compile()
	 */
	@Override
	public void compile() {
		if(GlContext.isExtensionSupported(PCF_EXTENSION_NAME)){
			super.compile();
		}
		else throw new GLException(GLES20.GL_INVALID_OPERATION,"GL_EXT_shadow_samplers not supported");
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
}
