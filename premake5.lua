workspace "DeltaEngine"
	architecture "x64"
  startproject "Sandbox"
  
  configurations
  {
    "Debug",
    "Release",
    "Dist"
  }

  flags 
  {
    "MultiProcessorCompile",
    "OmitDefaultLibrary"
  }

  linkoptions { "-IGNORE:4099", "-IGNORE:4098", "-IGNORE:4006" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
  
project "DeltaEngine"
  location "DeltaEngine"
  kind "StaticLib"
  language "C++"
  cppdialect "C++17"
  staticruntime "On"
  flags 
  {
    "NoPCH"
  }
  buildoptions
  {
    "/bigobj"
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
      "%{prj.name}/Dep/freetype/objs/x64/DebugStatic"
    }
    links 
    {
      "rttr_core_lib_s_d.lib",
      "fmodL_vc.lib",
      "fmodL.dll",
      "fmodstudioL.dll",
      "fmodstudioL_vc.lib"
    }
    postbuildcommands
    {
      "{COPY} Dep/fmod/core/lib/x64/fmodL.dll ../bin/" .. outputdir .. "/Sandbox",
      "{COPY} Dep/fmod/studio/lib/x64/fmodstudioL.dll ../bin/" .. outputdir .. "/Sandbox",
      "{COPY} Dep/freetype/objs/x64/DebugStatic/freetype.lib.pdb ../bin/" .. outputdir .. "/Sandbox",
      "{COPY} Dep/rttr/lib/rttr_core_lib_s.pdb ../bin/" .. outputdir .. "/Sandbox",
    }

    
  filter "configurations:Release"
    defines "DE_RELEASE"
    optimize "On"
    libdirs 
    {
      "%{prj.name}/Dep/glew/GL/lib/Release/x64",
      "%{prj.name}/Dep/freetype/objs/x64/ReleaseStatic"
    }
    links 
    {
      "rttr_core_lib_s.lib",
      "fmod_vc.lib",
      "fmod.dll",
      "fmodstudio.dll",
      "fmodstudio_vc.lib"
    }
    postbuildcommands
    {
      "{COPY} Dep/fmod/core/lib/x64/fmod.dll ../bin/" .. outputdir .. "/Sandbox",
      "{COPY} Dep/fmod/studio/lib/x64/fmodstudio.dll ../bin/" .. outputdir .. "/Sandbox"
      
    }


  filter "configurations:Dist"
    defines "DE_DIST"
    optimize "On"
    
project "Sandbox"
  location "Sandbox"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++17"
  staticruntime "On"
  flags 
  {
    "NoPCH"
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
  
  libdirs
  {
    "DeltaEngine/Dep/freetype/objs",
    "DeltaEngine/Dep/rttr/lib",
    "DeltaEngine/Dep/fmod/core/lib/x64",
    "DeltaEngine/Dep/fmod/studio/lib/x64"
  }
  
  links
  {
    "DeltaEngine"
  }
  
  filter "system:windows" 
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
  