#include "stdafx.h"

#include "cylinder.h"


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

typedef Cylinder::Vertex Vertex;
typedef std::vector<Vertex> Vertices;
typedef std::vector<D3D::Index> Indices;

static const D3DVERTEXELEMENT9 DefaultVertexDeclaration[] =  {
		{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
		{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};

void InitVertices(	Vertices& vertices, Indices& indices, 
					unsigned nPointsPerCircle, unsigned nPointsPerGeneratrix,
					float height, float Radius )
{
	assert( 2 < nPointsPerCircle );
	assert( 2 < nPointsPerGeneratrix );
	assert( 0 < height );
	assert( 0 < Radius );

	const float angleStep = 2*D3DX_PI / nPointsPerCircle;
	const float heightStep = height / (nPointsPerGeneratrix-1);

	float curHeight = 0;
	for( unsigned i = 0; i<nPointsPerGeneratrix; ++i )
	{
		float angle = 0;
		for( unsigned j = 0; j<nPointsPerCircle; ++j )
		{
			vertices.push_back( Vertex(	Radius*cosf(angle), curHeight, Radius*sinf(angle),
										cosf(angle), 0, sinf(angle),
										curHeight/height, 1-curHeight/height) );
			angle += angleStep;
		}
		curHeight+=heightStep;
	}

	for( unsigned level = 0; level<nPointsPerGeneratrix-1; ++level )
	{
		for( unsigned i = 0; i< nPointsPerCircle; ++i )
		{
			indices.push_back(level*nPointsPerCircle + i);
			indices.push_back((level+1)*nPointsPerCircle + i);
		}
		indices.push_back(level*nPointsPerCircle);
	}
	indices.push_back((nPointsPerGeneratrix-1)*nPointsPerCircle);
}

Cylinder::Cylinder(unsigned int nPointsPerCircle, unsigned int nPointsPerGeneratrix, 
				   float height, float Radius, D3D::GraphicDevice &device, float freq, float maxAngle,
				   D3DXCOLOR ambient, D3DXCOLOR emissive,
				   D3DXCOLOR diffuse, D3DXCOLOR specular,
				   float specularExp)
	: device_(device),
	  vertexDeclaration_(device, DefaultVertexDeclaration),
	  vertexBuffer_(device),
	  indexBuffer_(device),
	  shader_(device, L"cylinder.vsh"),
	  freq_(freq),
	  maxAngle_(maxAngle),
	  material_(ambient, emissive, diffuse, specular, specularExp)
{
	Vertices vertices;
	Indices indices;
	InitVertices( vertices, indices, nPointsPerCircle, nPointsPerGeneratrix,
				  height, Radius );

	nVertices_ = vertices.size();
	nPrimitives_ = indices.size() - 2;
	vertexBuffer_.SetVertices( &vertices[0], vertices.size() );
	indexBuffer_.SetIndices( &indices[0], indices.size() );
	SetPositionMatrix( UnityMatrix() );
	SetViewMatrix( UnityMatrix() );
	SetProjectiveMatrix( UnityMatrix() );
}
Cylinder::~Cylinder()
{
}
void Cylinder::Draw(const Lights& lights)
{
	float time = static_cast<float>(clock()) / CLOCKS_PER_SEC;
	float angle = (-cosf(freq_*time * (2*D3DX_PI)) + 1)/2 * maxAngle_;

	vertexBuffer_.Use(0,0);
	indexBuffer_.Use();
	vertexDeclaration_.Use();
	shader_.Use();
	shader_.SetMatrix( projectiveMatrix_*viewMatrix_, 0 );
	shader_.SetMatrix( positionMatrix_*RotateZMatrix( angle ), 4 );
	shader_.SetMatrix( positionMatrix_, 8);
	shader_.SetConstantF(64, material_, 4);
	shader_.SetConstantF(63, material_.specularExp);
	lights.SetLights(shader_);
	vertexDeclaration_.Use();

	device_->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, nVertices_, 0, nPrimitives_);
}
void Cylinder::SetPositionMatrix(const D3DXMATRIX& positionMatrix)
{
	positionMatrix_ = positionMatrix;
}
void Cylinder::SetViewMatrix(const D3DXMATRIX& viewMatrix)
{
	viewMatrix_ = viewMatrix;
}
void Cylinder::SetProjectiveMatrix(const D3DXMATRIX& projectiveMatrix)
{
	projectiveMatrix_ = projectiveMatrix;
}
