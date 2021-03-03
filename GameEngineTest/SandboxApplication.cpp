#include <Engine.h> 

using namespace GameEngineTest;
using namespace Math;
using namespace Graphics;


int main()
{		
	
	srand(time(NULL));

	Window window("Onuralp_Engine", 1920, 1080);
	
	Shader shader("src/shaders/vertex.shader", "src/shaders/fragment.shader");
	Shader shader2("src/shaders/vertex.shader", "src/shaders/lightsourceFrag.shader");
	Shader shader3("src/shaders/vertex.shader", "src/shaders/fragment.shader");

	//Texture texture("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\textures\\sample_gigatextures_4096_seamless2.png");
	//Texture texture2("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\textures\\tileable-grass_clover_tt7010116-sample_1.png");
	//Texture texture3("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\textures\\381f5a63791945.5abc4ccf1297d.png");
	//Texture* textures[] = {	&texture, &texture2, &texture3 };


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


	for (unsigned int i = 0; i < 900; i++)
	{
		shader.setUniform2f("offsets[" + std::to_string(i) + "]", translations[i]);
	}

	for (unsigned int i = 0; i < 900; i++)
	{
		shader2.setUniform2f("offsets[" + std::to_string(i) + "]", translations[i]);
	}


	delete[] translations;


	int layout[] = { UV_VALUE, POSITION_VALUE, NORMAL_VALUE};
	Renderable3D cube(Shape::vertices, 8 * 6 * 6, layout, 3, 8 * sizeof(float));
	Renderable3D cube2(Shape::vertices, 8 * 6 * 6, layout, 3, 8 * sizeof(float));
	int layout2[] = { POSITION_VALUE };
	Renderable3D light(Shape::lightSourceVertx, 3 * 6 * 6, layout2, 1, 3 * sizeof(float));


	std::vector<Renderable3D*> cubes;
	cubes.reserve(25);
	vec3 rotationDirection[25] = {};
	for (int i = 0; i < 25; i++)
	{
		cubes.push_back(new Renderable3D(Shape::vertices, 8 * 6 * 6, layout, 3, 8 * sizeof(float)));
		cubes[i]->translate(i, 0, 1 * (rand() % 100 < 50 ? -1 : 1));		
		rotationDirection[i] = vec3(rand() % 10 / 10.0f, rand() % 10 / 10.0f, rand() % 10 / 10.0f);
	}


	float lightX = 2, lightY = 3;
	light.translate(lightX, lightY, 0.0f);
	light.scale(0.3f, 0.3f, 0.3f);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	//texture3.Bind(0);

	//-----------shader settings------------
	shader.setUniform1i("tex", 0);
	//shader.setUniform3f("lightColor", vec3(0.505882f, 0.59609f, 0.70f)); //moonlight color
	shader.setUniform3f("lightColor", vec3(1.0f, 1.0f, 1.0f));
	shader.setUniform2f("light_pos", vec2(lightX, lightY));

	shader2.setUniform1i("tex", 0);
	shader2.setUniform4f("colour", vec4(1, 1, 1, 1)); //light box 
	//shader2.setUniform4f("colour", vec4(0.505882f, 0.59609f, 0.70f, 1)); //light box 
	shader2.setUniform2f("light_pos", vec2(lightX, lightY));


	shader3.setUniform1i("tex", 0);
	//shader.setUniform3f("lightColor", vec3(0.505882f, 0.59609f, 0.70f)); //moonlight color
	shader3.setUniform3f("lightColor", vec3(1.0f, 1.0f, 1.0f));
	shader3.setUniform2f("light_pos", vec2(lightX, lightY));

	glEnable(GL_DEPTH_TEST);

	glm::mat4 camera;
	Renderer3D renderer3D;

	Model backpack("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\Models\\backpack\\backpack.obj");
	Model basketball("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\Models\\ball\\uploads_files_2222080_ball_obj.obj"); 
	Model Arianna("H:\\ProgrammingProjects\\repos\\GameEngineTest\\GameEngineTest\\Models\\girl\\Girl_1.obj");
	
	basketball.scale(5, 5, 5);
	basketball.translate(0, 0.5f, 0);

	Arianna.scale(5, 5, 5);
	
	while (!window.closed()) {
		window.clear();
		camera = glm::lookAt(window.cameraPos, window.cameraPos + window.cameraFront, window.cameraUp); // camera movement operations on view matrix each frame.

		//backpack.Draw(shader3, camera);
		//basketball.Draw(shader3, camera);
		Arianna.Draw(shader3, camera);

		//int i = 0;
		//for (Renderable3D* item : cubes)
		//{
		//	renderer3D.singleDraw(*item, shader, camera, 36);
		//	item->rotate(0.0005f * 10.0f, rotationDirection[i].x, rotationDirection[i].y, rotationDirection[i].z);
		//	rotationDirection[i++];
		//}
		renderer3D.singleDraw(light, shader2, camera, 36);
		shader.setUniform2f("light_pos", vec2(lightX, lightY));
		window.update();
	}
	return 0;
}