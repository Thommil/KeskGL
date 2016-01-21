/**
* 	Vertex shader for native depth-map generation 
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
		 				
void main()                                                 	
{
	gl_Position = u_mvpMatrixMat4 * a_PositionVec4;           		  
}