/**
* 	Fragment shader for VSM depth-map generation  
* 	
*	@author Thomas MILLET
**/

#extension GL_OES_standard_derivatives : enable
 
#ifdef GL_OES_fragment_precision_high
	precision highp float;
#else
	precision mediump float;
#endif

//Varying
varying vec4 v_PositionVec4;
 	
//Constants
const float C_ZERO_FLOAT = 0.0;
const float C_HALF_FLOAT = 0.5;
const float C_QUARTER_FLOAT = 0.25;
 	
void main()                    		
{
	float l_depthFloat = ((v_PositionVec4.z / v_PositionVec4.w) * C_HALF_FLOAT) + C_HALF_FLOAT;

	float l_moment1Float = l_depthFloat;
	float l_moment2Float = l_depthFloat * l_depthFloat;
	float l_dxFloat = dFdx(l_depthFloat);
	float l_dyFloat = dFdy(l_depthFloat);
	
	l_moment2Float += C_QUARTER_FLOAT * ((l_dxFloat * l_dxFloat) + (l_dyFloat * l_dyFloat));
	
	gl_FragColor = vec4(l_moment1Float, l_moment2Float, C_ZERO_FLOAT, C_ZERO_FLOAT);
}	

/********************************************************************************************
*	RENDERING CODE -> #ifdef GL_OES_standard_derivatives
********************************************************************************************/

////Const
//const float C_ZERO_FLOAT = 0.0;
//const float C_ONE_FLOAT = 1.0;
//const float C_ZOFFSET_FLOAT = 0.005;
//const float C_VARIANCE_FLOAT = 0.05;
//
///**
//*	Get the stored shadow factor in sampler based on VSM approach.
//*
//*	@param sampler The sampler of the texture owning shadow map
//*	@param l_ShadowCoordVec4 The shadow map tranformation matrix 
//*	@return The factor [0,1], 1 is for fully lite, 0 for full shadow
//*/
//float getVSMShadowFactorFloat(sampler2D sampler, vec4 l_ShadowCoordVec4)
//{
//	vec4 l_nShadowCoordVec4 = l_ShadowCoordVec4;
//	l_nShadowCoordVec4.z -= C_ZOFFSET_FLOAT;
// 	l_nShadowCoordVec4 /= l_ShadowCoordVec4.w;
//
//	vec2 l_momentsVec2 = texture2D(u_shadowMapSampler2D, l_nShadowCoordVec4.xy).rg;
//	
//	float l_distance1Float = float(l_momentsVec2.x >= C_ONE_FLOAT || l_nShadowCoordVec4.z <= l_momentsVec2.x);
//	
//	float l_varianceFloat = max(l_momentsVec2.y - (l_momentsVec2.x*l_momentsVec2.x), C_VARIANCE_FLOAT);
//	float l_varianceDeltaFloat = l_nShadowCoordVec4.z - l_momentsVec2.x;
//	float l_distance2Float = (C_ONE_FLOAT - (l_varianceFloat / (l_varianceFloat + (l_varianceDeltaFloat * l_varianceDeltaFloat))));
//	
//	return max(l_distance1Float, l_distance2Float);
//}

	