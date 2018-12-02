#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
// texture coordinate location uv
layout(location = 2) in vec2 in_TexCoordinates;

//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;

// Vector 3 uniform for color
uniform vec3 Color;

//passed to the fragment shader
out vec3 pass_Normal;
out vec3 pass_Color;
out vec3 frag_pos;
out vec2 pass_TexCoordinates;


void main(void)
{
	gl_Position = (ProjectionMatrix  * ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0);
	pass_Normal = (NormalMatrix * vec4(in_Normal, 0.0)).xyz;
	pass_Color = Color;
	frag_pos = vec3(ModelMatrix * vec4(in_Position, 1.0));
	pass_TexCoordinates = in_TexCoordinates;
}
