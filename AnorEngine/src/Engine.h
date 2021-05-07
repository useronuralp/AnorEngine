#pragma once
#include "core.h"

#include <EngineKeyCodes.h>
#include <MouseButtonCodes.h>

//-----Engine modules---------------
#include "Graphics/shader.h"
/*#include <GL/glew.h>
#include "../utils/fileutils.h"
#include "../Math/maths.h"
#include <glm.hpp>
#include <gtc/type_ptr.hpp>*/
#include "Graphics/simpleRenderer2D.h"
/*#include "renderer2D.h"
#include "static_sprite.h"*/
#include "Graphics/texture.h"
/*#include "../vendor/stb_image/stb_image.h"
#include <GL/glew.h>*/
#include "Graphics/simplerenderer3D.h"
/*#include "renderable3D.h"
#include "shader.h"
#include "Camera.h"*/
#include "vendor/Models/cube.h"
#include "Graphics/model.h"
/*#include "../vendor/stb_image/stb_image.h"
#include "mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>*/

#include "utils/Logger.h"
/*#include "spdlog/logger.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"*/
#include "Events/EventHandler.h"
/*#include "Event.h"
#include <Application.h>
#include "Graphics/OpenGLWindow.h"
#include "ImGui/ImGuiLayer.h"*/

//Try to include this at the bottom. I don't know what kind of circular header inclusion fuckery goes on in the code base but I could not find the problem lol
#include "Graphics/Renderer/Renderer.h"
/*#include <glm.hpp>
#include <Graphics/Buffers/VertexArray.h>
#include "../Camera.h"
#include "../shader.h"*/
//--------------------Entry point--------------------
#include "EntryPoint.h"
/*#include "pch.h"
#include "Application.h"
#include "Graphics/Layers/LayerStack.h"*/
//--------------------Entry point--------------------
