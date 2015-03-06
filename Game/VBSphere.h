#ifndef _VB_SPHERE_H_
#define _VB_SPHERE_H_
#include "VBGO.h"
#include "vertex.h"
#include "gamedata.h"

class VBSphere : public VBGO
{
public:
	VBSphere(){};
	VBSphere(ID3D11Device* pd3dDevice);
	~VBSphere();

	void init();
	void Tick(GameData* GD, Vector3 _ParticlesPosition);
	void orbit(float _time);
	void Spawn();

protected:

	virtual void Transform()
	{
		for (int i = 0; i < m_numPrims * 3; i++)
		{
			m_vertices[i].Color *= ((i / 3) % 2) ? 1.0f : 0.5f;
			m_vertices[i].Color.w = 1.0f;

			Vector3 vertPosition = m_vertices[i].Position;
			Vector3 spherePosition = m_vertices[i].Position;

			spherePosition.Normalize();

			m_vertices[i].Position = 3.0f * spherePosition;
		}
	}
	VBSphere* m_parent;
	Vector3 m_point;
	int m_size;
	myVertex* m_vertices;
	float time;
	float orbitalDistance;
	Vector3 m_Force;
	const double GravitaionalConstant = 10;// 0.000000000066742;
	Vector3 m_XYZ_Distance;
	float m_distanceSquared;
	float m_GForce;
	Vector3 m_Offset;
};

#endif