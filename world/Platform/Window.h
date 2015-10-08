#pragma once

#include "..\Framework\String.h"
#include "..\Framework\Color.h"
#include "..\Renderer\RenderInterface.h"

// Trida window neni urcena k vytvareni uzivatelskeho rozhrani okennich aplikaci.
// Je to jen zapouzdreni funkcionality potrebne k zobrazeni obsahu BackBufferu v klientske oblasti obecneho okna.
//
class Window {
public:
	Window();
	virtual ~Window();
	
	// Neni mozne vytvaret kopie okna
	Window( const Window& ) = delete;
	Window &operator=( const Window& ) = delete;
	
	virtual void Destroy() = 0;
	virtual int GetWidth() const = 0;
	virtual int GetHeight() const = 0;
	virtual int GetClientWidth() const = 0;
	virtual int GetClientHeight() const = 0;
	virtual void SetBackgroundColor( const Color &color ) = 0;
	virtual const Color GetBackgroundColor() const = 0;
	virtual void SetPosition( const int x, const int y ) = 0;
	virtual void Show() = 0;
	virtual void Hide() = 0;
	virtual void SetName( const String &str ) = 0;
	virtual const String GetName() const = 0;
	
	// vyvola prekresleni klientske oblasti okna
	virtual void Update() = 0;
	
	// back buffering
	virtual void SetRenderDevice( Device * const device );
	virtual void PresentBackBuffer( const int vsync );
	BackBuffer *GetBackBuffer();
	
private:
	Device *device;
	BackBuffer *backBuffer;
};