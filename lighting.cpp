#include "stdafx.h"
#include "lighting.h"
#include "helper.h"

#include "graphics.h"
#include "colors.h"
#include "matrices.h"

#include "cylinder.h"
#include "sphere.h"
#include "light.h"
#include "plain.h"

#ifndef NDEBUG
	#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)
#endif

const float FrontClippingPlane = 0.5f;
const float BackClippingPlane = 1.0e13f;

const unsigned nPointsPerCircle = 32;
const unsigned nPointsPerGeneratrix = 16;
const float Height = 50.0f;
const float Radius = 8.0f;
const float CylinderFreq = 1.0f;
const float MaxAngle = D3DX_PI / 4;

const float SphereRadius = 8.0f;
const unsigned TesselationLevel = 5;
const float SphereFreq = 1.0f;

const float AngleStep = D3DX_PI / 8;

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
void Render(D3D::GraphicDevice& device, Helper::SpectatorCoords& ,
			Cylinder& cylinder, Sphere& sphere, const Lights& lights, Plain& plain)
{
	cylinder; plain; sphere;

	D3D::GraphicDevice::DC dc( device, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, Colors::Gray, 1.0f, 0 );
	//cylinder.Draw(lights);
	//sphere.Draw(lights);
	plain.Draw(lights);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE,
                     LPTSTR,
                     int nCmdShow)
{
{ // code block
	Helper::Window mainWindow(	hInstance, nCmdShow, &WndProc,
								L"Lighting", L"lighting", 5 );

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

	D3DXMATRIX viewMatrix = ViewMatrix( spectatorCoords.GetCartesianCoords(),
										D3DXVECTOR3(0.0f, 0.0f, 0.0f),
										D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	D3DXMATRIX projectiveMatrix = ProjectiveMatrix( FrontClippingPlane, BackClippingPlane );

	Cylinder cylinder(nPointsPerCircle, nPointsPerGeneratrix, Height, Radius, graphicDevice, CylinderFreq, MaxAngle,
					  Colors::White, Colors::Black, Colors::White, Colors::White);
	cylinder.SetPositionMatrix( TranslationMatrix(0, -Height/2, 0) );
	cylinder.SetViewMatrix( viewMatrix );
	cylinder.SetProjectiveMatrix( projectiveMatrix );

	Plain plain( graphicDevice,
				 Colors::Black, Colors::Black, Colors::White, Colors::White );
	plain.SetViewMatrix( viewMatrix );
	plain.SetProjectiveMatrix( projectiveMatrix );
	
	Sphere sphere( SphereRadius, TesselationLevel, graphicDevice, SphereFreq,
				   Colors::White, Colors::Black, Colors::White, Colors::White );
	sphere.SetPositionMatrix( TranslationMatrix( -2*SphereRadius - 2*Radius, 0.0f, 0.0f ) );
	sphere.SetViewMatrix( viewMatrix );
	sphere.SetProjectiveMatrix( projectiveMatrix );

	D3DXCOLOR ambient = Colors::Black;
	DirectionalLight directionalLight( D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
									   Colors::Black,
									   Colors::Black,
									   100.0f );
	PointLight pointLight( D3DXVECTOR3(10000.0f, 0.0f, 0.0f),
						   Colors::Black,
						   Colors::Black,
						   0.0f, 0.0001f, 0.000f, 
						   10.0f );
	SpotLight spotLight( D3DXVECTOR3(20.0f, 0.0f, 0.0f),
						 Colors::Green,
						 Colors::Red,
						 1.0f, 0.0f, 0.000f,
						 6*D3DX_PI/64, 16*D3DX_PI/64,
						 D3DXVECTOR3(-1.0f, 0.0f, 0.0f),
						 2.0f );

	Lights lights(ambient, directionalLight, pointLight, spotLight);
	lights.SetEye( spectatorCoords.GetCartesianCoords() );

	SetWindowLong(mainWindow.GetHWND(), 0			  , reinterpret_cast<LONG>(&spectatorCoords));
	SetWindowLong(mainWindow.GetHWND(), 1*sizeof(LONG), reinterpret_cast<LONG>(&cylinder));
	SetWindowLong(mainWindow.GetHWND(), 2*sizeof(LONG), reinterpret_cast<LONG>(&lights));
	SetWindowLong(mainWindow.GetHWND(), 3*sizeof(LONG), reinterpret_cast<LONG>(&sphere));
	SetWindowLong(mainWindow.GetHWND(), 4*sizeof(LONG), reinterpret_cast<LONG>(&plain));

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
			Render(graphicDevice, spectatorCoords, cylinder, sphere, lights, plain);
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
			Plain* plain = NULL;
			Sphere* sphere = NULL;
			pSpectatorCoords = reinterpret_cast<Helper::SpectatorCoords*>(
												GetWindowLong(hWnd, 0));
			cylinder = reinterpret_cast<Cylinder*>( GetWindowLong(hWnd,sizeof(LONG)) );
			lights = reinterpret_cast<Lights*>( GetWindowLong(hWnd, 2*sizeof(LONG)) );
			sphere = reinterpret_cast<Sphere*>( GetWindowLong(hWnd, 3*sizeof(LONG)) );
			plain = reinterpret_cast<Plain*>( GetWindowLong(hWnd, 4*sizeof(LONG)) );

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
			case 'T':
				cylinder->SetPositionMatrix( cylinder->GetPostionMatrix() * RotateYMatrix( AngleStep ) );
				break;
			case 'G':
				cylinder->SetPositionMatrix( cylinder->GetPostionMatrix() * RotateYMatrix( -AngleStep ) );
				break;
			case 'Y':
				sphere->SetPositionMatrix( sphere->GetPostionMatrix() * RotateYMatrix( AngleStep ) );
				break;
			case 'H':
				sphere->SetPositionMatrix( sphere->GetPostionMatrix() * RotateYMatrix( -AngleStep ) );
				break;
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			D3DXMATRIX viewMatrix = ViewMatrix( pSpectatorCoords->GetCartesianCoords(),
												 D3DXVECTOR3(0.0f, 0.0f, 0.0f),
												 D3DXVECTOR3(0.0f, 1.0f, 0.0f)) ;
			cylinder->SetViewMatrix( viewMatrix );
			sphere->SetViewMatrix( viewMatrix );
			plain->SetViewMatrix( viewMatrix );
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