package fr.kesk.gl.samples.s2_program;

import android.opengl.GLES20;
import android.opengl.GLException;
import fr.kesk.gl.ContextManager;
import fr.kesk.gl.shader.GlShader;
import fr.kesk.gl.ui.GlElement;
import fr.kesk.gl.ui.GlView;
import fr.kesk.gl.ui.GlView.FPSListener;

/**
 * Context and view simple demo and minimum implementation
 * of renderer.
 * 
 * @author Thomas MILLET
 */
public class Renderer extends GlElement implements FPSListener{

	/**
	 * Log tag
	 */
	public static final String TAG = Renderer.class.getName();
	
	GlShader shader;
	
	/* (non-Javadoc)
	 * @see fr.kesk.gl.ui.GlElement#getId()
	 */
	@Override
	public int getId() {
		android.util.Log.d(TAG,"getId()");
		return 0;
	}

	/* (non-Javadoc)
	 * @see fr.kesk.gl.ui.GlElement#onCreate(fr.kesk.gl.ContextManager)
	 */
	@Override
	protected void onCreate(ContextManager contextManager) throws GLException {
		android.util.Log.d(TAG,"onCreate()");
		((GlView)contextManager).setFPSListener(this);
		this.shader = new AmbientShader(contextManager);
		this.shader.compile();
		GLES20.glClearColor(0f, 0f, 0f, 1f);
	}

	/* (non-Javadoc)
	 * @see fr.kesk.gl.ui.GlElement#onResize(int, int)
	 */
	@Override
	protected void onResize(int width, int height) {
		android.util.Log.d(TAG,"onResize("+width+")");
	}

	/* (non-Javadoc)
	 * @see fr.kesk.gl.ui.GlElement#onLayout(long)
	 */
	@Override
	protected void onLayout(long elapsedTime) {
		//android.util.Log.d(TAG,"onLayout()");
	}

	/* (non-Javadoc)
	 * @see fr.kesk.gl.ui.GlElement#onDraw()
	 */
	@Override
	protected void onDraw() throws GLException {
		//android.util.Log.d(TAG,"onDraw()");
		GLES20.glClear(GLES20.GL_COLOR_BUFFER_BIT);
		this.shader.render();
	}

	/* (non-Javadoc)
	 * @see fr.kesk.gl.ui.GlElement#onFree()
	 */
	@Override
	protected void onFree() throws GLException {
		android.util.Log.d(TAG,"onFree()");
	}
	
	/* (non-Javadoc)
	 * @see fr.kesk.gl.ui.GlView.FPSListener#onFpsRefresh(int)
	 */
	@Override
	public void onFpsRefresh(int fps) {
		android.util.Log.d("FPS",""+fps);
	}

}
