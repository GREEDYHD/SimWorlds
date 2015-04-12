//add some light to the scene
#include "light.h"
#include "gamedata.h"
//a simple light defined by a colour and and ambient colour and used as a point light with no drop off in the standard shader used to draw VBGOs

Light::Light(Vector3 _Position, Color _colour, Color _ambientColour)
{
	m_Position = _Position;
	m_colour = _colour;
	m_ambientColour = _ambientColour;
}

Light::~Light()
{

}

void Light::Tick(GameData* _GD)
{

}