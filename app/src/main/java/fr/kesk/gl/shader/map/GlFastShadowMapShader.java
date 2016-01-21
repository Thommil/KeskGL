package fr.kesk.gl.shader.map;

import java.io.InputStream;

/**
 * Shader used to generate fast ShadowMap texture for reuse
 * <br/>
 * <br/>
 * After rendering, results are stored in :<br/>
 * 	- shadowMap -> The depth texture in [depth, 0.0, 0.0, 0.0]<br/>
 * 	- shadowMatrix -> The light BIAS_M_P matrix<br/>
 * <br/>
 * Simple and fast shadow map shader using only one slot of gl_FragColor. No
 * guarantee on precision but leave 3 slots available in color.
 * <br/>
 * These assets must be used in rendering shader to add shadows, see
 * fr.kesk.libgl.shader.gsgl.map.* for shader samples. 
 *
 * @author Thomas MILLET
 *
 */
public class GlFastShadowMapShader extends GlAbstractShadowMapShader {

	/**
	 * TAG log
	 */
	@SuppressWarnings("unused")
	private final static String TAG = GlFastShadowMapShader.class.getName();
	
	/**
	 * Location of ShadowMap vertex shader
	 */
	private final static String VERTEX_SHADER = "/fr/kesk/libgl/shader/gsgl/map/fast_shadowmap.vtx";
	
	/**
	 * Location of ShadowMap fragment shader
	 */
	private final static String FRAGMENT_SHADER = "/fr/kesk/libgl/shader/gsgl/map/fast_shadowmap.fgt";
		
	
	
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
