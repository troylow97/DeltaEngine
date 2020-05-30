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
  
  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
  
  pchheader "DEpch.h"
  pchsource "DeltaEngine/Source/DEpch.cpp"
  
  files
  {
    "%{prj.name}/Dep/imgui/*.h",
    "%{prj.name}/Dep/imgui/*.cpp",
    "%{prj.name}/Dep/imgui/examples/imgui_impl_opengl3.h",
    "%{prj.name}/Dep/imgui/examples/imgui_impl_opengl3.cpp",
    "%{prj.name}/Dep/imgui/examples/imgui_impl_win32.h",
    "%{prj.name}/Dep/imgui/examples/imgui_impl_win32.cpp",
    "%{prj.name}/Dep/SOIL/**.h",
    "%{prj.name}/Dep/SOIL/**.c",
    "%{prj.name}/Source/**.h",
    "%{prj.name}/Source/**.cpp"
  }

  includedirs
  {
    "%{prj.name}/Dep/imgui",
    "%{prj.name}/Dep/SOIL",
    "%{prj.name}/Dep/glew",
    "%{prj.name}/Dep/spdlog/include",
    "%{prj.name}/Source"
  }

  libdirs
  {
    "%{prj.name}/Dep/SOIL",
    "%{prj.name}/Dep/glew/GL"
  }

  links
  {
    "SOIL.lib",
    "glew32s.lib",
    "OpenGL32.lib"
  }

  filter "system:windows" 
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"
    
    defines
    {
      "WIN32",
      "GLEW_STATIC",
      "DE_PLATFORM_WINDOWS",
      "DE_BUILD_DLL"
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
  }
  
  includedirs
  {
    "DeltaEngine/Dep/imgui",
    "DeltaEngine/Dep/glew",
    "DeltaEngine/Dep/spdlog/include",
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
    symbols "On"
    
  filter "configurations:Release"
    defines "DE_RELEASE"
    optimize "On"

  filter "configurations:Dist"
    defines "DE_DIST"
    optimize "On"
  