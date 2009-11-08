#include "stdafx.h"
#include "light.h"

Lights::Lights(const D3DXCOLOR& ambient, const DirectionalLight& directionalLight,
			   const PointLight& pointLight, const SpotLight& spotLight)
	:ambient_(ambient), directionalLight_(directionalLight), pointLight_(pointLight), spotLight_(spotLight)
{
}

void Lights::SetLights(D3D::Shader& shader) const
{
	shader.SetConstantF(68, ambient_, 1);
	shader.SetConstantF(69, eye_, 1);
	shader.SetConstantF(72, directionalLight_, DirectionalLight::nVectors);
	shader.SetConstantF(76, pointLight_, PointLight::nVectors);
	shader.SetConstantF(84, spotLight_, SpotLight::nVectors);
}
void Lights::SetEye(const D3DXVECTOR3 &eye)
{
	eye_ = D3DXVECTOR4( eye, 0 );
}
PointLight::PointLight( const D3DXVECTOR3& position, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular,
						float a, float b, float c )
	:position_(position, 0.0f), diffuse_(diffuse), specular_(specular),
	 attenuation_(a, b, c, 0.0f)
{
}

SpotLight::SpotLight( const D3DXVECTOR3& position, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular,
					  float a, float b, float c, float innerAngle, float outerAngle,
					  const D3DXVECTOR3& direct )
	:position_(position, 0.0f), diffuse_(diffuse), specular_(specular),
	 attenuation_(a, b, c, 0.0f),
	 direct_(direct, 0)
{
	assert( innerAngle < outerAngle );
	assert( outerAngle < D3DX_PI / 2 );
	assert( innerAngle > 0 );

	D3DXVec4Normalize(&direct_, &direct_);
	float alpha = cosf(innerAngle);
	float beta = cosf(outerAngle);
	angles_ = D3DXVECTOR4( -beta/(alpha-beta), 1/(alpha - beta), alpha, beta );
}
