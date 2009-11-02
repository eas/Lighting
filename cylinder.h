#pragma once
#include "graphics.h"

#include <vector>
#include <d3dx9.h>
#include "light.h"



class Cylinder
{
public:
	struct Vertex;

	Cylinder( unsigned nPointsPerCircle, unsigned nPointsPerGeneratrix,
			  float height, float R,
			  D3D::GraphicDevice& device, float freq, float maxAngle,
		      D3DXCOLOR ambient, D3DXCOLOR emissive, D3DXCOLOR diffuse, D3DXCOLOR specular );
	void Draw(const Lights& lights);
	void SetPositionMatrix(const D3DXMATRIX& positionMatrix);
	void SetViewMatrix(const D3DXMATRIX& viewMatrix);
	void SetProjectiveMatrix(const D3DXMATRIX& projectionMatrix);
	D3DXMATRIX GetPostionMatrix() const
	{
		return positionMatrix_;
	}

	virtual ~Cylinder();
private:
	D3D::GraphicDevice device_;
	D3D::VertexDeclaration vertexDeclaration_;
	D3D::VertexBuffer<Vertex> vertexBuffer_;
	D3D::IndexBuffer indexBuffer_;
	D3D::Shader shader_;

	D3DXMATRIX positionMatrix_, viewMatrix_, projectiveMatrix_;
	unsigned nVertices_;
	unsigned nPrimitives_;
	const float freq_;
	const float maxAngle_;
	const Material material_;
};

struct Cylinder::Vertex
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	float weight1;
	float weight2;

	Vertex() {};
	Vertex( float rx, float ry, float rz,
			float nx, float ny, float nz,
			float weight1, float weight2)
		:position(rx, ry, rz),
		 normal(nx, ny, nz),
		 weight1(weight1), weight2(weight2) {}
};