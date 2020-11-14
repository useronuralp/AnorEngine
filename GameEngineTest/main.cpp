#include "src/Graphics/window.h"
#include "src/Math/maths.h"
#include "src/Graphics/shader.h"
#include "src/Graphics/Buffers/buffer.h"
#include "src/Graphics/Buffers/indexbuffer.h"
#include "src/Graphics/Buffers/vertexarray.h"
#include "src/Graphics/renderer2D.h"
#include "src/Graphics/renderable2D.h"
#include "src/Graphics/simpleRenderer2D.h"
#include "src/Graphics/static_sprite.h"
#include "src/Graphics/batchrenderer2D.h"
#include "src/Graphics/sprite.h"
#include <time.h>
#include "src/Graphics/Layers/tilelayer.h"
#include "src/Graphics/texture.h"
#include <gtc/matrix_transform.hpp>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>


using namespace GameEngineTest;
using namespace Math;
using namespace Graphics;


int main()
{


	std::vector<StaticSprite*> sprites;
	srand(time(NULL));

	Window window("Onuralp", 1920, 1080);
	

	GLint texIDs[] =
	{
		0,1,2,3,4,5,6,7,8,9  // You can't set these without enabling the shader.
	};


	Shader* shader = new Shader("src/shaders/vertex.shader", "src/shaders/fragment.shader");
	Shader* shader2 = new Shader("src/shaders/vertex.shader", "src/shaders/fragment.shader");



	Shader shader3("src/shaders/vertex.shader", "src/shaders/fragment.shader");
	Shader shader4("src/shaders/vertex.shader", "src/shaders/lightsourceFrag.shader");

	shader->enable();
	shader->setUniform2f("light_pos", vec2(4.0f, 1.5f));
	shader->setUniform1iv("u_Textures", texIDs, 10);
	shader->disable();
	shader2->enable();
	shader2->setUniform1iv("u_Textures", texIDs, 10);
	shader2->disable();

	shader3.enable();
	shader3.setUniform1i("tex", 0);
	shader3.setUniform3f("lightColor", vec3(1.0f, 1.0f, 1.0f));
	shader3.setUniform3f("objectColor", vec3(1.0f, 0.5f, 0.31f));		//this is object
	shader3.setUniform4f("colour", vec4(0.8f, 0.2f, 1,1));
	shader3.disable();

	shader4.enable();
	shader4.setUniform1i("tex", 0);
	shader4.setUniform2f("light_pos", vec2(1, 1));		//this is light
	shader4.setUniform4f("colour", vec4(1, 1, 1, 1));
	shader4.disable();

	Texture texture("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\textures\\sample_gigatextures_4096_seamless2.png");
	Texture texture2("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\textures\\tileable-grass_clover_tt7010116-sample_1.png");
	Texture texture3("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\textures\\381f5a63791945.5abc4ccf1297d.png");
	Texture* textures[] = {	&texture, &texture2, &texture3 };

	shader->enable();
	TileLayer ground(shader);
	shader->disable();
	shader2->enable();
	TileLayer player(shader2);
	shader2->disable();
	for (float i = -16.0f; i < 16.0f; i++)
	{
		//layer.add(new Sprite(i, j, 0.9f, 0.9f, Math::vec4(rand() % 100 / 100.0f, 0.2f, 0.6f, 1))); // since you're creating these Sprites using the 'new' keyword, you need to pass them as pointers. New keyword returns a void* pointer.
		ground.add(new Sprite(i, -9, 0.9f, 0.9f, &texture2));
	}

	player.add(new Sprite(0, 0, 0.9f, 0.9f, &texture3));




	double x, y;
	float gravity = 0;
	for (float i = -16.0f; i < 16.0f; i++)
	{
		for (float j = -9.0f; j < 9.0f; j++)
		{
			sprites.push_back(new StaticSprite(i, j, 0.9f, 0.9f, vec4(1, 1, 1, 1), *shader, texture));
		}
	}


	float lightSourceVertx[] =
	{
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,

		-0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		 0.5f,  0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f, -0.5f,
	};

	float vertices[] = {
		//position						UV						Normals
	-0.5f, -0.5f, -0.5f,			 0.0f, 0.0f,			0.0f, 0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,			 1.0f, 0.0f,			0.0f, 0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,			 1.0f, 1.0f,			0.0f, 0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,			 1.0f, 1.0f,			0.0f, 0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,			 0.0f, 1.0f,			0.0f, 0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,			 0.0f, 0.0f,			0.0f, 0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,			 0.0f, 0.0f,			0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,			 1.0f, 0.0f,			0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,			 1.0f, 1.0f,			0.0f, 0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,			 1.0f, 1.0f,			0.0f, 0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,			 0.0f, 1.0f,			0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,			 0.0f, 0.0f,			0.0f, 0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,			 1.0f, 0.0f,		   -1.0f, 0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,			 1.0f, 1.0f,		   -1.0f, 0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,			 0.0f, 1.0f,		   -1.0f, 0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,			 0.0f, 1.0f,		   -1.0f, 0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,			 0.0f, 0.0f,		   -1.0f, 0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,			 1.0f, 0.0f,		   -1.0f, 0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,			 1.0f, 0.0f,			1.0f, 0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,			 1.0f, 1.0f,			1.0f, 0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,			 0.0f, 1.0f,			1.0f, 0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,			 0.0f, 1.0f,			1.0f, 0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,			 0.0f, 0.0f,			1.0f, 0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,			 1.0f, 0.0f,			1.0f, 0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,			 0.0f, 1.0f,			0.0f,-1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,			 1.0f, 1.0f,			0.0f,-1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,			 1.0f, 0.0f,			0.0f,-1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,			 1.0f, 0.0f,			0.0f,-1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,			 0.0f, 0.0f,			0.0f,-1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,			 0.0f, 1.0f,			0.0f,-1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,			 0.0f, 1.0f,			0.0f, 1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,			 1.0f, 1.0f,			0.0f, 1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,			 1.0f, 0.0f,			0.0f, 1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,			 1.0f, 0.0f,			0.0f, 1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,			 0.0f, 0.0f,			0.0f, 1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,			 0.0f, 1.0f,			0.0f, 1.0f,  0.0f
	};





	vec2 *translations = new vec2[900];
	int index = 0;
	float offset = 0.00;
	vec2 translation;
	for (int i = -15; i < 15; i++)
	{
		for (int j = -15; j < 15; j++)
		{
			
			translation.x = (float)i * (1  + offset);
			translation.y = (float)j * (1  + offset);
			translations[index++] = translation;

		}

	}
	//delete[] translations;
	shader3.enable();
	for (unsigned int i = 0; i < 900; i++)
	{
		shader3.setUniform2f("offsets[" + std::to_string(i) + "]", translations[i]);
	}
	shader3.disable();



	GLuint VAO_LS;
	GLuint VBO_LS;
	glGenBuffers(1, &VBO_LS);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_LS);
	glBufferData(GL_ARRAY_BUFFER, 3 * 6 * 6 * sizeof(GLfloat), lightSourceVertx, GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO_LS);
	glBindVertexArray(VAO_LS);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint VBO;													//ABSTRACT HERE IN MAIN
	GLuint VAO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * 6 * 6 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);//pos
	glEnableVertexAttribArray(1);//uv coord 
	glEnableVertexAttribArray(4);//normal
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 model2 = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 view = glm::mat4(1.0f);


	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f, 100.0f);



	int modelLocation1 = glGetUniformLocation(shader3.getShaderID(), "ml_matrix");
	glUniformMatrix4fv(modelLocation1, 1, GL_FALSE, glm::value_ptr(model));
	
	int viewLocation1 = glGetUniformLocation(shader3.getShaderID(), "vw_matrix");
	glUniformMatrix4fv(viewLocation1, 1, GL_FALSE, glm::value_ptr(view));
	
	int projectionlLocation1 = glGetUniformLocation(shader3.getShaderID(), "pr_matrix");
	glUniformMatrix4fv(projectionlLocation1, 1, GL_FALSE, glm::value_ptr(projection));

	float a = 2, b = 1.5f;

	glm::vec3 LightPosCoor = glm::vec3(a,b, 0.0f);
	model2 = glm::translate(model2, LightPosCoor);
	float lightX, lightY;
	lightX = LightPosCoor.x;
	lightY = LightPosCoor.y;
	model2 = glm::scale(model2, glm::vec3(0.3f, 0.3f, 0.3f));


	int modelLocation2 = glGetUniformLocation(shader4.getShaderID(), "ml_matrix");
	glUniformMatrix4fv(modelLocation2, 1, GL_FALSE, glm::value_ptr(model2));
	
	int viewLocation2= glGetUniformLocation(shader4.getShaderID(), "vw_matrix");
	glUniformMatrix4fv(viewLocation2, 1, GL_FALSE, glm::value_ptr(view));
	
	int projectionLocation2 = glGetUniformLocation(shader4.getShaderID(), "pr_matrix");
	glUniformMatrix4fv(projectionLocation2, 1, GL_FALSE, glm::value_ptr(projection));

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	texture3.Bind(0);
	glEnable(GL_DEPTH_TEST);
	while (!window.closed()) {

		window.clear();
		view = glm::lookAt(window.cameraPos, window.cameraPos + window.cameraFront, window.cameraUp);// camera movement operations on view matrix each frame.


		shader3.enable();

		glBindVertexArray(VAO);
		//model = glm::rotate(model, 0.005f * glm::radians(50.0f), glm::vec3(0.0, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLocation1, 1, GL_FALSE, glm::value_ptr(model));
		shader3.setUniform3f("cameraPos", vec3(0.3f, 0.3f, 0.3f));
		shader3.setUniform2f("light_pos", vec2(lightX, lightY));
		glUniformMatrix4fv(viewLocation1, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionlLocation1, 1, GL_FALSE, glm::value_ptr(projection));
		glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 900);
		glBindVertexArray(0);
		shader3.disable();
		
		shader4.enable();
		glBindVertexArray(VAO_LS);
		glUniformMatrix4fv(modelLocation2, 1, GL_FALSE, glm::value_ptr(model2));
		shader4.setUniform2f("light_pos", vec2(lightX, lightY));
		glUniformMatrix4fv(viewLocation2, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLocation2, 1, GL_FALSE, glm::value_ptr(projection));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		shader4.disable();

	
		//batch.begin();
		//for(int i= 0; i < sprites.size(); i++)
		//{
		//	batch.submit(sprites[i]);
		//}											 //batch rendering example

		//batch.end();
		//batch.flush();
		//texture.Unbind();

		window.update();
	}
	return 0;
}