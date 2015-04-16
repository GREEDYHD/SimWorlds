#ifndef _PARTICLESPAWNER_H_
#define _PARTICLESPAWNER_H_
#include <dinput.h>
#include <vector>
#include "GameData.h"
#include "Particle.h"
#include "ParticleType.h"

using namespace std;

class Particle;
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
	float* GetGravity();
	void SetGravity(float _modifier);

protected:
	int maxParticles;
	bool usingCircularOrbits;
	vector<Particle *> Particles;
	Vector3 temp;
	bool m_runningSimulation;
	int currentParticle;
	float m_Gravity;
	int burn;
	
};
#endif