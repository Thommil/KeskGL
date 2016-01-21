package fr.kesk.gl.tools;

import android.opengl.GLES20;
import fr.kesk.gl.GlAssets;
import fr.kesk.gl.buffer.Chunk;
import fr.kesk.gl.buffer.GlBuffer;

/**
 * Tools on buffers
 * 
 * @author Thomas MILLET
 *
 */
public class BufferUtils {

	/**
	 * TAG log
	 */
	@SuppressWarnings("unused")
	private final static String TAG = BufferUtils.class.getName();
	
	/**
	 * Convert an asset element into a GlBuffer
	 * 
	 * @param element The input asset element
	 * 
	 * @return A new GlBuffer with element data
	 */
	public static final GlBuffer<float[]> elementToGlBuffer(final GlAssets.Geometry.Element element){
		//android.util.Log.d(TAG,"elementToGlBuffer()");
		@SuppressWarnings("unchecked")
		final Chunk<float[]>[] chunks = new Chunk[element.inputs.length];
		for(int iIndex=0; iIndex < element.inputs.length; iIndex++){
			chunks[iIndex] = new Chunk<float[]>(element.values[iIndex], element.inputs[iIndex][GlAssets.Geometry.Element.SIZE]);
		}
		return new GlBuffer<float[]>(chunks);
	}
	
	/**
	 * Write the buffer content to the given Vertex attribute using VBO if possible
	 * 
	 * @param attrHandle The vertex shader attribute handle
	 * @param chunkIndex The chunk index to use
	 */
	@SuppressWarnings("unchecked")
	public static void glBufferToVertexAttribute(@SuppressWarnings("rawtypes") final GlBuffer buffer, final int attrHandle, final int chunkIndex){
		//android.util.Log.d(TAG,"toVertexAttribute("+attrHandle+", "+chunkIndex+", "+useVBO+")");
		
		//VBO
		if(buffer.handle != GlBuffer.UNBIND_HANDLE){
			GLES20Utils.glVertexAttribPointer(attrHandle, buffer.chunks[chunkIndex].components, buffer.datatype, false, buffer.stride, buffer.chunks[chunkIndex].position*buffer.datasize);
		}
		//CPU -> GPU
		else{
			buffer.position(buffer.chunks[chunkIndex]);
			GLES20.glVertexAttribPointer(attrHandle, buffer.chunks[chunkIndex].components, buffer.datatype, false, buffer.stride, buffer.data);
		}
			
	}	
}
