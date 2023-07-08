project "vtflib"
	targetname "vtflib"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "Off"
	location (build_path)

	targetdir (bin_path.."/lib")
	objdir (bin_path.."/lib/obj")

	includedirs {
		"include/vtflib"
	}

	files {
		"src/*.h",
		"src/*.cpp"
	}

	links {
	}

	filter "system:windows"
		systemversion "latest"
		characterset "MBCS"