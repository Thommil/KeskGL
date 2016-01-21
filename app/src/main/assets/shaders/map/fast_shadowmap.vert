/**
* 	Vertex shader for fast depth-map generation 
* 	
*	@author Thomas MILLET
**/

#ifdef GL_OES_fragment_precision_high
	precision highp float;
#else
	precision mediump float;
#endif

//Context
uniform mat4 u_mvpMatrixMat4;      		       

//Attributes
attribute vec4 a_PositionVec4;   				 				

//Varying
varying vec4 v_PositionVec4;   				 				


void main()                                                 	
{
	v_PositionVec4 = u_mvpMatrixMat4 * a_PositionVec4;
	gl_Position = v_PositionVec4;                       		  
}