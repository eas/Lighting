#include "stdafx.h"
#include "lighting.h"
#include "helper.h"

#include "graphics.h"
#include "colors.h"
#include "matrices.h"

#include "cylinder.h"
#include "light.h"

#ifndef NDEBUG
	#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)
#endif

const float FrontClippingPlane = 0.5f;
const float BackClippingPlane = 1.0e13f;

const unsigned nPointsPerCircle = 32;
const unsigned nPointsPerGeneratrix = 16;
const float Height = 50.0f;
const float Radius = 8.0f;
const float Freq = 1.0f;
const float MaxAngle = D3DX_PI / 4;

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void Render(D3D::GraphicDevice& device, Helper::SpectatorCoords& ,
			Cylinder& cylinder, const Lights& lights)
{
	D3D::GraphicDevice::DC dc( device, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, Colors::Gray, 1.0f, 0 );
	cylinder.Draw(lights);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE,
                     LPTSTR,
                     int nCmdShow)
{
{ // code block
	Helper::Window mainWindow(	hInstance, nCmdShow, &WndProc,
								L"Lighting", L"lighting", 3 );

	D3DPRESENT_PARAMETERS params;
		ZeroMemory( &params, sizeof( params ) );

		params.Windowed = TRUE;
		params.SwapEffect = D3DSWAPEFFECT_DISCARD;
		params.BackBufferFormat = D3DFMT_UNKNOWN;
		params.EnableAutoDepthStencil = TRUE;
		params.AutoDepthStencilFormat = D3DFMT_D16;
		params.MultiSampleType = D3DMULTISAMPLE_NONE;

	D3D::GraphicDevice graphicDevice( mainWindow.GetHWND(), params );
	//graphicDevice.SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );
	graphicDevice.SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	Helper::SpectatorCoords spectatorCoords( 40.0f, D3DX_PI / 2, -D3DX_PI / 2 );

	Cylinder cylinder(nPointsPerCircle, nPointsPerGeneratrix, Height, Radius, graphicDevice, Freq, MaxAngle,
					  Colors::White, Colors::Black, Colors::White, Colors::White);
	cylinder.SetPositionMatrix( RotateZMatrix(D3DX_PI/2)*TranslationMatrix(0, -Height/2*0, 0) );
	cylinder.SetPositionMatrix( UnityMatrix() );
	cylinder.SetViewMatrix( ViewMatrix( spectatorCoords.GetCartesianCoords(),
										D3DXVECTOR3(0.0f, 0.0f, 0.0f),
										D3DXVECTOR3(0.0f, 1.0f, 0.0f)) );
	cylinder.SetProjectiveMatrix( ProjectiveMatrix( FrontClippingPlane, BackClippingPlane ) );

	D3DXCOLOR ambient = Colors::Black;
	DirectionalLight directionalLight( D3DXVECTOR3(0.0f, 0.0f, 1.0f),
									   Colors::Red,
									   Colors::Black );
	Lights lights(ambient, directionalLight);
	lights.SetEye( spectatorCoords.GetCartesianCoords() );

	SetWindowLong(mainWindow.GetHWND(), 0, reinterpret_cast<LONG>(&spectatorCoords));
	SetWindowLong(mainWindow.GetHWND(), sizeof(LONG), reinterpret_cast<LONG>(&cylinder));
	SetWindowLong(mainWindow.GetHWND(), 2*sizeof(LONG), reinterpret_cast<LONG>(&lights));

	MSG msg;

	ZeroMemory(&msg, sizeof(msg));
    while( msg.message != WM_QUIT )
    {
        if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
		{
			Render(graphicDevice, spectatorCoords, cylinder, lights);
		}
    }
} // code block

	_CrtDumpMemoryLeaks();

	return 0;
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		{
			Helper::SpectatorCoords* pSpectatorCoords = NULL;
			Cylinder* cylinder = NULL;
			Lights* lights = NULL;
			pSpectatorCoords = reinterpret_cast<Helper::SpectatorCoords*>(
												GetWindowLong(hWnd, 0));
			cylinder = reinterpret_cast<Cylinder*>( GetWindowLong(hWnd,sizeof(LONG)) );
			lights = reinterpret_cast<Lights*>( GetWindowLong(hWnd, 2*sizeof(LONG)) );
			switch(wParam)
			{
			case VK_UP:
				pSpectatorCoords->DecTheta();
				break;
			case VK_DOWN:
				pSpectatorCoords->IncTheta();
				break;
			case VK_RIGHT:
				pSpectatorCoords->IncPhi();
				break;
			case VK_LEFT:
				pSpectatorCoords->DecPhi();
				break;
			case VK_NEXT:
			case 'S':
				pSpectatorCoords->IncR();
				break;
			case VK_PRIOR:
			case 'W':
				pSpectatorCoords->DecR();
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			cylinder->SetViewMatrix( ViewMatrix( pSpectatorCoords->GetCartesianCoords(),
												 D3DXVECTOR3(0.0f, 0.0f, 0.0f),
												 D3DXVECTOR3(0.0f, 1.0f, 0.0f)) );
			lights->SetEye( pSpectatorCoords->GetCartesianCoords() );

			break;
		}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}