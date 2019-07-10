#pragma once
#include "framework.h"
#include <comdef.h>
#include <string>

void SendErrorMessage(HRESULT result, std::wstring errorMessage)
{
	_com_error error(result);
	std::wstring fullMessage = errorMessage + L"\n" + error.ErrorMessage();
	MessageBoxW(nullptr, fullMessage.c_str(), L"Error", MB_ICONERROR);

#if defined(DEBUG) || defined(_DEBUG)
	OutputDebugStringW(fullMessage.c_str());
#endif
}

void ThrowIfFailed(HRESULT result, std::wstring errorMessage)
{
	if (FAILED(result))
	{
		SendErrorMessage(result, errorMessage);
		throw std::exception();
	}
}
