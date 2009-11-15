#pragma once
#include "graphics.h"

#include <vector>
#include <d3dx9.h>
#include "light.h"



class Plane
{
public:
	struct Vertex;

	Plane( D3D::GraphicDevice& device,
		   const Material& material );
	void Draw(const Lights& lights);
	void SetViewMatrix(const D3DXMATRIX& viewMatrix);
	void SetProjectiveMatrix(const D3DXMATRIX& projectionMatrix);

	virtual ~Plane();
private:
	D3D::GraphicDevice device_;
	D3D::VertexDeclaration vertexDeclaration_;
	D3D::VertexBuffer<Vertex> vertexBuffer_;
	D3D::IndexBuffer indexBuffer_;
	D3D::Shader shader_;

	D3DXMATRIX viewMatrix_, projectiveMatrix_;
	unsigned nVertices_;
	unsigned nPrimitives_;
	const Material material_;
};

struct Plane::Vertex
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;

	Vertex() {}
	Vertex( float rx, float ry, float rz,
			float nx, float ny, float nz )
		:position(rx, ry, rz),
		 normal(nx, ny, nz)
	{
	}
};