#version 330 core

layout(location = 0) out vec4 color;
//out vec4 FragColor;

//uniform vec4 colour;
//uniform vec2 light_pos;
//uniform vec3 directionalLight = vec3(-0.2f, -1.0f, -0.3f);
//uniform vec3 objectColor;
//uniform vec3 lightColor;
//uniform vec3 cameraPos = vec3(0.3f, 0.3f, 0.3f); //fixed value for now. You should set this in client side if you want to dynamically change it


uniform sampler2D textureSampler;

in vec4 o_Color;
in vec2 o_Tex;

void main()
{

	vec4 texColor = texture(textureSampler, o_Tex);
	//color = vec4(result, 1.0);
	color = texColor;
}