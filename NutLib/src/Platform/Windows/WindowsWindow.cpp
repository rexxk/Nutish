#include "nutpch.h"
#include "WindowsWindow.h"

#include <NutLib/Event/Events.h>

#include "NutLib/Core/Keycodes.h"

#include "NutLib/Renderer/Renderer.h"
#include "NutLib/Renderer/RenderCommandQueue.h"

#include <glad/glad.h>


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


		wchar_t title[MAX_PATH];
		std::mbstowcs(title, m_Properties.Title.c_str(), MAX_PATH);

		m_Window = CreateWindowEx(WS_EX_OVERLAPPEDWINDOW, L"Nut", title, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, m_Properties.Width, m_Properties.Height,
			NULL, NULL, GetModuleHandle(NULL), NULL);

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

		RenderCommandQueue::Submit([&]()
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

			case WM_CREATE:
			{
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

					NUT_ASSERT(gladLoadGL(), "Failed to init OpenGL library (Glad)");
				}

				break;
			}

			case WM_DESTROY:
			{
				AddEvent(MakeEvent<WindowClosedEvent>());

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

