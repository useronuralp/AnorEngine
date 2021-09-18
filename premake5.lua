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
IncludeDir["GLEW"] = "AnorEngine/Vendor/GLEW/include"
IncludeDir["spdlog"] = "AnorEngine/Vendor/spdlog-1.x/include"
IncludeDir["GLFW"] = "AnorEngine/Vendor/GLFW/includex64"
IncludeDir["GLM"] = "AnorEngine/Vendor/GLM"
IncludeDir["ENTT"] = "AnorEngine/Vendor/Entt/include"
IncludeDir["IMGUIBUILDHELPERS"] = "AnorEngine/Vendor/ImGuiBuildHelpers"
IncludeDir["yaml"] = "AnorEngine/Vendor/yaml-cpp/include"
IncludeDir["assimp"] = "AnorEngine/Vendor/Assimp/assimp/include"

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
		"%{prj.name}/Source/**.h",
		"%{prj.name}/Source/**.cpp",
		"%{prj.name}/vendor/ImGuiBuildHelpers/**.h",
		"%{prj.name}/vendor/ImGuiBuildHelpers/**.cpp",
	}

	defines
	{
		"GLFW_INCLUDE_NONE",
		"GLEW_STATIC",
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
		"$(SolutionDir)%{IncludeDir.GLEW}",
		"$(SolutionDir)%{IncludeDir.spdlog}",
		"$(SolutionDir)%{IncludeDir.GLFW}",
		"$(SolutionDir)%{IncludeDir.GLM}",
		"$(SolutionDir)%{IncludeDir.ENTT}",
		"$(SolutionDir)%{IncludeDir.IMGUIBUILDHELPERS}",
		"$(SolutionDir)%{IncludeDir.yaml}",
		"$(SolutionDir)%{IncludeDir.assimp}"
	}

	libdirs 
	{ 
		"$(SolutionDir)AnorEngine/Vendor/FreeImage/lib",
		"$(SolutionDir)AnorEngine/Vendor/GLEW/lib/x64",
		"$(SolutionDir)AnorEngine/Vendor/GLFW/lib-vc2019x64",
		"$(SolutionDir)AnorEngine/Vendor/Assimp/assimp/lib/Release"
	}

	links 
	{
		"ImGui",
		"Yaml-cpp",
		"glfw3",
		"glew32s",
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
		"$(SolutionDir)%{IncludeDir.GLEW}",
		"$(SolutionDir)%{IncludeDir.spdlog}",
		"$(SolutionDir)%{IncludeDir.GLFW}",
		"$(SolutionDir)%{IncludeDir.GLM}",
		"$(SolutionDir)%{IncludeDir.ENTT}",
		"$(SolutionDir)%{IncludeDir.IMGUIBUILDHELPERS}",
		"$(SolutionDir)%{IncludeDir.yaml}",
		"$(SolutionDir)%{IncludeDir.assimp}"
	}


	defines
	{
		"GLFW_INCLUDE_NONE",
		"GLEW_STATIC",
		"WIN32",
		"ENGINE_PLATFORM_WINDOWS",
		"__SOLUTION_DIR=R\"($(SolutionDir))\"",
		"_CRT_SECURE_NO_WARNINGS",
		"_CONSOLE"
	}

	postbuildcommands
	{
		"XCOPY \"$(SolutionDir)/AnorEditor/imgui.ini\" \"%{cfg.targetdir}\" /y",
		"XCOPY \"$(SolutionDir)AnorEngine/Vendor/Assimp/assimp/bin/Release/assimp-vc142-mt.dll\" \"%{cfg.targetdir}\" /y"

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


project "Sandbox"
	location "Sandbox"
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
		"Sandbox/**.h",
		"Sandbox/**.cpp"
	}

	includedirs
	{
		"$(SolutionDir)AnorEngine/Source",
		"$(SolutionDir)%{IncludeDir.FreeImage}",
		"$(SolutionDir)%{IncludeDir.ImGui}",
		"$(SolutionDir)%{IncludeDir.GLEW}",
		"$(SolutionDir)%{IncludeDir.spdlog}",
		"$(SolutionDir)%{IncludeDir.GLFW}",
		"$(SolutionDir)%{IncludeDir.GLM}",
		"$(SolutionDir)%{IncludeDir.ENTT}",
		"$(SolutionDir)%{IncludeDir.IMGUIBUILDHELPERS}",
		"$(SolutionDir)%{IncludeDir.yaml}",
		"$(SolutionDir)%{IncludeDir.assimp}"
	}


	defines
	{
		"GLFW_INCLUDE_NONE",
		"GLEW_STATIC",
		"WIN32",
		"ENGINE_PLATFORM_WINDOWS",
		"__SOLUTION_DIR=R\"($(SolutionDir))\"",
		"_CRT_SECURE_NO_WARNINGS",
		"_CONSOLE"
	}

	filter "system:windows"
		systemversion "latest"
		
	filter "configurations:Debug"
		defines "_DEBUG"
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

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

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

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

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