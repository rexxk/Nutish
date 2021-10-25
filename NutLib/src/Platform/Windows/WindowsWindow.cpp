#include "nutpch.h"
#include "WindowsWindow.h"

#include <NutLib/Event/Events.h>

#include "NutLib/Core/Keycodes.h"

#include "NutLib/Renderer/Renderer.h"
#include "NutLib/Renderer/RenderThread.h"

#include <glad/glad.h>

#include "Platform/OpenGL/glext.h"
#include "Platform/OpenGL/wglext.h"



namespace Nut
{

#ifdef _WIN32
	LRESULT WINAPI WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam);

	Ref<Window> Window::Create(const WindowProperties& props)
	{
		return CreateRef<WindowsWindow>(props);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& props)
		: m_Properties(props)
	{
		Create();
	}

	WindowsWindow::~WindowsWindow()
	{
		if (m_Window)
		{
			DestroyWindow(m_Window);
		}

		HGLRC currentContext = wglGetCurrentContext();

		if (currentContext)
		{
			wglDeleteContext(currentContext);
		}

	}

	void WindowsWindow::Create()
	{
		WNDCLASSEX wcex{};
		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wcex.hInstance = GetModuleHandle(NULL);
		wcex.lpszClassName = L"Nut";
		wcex.lpfnWndProc = WndProc;
		wcex.hbrBackground = (HBRUSH)GetStockObject(COLOR_BACKGROUND + 1);
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);

		if (!RegisterClassEx(&wcex))
		{
			LOG_CORE_ERROR("Failed to register window class\n");
			return;
		}

		PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB;
		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

		HWND fakeWindow;
		HDC fakeDC;
		HGLRC fakeRC;

		if (Renderer::API() == RendererAPIType::OpenGL)
		{
			fakeWindow = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"Nut", L"Fake window", WS_OVERLAPPEDWINDOW, 0, 0, 1, 1, NULL, NULL, GetModuleHandle(NULL), NULL);

			fakeDC = GetDC(fakeWindow);

			PIXELFORMATDESCRIPTOR fakePFD;
			ZeroMemory(&fakePFD, sizeof(PIXELFORMATDESCRIPTOR));
			fakePFD.nSize = sizeof(fakePFD);
			fakePFD.nVersion = 1;
			fakePFD.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
			fakePFD.iPixelType = PFD_TYPE_RGBA;
			fakePFD.cColorBits = 32;
			fakePFD.cAlphaBits = 8;
			fakePFD.cDepthBits = 24;

			const int fakePFDID = ChoosePixelFormat(fakeDC, &fakePFD);
			SetPixelFormat(fakeDC, fakePFDID, &fakePFD);

			fakeRC = wglCreateContext(fakeDC);

			wglMakeCurrent(fakeDC, fakeRC);

			wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));
			wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
		}



		wchar_t title[MAX_PATH];
		std::mbstowcs(title, m_Properties.Title.c_str(), MAX_PATH);

		m_Window = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"Nut", title, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, m_Properties.Width, m_Properties.Height,
			NULL, NULL, GetModuleHandle(NULL), NULL);

		if (Renderer::API() == RendererAPIType::OpenGL)
		{
			HDC dc = GetDC(m_Window);

			const int pixelAttribs[] =
			{
				WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
				WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
				WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
				WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
				WGL_COLOR_BITS_ARB, 32,
				WGL_ALPHA_BITS_ARB, 8,
				WGL_DEPTH_BITS_ARB, 24,
				WGL_STENCIL_BITS_ARB, 8,
//				WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
//				WGL_SAMPLES_ARB, 4,
				0
			};

			int pixelFormatID;
			uint32_t numFormats;

			const bool status = wglChoosePixelFormatARB(dc, pixelAttribs, nullptr, 1, &pixelFormatID, &numFormats);

			if (status == false || numFormats == 0)
			{
				LOG_CORE_ERROR("wglChoosePixelFormatARB() failed");
			}

			PIXELFORMATDESCRIPTOR pfd;
			DescribePixelFormat(dc, pixelFormatID, sizeof(pfd), &pfd);
			SetPixelFormat(dc, pixelFormatID, &pfd);

			const int major_min = 4;
			const int minor_min = 5;

			const int contextAttribs[] =
			{
				WGL_CONTEXT_MAJOR_VERSION_ARB, major_min,
				WGL_CONTEXT_MINOR_VERSION_ARB, minor_min,
				WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
				0,
			};

			HGLRC rc = wglCreateContextAttribsARB(dc, 0, contextAttribs);

			if (rc == NULL)
			{
				LOG_CORE_ERROR("wglCreateContextAttribsARB() failed");
			}

			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(fakeRC);
			ReleaseDC(fakeWindow, fakeDC);
			DestroyWindow(fakeWindow);

			wglMakeCurrent(dc, rc);

			NUT_ASSERT(gladLoadGL(), "Failed to init OpenGL library (Glad)");

		}

		SetWindowLongPtr(m_Window, GWLP_USERDATA, (LONG_PTR)&m_Properties);

		RECT rc;
		GetWindowRect(m_Window, &rc);
		rc.right = rc.left + m_Properties.Width;
		rc.bottom = rc.top + m_Properties.Height;

		AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_OVERLAPPEDWINDOW);

		SetWindowPos(m_Window, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOMOVE);

		m_RenderContext = RenderContext::Create(wglGetCurrentContext());

		wglSwapInterval = (PFNWGLSWAPINTERVALEXT)wglGetProcAddress("wglSwapIntervalEXT");

		wglSwapInterval(0);

		ShowWindow(m_Window, SW_SHOW);

		wglMakeCurrent(NULL, NULL);
	}

	void WindowsWindow::Update()
	{
		MSG msg;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}

	void WindowsWindow::Present()
	{
		SwapBuffers(GetDC(m_Window));
	}

	bool WindowsWindow::VSync() const
	{
		return m_Properties.VSync;
	}

	void WindowsWindow::EnableVSync(bool value)
	{
		m_Properties.VSync = value;

		RenderThread::Submit([&]()
			{
				if (value == true)
					wglSwapInterval(1);
				else
					wglSwapInterval(0);

			});

/*	
	if (VSync())
			wglSwapInterval(1);
		else
			wglSwapInterval(0);
*/
	}



	LRESULT WINAPI WndProc(HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		WindowProperties* props = (WindowProperties*)GetWindowLongPtr(wnd, GWLP_USERDATA);

		switch (msg)
		{
			case WM_SIZE:
			{
				props->Width = LOWORD(lParam);
				props->Height = HIWORD(lParam);

				ProcessEvent(MakeEvent<WindowResizedEvent>(props->Width, props->Height));

				break;
			}

//			case WM_CREATE:
//			{
/*
				if (Renderer::API() == RendererAPIType::OpenGL)
				{
					PIXELFORMATDESCRIPTOR pfd =
					{
						sizeof(PIXELFORMATDESCRIPTOR),
						1,
						PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
						PFD_TYPE_RGBA,
						32,
						0, 0, 0, 0, 0, 0,
						0,
						0,
						0,
						0, 0, 0, 0,
						24,
						8,
						0,
						PFD_MAIN_PLANE,
						0,
						0, 0, 0
					};

					HDC currentDeviceContext = GetDC(wnd);

					
					int pixelFormat = ChoosePixelFormat(currentDeviceContext, &pfd);
					SetPixelFormat(currentDeviceContext, pixelFormat, &pfd);

					HGLRC renderContext = wglCreateContext(currentDeviceContext);
					wglMakeCurrent(currentDeviceContext, renderContext);

					using PFNWGLCHOOSEPIXELFORMATARB = bool(*)(HDC, const int*, const float*, uint32_t, int*, uint32_t*);
					PFNWGLCHOOSEPIXELFORMATARB wglChoosePixelFormatARB;
					wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARB)wglGetProcAddress("wglChoosePixelFormatARB");
					
					using PFNCREATECONTEXTATTRIBSARB = HGLRC(*)(HDC, HGLRC, const int*);
					PFNCREATECONTEXTATTRIBSARB wglCreateContextAttribsARB;
					wglCreateContextAttribsARB = (PFNCREATECONTEXTATTRIBSARB)wglGetProcAddress("wglCreateContextAttribsARB");

					wglDeleteContext(renderContext);
					wglMakeCurrent(NULL, NULL);

					const int attribList[] =
					{
						
						WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
						WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
						WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
						WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
						WGL_COLOR_BITS_ARB, 32,
						WGL_DEPTH_BITS_ARB, 24,
						WGL_STENCIL_BITS_ARB, 8,
					};

					int pFormat[1];

					uint32_t numFormats;
					NUT_CORE_ASSERT(wglChoosePixelFormatARB(currentDeviceContext, attribList, nullptr, 1, pFormat, &numFormats), "Unable to find a suitable pixel format for OpenGL!");

					NUT_CORE_ASSERT(SetPixelFormat(currentDeviceContext, pFormat[0], &pfd), "Unable to set pixel format!");

					const int versionList[] =
					{
						WGL_CONTEXT_MAJOR_VERSION_ARB,
						4,
						WGL_CONTEXT_MINOR_VERSION_ARB,
						5,
					};

					renderContext = wglCreateContextAttribsARB(currentDeviceContext, 0, versionList);
					wglMakeCurrent(currentDeviceContext, renderContext);

					NUT_ASSERT(gladLoadGL(), "Failed to init OpenGL library (Glad)");


				}
*/
//				break;
//			}

			case WM_DESTROY:
			{
				AddEvent(MakeEvent<WindowClosedEvent>(wnd));

				PostQuitMessage(0);

				return 0;
			}

			case WM_MOUSEWHEEL:
			{
				int16_t delta = HIWORD(wParam);
				AddEvent(MakeEvent<MouseScrolledEvent>((int32_t)delta > 0 ? 1 : -1, LOWORD(lParam), HIWORD(lParam)));

				break;
			}

			case WM_MOUSEMOVE:
			{
				AddEvent(MakeEvent<MouseMovedEvent>(LOWORD(lParam), HIWORD(lParam)));

				break;
			}

			case WM_LBUTTONDOWN:
			{
				AddEvent(MakeEvent<MouseButtonPressed>(LeftButton, (int16_t)LOWORD(lParam), (int16_t)HIWORD(lParam)));

				SetCapture(wnd);

				break;
			}

			case WM_RBUTTONDOWN:
			{
				AddEvent(MakeEvent<MouseButtonPressed>(RightButton, (int16_t)LOWORD(lParam), (int16_t)HIWORD(lParam)));

				SetCapture(wnd);

				break;
			}

			case WM_MBUTTONDOWN:
			{
				AddEvent(MakeEvent<MouseButtonPressed>(MiddleButton, (int16_t)LOWORD(lParam), (int16_t)HIWORD(lParam)));

				SetCapture(wnd);

				break;
			}

			case WM_LBUTTONUP:
			{
				AddEvent(MakeEvent<MouseButtonReleased>(LeftButton, (int16_t)LOWORD(lParam), (int16_t)HIWORD(lParam)));

				ReleaseCapture();

				break;
			}

			case WM_RBUTTONUP:
			{
				AddEvent(MakeEvent<MouseButtonReleased>(RightButton, (int16_t)LOWORD(lParam), (int16_t)HIWORD(lParam)));

				ReleaseCapture();

				break;
			}

			case WM_MBUTTONUP:
			{
				AddEvent(MakeEvent<MouseButtonReleased>(MiddleButton, (int16_t)LOWORD(lParam), (int16_t)HIWORD(lParam)));

				ReleaseCapture();

				break;
			}

			case WM_SYSKEYDOWN:
			case WM_KEYDOWN:
			{
				AddEvent(MakeEvent<KeyPressedEvent>((uint32_t)wParam));

				break;
			}

			case WM_SYSKEYUP:
			case WM_KEYUP:
			{
				AddEvent(MakeEvent<KeyReleasedEvent>((uint32_t)wParam));

				break;
			}
		}


		return DefWindowProc(wnd, msg, wParam, lParam);
	}


#endif

}

