#include "ParticleSpawner.h"
#include "GameData.h"


ParticleSpawner::ParticleSpawner(ID3D11Device* _pd3dDevice)
{
	m_Position = Vector3(0.0f, 0.0f, 0.0f);
	maxParticles = 30;
	m_run = false;
	temp = Vector3(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < maxParticles; i++)
	{
		Particles.push_back(new VBSphere(_pd3dDevice));
	}
}
ParticleSpawner::~ParticleSpawner()
{

}
void ParticleSpawner::Create(GameData* GD)
{
	if (GD->keyboard[DIK_A] & 0x80 )
	{
		m_run = true;
		temp = Vector3(0, 0, 0);
		for (vector<VBSphere *>::iterator it = Particles.begin(); it != Particles.end(); it++)
		{
			(*it)->Spawn();
		}
	}
}
void ParticleSpawner::Draw(DrawData* DD)
{
	if (m_run)
	{
		for (vector<VBSphere *>::iterator it = Particles.begin(); it != Particles.end(); it++)
		{
			(*it)->Draw(DD);
		}
	}
}
void ParticleSpawner::Tick(GameData* GD)
{	
	if (m_run)
	{
		//temp = Vector3(0, 0, 0);
		for (vector<VBSphere *>::iterator it = Particles.begin(); it != Particles.end(); it++)
		{
			for (vector<VBSphere *>::iterator it = Particles.begin(); it != Particles.end(); it++)
			{
				temp = XMVectorAdd((*it)->GetPos(), temp);
			}
			(*it)->Tick(GD, temp);
		}
	}
	Create(GD);
}