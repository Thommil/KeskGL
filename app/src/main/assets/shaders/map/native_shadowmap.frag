/**
* 	Fragment shader for native depth-map generation  
* 	
*	@author Thomas MILLET
**/

#ifdef GL_OES_fragment_precision_high
	precision highp float;
#else
	precision mediump float;
#endif
 	
void main()                    		
{
	//NOP
}	

/********************************************************************************************
*	RENDERING CODE -> #ifdef GL_OES_depth_texture
********************************************************************************************/

//#extension GL_OES_depth_texture : require
//
////Const
//const float C_ZERO_FLOAT = 0.0;
//const float C_ONE_FLOAT = 1.0;
//const float C_ZOFFSET_FLOAT = 0.005;
//const float C_THRESHOLD_FLOAT = C_ONE_FLOAT - C_ZOFFSET_FLOAT;
//
///**
//*	Get the stored shadow factor in sampler in first component
//*
//*	@param sampler The sampler of the texture owning shadow map
//*	@param l_ShadowCoordVec4 The shadow map tranformation matrix 
//*	@return The factor [0,1], 1 is for fully lite, 0 for full shadow
//*/
//float getNativeShadowFactorFloat(sampler2D sampler, vec4 l_ShadowCoordVec4)
//{
//	vec4 l_nShadowCoordVec4 = l_ShadowCoordVec4;
//	l_nShadowCoordVec4.z -= C_ZOFFSET_FLOAT;
// 	l_nShadowCoordVec4 /= l_ShadowCoordVec4.w;
//
//	float l_momentFloat = texture2D(u_shadowMapSampler2D, l_nShadowCoordVec4.xy).r;
//	
//	return float(l_momentFloat >= C_THRESHOLD_FLOAT || l_nShadowCoordVec4.z <= l_momentFloat);
//}

	