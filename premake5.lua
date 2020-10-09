workspace "DeltaEngine"
	architecture "x64"
  startproject "Sandbox"
  
  configurations
  {
    "Debug",
    "Release",
    "Dist"
  }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
  
project "DeltaEngine"
  location "DeltaEngine"
  kind "SharedLib"
  language "C++"
  buildoptions "/NODEFAULTLIB:library"

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
  
  files
  {
    "%{prj.name}/Dep/imgui/*.h",
    "%{prj.name}/Dep/imgui/*.cpp",
    "%{prj.name}/Dep/imgui/examples/imgui_impl_opengl3.h",
    "%{prj.name}/Dep/imgui/examples/imgui_impl_opengl3.cpp",
    "%{prj.name}/Dep/imgui/examples/imgui_impl_win32.h",
    "%{prj.name}/Dep/imgui/examples/imgui_impl_win32.cpp",
    "%{prj.name}/Dep/freetype/include/ft2build.h",
    "%{prj.name}/Dep/stb_image/stb_image.h",
    "%{prj.name}/Dep/stb_image/stb_image.cpp",
    "%{prj.name}/Dep/rttr/include/*.h",
    "%{prj.name}/Dep/rttr/include/*.cpp",
    "%{prj.name}/Dep/rapidjson/*.cpp",
    "%{prj.name}/Dep/rapidjson/*.h",
    "%{prj.name}/Source/**.h",
    "%{prj.name}/Source/**.cpp",
    "%{prj.name}/Source/**.inl"
  }

  includedirs
  {
    "%{prj.name}/Dep/imgui",
    "%{prj.name}/Dep/glew",
    "%{prj.name}/Dep/spdlog/include",
    "%{prj.name}/Dep/freetype/include",
    "%{prj.name}/Dep/stb_image",
    "%{prj.name}/Dep/rttr/include",
    "%{prj.name}/Dep/rapidjson",
    "%{prj.name}/Source"
  }

  libdirs
  {
    "%{prj.name}/Dep/glew/GL",
    "%{prj.name}/Dep/freetype/objs",
    "%{prj.name}/Dep/rttr/lib",
  }

  links
  {
    "glew32s.lib",
    "OpenGL32.lib",
    "freetype.lib",
    "rttr_core_lib_s_d.lib"
  }

  filter "system:windows" 
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"
    warnings "Extra"
    
    defines
    {
      "WIN32",
      "GLEW_STATIC",
      "DE_PLATFORM_WINDOWS",
      "DE_BUILD_DLL",
      "FT2_BUILD_LIBRARY"
    }
  
  postbuildcommands
  {
   ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
  }

  filter "configurations:Debug"
    defines "DE_DEBUG"
    symbols "On"
    
  filter "configurations:Release"
    defines "DE_RELEASE"
    optimize "On"

  filter "configurations:Dist"
    defines "DE_DIST"
    optimize "On"
    
project "Sandbox"
  location "Sandbox"
  language "C++"
  kind "ConsoleApp"
  
  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
  
  files
  {
    "%{prj.name}/Source/**.h",
    "%{prj.name}/Source/**.cpp",
    "%{prj.name}/Source/**.inl"
  }
  
  includedirs
  {
    "DeltaEngine/Dep/imgui",
    "DeltaEngine/Dep/glew",
    "DeltaEngine/Dep/spdlog/include",
    "DeltaEngine/Dep/freetype/include",
    "DeltaEngine/Dep/stb_image",
    "DeltaEngine/Dep/rttr/include",
    "DeltaEngine/Dep/rapidjson",
    "DeltaEngine/Source"
  }
  
  links
  {
    "DeltaEngine"
  }
  
  filter "system:windows" 
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"
    
    defines
    {
      "DE_PLATFORM_WINDOWS"
    }
  
  filter "configurations:Debug"
    defines "DE_DEBUG"
    defines "DEBUG"
    symbols "On"
    
  filter "configurations:Release"
    defines "DE_RELEASE"
    optimize "On"

  filter "configurations:Dist"
    defines "DE_DIST"
    optimize "On"
  