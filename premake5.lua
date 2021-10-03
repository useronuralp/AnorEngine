workspace "AnorEngine"
	architecture "x86_64"
	startproject "AnorEditor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["FreeImage"] = "AnorEngine/Vendor/FreeImage/include"
IncludeDir["ImGui"] = "AnorEngine/Vendor/ImGui"
IncludeDir["GLAD"] = "AnorEngine/Vendor/GLAD/include"
IncludeDir["spdlog"] = "AnorEngine/Vendor/spdlog-1.x/include"
IncludeDir["GLFW"] = "AnorEngine/Vendor/GLFW/includex64"
IncludeDir["GLM"] = "AnorEngine/Vendor/GLM"
IncludeDir["ENTT"] = "AnorEngine/Vendor/Entt/include"
IncludeDir["IMGUIBUILDHELPERS"] = "AnorEngine/Vendor/ImGuiBuildHelpers"
IncludeDir["yaml"] = "AnorEngine/Vendor/yaml-cpp/include"
IncludeDir["assimp"] = "AnorEngine/Vendor/Assimp/assimp/include"
IncludeDir["stb_image"] = "AnorEngine/Vendor/stb_image/include"

project "AnorEngine"
	location "AnorEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("$(SolutionDir)bin/" .. outputdir .. "/%{prj.name}")
	objdir ("$(SolutionDir)bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "AnorEngine/Source/pch.cpp"

	files
	{
		"AnorEngine/Source/**.h",
		"AnorEngine/Source/**.cpp",
		"AnorEngine/Assets/Shaders/**.shader",
		"AnorEngine/Vendor/stb_image/**.h",
		"AnorEngine/Vendor/stb_image/**.cpp",
		"AnorEngine/Vendor/ImGuiBuildHelpers/**.h",
		"AnorEngine/Vendor/ImGuiBuildHelpers/**.cpp",
		"AnorEngine/Vendor/Assimp/assimp/include/**.h",
		"AnorEngine/Vendor/Assimp/assimp/include/**.hpp",
		"AnorEngine/Vendor/Assimp/assimp/include/**.inl",
		"AnorEngine/Vendor/Assimp/assimp/include/**.in"
	}

	defines
	{
		"GLFW_INCLUDE_NONE",
		"WIN32",
		"ENGINE_PLATFORM_WINDOWS",
		"ENGINE_BUILD_DLL",
		"__SOLUTION_DIR=R\"($(SolutionDir))\"",
		"_CRT_SECURE_NO_WARNINGS",
		"_CONSOLE"
	}

	buildoptions 
	{
		"/Zm1000"
	}

	includedirs 
	{		
		"$(SolutionDir)AnorEngine/Source",
		"$(SolutionDir)%{IncludeDir.FreeImage}",
		"$(SolutionDir)%{IncludeDir.ImGui}",
		"$(SolutionDir)%{IncludeDir.GLAD}",
		"$(SolutionDir)%{IncludeDir.spdlog}",
		"$(SolutionDir)%{IncludeDir.GLFW}",
		"$(SolutionDir)%{IncludeDir.GLM}",
		"$(SolutionDir)%{IncludeDir.ENTT}",
		"$(SolutionDir)%{IncludeDir.IMGUIBUILDHELPERS}",
		"$(SolutionDir)%{IncludeDir.yaml}",
		"$(SolutionDir)%{IncludeDir.assimp}",
		"$(SolutionDir)%{IncludeDir.stb_image}"
	}

	libdirs 
	{ 
		"$(SolutionDir)AnorEngine/Vendor/FreeImage/lib",
		"$(SolutionDir)AnorEngine/Vendor/GLFW/lib-vc2019x64",
		"$(SolutionDir)AnorEngine/Vendor/Assimp/assimp/lib/Release"
	}

	links 
	{
		"ImGui",
		"Yaml-cpp",
		"GLAD",
		"glfw3",
		"assimp-vc142-mt",
		"opengl32",
		"FreeImage"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines	"_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines	"NDEBUG"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "NDEBUG"
		runtime "Release"
		optimize "on"

project "AnorEditor"
	location "AnorEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("$(SolutionDir)bin/" .. outputdir .. "/%{prj.name}")
	objdir ("$(SolutionDir)bin-int/" .. outputdir .. "/%{prj.name}")

	links
	{
		"AnorEngine"
	}

	files
	{
		"AnorEditor/**.h",
		"AnorEditor/**.cpp"
	}

	includedirs
	{
		"$(SolutionDir)AnorEditor/Source",
		"$(SolutionDir)AnorEngine/Source",
		"$(SolutionDir)%{IncludeDir.FreeImage}",
		"$(SolutionDir)%{IncludeDir.ImGui}",
		"$(SolutionDir)%{IncludeDir.GLAD}",
		"$(SolutionDir)%{IncludeDir.spdlog}",
		"$(SolutionDir)%{IncludeDir.GLFW}",
		"$(SolutionDir)%{IncludeDir.GLM}",
		"$(SolutionDir)%{IncludeDir.ENTT}",
		"$(SolutionDir)%{IncludeDir.IMGUIBUILDHELPERS}",
		"$(SolutionDir)%{IncludeDir.yaml}",
		"$(SolutionDir)%{IncludeDir.assimp}",
		"$(SolutionDir)%{IncludeDir.stb_image}"
	}


	defines
	{
		"GLFW_INCLUDE_NONE",
		"WIN32",
		"ENGINE_PLATFORM_WINDOWS",
		"__SOLUTION_DIR=R\"($(SolutionDir))\"",
		"_CRT_SECURE_NO_WARNINGS",
		"_CONSOLE"
	}

	postbuildcommands
	{
		"XCOPY \"$(SolutionDir)AnorEditor\\imgui.ini\" \"%{cfg.targetdir}\" /y",
		"XCOPY \"$(SolutionDir)AnorEngine\\Vendor\\Assimp\\assimp\\bin\\Release\\assimp-vc142-mt.dll\" \"%{cfg.targetdir}\" /y"

	}

	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines	"_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "NDEBUG"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines	"NDEBUG"
		runtime "Release"
		optimize "on"


project "ImGui"
	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("$(SolutionDir)bin/" .. outputdir .. "/%{prj.name}")
	objdir ("$(SolutionDir)bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"AnorEngine/Vendor/ImGui/*.cpp",
		"AnorEngine/Vendor/ImGui/*.h"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"


project "Yaml-cpp"
	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("$(SolutionDir)bin/" .. outputdir .. "/%{prj.name}")
	objdir ("$(SolutionDir)bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"AnorEngine/Vendor/yaml-cpp/src/**.cpp",
		"AnorEngine/Vendor/yaml-cpp/src/**.h",
		"AnorEngine/Vendor/yaml-cpp/include/**.h"
	}

	includedirs
	{
		"AnorEngine/Vendor/yaml-cpp/include"
	}
	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

project "GLAD"
	kind "StaticLib"
	language "C"
	staticruntime "off"

	targetdir ("$(SolutionDir)bin/" .. outputdir .. "/%{prj.name}")
	objdir ("$(SolutionDir)bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"AnorEngine/Vendor/GLAD/include/glad/glad.h",
		"AnorEngine/Vendor/GLAD/include/KHR/khrplaform.h",
		"AnorEngine/Vendor/GLAD/src/glad.c"
	}

	includedirs
	{
		"AnorEngine/Vendor/GLAD/include"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
