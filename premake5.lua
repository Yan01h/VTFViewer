root_path = tostring(os.getcwd())
build_path = root_path.."/build"
bin_path = root_path.."/bin/%{cfg.system}-%{cfg.buildcfg}%{cfg.platform}"

workspace "vtf_viewer"
	startproject "vtf_viewer"
	location (build_path)

	configurations {
		"Debug",
		"Release"
	}

	flags {
		"MultiProcessorCompile"
	}

	filter "system:windows"
		systemversion "latest"
		platforms {
			"x86",
			"x64"
		}
		defines {
			"VTFV_WINDOWS"
		}

	filter "system:macosx"
		defines {
			"VTFV_MACOS"
		}

	filter "system:linux"
		defines {
			"VTFV_LINUX"
		}

	filter "platforms:x86"
		architecture "x86"

	filter "platforms:x64"
		architecture "x86_64"

	filter "configurations:Debug"
		symbols "On"
		optimize "Off"
		runtime "Debug"
		defines {
			"VTFV_DEBUG"
		}

	filter "configurations:Release"
		symbols "Off"
		optimize "Size"
		runtime "Release"
		defines {
			"VTFV_RELEASE"
		}

	include "thirdparties/glfw"
	include "thirdparties/imgui"
	include "thirdparties/vtflib"

	project "vtf_viewer"
		targetname "VTFViewer"
		language "C++"
		cppdialect "C++17"
		location (build_path)

		targetdir (bin_path)
		objdir (bin_path.."/obj")

		includedirs {
			"src",
			"thirdparties/glfw/include",
			"thirdparties/imgui/include",
			"thirdparties/vtflib/include"
		}

		files {
			"src/core/*.h",
			"src/core/*.cpp",
			"src/debug/*.h",
			"src/debug/*.cpp",
			"src/fonts/*.h",
			"src/fonts/*.cpp"
		}

		links {
			"glfw",
			"imgui",
			"vtflib",
			"opengl32.lib"
		}

		filter "system:windows"
			files {
				"src/win32/*.h",
				"src/win32/*.cpp",
				"src/win32/*.rc",
				"src/win32/*.ico",
			}
			defines {
				"NOMINMAX"
			}

		filter "configurations:Debug"
			kind "ConsoleApp"
			debugdir (root_path)
			
		filter "configurations:Release"
			kind "WindowedApp"