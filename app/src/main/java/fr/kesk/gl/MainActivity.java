package fr.kesk.gl;

import fr.kesk.gl.ui.GlElement;
import fr.kesk.gl.ui.GlView;
import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.opengl.GLES20;
import android.opengl.GLException;
import android.os.Bundle;

/**
 * Main entry point for a Java GL application
 * <ul>
 * 	<li>-> main view is a {@link GlView}</li>
 *  <li>-> GL Tread is handled in Renderer</li>
 * </ul> 
 *  
 *  No code is needed in this class and all rendering is done
 *  in {@link GlElement} set in Android manifest.
 * 
 * @author Thomas MILLET
 *
 */
public class MainActivity extends Activity {

	/**
	 * Log tag
	 */
	public static final String TAG = MainActivity.class.getName();
	
	/**
	 * Log tag
	 */
	public static final String RENDERER_META = "fr.kesk.gl.renderer";
	
	/**
	 * Reference on the main GL view 
	 */
	private GlView mainView;
	
	/* (non-Javadoc)
	 * @see android.app.Activity#onCreate(android.os.Bundle)
	 */
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		//android.util.Log.d(TAG,"onCreate()");
		super.onCreate(savedInstanceState);
		this.mainView = new GlView(this);
		String rendererClassname = null;
		try{
			final ActivityInfo ai = this.getPackageManager().getActivityInfo(this.getComponentName(), PackageManager.GET_ACTIVITIES | PackageManager.GET_META_DATA);
			rendererClassname = (String)ai.metaData.get(RENDERER_META);
			Class<?> rendererClass = this.getClassLoader().loadClass(rendererClassname);
			if(GlElement.class.isAssignableFrom(rendererClass)){
				this.mainView.setRootElement((GlElement)this.getClassLoader().loadClass(rendererClassname).newInstance());
				this.setContentView(this.mainView);
			}
			else{
				throw new GLException(GLES20.GL_INVALID_ENUM,"Class '"+rendererClassname+"' cannot be assigned as a GlElement");
			}
		}catch(InstantiationException ie){
			throw new GLException(GLES20.GL_INVALID_ENUM,"Failed to instanciate renderer '"+rendererClassname+"'");
		}catch(IllegalAccessException iae){
			throw new GLException(GLES20.GL_INVALID_ENUM,"Failed to instanciate renderer '"+rendererClassname+"'");
		}catch(ClassNotFoundException cnfe){
			throw new GLException(GLES20.GL_INVALID_ENUM,"Renderer class '"+rendererClassname+"' not found");
		}catch(PackageManager.NameNotFoundException nnfe){
			throw new GLException(GLES20.GL_INVALID_OPERATION,"Missing renderer classname, '"+RENDERER_META+"' meta-data not found in manifest");
		} 
	}
	

}
