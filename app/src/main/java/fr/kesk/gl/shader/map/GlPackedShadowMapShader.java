package fr.kesk.gl.shader.map;

import java.io.InputStream;

/**
 * Shader used to generate packed float to vec3 ShadowMap texture for reuse
 * <br/>
 * <br/>
 * After rendering, results are stored in :<br/>
 * 	- shadowMap -> The depth texture in [vec3PackX,vec3PackY,vec3PackZ, 0.0]<br/>
 * 	- shadowMatrix -> The light BIAS_M_P matrix<br/>
 * <br/>
 * Basic shadow map offers a good compromise, a slot is available in texture for 
 * additional treatments (blur, bloom ...)
 * <br/>
 * These assets must be used in rendering shader to add shadows, see
 * fr.kesk.libgl.shader.gsgl.map.* for shader samples. 
 *
 * @author Thomas MILLET
 *
 */
public class GlPackedShadowMapShader extends GlAbstractShadowMapShader{

	/**
	 * TAG log
	 */
	@SuppressWarnings("unused")
	private final static String TAG = GlPackedShadowMapShader.class.getName();
	
	/**
	 * Location of ShadowMap vertex shader
	 */
	private final static String VERTEX_SHADER = "/fr/kesk/libgl/shader/gsgl/map/packed_shadowmap.vtx";
	
	/**
	 * Location of ShadowMap fragment shader
	 */
	private final static String FRAGMENT_SHADER = "/fr/kesk/libgl/shader/gsgl/map/packed_shadowmap.fgt";
	
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
