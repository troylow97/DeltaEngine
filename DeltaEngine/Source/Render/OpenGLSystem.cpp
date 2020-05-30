#include "DEpch.h"
#include "OpenGLSystem.h"
#include "Window.h"
#include "../Core/Log.h"
#include <imgui.h>
#include <examples/imgui_impl_win32.h>
#include <examples/imgui_impl_opengl3.h>

namespace DeltaEngine
{
	namespace RenderModule
	{
		OpenGLSystem::OpenGLSystem()
			: m_wglDC{}, m_windowDC{}
		{

		}

		OpenGLSystem::~OpenGLSystem()
		{

		}

		void OpenGLSystem::Init()
		{
			DeltaEngine_CORE_INFO("Initializing OpenGL...");
			InitializeRenderingEnvironment();

			glClearColor(0.2f, 0.2f, 0.2f, 1.0f);//RGBA

			if (glewInit() != GLEW_OK)
				DeltaEngine_CORE_ERROR("glewInit() failed!");

			DeltaEngine_CORE_INFO("Initializing OpenGL successful");


			// ----------------
			// ImGui setup
			// -----------------

			DeltaEngine_CORE_INFO("Initializing imgui...");
			// Application init: create a dear imgui context, setup some options, load fonts
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();
			io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
			// TODO: Set optional io.ConfigFlags values, e.g. 'io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard' to enable keyboard controls.
			// TODO: Fill optional fields of the io structure later.
			// TODO: Load TTF/OTF fonts if you don't want to use the default font.
			ImGuiStyle& style = ImGui::GetStyle();

			// Initialize helper Platform and Renderer bindings (here we are using imgui_impl_win32.cpp and imgui_impl_dx11.cpp)
			ImGui_ImplWin32_Init(mainHWND);
			ImGui_ImplOpenGL3_Init();

			// ----------------
			// ImGui setup end
			// -----------------
		}

		void OpenGLSystem::Update()
		{
			//update opengl
			glClear(GL_COLOR_BUFFER_BIT);

			RECT rect;
			GetClientRect(mainHWND, &rect);
			glViewport((GLint)0, (GLint)0, rect.right - rect.left, rect.bottom - rect.top);
		}

		void OpenGLSystem::TestRender(std::vector<SpriteRenderer*> sprites)
		{
			// ----------------
			// ImGui render
			// -----------------

			static bool my_tool_active = true;

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			// 1. Show a simple window.
			// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
			{
				ImGui::Begin("Sprite");
				static float f = 0.0f;
				ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
				ImGui::SliderFloat("rotate", &f, -180.0f, 180.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
				sprites[0]->transform.rotation = Quaternion::AngleAxis(f, Vector3::forward());
				ImGui::DragFloat3("pos", (float*)&sprites[0]->transform.position, 0.01f);
				ImGui::DragFloat3("scale", (float*)&sprites[0]->transform.scale, 0.01f);
				ImGui::ColorEdit3("clear color", (float*)&sprites[0]->color); // Edit 3 floats representing a color
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}

			ImGui::Render();
			Update();
			std::for_each(sprites.begin(), sprites.end(), [](SpriteRenderer* s) { s->Update(); });
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			SwapBuffers();
			// ----------------
			// ImGui render end
			// -----------------
		}

		void OpenGLSystem::Exit()
		{
			CleanRenderingEnvironment();
			DeltaEngine_CORE_INFO("OpenGL system exited");
		}

		bool OpenGLSystem::InitializeRenderingEnvironment()
		{
			//create rendering window
			m_windowDC = GetDC(mainHWND);

			DEVMODE devMode = { 0 };
			devMode.dmSize = sizeof(DEVMODE);
			BOOL b = EnumDisplaySettings(0, ENUM_CURRENT_SETTINGS, &devMode);
			if (b == 0)
				return false;

			//drawing surface format
			PIXELFORMATDESCRIPTOR pfdesc;
			memset(&pfdesc, 0, sizeof(PIXELFORMATDESCRIPTOR));

			pfdesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
			pfdesc.nVersion = 1;
			pfdesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER;
			pfdesc.iPixelType = PFD_TYPE_RGBA;
			pfdesc.cColorBits = (BYTE)devMode.dmBitsPerPel;//32; //24 bit color for front and back buffer
			pfdesc.cDepthBits = 24;//24 bit depth buffer - not used in this demo
			pfdesc.cStencilBits = 8; //8 bit stencil buffer - not used in this demo

			int pf = ChoosePixelFormat(m_windowDC, &pfdesc);//checks if the graphics card can support the pixel format requested
			if (pf == 0)
			{
				ReleaseDC(mainHWND, m_windowDC);
				return false;
			}


			BOOL ok = SetPixelFormat(m_windowDC, pf, &pfdesc);
			if (!ok)
			{
				ReleaseDC(mainHWND, m_windowDC);
				return false;
			}


			//set the OpenGL context
			m_wglDC = wglCreateContext(m_windowDC);
			if (!m_wglDC)
			{
				ReleaseDC(mainHWND, m_windowDC);
				return false;
			}


			ok = wglMakeCurrent(m_windowDC, m_wglDC);
			if (!ok)
			{
				wglDeleteContext(m_wglDC);
				ReleaseDC(mainHWND, m_windowDC);
				return false;
			}

			return true;
		}

		void OpenGLSystem::CleanRenderingEnvironment()
		{
			if (m_wglDC)
			{
				if (!wglMakeCurrent(NULL, NULL))
				{
					//log
				}
			}

			if (!wglDeleteContext(m_wglDC))
			{
				//log
			}
			m_wglDC = NULL;

			if (m_windowDC && !ReleaseDC(mainHWND, m_windowDC))
			{
				m_windowDC = NULL;
			}
		}

		void OpenGLSystem::SwapBuffers()
		{
			::SwapBuffers(m_windowDC); //using double buffering
		}
	}
}
