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
  flags 
  {
    "MultiProcessorCompile",
    "NoPCH",
  }

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
    "%{prj.name}/Dep/rapidjson-master/include/*.cpp",
    "%{prj.name}/Dep/rapidjson-master/include*.h",
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
    "%{prj.name}/Dep/rapidjson-master/include",
    "%{prj.name}/Dep/fmod/core/inc",
    "%{prj.name}/Dep/fmod/studio/inc",
    "%{prj.name}/Source"
  }

  libdirs
  {
    "%{prj.name}/Dep/freetype/objs",
    "%{prj.name}/Dep/rttr/lib",
    "%{prj.name}/Dep/fmod/core/lib/x64",
    "%{prj.name}/Dep/fmod/studio/lib/x64"
  }

  links
  {
    "glew32s.lib",
    "freetype.lib",
    "OpenGL32.lib",
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
    }
  
  postbuildcommands
  {
    "{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox",   
  }

  filter "configurations:Debug"
    defines "DE_DEBUG"
    symbols "On"
    libdirs 
    {
      "%{prj.name}/Dep/glew/GL/lib/Debug/x64",
      "%{prj.name}/Dep/freetype/objs/x64/Debug Static"
    }
    links 
    {
      "rttr_core_lib_s_d.lib",
      "fmodL_vc.lib",
      "fmodL.dll"
    }

    
  filter "configurations:Release"
    defines "DE_RELEASE"
    optimize "On"
    libdirs 
    {
      "%{prj.name}/Dep/glew/GL/lib/Release/x64",
      "%{prj.name}/Dep/freetype/objs/x64/Release Static"
    }
    links 
    {
      "rttr_core_lib_s.lib",
      "fmod_vc.lib",
      "fmod.dll"
    }


  filter "configurations:Dist"
    defines "DE_DIST"
    optimize "On"
    
project "Sandbox"
  location "Sandbox"
  language "C++"
  kind "ConsoleApp"
  flags 
  {
    "MultiProcessorCompile",
    "NoPCH",
  }
  
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
    "DeltaEngine/Dep/rapidjson-master/include",
    "DeltaEngine/Dep/fmod/core/inc",
    "DeltaEngine/Dep/fmod/studio/inc",
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
  