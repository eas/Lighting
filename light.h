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
	DirectionalLight( const D3DXVECTOR3& direct, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular, float exp )
		:direct_(direct, 0), diffuse_(diffuse), specular_(specular), specularConst_(exp, exp, exp, exp)
	{
		D3DXVec4Normalize(&direct_, &direct_);
	}

	D3DXVECTOR4 direct_;
	D3DXCOLOR diffuse_;
	D3DXCOLOR specular_;
	D3DXVECTOR4 specularConst_;
	operator float*()
	{
		return direct_;
	}
	operator const float*() const
	{
		return direct_;
	}

	static const unsigned nVectors = 4;
};
struct PointLight
{
	PointLight( const D3DXVECTOR3& position, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular,
				float a, float b, float c, float exp );
	D3DXVECTOR4 position_;
	D3DXCOLOR diffuse_;
	D3DXCOLOR specular_;
	D3DXVECTOR4 attenuation_; // (a,b,c,0) 1/(a+b*d+c*d^2)
	D3DXVECTOR4 specularConst_;
	operator float*()
	{
		return position_;
	}
	operator const float*() const
	{
		return position_;
	}

	static const unsigned nVectors = 5;
};
struct SpotLight
{
	SpotLight( const D3DXVECTOR3& position, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular,
			   float a, float b, float c, float innerAngle, float outerAngle,
			   const D3DXVECTOR3& direct, float exp );

	D3DXVECTOR4 position_;
	D3DXCOLOR diffuse_;
	D3DXCOLOR specular_;
	D3DXVECTOR4 attenuation_; // (a,b,c,0) 1/(a+b*d+c*d^2)
	D3DXVECTOR4 angles_;
	D3DXVECTOR4 direct_;
	D3DXVECTOR4 specularConst_;
	operator float*()
	{
		return position_;
	}
	operator const float*() const
	{
		return position_;
	}

	static const unsigned nVectors = 7;
};


class Lights
{
public:
	Lights(const D3DXCOLOR& ambient, const DirectionalLight& directionalLight,
		   const PointLight& pointLight, const SpotLight& spotLight);
	void SetLights(D3D::Shader& shader) const;
	void SetEye(const D3DXVECTOR3& eye);
private:
	Lights(const Lights&);
	Lights& operator=(const Lights&);

	D3DXCOLOR ambient_;
	D3DXVECTOR4 eye_;
	DirectionalLight directionalLight_;
	PointLight pointLight_;
	SpotLight spotLight_;
};
