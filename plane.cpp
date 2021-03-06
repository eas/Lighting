#include "stdafx.h"

#include "plane.h"


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


typedef Plane::Vertex Vertex;
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
	
	const float edgeSize = 100.0f;
	const unsigned nVerticesPerEdge = 201;
	for( unsigned i=0; i<nVerticesPerEdge; ++i )
	{
		for( unsigned j=0; j<nVerticesPerEdge; ++j )
		{
			vertices.push_back(
				Vertex( 0.0f, edgeSize*i/(nVerticesPerEdge-1) - edgeSize/2, edgeSize*j/(nVerticesPerEdge-1) - edgeSize/2,
				1.0f, 0.0f, 0.0f ) );
		}
	}

	for( unsigned i=0; i<nVerticesPerEdge-1; ++i )
	{
		for( unsigned j=0; j<nVerticesPerEdge-1; ++j )
		{
			indices.push_back( (i)*nVerticesPerEdge + (j) );
			indices.push_back( (i+1)*nVerticesPerEdge + (j) );
			indices.push_back( (i+1)*nVerticesPerEdge + (j+1) );

			indices.push_back( (i)*nVerticesPerEdge + (j) );
			indices.push_back( (i+1)*nVerticesPerEdge + (j+1) );
			indices.push_back( (i)*nVerticesPerEdge + (j+1) );
		}
	}
}
Plane::Plane( D3D::GraphicDevice &device,
			  const Material& material )
	: device_(device),
	  vertexDeclaration_(device, DefaultVertexDeclaration),
	  vertexBuffer_(device),
	  indexBuffer_(device),
	  shader_(device, L"plane.vsh"),
	  material_(material)
{
	Vertices vertices;
	Indices indices;
	InitVertices( vertices, indices );

	nVertices_ = vertices.size();
	nPrimitives_ = indices.size()/3;
	vertexBuffer_.SetVertices( &vertices[0], vertices.size() );
	indexBuffer_.SetIndices( &indices[0], indices.size() );
	SetViewMatrix( UnityMatrix() );
	SetProjectiveMatrix( UnityMatrix() );
}
Plane::~Plane()
{
}
void Plane::Draw(const Lights& lights)
{
	vertexBuffer_.Use(0,0);
	indexBuffer_.Use();
	vertexDeclaration_.Use();
	shader_.Use();

	shader_.SetMatrix( projectiveMatrix_*viewMatrix_, 0 );
	material_.SetMaterial(shader_);
	lights.SetLights(shader_);
	vertexDeclaration_.Use();

	device_->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, nVertices_, 0, nPrimitives_);
}

void Plane::SetViewMatrix(const D3DXMATRIX& viewMatrix)
{
	viewMatrix_ = viewMatrix;
}
void Plane::SetProjectiveMatrix(const D3DXMATRIX& projectiveMatrix)
{
	projectiveMatrix_ = projectiveMatrix;
}
