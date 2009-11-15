#include "stdafx.h"
#include "light.h"


Material::Material( D3DXCOLOR ambient, D3DXCOLOR emissive,
					D3DXCOLOR diffuse, D3DXCOLOR specular, float specularExp )
	  : ambient_(ambient),
	    emissive_(emissive),
		diffuse_(diffuse),
		specular_(specular),
		specularExp_(specularExp)
{
}
Material::Material( D3DXCOLOR colors[], float specularExp )
	  : ambient_(colors[ambient]),
	    emissive_(colors[emissive]),
		diffuse_(colors[diffuse]),
		specular_(colors[specular]),
		specularExp_(specularExp)
{
}

void Material::SetMaterial(D3D::Shader &shader) const
{
	shader.SetConstantF(63, specularExp_);
	shader.SetConstantF(64, ambient_, 4);
}



DirectionalLight::DirectionalLight( const D3DXVECTOR3& direct, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular, float exp )
	:direct_(direct, 0), diffuse_(diffuse), specular_(specular), specularConst_(exp, exp, exp, exp)
{
	D3DXVec4Normalize(&direct_, &direct_);
}
void DirectionalLight::Set(D3D::Shader &shader) const
{
	shader.SetConstantF( startRegister, direct_, nVectors );
}



PointLight::PointLight( const D3DXVECTOR3& position, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular,
						float a, float b, float c )
	:position_(position, 0.0f), diffuse_(diffuse), specular_(specular),
	 attenuation_(a, b, c, 0.0f)
{
}
void PointLight::Set(D3D::Shader &shader) const
{
	shader.SetConstantF( startRegister, position_, nVectors );
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
void SpotLight::Set(D3D::Shader &shader) const
{
	shader.SetConstantF( startRegister, position_, nVectors );
}



Lights::Lights(const D3DXCOLOR& ambient, const DirectionalLight& directionalLight,
			   const PointLight& pointLight, const SpotLight& spotLight)
	:ambient_(ambient), directionalLight_(directionalLight),
	 pointLight_(pointLight), spotLight_(spotLight)
{
}
void Lights::SetLights(D3D::Shader& shader) const
{
	shader.SetConstantF(68, ambient_, 1);
	shader.SetConstantF(69, eye_, 1);

	directionalLight_.Set(shader);
	pointLight_.Set(shader);
	spotLight_.Set(shader);
}
void Lights::SetEye(const D3DXVECTOR3 &eye)
{
	eye_ = D3DXVECTOR4( eye, 0 );
}



