#include "DEpch.h"
#include "OpenGLSystem.h"
#include "Window.h"
#include "Camera.h"
#include "Mesh.h"
#include "Font.h"
#include "Core/Debugging/Gizmos.h"
#include "Core/Debugging/Logger/Log.h"
#include "Core/GlobalStruct.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include <imgui.h>
#include <examples/imgui_impl_win32.h>
#include <examples/imgui_impl_opengl3.h>

namespace DeltaEngine
{
	namespace RenderModule
	{
		OpenGLSystem* openGLSystem;
		std::vector<Renderer*> allRenderers;

		OpenGLSystem::OpenGLSystem()
			: m_wglDC{}, m_windowDC{}
		{

		}

		OpenGLSystem::~OpenGLSystem()
		{
			DeltaEngine_CORE_INFO("OpenGL system deleted");
		}

		void OpenGLSystem::Init()
		{
			DeltaEngine_CORE_INFO("Initializing OpenGL...");
			//ImGui_ImplWin32_EnableDpiAwareness();
			InitializeRenderingEnvironment();

			glClearColor(0, 0, 0, 1);//RGBA

			if (glewInit() != GLEW_OK)
				DeltaEngine_CORE_ERROR("glewInit() failed!");
			else
			{
				DeltaEngine_CORE_INFO("Initializing OpenGL successful");
				DeltaEngine_CORE_INFO("OpenGL version: {0}", glGetString(GL_VERSION));
			}

			// Initialize common meshes
			Mesh::Init();
			Font::Init();
			Gizmos::Init();

			Camera* editorCam = new Camera(true);
			editorCam->backgroundColor = Color(71 / 255.0f, 71 / 255.0f, 71 / 255.0f, 1);
		}

		void OpenGLSystem::Update()
		{

			//ImGui_ImplOpenGL3_NewFrame();
			//ImGui_ImplWin32_NewFrame();
			//ImGui::NewFrame();
			//{
			//	ImGui::Begin("Camera Props");
			//	static float f = 0.0f;
			//	ImGui::Text("Edit Camera Props");                           // Display some text (you can use a format string too)
			//	ImGui::DragFloat3("pos", (float*)&Camera::editorCamera->transform.position, 0.01f);
			//	ImGui::DragFloat("size", (float*)&Camera::editorCamera->_size, 0.01f);
			//	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			//	ImGui::End();
			//}
			//ImGui::ShowDemoWindow();
			//ImGui::Render();
			//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			//if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			//{
			//	HDC backup_current_context = m_windowDC;
			//	ImGui::UpdatePlatformWindows();
			//	ImGui::RenderPlatformWindowsDefault();
			//	wglMakeCurrent(backup_current_context, m_wglDC);
			//}


			//update opengl
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			RECT rect;
			GetClientRect(env.pWin->GetHandle(), &rect);
			glViewport((GLint)0, (GLint)0, rect.right - rect.left, rect.bottom - rect.top);

			Camera::editorCamera->Render();

			::SwapBuffers(m_windowDC); //using double buffering
		}
		void OpenGLSystem::TestRender()
		{
			// ----------------
			// ImGui render
			// -----------------

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			//// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
			//{
			//	ImGui::Begin("Sprite");
			//	static float f = 0.0f;
			//	ImGui::Text("Edit Sprite Props");                           // Display some text (you can use a format string too)
			//	ImGui::SliderFloat("rotate", &f, -180.0f, 180.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			//	ps[0]->transform.rotation = Quaternion::AngleAxis(f, Vector3::forward());
			//	ImGui::DragFloat3("pos", (float*)&ps[0]->transform.position, 0.01f);
			//	ImGui::DragFloat3("scale", (float*)&ps[0]->transform.scale, 0.01f);
			//	ImGui::ColorEdit3("clear color", (float*)&sprites[0]->color); // Edit 3 floats representing a color
			//	ImGui::Text("Active particles: %u", ps[0]->GetActiveParticleCount());
			//	ImGui::End();
			//}

			{
				ImGui::Begin("Camera");
				static float f = 0.0f;
				ImGui::Text("Edit Camera Props");                           // Display some text (you can use a format string too)
				ImGui::DragFloat3("pos", (float*)&Camera::editorCamera->transform.position, 0.01f);
				ImGui::DragFloat("size", (float*)&Camera::editorCamera->_size, 0.01f);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}
			//ImGui::ShowDemoWindow();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glViewport(0, 0, (int)ImGui::GetIO().DisplaySize.x, (int)ImGui::GetIO().DisplaySize.y);
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			Update();
			//frameBuffer->Resize(width, height);
			//frameBuffer->Bind();
			//glClearColor(1.0f, 0.1f, 0.1f, 1.0f);
			//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			//Gizmos::DrawWorldGrid();
			//Gizmos::Draw2DWireBox();
			//Gizmos::Draw2DWireCircle();
			//std::for_each(sprites.begin(), sprites.end(), [](SpriteRenderer* s) { s->Render(*Camera::editorCamera); });
			//std::for_each(ps.begin(), ps.end(), [](ParticleSystem* p) { p->Update(); p->Render(*Camera::editorCamera); });
			//text->Render(*Camera::editorCamera);
			//frameBuffer->Unbind();

			Camera::editorCamera->Render();

			// Update and Render additional Platform Windows
			// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
			//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
			if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				HDC backup_current_context = m_windowDC;
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				wglMakeCurrent(backup_current_context, m_wglDC);
			}
			// ----------------
			// ImGui render end
			// -----------------

			::SwapBuffers(m_windowDC); //using double buffering
		}

		void OpenGLSystem::Exit()
		{
			Gizmos::Exit();
			Font::Exit();
			Mesh::Exit();
			CleanRenderingEnvironment();
			DeltaEngine_CORE_INFO("OpenGL system exited");
		}
		HDC OpenGLSystem::GetWindowContext()
		{
			return m_windowDC;
		}
		HGLRC OpenGLSystem::GetGLContext()
		{
			return m_wglDC;
		}

		bool OpenGLSystem::InitializeRenderingEnvironment()
		{
			//create rendering window
			m_windowDC = GetDC(env.pWin->GetHandle());

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
				ReleaseDC(env.pWin->GetHandle(), m_windowDC);
				return false;
			}


			BOOL ok = SetPixelFormat(m_windowDC, pf, &pfdesc);
			if (!ok)
			{
				ReleaseDC(env.pWin->GetHandle(), m_windowDC);
				return false;
			}


			//set the OpenGL context
			m_wglDC = wglCreateContext(m_windowDC);
			if (!m_wglDC)
			{
				ReleaseDC(env.pWin->GetHandle(), m_windowDC);
				return false;
			}


			ok = wglMakeCurrent(m_windowDC, m_wglDC);
			if (!ok)
			{
				wglDeleteContext(m_wglDC);
				ReleaseDC(env.pWin->GetHandle(), m_windowDC);
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

			if (m_windowDC && !ReleaseDC(env.pWin->GetHandle(), m_windowDC))
			{
				m_windowDC = NULL;
			}
		}
	}
}
