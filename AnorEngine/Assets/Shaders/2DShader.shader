#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIndex;

uniform mat4 u_ViewProjMat;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;

void main()
{
	gl_Position = (u_ViewProjMat) * vec4(a_Position, 1.0f);
	v_TexCoord = a_TexCoord;
	v_Color = a_Color;
	v_TexIndex = a_TexIndex;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[32];

void main()
{
	o_Color = v_Color;
	switch (int(v_TexIndex))
	{
		case 0: o_Color = texture(u_Textures[0], v_TexCoord) * v_Color; break;
		case 1: o_Color = texture(u_Textures[1], v_TexCoord) * v_Color; break;
		case 2: o_Color = texture(u_Textures[2], v_TexCoord) * v_Color; break;
		case 3: o_Color = texture(u_Textures[3], v_TexCoord) * v_Color; break;
		case 4: o_Color = texture(u_Textures[4], v_TexCoord) * v_Color; break;
		case 5: o_Color = texture(u_Textures[5], v_TexCoord) * v_Color; break;
		case 6: o_Color = texture(u_Textures[6], v_TexCoord) * v_Color; break;
		case 7: o_Color = texture(u_Textures[7], v_TexCoord) * v_Color; break;
		case 8: o_Color = texture(u_Textures[8], v_TexCoord) * v_Color; break;
		case 9: o_Color = texture(u_Textures[9], v_TexCoord) * v_Color; break;
		case 10: o_Color = texture(u_Textures[10], v_TexCoord) * v_Color; break;
		case 11: o_Color = texture(u_Textures[11], v_TexCoord) * v_Color; break;
		case 12: o_Color = texture(u_Textures[12], v_TexCoord) * v_Color; break;
		case 13: o_Color = texture(u_Textures[13], v_TexCoord) * v_Color; break;
		case 14: o_Color = texture(u_Textures[14], v_TexCoord) * v_Color; break;
		case 15: o_Color = texture(u_Textures[15], v_TexCoord) * v_Color; break;
		case 16: o_Color = texture(u_Textures[16], v_TexCoord) * v_Color; break;
		case 17: o_Color = texture(u_Textures[17], v_TexCoord) * v_Color; break;
		case 18: o_Color = texture(u_Textures[18], v_TexCoord) * v_Color; break;
		case 19: o_Color = texture(u_Textures[19], v_TexCoord) * v_Color; break;
		case 20: o_Color = texture(u_Textures[20], v_TexCoord) * v_Color; break;
		case 21: o_Color = texture(u_Textures[21], v_TexCoord) * v_Color; break;
		case 22: o_Color = texture(u_Textures[22], v_TexCoord) * v_Color; break;
		case 23: o_Color = texture(u_Textures[23], v_TexCoord) * v_Color; break;
		case 24: o_Color = texture(u_Textures[24], v_TexCoord) * v_Color; break;
		case 25: o_Color = texture(u_Textures[25], v_TexCoord) * v_Color; break;
		case 26: o_Color = texture(u_Textures[26], v_TexCoord) * v_Color; break;
		case 27: o_Color = texture(u_Textures[27], v_TexCoord) * v_Color; break;
		case 28: o_Color = texture(u_Textures[28], v_TexCoord) * v_Color; break;
		case 29: o_Color = texture(u_Textures[29], v_TexCoord) * v_Color; break;
		case 30: o_Color = texture(u_Textures[30], v_TexCoord) * v_Color; break;
		case 31: o_Color = texture(u_Textures[31], v_TexCoord) * v_Color; break;
	}
	//o_Color = texture(u_Textures[int(v_TexIndex)], v_TexCoord) * v_Color;
	//o_Color = vec4(1, 1, 1, 1);

}