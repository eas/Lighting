#pragma once
#include "graphics.h"

#include <vector>
#include <d3dx9.h>



class Cylinder
{
public:
	struct Vertex;

	Cylinder( unsigned nPointsPerCircle, unsigned nPointsPerGeneratrix,
			  float height, float R,
			  D3D::GraphicDevice& device, float freq, float maxAngle );
	void Draw();
	void SetPositionMatrix(const D3DXMATRIX& positionMatrix);
	void SetViewMatrix(const D3DXMATRIX& viewMatrix);
	void SetProjectiveMatrix(const D3DXMATRIX& projectionMatrix);

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
	//vertexDeclaration.Use();
};

struct Cylinder::Vertex
{
	float x, y, z;
	DWORD color;
	float weight1;
	float weight2;

	Vertex() {};
	Vertex( float x, float y, float z, DWORD color, float weight1, float weight2)
		:x(x), y(y), z(z), color(color), weight1(weight1), weight2(weight2) {}
};