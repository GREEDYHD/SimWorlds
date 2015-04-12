#ifndef _VB_SPHERE_H_
#define _VB_SPHERE_H_
#include "VBGO.h"
#include "Trails.h"
#include "vertex.h"
#include "gamedata.h"
#include <vector>
using namespace std;
class Trails;
class VBSphere : public VBGO
{
public:
	VBSphere(ID3D11Device* pd3dDevice, Vector3 _position);
	~VBSphere();

	void Tick(GameData* GD);
	void Draw(DrawData* DD);
	void Spawn(Vector3 _position);
	void DeSpawn();
	bool isAlive();
	void CalculateVelocity(Vector3 _ParticlesPosition, int _ParticlesMass, Vector3 _ParticlesVelocity);
	void CirculariseOrbit(Vector3 _ParticlesPosition);
	void SetNewVelocity();
	Vector3 GetVelocity();
	Color GetColor();
	int GetMass();

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

	int m_size;
	int m_mass;
	myVertex* m_vertices;
	float m_distanceSquared;
	float m_GForce;
	vector<Trails*> SphereTrails;
	int trailNumber;
	int currentTrailNumber;
	bool m_alive;
	Vector3 m_NewVelocity;

};

#endif