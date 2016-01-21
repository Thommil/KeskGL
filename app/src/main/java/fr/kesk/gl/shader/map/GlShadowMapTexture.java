package fr.kesk.gl.shader.map;

import java.nio.ByteBuffer;

import android.opengl.GLES20;
import fr.kesk.gl.buffer.GlFrameBufferObject;
import fr.kesk.gl.texture.GlTexture;
import fr.kesk.gl.tools.MatrixUtils;

/**
 * Specific implementation of GlTexture to store ShadowMap texture
 * 
 * @author Thomas MILLET
 *
 */
public class GlShadowMapTexture extends GlTexture{
	
	/**
	 * The shadow map ID
	 */
	public final int id;
	
	/**
	 * The shadow map texture border size
	 */
	public final int size;
	
	/**
	 * The bound FBO
	 */
	private final GlFrameBufferObject fbo;
	
	/**
	 * The associated shadow matrix
	 */
	public final float[]shadowMatrix = new float[16];
	
	/**
	 * Default constructor
	 * 
	 * @param size The shadow map texture size
	 * @param fbo The FBO owning this attachment for buffer access
	 */
	public GlShadowMapTexture(final int id, final int size, final GlFrameBufferObject fbo){
		super();
		//android.util.Log.d(TAG,"NEW");
		this.id = id;
		this.size = size;
		this.fbo = fbo;
		MatrixUtils.setIdentityM(this.shadowMatrix, 0);
	}
	
	/* (non-Javadoc)
	 * @see fr.kesk.libgl.texture.GlTexture#getBytes()
	 */
	@Override
	public ByteBuffer getBytes() {
		if(this.fbo != null){
			return (ByteBuffer)this.fbo.read(0, 0, this.size, this.size).position(0);
		}
		else{
			return null;
		}
	}

	/* (non-Javadoc)
	 * @see fr.kesk.libgl.texture.GlTexture#getId()
	 */
	@Override
	public int getId() {
		return this.id;
	}

	/* (non-Javadoc)
	 * @see fr.kesk.libgl.texture.GlTexture#getHeight()
	 */
	@Override
	public int getHeight() {
		return this.size;
	}

	/* (non-Javadoc)
	 * @see fr.kesk.libgl.texture.GlTexture#getWidth()
	 */
	@Override
	public int getWidth() {
		return this.size;
	}

	/* (non-Javadoc)
	 * @see fr.kesk.libgl.texture.GlTexture#getSize()
	 */
	@Override
	public int getSize() {
		return this.size * this.size * SIZEOF_UNSIGNED_BYTE;
	}

	/* (non-Javadoc)
	 * @see fr.kesk.libgl.texture.GlTexture#getMagnificationFilter()
	 */
	@Override
	public int getMagnificationFilter() {
		return GLES20.GL_NEAREST;
	}

	/* (non-Javadoc)
	 * @see fr.kesk.libgl.texture.GlTexture#getMinificationFilter()
	 */
	@Override
	public int getMinificationFilter() {
		return GLES20.GL_NEAREST;
	}

	/* (non-Javadoc)
	 * @see fr.kesk.libgl.texture.GlTexture#getFormat()
	 */
	@Override
	public int getFormat() {
		return GLES20.GL_RGBA;
	}

	/* (non-Javadoc)
	 * @see fr.kesk.libgl.texture.GlTexture#getType()
	 */
	@Override
	public int getType() {
		return GLES20.GL_UNSIGNED_BYTE;
	}

	/* (non-Javadoc)
	 * @see fr.kesk.libgl.texture.GlTexture#getWrapMode(int)
	 */
	@Override
	public int getWrapMode(int axeId) {
		return GLES20.GL_CLAMP_TO_EDGE;
	}
}
