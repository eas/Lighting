#pragma once
#include "graphics.h"
#include <vector>

class Material
{
public:
	Material( D3DXCOLOR ambient, D3DXCOLOR emissive,
			  D3DXCOLOR diffuse, D3DXCOLOR specular, float specularExp );
	Material( D3DXCOLOR colors[], float specularExp );

	void SetMaterial(D3D::Shader& shader) const;

	enum Colors{ ambient, emissive, diffuse, specular };
private:
	float specularExp_;
	D3DXCOLOR ambient_;
	D3DXCOLOR emissive_;
	D3DXCOLOR diffuse_;
	D3DXCOLOR specular_;
};

class DirectionalLight
{
public:
	DirectionalLight( const D3DXVECTOR3& direct, const D3DXCOLOR& diffuse,
					  const D3DXCOLOR& specular );

	void Set(D3D::Shader& shader) const;
	void ChangeState();
private:
	D3DXVECTOR4 direct_;
	D3DXCOLOR diffuse_;
	D3DXCOLOR specular_;
	bool isActive_;

	static const unsigned nVectors = 3;
	static const unsigned startRegister = 72;
};

class PointLight
{
public:
	PointLight( const D3DXVECTOR3& position, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular,
				float a, float b, float c );
	void Set(D3D::Shader& shader) const;
	void ChangeState();

private:
	D3DXVECTOR4 position_;
	D3DXCOLOR diffuse_;
	D3DXCOLOR specular_;
	D3DXVECTOR4 attenuation_; // (a,b,c,0) 1/(a+b*d+c*d^2)
	bool isActive_;

	static const unsigned nVectors = 4;
	static const unsigned startRegister = 76;
};
class SpotLight
{
public:
	SpotLight( const D3DXVECTOR3& position, const D3DXCOLOR& diffuse, const D3DXCOLOR& specular,
			   float a, float b, float c, float innerAngle, float outerAngle,
			   const D3DXVECTOR3& direct );
	void Set(D3D::Shader& shader) const;
	void ChangeState();

private:
	D3DXVECTOR4 position_;
	D3DXCOLOR diffuse_;
	D3DXCOLOR specular_;
	D3DXVECTOR4 attenuation_; // (a,b,c,0) 1/(a+b*d+c*d^2)
	D3DXVECTOR4 angles_;
	D3DXVECTOR4 direct_;
	bool isActive_;

	static const unsigned nVectors = 6;
	static const unsigned startRegister = 84;
};


class Lights
{
public:
	Lights(const D3DXCOLOR& ambient, const DirectionalLight& directionalLight,
		   const PointLight& pointLight, const SpotLight& spotLight);
	void SetLights(D3D::Shader& shader) const;
	void SetEye(const D3DXVECTOR3& eye);
	
	void ChangeDirectional();
	void ChangePoint();
	void ChangeSpot();
private:
	Lights(const Lights&);
	Lights& operator=(const Lights&);

	D3DXCOLOR ambient_;
	D3DXVECTOR4 eye_;
	DirectionalLight directionalLight_;
	PointLight pointLight_;
	SpotLight spotLight_;
};
