#include <d3d11.h>
#include <dxgi1_2.h>
#include "..\DX11\DX11RenderInterface.h"
#include "WindowsGraphicsInfrastructure.h"

// WindowsAdapter

WindowsAdapter::WindowsAdapter( IDXGIAdapter1* const adapter ) {
	this->adapter = adapter;
}

WindowsAdapter::~WindowsAdapter() {
	if ( adapter != nullptr ) {
		adapter->Release();
	}
}

int WindowsAdapter::GetOutputsCount() noexcept {
	UINT counter = 0;
	IDXGIOutput* output = nullptr;
	while ( adapter->EnumOutputs( counter, &output ) != DXGI_ERROR_NOT_FOUND ) {
		output->Release();
		counter += 1;
	}
	return static_cast< int >( counter );
}

bool WindowsAdapter::CheckCapabilities( const WindowsAdapterCapabilities& capabilities ) noexcept {

	// adapter description
	DXGI_ADAPTER_DESC adapterDesc;
	adapter->GetDesc( &adapterDesc );

	// porovnat mnozstvi dostupne a pozadovane dedikovane pameti
	if ( static_cast< unsigned int >( adapterDesc.DedicatedVideoMemory ) < capabilities.requiredVideoMemory ) {
		return false;
	}
	// pokusit se vytvorit ID3D11Device objekt
	if ( WindowsRenderApi::DIRECTX_11_0 == capabilities.api ) {

		D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;
		D3D_FEATURE_LEVEL createdFeatureLevel;
		
		// create temporary device
		ID3D11Device* device = nullptr;
		HRESULT hresult = D3D11CreateDevice(
			adapter,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			0,
			&featureLevel,
			1,
			D3D11_SDK_VERSION,
			&device,
			&createdFeatureLevel,
			NULL
		);
		if ( FAILED( hresult ) ) {
			return false;
		}
		// release temporary device
		device->Release();
		return true;
	}
	return false;
}

RenderInterface::PDevice WindowsAdapter::CreateDX11Device() noexcept {
	std::shared_ptr< DX11Device > device( new( std::nothrow ) DX11Device() );
	if ( device == nullptr ) {
		return nullptr;
	}
	if ( !device->Create( adapter, D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0 ) ) {
		return nullptr;
	}
	return device;
}

// WindowsGraphicsInfrastructure

WindowsGraphicsInfrastructure::WindowsGraphicsInfrastructure() {
	HRESULT hresult = CreateDXGIFactory1( __uuidof( IDXGIFactory1 ), reinterpret_cast< void** >( &factory ) );
	if ( FAILED( hresult ) ) {
		return;
	}
}

WindowsGraphicsInfrastructure::~WindowsGraphicsInfrastructure() {
	if ( factory != nullptr ) {
		factory->Release();
		factory = nullptr;
	}
}

std::unique_ptr< WindowsAdapter > WindowsGraphicsInfrastructure::CreateAdapter( const int id ) noexcept {
	if ( factory == nullptr ) {
		return nullptr;
	}
	IDXGIAdapter* adapter = nullptr; 
	HRESULT hresult = factory->EnumAdapters( id, &adapter );
	if ( FAILED( hresult ) ) {
		return nullptr;
	}
	return std::make_unique< WindowsAdapter >( adapter );
}

std::unique_ptr< WindowsAdapter > WindowsGraphicsInfrastructure::CreateAdapter( const WindowsAdapterCapabilities& capabilities ) noexcept {
	if ( factory == nullptr ) {
		return nullptr;
	}
	int id = 0;
	auto adapter = CreateAdapter( id );
	while ( adapter != nullptr ) {
		if ( adapter->CheckCapabilities( capabilities ) ) {
			return adapter;
		}
		// next adapter
		id += 1;
		adapter = CreateAdapter( id );
	}
	return nullptr;
}