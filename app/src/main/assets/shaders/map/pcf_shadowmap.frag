/**
* 	Fragment shader for PCF depth-map generation  
* 	
*	@author Thomas MILLET
**/

#ifdef GL_OES_fragment_precision_high
	precision highp float;
#else
	precision mediump float;
#endif

//Varying
varying vec4 v_PositionVec4;
 	
void main()                    		
{
	gl_FragColor = v_PositionVec4;
}	

/********************************************************************************************
*	RENDERING CODE -> -> #ifdef GL_EXT_shadow_samplers
********************************************************************************************/

//#extension GL_EXT_shadow_samplers : require
//
///**
//*	Get the stored shadow factor in sampler in first component
//*
//*	@param sampler The sampler of the texture owning shadow map
//*	@param l_ShadowCoordVec4 The shadow map tranformation matrix 
//*	@return The factor [0,1], 1 is for fully lite, 0 for full shadow
//*/
//float getPCFShadowFactorFloat(sampler2DShadow sampler, vec4 l_ShadowCoordVec4)
//{
//	return shadow2DProj(sampler, l_ShadowCoordVec4).r;
//}

	