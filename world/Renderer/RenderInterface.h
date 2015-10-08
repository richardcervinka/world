#pragma once

class Window;
class Device;
class CommandInterface;
class CommandList;
class RenderTarget;
	class RenderBuffer;
	class BackBuffer;
class DepthStencilBuffer;
class TextureSampler;

#include "..\Framework\Color.h"

const int MAX_RENDER_TARGETS = 8;
const int MAX_MULTISAMPLE_QUALITY = -1;
const int MAX_TEXTURE_LOD = -1;

// Informace potrebne k vytvoreni objektu CreateDX11DeviceParams
struct CreateDX11DeviceParams {
	int adapter;				// id adapteru, 0 je vychozi adapter
	int majorFeatureLevels;
	int minorFeatureLevels;
};

// Vytvori instanci objektu device implementovanou v DirectX
Device *CreateDX11Device( const CreateDX11DeviceParams &params );

// Buffer formaty graficke karty
enum class Format {
	UNKNOWN = 0,
	R8G8B8A8_UNORM,
	R8G8B8A8_SNORM,
	R16G16B16A16_FLOAT,
	R16G16_FLOAT,
	R8_UNORM,
	R16_FLOAT,
	R32_FLOAT
};

// Rezim displeje
struct DisplayMode {
	int width;
	int height;
	int refreshRateNumerator;
	int refreshRateDenominator;
};

struct RenderBufferDesc {
	Format format;
	int width;
	int height;
	int samplesCount;
	int multisampleQuality;
};

struct DepthStencilBufferDesc {
	int width;
	int height;
	int samplesCount;
	int multisampleQuality;
};

// Rezimy filtrovani textur
enum class TextureFilter {
	POINT,
	POIN_MIP_LINEAR,
	MIN_POINT_MAG_LINEAR_MIP_POINT,
	MIN_POINT_MAG_MIP_LINEAR,
	MIN_LINEAR_MAG_MIP_POINT,
	MIN_LINEAR_MAG_POINT_MIP_LINEAR,
	LINEAR_MIP_POINT,
	LINEAR,
	ANISOTROPIC
};

// Rezimy adresovani textur
enum class TextureAddress {
	WRAP = 0,
	MIRROR,
	CLAMP
};

struct TextureSamplerDesc {
	TextureFilter filter;
	TextureAddress uAddress;
	TextureAddress vAddress;
	TextureAddress wAddress;
	int maxAnisotropy;
	float minLOD;
	float maxLOD;	// hodnota v rozsahu <0; MAX_TEXTURE_LOD>
};

// Identifikace shader stag, hodnoty lze kombinovat operatorem OR
typedef unsigned int ShaderStage;
const ShaderStage VERTEX_SHADER_STAGE	= 0x0001;
const ShaderStage PIXEL_SHADER_STAGE	= 0x0002;
const ShaderStage GEOMETRY_SHADER_STAGE	= 0x0004;

/*
Zakladni trida pro vsechny render interface objekty.
*/
class RenderInterfaceObject {
public:
	RenderInterfaceObject();
	virtual ~RenderInterfaceObject();

	// Pokud je pocet referenci 0, uvolni objekt z pameti a nesmi byt dale pouzivan!
	void Release();

	void AddRef();

private:
	int references;
};

/*
Device se pouziva k vytvareni vsech RenderInterfaceObject objektu
*/
class Device: public RenderInterfaceObject {
public:
	virtual ~Device() {}

	// vytvareni render objektu
	virtual CommandInterface *CreateCommandInterface() = 0;
	virtual BackBuffer *CreateBackBuffer( Window &window ) = 0;
	virtual RenderBuffer *CreateRenderBuffer( const RenderBufferDesc &desc ) = 0;
	virtual DepthStencilBuffer *CreateDepthStencilBuffer( const DepthStencilBufferDesc &desc ) = 0;
	virtual TextureSampler *CreateTextureSampler( const TextureSamplerDesc &desc ) = 0;

	// vlastnosti grafickeho adapteru
	virtual int GetMultisampleQuality( const int samplesCount ) const = 0;

	// zmeni velikost back bufferu objektu RenderOutput; velikost se ziska z asociovaneho okna objektu RenderOutput
	//virtual void ResizBackBuffer( RenderOutput * const output ) = 0;
};

