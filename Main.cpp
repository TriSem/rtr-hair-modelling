#include "framework.h"
#include "Application.h"

int APIENTRY wWinMain(_In_ HINSTANCE instanceHandle,
	_In_opt_ HINSTANCE previousInstance,
	_In_ LPWSTR    commandLine,
	_In_ int       nCmdShow)
{
	auto app = std::make_unique<Application>(instanceHandle, nCmdShow, L"Mesh Simplification");
	app->Run();
	return 0;
 }

