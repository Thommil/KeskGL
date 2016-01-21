package fr.kesk.gl.shader.map;

import java.io.InputStream;

import android.opengl.GLES20;
import android.opengl.GLException;

import fr.kesk.gl.GlContext;

/**
 * Shader used to generate VSM ShadowMap texture for reuse
 * <br/>
 * <br/>
 * After rendering, results are stored in :<br/>
 * 	- shadowMap -> The depth texture in [depth, depth*depth, 0.0, 0.0]<br/>
 * 	- shadowMatrix -> The light BIAS_M_P matrix<br/>
 * <br/>
 * VSM shadow map allows precise shadowing with 2 slots in texture for additional
 * treatments (blur, bloom ...). It should be used for specific needs as memory
 * consumption can be huge.
 * <br/>
 * These assets must be used in rendering shader to add shadows, see
 * fr.kesk.libgl.shader.gsgl.map.* for shader samples. 
 *
 * @author Thomas MILLET
 *
 */
public class GlVSMShadowMapShader extends GlAbstractShadowMapShader{

	/**
	 * TAG log
	 */
	@SuppressWarnings("unused")
	private final static String TAG = GlVSMShadowMapShader.class.getName();
	
	/**
	 * Location of ShadowMap vertex shader
	 */
	private final static String VERTEX_SHADER = "/fr/kesk/libgl/shader/gsgl/map/vsm_shadowmap.vtx";
	
	/**
	 * Location of ShadowMap fragment shader
	 */
	private final static String FRAGMENT_SHADER = "/fr/kesk/libgl/shader/gsgl/map/vsm_shadowmap.fgt";
	
	/**
	 * The extension used for this shader
	 */
	public final static String VSM_EXTENSION_NAME = "GL_OES_standard_derivatives";
	
	/* (non-Javadoc)
	 * @see fr.kesk.libgl.shader.map.GlAbstractShadowMapShader#compile()
	 */
	@Override
	public void compile() {
		if(GlContext.isExtensionSupported(VSM_EXTENSION_NAME)){
			super.compile();
		}
		else throw new GLException(GLES20.GL_INVALID_OPERATION,"GL_OES_standard_derivatives not supported");
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