/*
CommandInterface slouzi ke generovani commandu graficke karty
*/
class CommandInterface: public RenderInterfaceObject {
public:
	virtual ~CommandInterface() {}

	// zahajeni posilani commanu primo do grafickeho zarizeni
	virtual void Begin( Device * const device ) = 0;

	// zahajeni ukladani commandu do CommandListu
	virtual void Begin( CommandList * const commandList ) = 0;

	// ukonceni sekvence commandu
	virtual void End() = 0;

	// Nastavi cil vykreslovani
	virtual void SetRenderTargets( RenderTarget * const renderTargets, const int count, DepthStencilBuffer * const depthStencilBuffer ) = 0;

	virtual void ClearRenderTarget( RenderTarget * const renderTarget, const Color &color ) = 0;

	//virtual void ClearRenderTarget();
	//virtual void ClearDepthStencil();

	//void ClearState();
	//void SetRenderTarget( RenderBuffer )
	//void SetPipelineState()
};

/*
CommandList slouzi k jednorazovemu zaznemenani commandu a jejich pozdejsimu prehrani.
*/
class CommandList: public RenderInterfaceObject {
public:
	//virtual ~CommandList();
	//virtual void ExecuteCommandBatch( CommandBatch *batch );
};

/*
Zakladni trida pro RenderOutput a RenderBuffer
*/
class RenderTarget: public RenderInterfaceObject {
public:
	virtual ~RenderTarget() = 0 {}
};

/*

ODSTRANIT

RenderOutput reprezentuje vystupni monitor a back buffer zobrazovany na tomto monitoru.
Neobsahuje depth stencil buffer, protoze depth test se provadi v jine casti deferred renderingu, do back bufferu se uklada jen vysledek operace Resolve.
Pomoci tridy je mozne menit fullscreen rezim displeje.
*/
/*
class RenderOutput: public RenderTarget {
public:
	virtual ~RenderOutput() {}

	virtual void Present( bool vsync ) = 0;

	virtual DisplayMode GetDisplayMode( const int id ) = 0;

	//vrati id rezimu, ktery nejblize odpovida pozadovanym parametrum
	virtual int GetDisplayModeId( const int width, const int height, const float refreshRate ) = 0;

	// nastavi display mode podle id
	virtual void SetDisplayMode( const int id, const bool fullscreen ) = 0;

	// nastavi rezim nejblizsi pozadovanym parametrum
	virtual void SetDisplayMode( const int width, const int height, const float refreshRate, const bool fullscreen ) = 0;

	//virtual int GetBackBufferWidth() const = 0;
	//virtual int GetBackBufferHeight() const = 0;
};
*/

/*
Reprezentuje back buffer ktery je pri vytvoreni propojen s cilovym oknem.
Slouzi jako render target a zaroven umoznuje zobrazeni obsahu do klientske oblasti okna.
*/
class BackBuffer: public RenderTarget {
public:
	virtual ~BackBuffer() {}
	virtual void Present( const int vsync ) = 0;
};

/*
RenderBuffer je vyuzivany pro G-Buffery. Umoznuje cteni i zapis pomoci GPU, pristup pres CPU neni povolen.
Podporuje multisampling.
*/
class RenderBuffer: public RenderTarget {
public:
	virtual ~RenderBuffer() {}
	virtual RenderBufferDesc GetDesc() const = 0;
	// GetSamplesCount()
	// GetWidth()
	// GetHeight()
	// GetFormat()
};

/*
DepthStencilBuffer
*/
class DepthStencilBuffer: public RenderInterfaceObject {
public:
	virtual ~DepthStencilBuffer() {}
	virtual DepthStencilBufferDesc GetDesc() const = 0;
};

/*
TextureSampler
*/
class TextureSampler: public RenderInterfaceObject {
public:
	virtual ~TextureSampler() {};
	virtual TextureSamplerDesc GetDesc() const = 0;
};

struct BlendStateDescription {
};

struct RasterizeStateDescription {
};

struct PipelineStateDescription {
	BlendStateDescription blendStateDesc;
	RasterizeStateDescription rasterizeStateDesc;
};

class PipelineState: public RenderInterfaceObject {
public:

private:
};

/*
Zakladni trida pro vsechny buffer objekty, jako jsou Textury apod.
*/
class Buffer: public RenderInterfaceObject {
public:
	// int GetWidth() const;
	// int GetHeight() const;
	// int GetByteSize() const;

protected:
};