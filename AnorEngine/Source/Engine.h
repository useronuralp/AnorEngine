#pragma once
#include "core.h"

#include <EngineKeyCodes.h>
#include <MouseButtonCodes.h>

//-----Engine modules---------------
#include "Graphics/shader.h"
#include "Graphics/simpleRenderer2D.h"
#include "Graphics/texture.h"
#include "Graphics/simplerenderer3D.h"
#include <../Assets/Models/cube.h>
#include "Graphics/model.h"
#include <imgui.h>
#include "utils/Logger.h"
#include "Events/EventHandler.h"
#include "Graphics/OrthographicCameraController.h"

//Try to include this at the bottom. I don't know what kind of circular header inclusion fuckery goes on in the code base but I could not find the problem lol
#include "Renderer/Renderer.h"

//--------------------Entry point--------------------
#include "EntryPoint.h"

//--------------------Entry point--------------------
