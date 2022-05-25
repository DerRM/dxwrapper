/**
* Copyright (C) 2021 Elisha Riedlinger
*
* This software is  provided 'as-is', without any express  or implied  warranty. In no event will the
* authors be held liable for any damages arising from the use of this software.
* Permission  is granted  to anyone  to use  this software  for  any  purpose,  including  commercial
* applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*   1. The origin of this software must not be misrepresented; you must not claim that you  wrote the
*      original  software. If you use this  software  in a product, an  acknowledgment in the product
*      documentation would be appreciated but is not required.
*   2. Altered source versions must  be plainly  marked as such, and  must not be  misrepresented  as
*      being the original software.
*   3. This notice may not be removed or altered from any source distribution.
*/

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// for GET_X_LPARAM and GET_Y_LPARAM
#include <windowsx.h>

#include "Wrappers\wrapper.h"
#include "GDI.h"
#include "Settings\Settings.h"
#include "Logging\Logging.h"

namespace GdiWrapper
{
	FARPROC CreateWindowExA_out = nullptr;
	FARPROC CreateWindowExW_out = nullptr;
	WNDPROC OriginalWndProc = nullptr;
	uint32_t GameWidth = 800;
	uint32_t GameHeight = 600;
	uint32_t NativeWidth = 1920;
	uint32_t NativeHeight = 1080;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Logging::LogDebug() << __FUNCTION__ << " " << Logging::hex(uMsg);

	if (!GdiWrapper::OriginalWndProc)
	{
		//LOG_LIMIT(100, __FUNCTION__ << " Error: no WndProc specified " << Logging::hex(uMsg));
		return NULL;
	}

	if (GdiWrapper::OriginalWndProc == WndProc) {
		return NULL;
	}

	uint32_t newLparam = lParam;

	switch (uMsg) {
	case WM_MOUSEMOVE:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONUP:
	case WM_MBUTTONDBLCLK:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_RBUTTONDBLCLK:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	{
		uint32_t x = GET_X_LPARAM(lParam);
		uint32_t y = GET_Y_LPARAM(lParam);
		Logging::LogDebug() << __FUNCTION__ << " " << Logging::hex(uMsg) << " x " << x << " y " << y;

		float value = (float(GdiWrapper::NativeWidth) / float(GdiWrapper::NativeHeight)) * float(GdiWrapper::GameHeight);

		newLparam = (((uint32_t)(max((x / float(GdiWrapper::NativeWidth)) * value - ((value - float(GdiWrapper::GameWidth)) * 0.5f), 0.0f)) & 0xffff)) | ((((uint32_t)((y / float(GdiWrapper::NativeHeight)) * float(GdiWrapper::GameHeight))) & 0xffff) << 16);
	}
	break;
	}

	return GdiWrapper::OriginalWndProc(hWnd, uMsg, wParam, newLparam);
}

using namespace GdiWrapper;

HWND user_CreateWindowEx_out(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	static CreateWindowExAProc m_pCreateWindowExA = (Wrapper::ValidProcAddress(CreateWindowExA_out)) ? (CreateWindowExAProc)CreateWindowExA_out : nullptr;

	if (!m_pCreateWindowExA)
	{
		return nullptr;
	}

	return m_pCreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

HWND user_CreateWindowEx_out(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	static CreateWindowExWProc m_pCreateWindowExW = (Wrapper::ValidProcAddress(CreateWindowExW_out)) ? (CreateWindowExWProc)CreateWindowExW_out : nullptr;

	if (!m_pCreateWindowExW)
	{
		return nullptr;
	}

	return m_pCreateWindowExW(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

template <class T>
HWND user_CreateWindowEx(DWORD dwExStyle, T lpClassName, T lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	Logging::LogDebug() << __FUNCTION__ << " " << lpClassName << " " << lpWindowName << " " << Logging::hex(dwExStyle) << " " << Logging::hex(dwStyle) << " " << X << "x" << Y << " " << nWidth << "x" << nHeight << " " << Logging::hex((DWORD)hWndParent) << " " << hWndParent << " " << hMenu << " " << hInstance;

	HWND hwnd = NULL;

	// Handle popup window type
	if ((dwStyle & WS_POPUP) && (dwStyle & WS_VISIBLE) && !(dwStyle & WS_CLIPSIBLINGS) && !hWndParent)
	{
		DWORD dwNewStyle = dwStyle;

		// Remove popup style
		dwNewStyle = dwStyle & ~WS_POPUP;

		hwnd = user_CreateWindowEx_out(dwExStyle, lpClassName, lpWindowName, dwNewStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);

		if (hwnd)
		{
			LOG_LIMIT(100, __FUNCTION__ << " Removed WS_POPUP window style! " << hwnd);

			SetWindowLong(hwnd, GWL_STYLE, dwNewStyle);
		}

		if (hwnd)
		{
			OriginalWndProc = (WNDPROC)GetWindowLong(hwnd, GWL_WNDPROC);

			Logging::LogDebug() << __FUNCTION__ << " Add custom WndProc to " << lpClassName << " " << lpWindowName << " " << Logging::hex((DWORD)hWndParent) << " " << hWndParent << " " << hMenu << " " << hInstance;

			// Set new WndProc
			if (!OriginalWndProc || !SetWindowLong(hwnd, GWL_WNDPROC, (LONG)WndProc))
			{
				Logging::Log() << __FUNCTION__ << " Failed to overload WndProc!";
				OriginalWndProc = nullptr;
			}
		}
	}
	else {
		hwnd = user_CreateWindowEx_out(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
	}

	return hwnd;
}

HWND WINAPI user_CreateWindowExA(DWORD dwExStyle, LPCSTR lpClassName, LPCSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	return user_CreateWindowEx<LPCSTR>(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}

HWND WINAPI user_CreateWindowExW(DWORD dwExStyle, LPCWSTR lpClassName, LPCWSTR lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, LPVOID lpParam)
{
	return user_CreateWindowEx<LPCWSTR>(dwExStyle, lpClassName, lpWindowName, dwStyle, X, Y, nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam);
}
