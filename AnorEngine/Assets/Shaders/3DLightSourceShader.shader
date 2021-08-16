#type vertex
#version 330 core

layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec2 a_UV; //These values are grabbed from the glVertexAttribPointer() function in OpenGL. The index (first parameter) value of the attrib function is what the "locations" here become.
layout(location = 2) in float tid;
layout(location = 3) in vec4 color; //The pipeline is CPU --> OpenGL --> Vertex Shader --> Fragment Shader -->
layout(location = 4) in vec3 a_Normal;

uniform mat4 u_Transform;
uniform mat4 u_ViewProjectionMatrix = mat4(1.0);

out DATA
{
	vec4 position;
	vec2 UV;
	float tid;
	vec4 color;
	vec3 Normal;
	vec3 FragPos;
}vs_out;


void main()
{


	vs_out.Normal = mat3(transpose(inverse(u_Transform))) * aNormal;
	vs_out.Normal = a_Normal;
	gl_Position = u_ViewProjectionMatrix * u_Transform  * a_Position; //+ vec4(offset, 0.0, 1.0)); 
	vs_out.UV = a_UV;
	vs_out.tid = tid;
	vs_out.color = color;
	vs_out.FragPos = vec3(u_Transform * position);
}

#type fragment
#version 330 core

layout (location = 0) out vec4 color;

in DATA
{
	vec4 position;
	vec2 UV;
	float tid;
	vec4 color;
	vec3 Normal;
	vec3 FragPos;
}fs_out;


void main()
{	
	float intensity = 1.0 / length(fs_out.position.xy - light_pos);
	//color = /*fs_out.color */ colour * intensity; // if you uncomment colour here, you use uniform colour that you can set in the CPU. However, if you uncomment "fs_out.color" you use the attribute color of the vertex.

	colour = vec4(1, 1, 1, 1); //bright white
}