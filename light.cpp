#include "stdafx.h"
#include "light.h"

Lights::Lights(const D3DXCOLOR& ambient, const DirectionalLight& directionalLight, const PointLight& pointLight)
	:ambient_(ambient), directionalLight_(directionalLight), pointLight_(pointLight)
{
}

void Lights::SetLights(D3D::Shader& shader) const
{
	shader.SetConstantF(68, ambient_, 1);
	shader.SetConstantF(69, eye_, 1);
	shader.SetConstantF(72, directionalLight_, DirectionalLight::nVectors);
	shader.SetConstantF(76, pointLight_, PointLight::nVectors);
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
					  float a, float b, float c, float innerAngle, float outerAngle )
	:position_(position, 0.0f), diffuse_(diffuse), specular_(specular),
	 attenuation_(a, b, c, 0.0f), angles_(innerAngle, outerAngle, 0, 0)
{
}
