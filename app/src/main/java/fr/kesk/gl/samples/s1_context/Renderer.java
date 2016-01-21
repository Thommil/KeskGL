package fr.kesk.gl.samples.s1_context;

import android.opengl.GLES20;
import android.opengl.GLException;
import fr.kesk.gl.ContextManager;
import fr.kesk.gl.GlContext;
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
		GLES20.glClearColor(0f, 0f, 0f, 1f);
		((GlView)contextManager).setFPSListener(this);
		android.util.Log.d(TAG,"GL Renderer : "+GlContext.getRenderer());
		android.util.Log.d(TAG,"GL Vendor : "+GlContext.getVendor());
		android.util.Log.d(TAG,"GL Version : "+GlContext.getVersion());
		android.util.Log.d(TAG,"GL Extensions : "+GlContext.getExtensions());
	}

	/* (non-Javadoc)
	 * @see fr.kesk.gl.ui.GlElement#onResize(int, int)
	 */
	@Override
	protected void onResize(int width, int height) {
		android.util.Log.d(TAG,"onResize()");
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
