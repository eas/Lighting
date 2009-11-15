#include "stdafx.h"
#include "lighting.h"
#include "helper.h"

#include "graphics.h"
#include "colors.h"
#include "matrices.h"

#include "scene.h"

#ifndef NDEBUG
	#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)
#endif

const float FrontClippingPlane = 0.5f;
const float BackClippingPlane = 1.0e13f;


LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void Render(D3D::GraphicDevice& device,	Scene& scene)
{
	D3D::GraphicDevice::DC dc( device, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, Colors::Gray, 1.0f, 0 );
	scene.Draw();
}
void MessageLoop( D3D::GraphicDevice& graphicDevice, Scene& scene )
{
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
			Render(graphicDevice, scene);
		}
    }
}
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE,
                     LPTSTR,
                     int nCmdShow)
{
{ // code block
	Helper::Window mainWindow(	hInstance, nCmdShow, &WndProc,
								L"Lighting", L"lighting", 2 );

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

	Helper::SpectatorCoords spectatorCoords( 40.0f, D3DX_PI / 2, -0*D3DX_PI / 2 );

	Scene scene(graphicDevice);
	scene.SetEye( spectatorCoords.GetCartesianCoords() );
	scene.SetProjectiveMatrix( ProjectiveMatrix( FrontClippingPlane, BackClippingPlane ) );

	SetWindowLong(mainWindow.GetHWND(), 0			  , reinterpret_cast<LONG>(&spectatorCoords));
	SetWindowLong(mainWindow.GetHWND(), 1*sizeof(LONG), reinterpret_cast<LONG>(&scene));

	MessageLoop( graphicDevice, scene );

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
			Scene* scene = NULL;
			Helper::SpectatorCoords* spectatorCoords = NULL;
			spectatorCoords = reinterpret_cast<Helper::SpectatorCoords*>(
												GetWindowLong(hWnd, 0));
			scene = reinterpret_cast<Scene*>( GetWindowLong(hWnd,sizeof(LONG)) );

			switch(wParam)
			{
			case VK_UP:
				spectatorCoords->DecTheta();
				break;
			case VK_DOWN:
				spectatorCoords->IncTheta();
				break;
			case VK_RIGHT:
				spectatorCoords->IncPhi();
				break;
			case VK_LEFT:
				spectatorCoords->DecPhi();
				break;
			case VK_NEXT:
			case 'S':
				spectatorCoords->IncR();
				break;
			case VK_PRIOR:
			case 'W':
				spectatorCoords->DecR();
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			scene->SetEye( spectatorCoords->GetCartesianCoords() );

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