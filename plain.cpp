#include "stdafx.h"

#include "plain.h"


#include "graphics.h"
#include "colors.h"
#include "matrices.h"

#include <vector>
#include <assert.h>
#include <algorithm>
#include <time.h>


#ifndef NDEBUG
	#define new new( _CLIENT_BLOCK, __FILE__, __LINE__)
#endif


typedef Plain::Vertex Vertex;
typedef std::vector<Vertex> Vertices;
typedef std::vector<D3D::Index> Indices;

static const D3DVERTEXELEMENT9 DefaultVertexDeclaration[] =  {
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		D3DDECL_END()
	};

void InitVertices( Vertices& vertices, Indices& indices )
{
	assert( 0 == vertices.size() );
	assert( 0 == indices.size() );
	
	const float size = 100.0f;
	vertices.push_back( Vertex( 0.0f,  size,  size, 1.0f, 0.0f, 0.0f ) );
	vertices.push_back( Vertex( 0.0f, -size,  size, 1.0f, 0.0f, 0.0f ) );
	vertices.push_back( Vertex( 0.0f, -size, -size, 1.0f, 0.0f, 0.0f ) );
	vertices.push_back( Vertex( 0.0f,  size, -size, 1.0f, 0.0f, 0.0f ) );

	indices.push_back( 3 );
	indices.push_back( 0 );
	indices.push_back( 2 );
	indices.push_back( 1 );
}
Plain::Plain( D3D::GraphicDevice &device,
			  D3DXCOLOR ambient, D3DXCOLOR emissive, D3DXCOLOR diffuse, D3DXCOLOR specular)
	: device_(device),
	  vertexDeclaration_(device, DefaultVertexDeclaration),
	  vertexBuffer_(device),
	  indexBuffer_(device),
	  shader_(device, L"plain.vsh"),
	  material_(ambient, emissive, diffuse, specular)
{
	Vertices vertices;
	Indices indices;
	InitVertices( vertices, indices );

	nVertices_ = vertices.size();
	nPrimitives_ = indices.size() - 2;
	vertexBuffer_.SetVertices( &vertices[0], vertices.size() );
	indexBuffer_.SetIndices( &indices[0], indices.size() );
	SetPositionMatrix( UnityMatrix() );
	SetViewMatrix( UnityMatrix() );
	SetProjectiveMatrix( UnityMatrix() );
}
Plain::~Plain()
{
}
void Plain::Draw(const Lights& lights)
{
	vertexBuffer_.Use(0,0);
	indexBuffer_.Use();
	vertexDeclaration_.Use();
	shader_.Use();

	shader_.SetMatrix( projectiveMatrix_*viewMatrix_, 0 );
	shader_.SetConstantF(64, material_, 4);

	lights.SetLights(shader_);
	vertexDeclaration_.Use();

	device_->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, nVertices_, 0, nPrimitives_);
}
void Plain::SetPositionMatrix(const D3DXMATRIX& positionMatrix)
{
	positionMatrix_ = positionMatrix;
}
void Plain::SetViewMatrix(const D3DXMATRIX& viewMatrix)
{
	viewMatrix_ = viewMatrix;
}
void Plain::SetProjectiveMatrix(const D3DXMATRIX& projectiveMatrix)
{
	projectiveMatrix_ = projectiveMatrix;
}
