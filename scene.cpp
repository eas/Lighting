#include "stdafx.h"
#include "scene.h"
#include "colors.h"
#include "matrices.h"

namespace
{
	namespace CylinderConstants
	{
		const unsigned nPointsPerCircle = 32;
		const unsigned nPointsPerGeneratrix = 16;
		const float Height = 13.0f;
		const float Radius = 4.0f;
		const float Freq = 1.00f;
		const float MaxAngle = D3DX_PI / 4;

		const Material material( Colors::Black,
								 Colors::Black,
								 Colors::White,
								 Colors::White,
								 5.0f );
		const D3DXMATRIX positionMatrix = TranslationMatrix(0, -Height/2, 0);
	}

	namespace SphereConstants
	{
		const float Radius = 8.0f;
		const unsigned TesselationLevel = 5;
		const float Freq = 1.0f;

		const Material material( Colors::Black,
								 Colors::Black,
								 Colors::White,
								 Colors::White,
								 5.0f );

		const D3DXMATRIX positionMatrix = TranslationMatrix( -2*Radius - 2*Radius, 0.0f, 0.0f );
	}

	namespace PlaneConstants
	{
		const Material material( Colors::Black,
								 Colors::Black,
								 Colors::White,
								 Colors::White,
								 5.0f );
	}

	namespace LightingConstants
	{
		const DirectionalLight directionalLight( D3DXVECTOR3(0.0f, -1.0f, 0.0f),
												 Colors::Red / 3.0f, 
												 Colors::Red / 3.0f );
		const PointLight pointLight( D3DXVECTOR3(-8.0f, -6.0f, -2.0f),
									 Colors::Green / 3.0f,
									 Colors::Green / 3.0f,
									 1.0f, 0.00f, 0.0f );
		const SpotLight spotLight( D3DXVECTOR3(12.0f, -5.0f, 0.0f),
								   Colors::Blue / 3.0f,
								   Colors::Blue / 3.0f,
								   1.0f, 0.0f, 0.000f,
								   6*D3DX_PI/64, 16*D3DX_PI/64,
								   D3DXVECTOR3(-12.0f, 5.0f, 0.0f) );
		
		const D3DXCOLOR ambient = Colors::Black;
	}
}

Scene::Scene(D3D::GraphicDevice& device)
	: cylinder_( CylinderConstants::nPointsPerCircle,
				 CylinderConstants::nPointsPerGeneratrix,
				 CylinderConstants::Height,
				 CylinderConstants::Radius,
				 device,
				 CylinderConstants::Freq,
				 CylinderConstants::MaxAngle,
				 CylinderConstants::material ),
	  sphere_( SphereConstants::Radius,
			   SphereConstants::TesselationLevel,
			   device,
			   SphereConstants::Freq,
			   SphereConstants::material ),
	  plane_( device,
			  PlaneConstants::material ),
	  lights_( LightingConstants::ambient,
			   LightingConstants::directionalLight,
			   LightingConstants::pointLight,
			   LightingConstants::spotLight )
{
	cylinder_.SetPositionMatrix( CylinderConstants::positionMatrix );
	sphere_.SetPositionMatrix( SphereConstants::positionMatrix );
}

void Scene::SetEye( const D3DXVECTOR3& eye )
{
	lights_.SetEye( eye );

	const D3DXMATRIX viewMatrix = ViewMatrix( eye,
											  D3DXVECTOR3(0.0f, 0.0f, 0.0f),
											  D3DXVECTOR3(0.0f, 1.0f, 0.0f) );
	cylinder_.SetViewMatrix( viewMatrix );
	sphere_.SetViewMatrix( viewMatrix );
	plane_.SetViewMatrix( viewMatrix );
}
void Scene::SetProjectiveMatrix( const D3DXMATRIX& projectiveMatrix )
{
	cylinder_.SetProjectiveMatrix( projectiveMatrix );
	sphere_.SetProjectiveMatrix( projectiveMatrix );
	plane_.SetProjectiveMatrix( projectiveMatrix );
}
void Scene::Draw()
{
	cylinder_.Draw(lights_);
	sphere_.Draw(lights_);
	//plane_.Draw(lights_);
}
void Scene::ChangeDirectional()
{
	lights_.ChangeDirectional();	
}
void Scene::ChangePoint()
{
	lights_.ChangePoint();	
}
void Scene::ChangeSpot()
{
	lights_.ChangeSpot();	
}