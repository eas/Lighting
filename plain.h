#pragma once
#include "graphics.h"

#include <vector>
#include <d3dx9.h>
#include "light.h"



class Plain
{
public:
	struct Vertex;

	Plain( D3D::GraphicDevice& device,
		   D3DXCOLOR ambient, D3DXCOLOR emissive, D3DXCOLOR diffuse, D3DXCOLOR specular );
	void Draw(const Lights& lights);
	void SetPositionMatrix(const D3DXMATRIX& positionMatrix);
	void SetViewMatrix(const D3DXMATRIX& viewMatrix);
	void SetProjectiveMatrix(const D3DXMATRIX& projectionMatrix);

	virtual ~Plain();
private:
	D3D::GraphicDevice device_;
	D3D::VertexDeclaration vertexDeclaration_;
	D3D::VertexBuffer<Vertex> vertexBuffer_;
	D3D::IndexBuffer indexBuffer_;
	D3D::Shader shader_;

	D3DXMATRIX positionMatrix_, viewMatrix_, projectiveMatrix_;
	unsigned nVertices_;
	unsigned nPrimitives_;
	Material material_;
};

struct Plain::Vertex
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