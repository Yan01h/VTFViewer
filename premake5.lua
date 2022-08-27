workspace "VTFViewer"
	startproject "VTFViewer"
	
	configurations {
		"Debug32", "Debug64", "Release32", "Release64"
	}
	
	flags { "MultiProcessorCompile" }

include "dependencies/premake5.lua"
	
project "VTFViewer"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	targetdir ("bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}")
	objdir ("bin/int/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}")
	
	files {
		"src/**.h",
		"src/**.cpp"
	}

	includedirs {
		"dependencies/glfw/include",
		"dependencies/imgui/include"
	}
	
	links {
		"GLFW",
		"ImGui",
		"opengl32.lib"
	}
	
	filter "system:windows"
		systemversion "latest"
		defines { "_WINDOWS" }
	
	filter "system:linux"
		defines { "_LINUX" }
		
	filter "system:macosx"
		defines { "_MACOS" }
		
	filter "configurations:Debug*"
		kind "ConsoleApp"
		runtime "Debug"
		symbols "on"
		defines { "_DEBUG" }
		
	filter "configurations:Release*"
		kind "WindowedApp"
		runtime "Release"
		optimize "on"
		defines { "_RELEASE" }
		
	filter "configurations:*32"
		architecture "x86"
		
	filter "configurations:*64"
		architecture "x86_64"