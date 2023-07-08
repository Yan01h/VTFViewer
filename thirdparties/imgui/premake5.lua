project "imgui"
	targetname "imgui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "Off"
	location (build_path)

	targetdir (bin_path.."/lib")
	objdir (bin_path.."/lib/obj")

	includedirs {
		"include/imgui",
		"../glfw/include"
	}

	files {
		"src/**.h",
		"src/**.cpp"
	}

	links {
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"
		defines {
			"_CRT_SECURE_NO_WARNINGS"
		}