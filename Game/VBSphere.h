#ifndef _VB_SPHERE_H_
#define _VB_SPHERE_H_
#include "VBGO.h"
#include "vertex.h"
#include "gamedata.h"

class VBSphere : public VBGO
{
public:
	VBSphere(){};
	virtual ~VBSphere(){};

	void init(Vector3 _pos, int _size, ID3D11Device* GD, Vector3 _point);
	void Tick(GameData* GD);
	void orbit(float _time);

protected:

	virtual void Transform()
	{
		for (int i = 0; i < m_numPrims * 3; i++)
		{
			m_vertices[i].Color *= ((i / 3) % 2) ? 1.0f : 0.5f;
			m_vertices[i].Color.w = 1.0f;

			Vector3 vertPos = m_vertices[i].Pos;
			Vector3 spherePos = m_vertices[i].Pos;

			spherePos.Normalize();

			m_vertices[i].Pos = 3.0f * spherePos;
		}
	}
	VBSphere* m_parent;
	Vector3 m_point;
	int m_size;
	myVertex* m_vertices;
};

#endif