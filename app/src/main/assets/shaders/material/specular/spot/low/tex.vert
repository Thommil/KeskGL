/**
* 	Specular spot lighting helper shader in PerVertex 
* 	
*	@author Thomas MILLET
**/

precision mediump float;

//Context
uniform mat4 u_mvpMatrixMat4;      		       
uniform mat4 u_mvMatrixMat4;

//Material
uniform vec4 u_MaterialAmbientVec4;
uniform vec4 u_MaterialSpecularVec4;
uniform float u_MaterialShininessFloat;

//Light
uniform vec4 u_LightAmbientColorVec4;
uniform vec4 u_LightColorVec4;
uniform vec3 u_LightDirectionVec3;
uniform vec3 u_LightPositionVec3;
uniform vec4 u_LightAttenuationVec4;

//Attributes
attribute vec4 a_PositionVec4;   				 				
attribute vec3 a_NormalVec3;       		
attribute vec2 a_TexCoordinateVec2;	  
		  
//Varying		  
varying vec4 v_DiffuseBaseColorVec4; 
varying vec4 v_SpecularBaseColorVec4;
varying vec4 v_BaseColorVec4;
varying vec2 v_TexCoordinateVec2; 

//Const
const float C_ZERO_FLOAT = 0.0;
const float C_ONE_FLOAT = 1.0;

void main()                                                 	
{
	vec3 l_PositionVec3 = vec3(u_mvMatrixMat4 * a_PositionVec4);
	vec3 l_NormalVec3 = normalize(vec3(u_mvMatrixMat4 * vec4(a_NormalVec3, C_ZERO_FLOAT)));
	vec3 l_LightRawVectorVec3 = u_LightPositionVec3 - l_PositionVec3;
	vec3 l_LightVectorVec3 = normalize(l_LightRawVectorVec3);
	float l_DistanceFloat = length(l_LightRawVectorVec3);
	float l_DotNormalLightFloat = dot(l_NormalVec3, l_LightVectorVec3);
	float l_SpotEffectFloat = dot(l_LightVectorVec3, normalize(-u_LightDirectionVec3));
	float l_DiffuseFloat = pow(l_SpotEffectFloat, u_LightAttenuationVec4.w) / (u_LightAttenuationVec4.x + (l_DistanceFloat * u_LightAttenuationVec4.y) + (l_DistanceFloat * l_DistanceFloat * u_LightAttenuationVec4.z));
	vec3 l_EyeVec3 = normalize(-l_PositionVec3);
	vec3 l_HVec3 = normalize(l_LightVectorVec3 + l_EyeVec3);
	float l_SpecularTermFloat = max(dot(l_HVec3,l_NormalVec3), C_ZERO_FLOAT);
	
	v_BaseColorVec4 = u_MaterialAmbientVec4 * u_LightAmbientColorVec4;
	v_DiffuseBaseColorVec4 = u_LightColorVec4 * l_DiffuseFloat * max(l_DotNormalLightFloat, C_ZERO_FLOAT);
	v_SpecularBaseColorVec4 = l_DiffuseFloat * u_MaterialSpecularVec4 * u_LightColorVec4 * pow(l_SpecularTermFloat, u_MaterialShininessFloat);		
	v_TexCoordinateVec2 = a_TexCoordinateVec2;
	gl_Position = u_mvpMatrixMat4 * a_PositionVec4;                     		  
}