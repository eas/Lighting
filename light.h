#pragma once
#include "graphics.h"

struct Material
{
	D3DXCOLOR ambient;
	D3DXCOLOR emissive;
	D3DXCOLOR diffuse;
	D3DXCOLOR specular;
	
	Material( D3DXCOLOR ambient, D3DXCOLOR emissive,
			  D3DXCOLOR diffuse, D3DXCOLOR specular )
		  : ambient(ambient),
		    emissive(emissive),
			diffuse(diffuse),
			specular(specular)
	{
	}
	operator float*()
	{
		return ambient;
	}
};

struct DirectionalLight
{
	DirectionalLight( const D3DXVECTOR3& direct, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular )
		:direct_(direct, 0), diffuse_(diffuse), specular_(specular)
	{
		D3DXVec4Normalize(&direct_, &direct_);
	}

	D3DXVECTOR4 direct_;
	D3DXCOLOR diffuse_;
	D3DXCOLOR specular_;
	operator float*()
	{
		return direct_;
	}
	operator const float*() const
	{
		return direct_;
	}
};


class Lights
{
public:
	Lights(const D3DXCOLOR& ambient, const DirectionalLight& directionalLight);
	void SetLights(D3D::Shader& shader) const;
	void SetEye(const D3DXVECTOR3& eye);
private:
	Lights(const Lights&);
	Lights& operator=(const Lights&);

	D3DXCOLOR ambient_;
	D3DXVECTOR3 eye_;
	DirectionalLight directionalLight_;
};
