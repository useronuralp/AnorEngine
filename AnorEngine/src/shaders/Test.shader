#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 pr_matrix;
uniform mat4 vw_matrix = mat4(1.0);
uniform mat4 ml_matrix = mat4(1.0);

out vec4 pos;

void main()
{

	gl_Position = (pr_matrix * ml_matrix * vw_matrix) * position;
	pos = position;
}


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;


uniform vec4 colour;
uniform vec2 light_pos;

in vec4 pos;


void main()
{
	float intensity = 1.0 / length(pos.xy - light_pos);
	color = colour * intensity * 0.5f;

}