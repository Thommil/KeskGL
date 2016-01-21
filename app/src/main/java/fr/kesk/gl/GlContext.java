package fr.kesk.gl;

import java.util.Arrays;
import java.util.StringTokenizer;

import fr.kesk.gl.tools.MatrixUtils;

import android.opengl.GLES20;
import android.opengl.GLException;

/**
 * OpenGL context for Android 2.2+<br/>
 * <br/>
 * Based on OpenGLES 2.0
 * 
 * @author Thomas MILLET
 *
 */
public class GlContext{
	
	/**
	 * TAG log
	 */
	@SuppressWarnings("unused")
	private final static String TAG = GlContext.class.getName();
	
	/**
	 * Store the list of extensions for fast access
	 */
	private static int[]extensions;
	
	/**
	 * VIEW matrix for overall context
	 */
	public final float[] vMatrix = new float[16];
		
	/**
	 * PROJECTION matrix for overall context
	 */
	public final float[] pMatrix = new float[16];
		
	/**
	 * Graphic assets bound to this context
	 */
	public final GlAssets assets = new GlAssets();
	
	/**
	 * Default constructor
	 */
	public GlContext(){
		//android.util.Log.i(TAG,"NEW");
		MatrixUtils.setIdentityM(this.vMatrix, 0);
		MatrixUtils.setIdentityM(this.pMatrix, 0);
	}
		
	/**
	 * Check current ERROR status and throws an Exception if error found
	 * 
	 * @throws GLException
	 */
	public static void glCheckError() throws GLException {
		//android.util.Log.d(TAG,"checkGlError()");
		int error;
		while ((error = GLES20.glGetError()) != GLES20.GL_NO_ERROR) {
			throw new GLException(error);
		}
	}
	
	/**
	 * Unified GL state accessor
	 * 
	 * @param key The key of the state to get
	 * @return The state value(s) in a float array
	 */
	public static float[] glGetState(final int key){
		//android.util.Log.d(TAG,"getGlState("+key+")");
		final float[] result;
		switch(key){
			//2
			case GLES20.GL_ALIASED_LINE_WIDTH_RANGE :
			case GLES20.GL_ALIASED_POINT_SIZE_RANGE :
			case GLES20.GL_DEPTH_RANGE :	
			case GLES20.GL_MAX_VIEWPORT_DIMS :
				result = new float[2];
				break;
			//4
			case GLES20.GL_BLEND_COLOR :
			case GLES20.GL_COLOR_CLEAR_VALUE :
			case GLES20.GL_COLOR_WRITEMASK :
			case GLES20.GL_SCISSOR_BOX :
			case GLES20.GL_VIEWPORT :
				result = new float[4];
				break;
			//Dynamic	
			case GLES20.GL_COMPRESSED_TEXTURE_FORMATS :
				final int[]tmp1 = new int[1];
				GLES20.glGetIntegerv(GLES20.GL_NUM_COMPRESSED_TEXTURE_FORMATS, tmp1, 0);
				result = new float[tmp1[0]];
				break;	
			case GLES20.GL_SHADER_BINARY_FORMATS :	
				final int[]tmp2 = new int[1];
				GLES20.glGetIntegerv(GLES20.GL_NUM_SHADER_BINARY_FORMATS, tmp2, 0);
				result = new float[tmp2[0]];
				break;
			//1	
			default : 
				result = new float[1];
		}
		GLES20.glGetFloatv(key, result, 0);
		return result;
	}
	
	/**
	 * Get the OpenGL extensions list in a String separated by spaces 
	 * 
	 * @return The list of extensions in a String
	 */
	public static String getExtensions(){
		//android.util.Log.d(TAG,"getExtensions()");
		return GLES20.glGetString(GLES20.GL_EXTENSIONS);
	}
	
	/**
	 * Indicates if the specified extension is supported
	 * 
	 * @param extension The extension name (case insensitive)
	 * @return True if the extension is supported, false otherwise
	 */
	public static boolean isExtensionSupported(final String extension){
		//android.util.Log.d(TAG,"isExtensionSupported("+extension+")");
		if(GlContext.extensions == null){
			final StringTokenizer tokenizer = new StringTokenizer(GlContext.getExtensions());
			GlContext.extensions = new int[tokenizer.countTokens()];
			int tokenIndex = 0;
			while(tokenizer.hasMoreTokens()){
				GlContext.extensions[tokenIndex++] = tokenizer.nextToken().hashCode();
			}
			Arrays.sort(GlContext.extensions);
		}
		
		return (Arrays.binarySearch(GlContext.extensions, extension.hashCode()) >= 0);
	}
	
	/**
	 * Disable extension for futur calls to isExtensionSupported()
	 * 
	 * @param extension The extension to disable
	 * @return True if the extension was enabled, false otherwise
	 */
	public static boolean disableExtension(final String extension){
		if(isExtensionSupported(extension)){
			final int extensionIndex = Arrays.binarySearch(GlContext.extensions, extension.hashCode());
			GlContext.extensions[extensionIndex] = GLES20.GL_FALSE;
			Arrays.sort(GlContext.extensions);
			return true;
		}
		else return false;
	}
	
	/**
	 * Enable extension for futur calls to isExtensionSupported(). This method mainly
	 * allows user to implement custom extensions and forces GL behavior.
	 * 
	 * @param extension The extension to enable
	 */
	public static void enableExtension(final String extension){
		final int extensionIndex = Arrays.binarySearch(GlContext.extensions, extension.hashCode());
		if(extensionIndex < 0){
			final int freeSlotIndex = Arrays.binarySearch(GlContext.extensions, GLES20.GL_FALSE);
			if(freeSlotIndex >= 0){
				GlContext.extensions[extensionIndex] = extension.hashCode();
			}
			else{
				final int[] clone = new int[GlContext.extensions.length + 1];
				System.arraycopy(GlContext.extensions, 0, clone, 0, GlContext.extensions.length);
				clone[GlContext.extensions.length] = extension.hashCode();
				GlContext.extensions = clone;
				Arrays.sort(GlContext.extensions);
			}
		}
	}
	
	/**
	 * Get the OpenGL renderer 
	 * 
	 * @return The renderer in a String
	 */
	public static String getRenderer(){
		//android.util.Log.d(TAG,"getRenderer()");
		return GLES20.glGetString(GLES20.GL_RENDERER);
	}
	
	/**
	 * Get the OpenGL vendor 
	 * 
	 * @return The vendor in a String
	 */
	public static String getVendor(){
		//android.util.Log.d(TAG,"getVendor()");
		return GLES20.glGetString(GLES20.GL_VENDOR);
	}
	
	/**
	 * Get the OpenGL version 
	 * 
	 * @return The version in a String
	 */
	public static String getVersion(){
		//android.util.Log.d(TAG,"getVersion()");
		return GLES20.glGetString(GLES20.GL_VERSION);
	}
}
