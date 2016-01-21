package fr.kesk.gl.shader;

import fr.kesk.gl.GlAssets.Node;

/**
 * Generic Shader API for drawing operations.
 * 
 * @author Thomas MILLET
 *
 */
public interface GlShader {
	
	/**
	 * VERY_LOW quality settings
	 */
	public static final int QUALITY_VERY_LOW = 64;
	
	/**
	 * LOW quality settings
	 */
	public static final int QUALITY_LOW = 128;
	
	/**
	 * MEDIUM quality settings
	 */
	public static final int QUALITY_MEDIUM = 256;
	
	/**
	 * HIGH quality settings
	 */
	public static final int QUALITY_HIGH = 512;
	
	/**
	 * VERY_HIGH quality settings
	 */
	public static final int QUALITY_VERY_HIGH = 1024;
	
	/**
	 * Set the shader quality based on shader available quality modes
	 * 
	 * @param quality The quality indice depending on implementation
	 */
	public void setQuality(final int quality);
	
	/**
	 * Compile the shader (implementation must authorized multiple calls)
	 */
	public void compile();
	
	/**
	 * Called on draw state to render current shader state
	 */
	public void render();
	
	/**
	 * Called on draw state to render a node and its children using current shader
	 * 
	 * @param node The nodes instance to render
	 */
	public void render(final Node nodeInstance);
		
	/**
	 * Free resources allocated by this shader
	 */
	public void free();
}
