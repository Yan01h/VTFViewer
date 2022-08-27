project "GLFW"
	kind "StaticLib"
	language "C"
	staticruntime "off"
	
	targetdir ("../bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}")
	objdir ("../bin/int/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}")
	
	files {
		"glfw/include/GLFW/glfw3.h",
		"glfw/include/GLFW/glfw3native.h",
		"glfw/src/internal.h",
		"glfw/src/platform.h",
		"glfw/src/mappings.h",
		"glfw/src/context.c",
		"glfw/src/init.c",
		"glfw/src/input.c",
		"glfw/src/monitor.c",
		"glfw/src/platform.c",
		"glfw/src/vulkan.c",
		"glfw/src/window.c",
		"glfw/src/egl_context.c",
		"glfw/src/osmesa_context.c",
		"glfw/src/null_platform.h",
		"glfw/src/null_joystick.h",
		"glfw/src/null_init.c",
		"glfw/src/null_monitor.c",
		"glfw/src/null_window.c",
		"glfw/src/null_joystick.c"
	}
	
	links {
		"opengl32.lib"
	}
	
	filter "system:windows"
		systemversion "latest"

		files {
			"glfw/src/win32_time.h",
			"glfw/src/win32_thread.h",
			"glfw/src/win32_module.c",
			"glfw/src/win32_time.c",
			"glfw/src/win32_thread.c",
			"glfw/src/win32_platform.h",
			"glfw/src/win32_joystick.h",
			"glfw/src/win32_init.c",
			"glfw/src/win32_joystick.c",
			"glfw/src/win32_monitor.c",
			"glfw/src/win32_window.c",
			"glfw/src/wgl_context.c"
		}

		defines { 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

		links { "Dwmapi.lib" }
		
	filter "configurations:Debug*"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release*"
		runtime "Release"
		optimize "on"
		
	filter "configurations:*32"
		architecture "x86"
		
	filter "configurations:*64"
		architecture "x86_64"

project "ImGui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
	
	targetdir ("../bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}")
	objdir ("../bin/int/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}")
	
	files {
		"imgui/include/ImGui/imgui.h",
		"imgui/src/imconfig.h",
		"imgui/src/imgui_internal.h",
		"imgui/src/imstb_rectpack.h",
		"imgui/src/imstb_textedit.h",
		"imgui/src/imstb_truetype.h",
		"imgui/src/imgui.cpp",
		"imgui/src/imgui_demo.cpp",
		"imgui/src/imgui_draw.cpp",
		"imgui/src/imgui_tables.cpp",
		"imgui/src/imgui_widgets.cpp"
	}

	includedirs {
		"glfw/include"
	}
	
	filter "system:windows"
		systemversion "latest"

		files {
			"imgui/include/ImGui/imgui_impl_glfw.h",
			"imgui/include/ImGui/imgui_impl_opengl2.h",
			"imgui/src/imgui_impl_glfw.cpp",
			"imgui/src/imgui_impl_opengl2.cpp"
		}

		links {
			"opengl32.lib"
		}
		
	filter "configurations:Debug*"
		runtime "Debug"
		symbols "on"
		
	filter "configurations:Release*"
		runtime "Release"
		optimize "on"
		
	filter "configurations:*32"
		architecture "x86"
		
	filter "configurations:*64"
		architecture "x86_64"