#ifndef _PARTICLESPAWNER_H_
#define _PARTICLESPAWNER_H_
#include <dinput.h>
#include <vector>
#include "GameData.h"
#include "VBSphere.h"

using namespace std;

class VBSphere;
class ParticleSpawner : public GameObject
{
public:
	ParticleSpawner(ID3D11Device* _pd3dDevice);
	~ParticleSpawner();

	void Tick(GameData* GD);
	void Draw(DrawData* DD);
	void Create(GameData* GD);
	int* GetCurrentParticle();
	bool* GetParticleAliveState();
	Vector3* GetParticlePosition();
	Vector3* GetParticleVelocity();
	Color* GetParticleColor();
	int* GetMaxParticles();
	int* GetParticleMass();
	Vector2 GetMousePosition();
protected:
	int maxParticles;
	bool usingCircularOrbits;
	vector<VBSphere *> Particles;
	Vector3 temp;
	bool m_runningSimulation;
	int currentParticle;
	
};
#endif