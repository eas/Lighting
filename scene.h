#pragma once
#include "cylinder.h"
#include "sphere.h"
#include "plane.h"
#include "light.h"
#include "graphics.h"

class Scene
{
public:
	Scene(D3D::GraphicDevice& device);
	void SetProjectiveMatrix( const D3DXMATRIX& projectiveMatrix );
	void SetEye( const D3DXVECTOR3& eye );
	void Draw();
private:
	Scene(const Scene&);
	Scene& operator=(const Scene&);

	Cylinder cylinder_;
	Sphere sphere_;
	Plane plane_;
	Lights lights_;
};