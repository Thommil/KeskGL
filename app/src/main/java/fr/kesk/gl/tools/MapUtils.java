package fr.kesk.gl.tools;

import fr.kesk.gl.GlContext;
import fr.kesk.gl.shader.map.GlAbstractShadowMapShader;
import fr.kesk.gl.shader.map.GlFastShadowMapShader;
import fr.kesk.gl.shader.map.GlNativeShadowMapShader;
import fr.kesk.gl.shader.map.GlVSMShadowMapShader;

/**
 * Tools for maps :<br/>
 * <ul>
 * 	<li>shadow maps selector</li>
 * </ul>
 * 
 * @author Thomas MILLET
 *
 */
public class MapUtils {

	/**
	 * TAG log
	 */
	@SuppressWarnings("unused")
	private final static String TAG = MapUtils.class.getName();
	
	/**
	 * Get the most suitable shadow map shader for current GPU based on
	 * Native -> VSM -> Fast order.
	 * This method must be called in GL thread.<br/>
	 * <br/>
	 * This method must be also applied in target shader. The code below shows
	 * an implementation of method getShadowFactorFloat() depending on shader in use :
	 * 	<pre>
	 * {@code
		//Const
		const float C_ZERO_FLOAT = 0.0;
		
		//NATIVE
		#ifdef GL_OES_depth_texture
		    #extension GL_OES_depth_texture : require
		    
			//Const
		    const float C_ONE_FLOAT = 1.0;
		    const float C_ZOFFSET_FLOAT = 0.005;
		    const float C_THRESHOLD_FLOAT = C_ONE_FLOAT - C_ZOFFSET_FLOAT;
		    
		    float getShadowFactorFloat(sampler2D sampler, vec4 l_ShadowCoordVec4)
		    {
		    	vec4 l_nShadowCoordVec4 = l_ShadowCoordVec4;
		    	l_nShadowCoordVec4.z -= C_ZOFFSET_FLOAT;
		     	l_nShadowCoordVec4 /= l_ShadowCoordVec4.w;
		    
		    	float l_momentFloat = texture2D(u_shadowMapSampler2D, l_nShadowCoordVec4.xy).r;
		    	
		    	return float(l_momentFloat >= C_THRESHOLD_FLOAT || l_nShadowCoordVec4.z <= l_momentFloat);
		    }	
		
		//VSM
		#elif defined GL_OES_standard_derivatives
			//Const
		    const float C_ONE_FLOAT = 1.0;
		    const float C_ZOFFSET_FLOAT = 0.005;
		    const float C_VARIANCE_FLOAT = 0.05;
		    
		    float getShadowFactorFloat(sampler2D sampler, vec4 l_ShadowCoordVec4)
		    {
		    	vec4 l_nShadowCoordVec4 = l_ShadowCoordVec4;
		    	l_nShadowCoordVec4.z -= C_ZOFFSET_FLOAT;
		     	l_nShadowCoordVec4 /= l_ShadowCoordVec4.w;
		    
		    	vec2 l_momentsVec2 = texture2D(u_shadowMapSampler2D, l_nShadowCoordVec4.xy).rg;
		    	
		    	float l_distance1Float = float(l_momentsVec2.x >= C_ONE_FLOAT || l_nShadowCoordVec4.z <= l_momentsVec2.x);
		    	
		    	float l_varianceFloat = max(l_momentsVec2.y - (l_momentsVec2.x*l_momentsVec2.x), C_VARIANCE_FLOAT);
		    	float l_varianceDeltaFloat = l_nShadowCoordVec4.z - l_momentsVec2.x;
		    	float l_distance2Float = (C_ONE_FLOAT - (l_varianceFloat / (l_varianceFloat + (l_varianceDeltaFloat * l_varianceDeltaFloat))));
		    	
		    	return max(l_distance1Float, l_distance2Float);
		    }
		//Fast
		#else
			//Const
		    const float C_ONE_FLOAT = 1.0;
		    const float C_ZOFFSET_FLOAT = 0.005;
		    const float C_THRESHOLD_FLOAT = C_ONE_FLOAT - C_ZOFFSET_FLOAT;
		    
		    float getShadowFactorFloat(sampler2D sampler, vec4 l_ShadowCoordVec4)
		    {
		    	vec4 l_nShadowCoordVec4 = l_ShadowCoordVec4;
		    	l_nShadowCoordVec4.z -= C_ZOFFSET_FLOAT;
		     	l_nShadowCoordVec4 /= l_ShadowCoordVec4.w;
		    
		    	float l_momentFloat = texture2D(u_shadowMapSampler2D, l_nShadowCoordVec4.xy).r;
		    	
		    	return float(l_momentFloat >= C_THRESHOLD_FLOAT || l_nShadowCoordVec4.z <= l_momentFloat);
		    }
		#endif
	 * }
	 * </pre>
	 * 
	 *  
	 * @return A shadow map shader in a GlAbstractShadowMapShader instance
	 */
	public static GlAbstractShadowMapShader getBestShadowMapShader(){
		//android.util.Log.d(TAG,"getBestShadowMapShader()");
		if(GlContext.isExtensionSupported(GlNativeShadowMapShader.NATIVE_EXTENSION_NAME)){
			return new GlNativeShadowMapShader();
		}
		else if(GlContext.isExtensionSupported(GlVSMShadowMapShader.VSM_EXTENSION_NAME)){
			return new GlVSMShadowMapShader();
		}
		return new GlFastShadowMapShader();
	}
}
