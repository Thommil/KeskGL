package fr.kesk.gl.samples.s2_program;

import java.io.IOException;
import java.io.InputStream;
import java.nio.FloatBuffer;

import android.opengl.GLES20;

import fr.kesk.gl.ContextManager;
import fr.kesk.gl.GlAssets.Node;
import fr.kesk.gl.shader.GlProgram;
import fr.kesk.gl.shader.GlShader;
import fr.kesk.gl.tools.ByteBufferPool;
import fr.kesk.gl.tools.MatrixUtils;

/**
 * @author Thomas MILLET
 *
 */
public class AmbientShader implements GlShader {

	/**
	 * Log tag
	 */
	public static final String TAG = AmbientShader.class.getName();
	
	final float[] mvpMatrix = new float[16];
	final float[] mMatrix = new float[16];
	ContextManager contextManager;
	GlProgram program;
	int u_mvpMatrixMat4Handle;
	int u_MaterialAmbientVec4Handle;
	int u_LightAmbientColorVec4Handle;
	int a_PositionVec4Handle;
	
	FloatBuffer verticesBuffer;
	
	public AmbientShader(ContextManager contextManager){
		android.util.Log.d(TAG,"NEW");
		this.contextManager = contextManager;
	}
	
	/* (non-Javadoc)
	 * @see fr.kesk.gl.shader.GlShader#setQuality(int)
	 */
	@Override
	public void setQuality(int quality) {
		//NOP
	}

	/* (non-Javadoc)
	 * @see fr.kesk.gl.shader.GlShader#compile()
	 */
	@Override
	public void compile() {
		android.util.Log.d(TAG,"compile()");
		InputStream vertIn = null;
		InputStream fragIn = null;
		try{
			vertIn = this.contextManager.getUIContext().getAssets().open("shaders/material/ambient/notex.vert");
			fragIn = this.contextManager.getUIContext().getAssets().open("shaders/material/ambient/notex.frag");
			
			this.program = new GlProgram(vertIn, fragIn);
			
			this.u_mvpMatrixMat4Handle = this.program.getUniformHandle("u_mvpMatrixMat4");
			this.u_MaterialAmbientVec4Handle = this.program.getUniformHandle("u_MaterialAmbientVec4");
			this.u_LightAmbientColorVec4Handle = this.program.getUniformHandle("u_LightAmbientColorVec4");
			this.a_PositionVec4Handle = this.program.getAttributeHandle("a_PositionVec4");
			
			MatrixUtils.setLookAtM(this.contextManager.getGLContext().vMatrix, 0, 0, 0, 1f, 0, 0, 0, 0, 1, 0);
			MatrixUtils.orthoM(this.contextManager.getGLContext().pMatrix, 0, -1, 1, -1, 1, 0.1f, 2f);
			MatrixUtils.setIdentityM(this.mMatrix, 0);
			
			final float vertices[] = new float[]{
					-0.5f, 0.5f, 0.0f,
					-0.5f, -0.5f, 0.0f,
					0.5f, -0.5f, 0.0f,
					0.5f, 0.5f, 0.0f
			};

			this.verticesBuffer = ByteBufferPool.getInstance().getDirectFloatBuffer(12);
			this.verticesBuffer.put(vertices);
			
		}catch(Exception e){
			e.printStackTrace();
			android.util.Log.e(TAG,"ERROR : "+e.getMessage());
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

	/* (non-Javadoc)
	 * @see fr.kesk.gl.shader.GlShader#render()
	 */
	@Override
	public void render() {
		//android.util.Log.d(TAG,"render()");
		this.program.start();
		this.program.enableAttributes();

		MatrixUtils.rotateM(this.mMatrix, 0, 2, 0, 0, 1);
		MatrixUtils.multiplyMM(this.mvpMatrix, 0, this.contextManager.getGLContext().vMatrix, 0, this.mMatrix, 0);
		MatrixUtils.multiplyMM(this.mvpMatrix, 0, this.contextManager.getGLContext().pMatrix, 0, this.mvpMatrix, 0);

		GLES20.glUniform4f(this.u_MaterialAmbientVec4Handle, 1, 1, 1, 1);
		GLES20.glUniform4f(this.u_LightAmbientColorVec4Handle, 1, 1, 1, 1);
		GLES20.glUniformMatrix4fv(this.u_mvpMatrixMat4Handle,1,false,this.mvpMatrix,0);

		this.verticesBuffer.position(0);
		GLES20.glVertexAttribPointer(this.a_PositionVec4Handle,3,GLES20.GL_FLOAT,false,0,this.verticesBuffer);
		GLES20.glDrawArrays(GLES20.GL_TRIANGLE_FAN,0,4);
		
		this.program.disableAttributes();
		this.program.stop();
	}

	/* (non-Javadoc)
	 * @see fr.kesk.gl.shader.GlShader#render(fr.kesk.gl.GlAssets.Node)
	 */
	@Override
	public void render(Node nodeInstance) {
		this.render();
	}

	/* (non-Javadoc)
	 * @see fr.kesk.gl.shader.GlShader#free()
	 */
	@Override
	public void free() {
		android.util.Log.d(TAG,"free()");
		ByteBufferPool.getInstance().returnDirectBuffer(this.verticesBuffer);
		if(this.program != null){
			this.program.free();
		}
	}

}
