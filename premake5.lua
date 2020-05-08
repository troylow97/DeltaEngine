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
    "%{prj.name}/Source/**.h",
    "%{prj.name}/Source/**.cpp"
  }
  
  includedirs
  {
    "%{prj.name}/Dep/spdlog/include",
    "%{prj.name}/Source",
  }
  
  filter "system:windows" 
    cppdialect "C++17"
    staticruntime "On"
    systemversion "latest"
    
    defines
    {
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
  