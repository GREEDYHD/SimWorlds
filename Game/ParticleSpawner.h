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
	



protected:
	int maxParticles;
	vector<VBSphere *> Particles;
	Vector3 temp;
	bool m_runningSimulation;
	int angleA, angleB;
};



#endif