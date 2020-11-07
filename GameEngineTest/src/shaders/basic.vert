#version 330 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 uv; //These values are grabbed from the glVertexAttribPointer() function in OpenGL. The index (first parameter) value of the attrib function is what the "locations" here become.
layout (location = 2) in float tid;
layout (location = 3) in vec4 color; //The pipeline is CPU --> OpenGL --> Vertex Shader --> Fragment Shader -->


uniform mat4 pr_matrix;
uniform mat4 vw_matrix = mat4(1.0);
uniform mat4 ml_matrix = mat4(1.0);



out DATA
{
	vec4 position;
	vec2 uv;
	float tid;
	vec4 color;

}vs_out;


void main()
{	
	
	gl_Position = (pr_matrix * vw_matrix * ml_matrix ) * position; 
	vs_out.uv = uv;
	vs_out.tid = tid;
	vs_out.position = ml_matrix * position;
	vs_out.color = color;
	//vs_out.v_TexCoord = texCoord;
}