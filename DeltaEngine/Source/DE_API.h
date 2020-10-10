#pragma once
//#include <SFML/Window.hpp>
//Contains basic macros

#ifdef DE_PLATFORM_WINDOWS
#ifdef DE_BUILD_DLL
#define DE_API __declspec(dllexport)
#else
#define DE_API __declspec(dllimport)
#endif	
#else
#error DeltaEngine only supports Windows.

#endif

#define BIT(x) (1 << x)

//do not need as we are just directly using winmain not glfw
//#define DE_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }
//#define DE_BIND_EVENT_FN_PURE(fn) [=](auto&&... args) -> decltype(auto) { return fn(std::forward<decltype(args)>(args)...); }