#pragma once
#include "graphics.h"
#include "light.h"

class Sphere
{
public:
	struct Vertex;

	Sphere( float radius, unsigned tesselationLevel, D3D::GraphicDevice device, float freq,
			D3DXCOLOR ambient, D3DXCOLOR emissive,
			D3DXCOLOR diffuse, D3DXCOLOR specular,
			float specularExp );
	void Draw(const Lights& lights);
	void SetPositionMatrix(const D3DXMATRIX& positionMatrix);
	void SetViewMatrix(const D3DXMATRIX& viewMatrix);
	void SetProjectiveMatrix(const D3DXMATRIX& projectionMatrix);
	D3DXMATRIX GetPostionMatrix() const
	{
		return positionMatrix_;
	}
	virtual ~Sphere();
private:
	Sphere(const Sphere&);
	Sphere& operator=(const Sphere&);

private:
	D3D::GraphicDevice device_;
	D3D::VertexDeclaration vertexDeclaration_;
	D3D::VertexBuffer<Vertex> vertexBuffer_;
	D3D::IndexBuffer indexBuffer_;
	D3D::Shader shader_;

	D3DXMATRIX positionMatrix_, viewMatrix_, projectiveMatrix_;
	unsigned nVertices_;
	unsigned nPrimitives_;
	const float radius_;
	const unsigned tesselationLevel_;
	const float freq_;
	const Material material_;
};

struct Sphere::Vertex
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;

	Vertex() {}
	Vertex( float x, float y, float z,
			float nx, float ny, float nz )
		:position(x, y, z), normal(nx, ny, nz)
	{
		D3DXVec3Normalize( &(this->normal), &(this->normal) );
	}
	Vertex( const D3DXVECTOR3& position, const D3DXVECTOR3& normal )
		:position(position), normal(normal) 
	{
		D3DXVec3Normalize( &(this->normal), &(this->normal) );
	}
};