#include "tilelayer.h"

GameEngineTest::Graphics::TileLayer::TileLayer(Shader* shader)
	:Layer(new BatchRenderer2D(), shader, Math::mat4::orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f))
{

}

GameEngineTest::Graphics::TileLayer::~TileLayer()
{
	//for (Renderable2D* renderable : m_Renderables)
	//{
	//	delete renderable;
	//}
}
