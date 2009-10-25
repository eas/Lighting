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
	shader.SetConstantF(72, directionalLight_, 3);
}
void Lights::SetEye(const D3DXVECTOR3 &eye)
{
	eye_ = D3DXVECTOR4( eye, 0 );
}