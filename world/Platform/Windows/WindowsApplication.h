#pragma once

#include <Windows.h>
#include "..\Application.h"

class WindowsApplication: public Application {
public:
	WindowsApplication();
	virtual ~WindowsApplication();
	
	bool Create( const HINSTANCE hInstance );
	
private:
	virtual void ExitApp();
	virtual void AbortApp();
	virtual void AbortApp( const String &errorMessage );
	virtual bool ProcessPlatformMessages();
	bool ProcessMessage( const MSG &msg );
	//void OnRawInput( const WPARAM wParam, const LPARAM lParam );
	
private:
	HINSTANCE handle;
};