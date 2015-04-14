#include "game.h"
#include "gameobject.h"
#include "GameObject2D.h"
#include "ObjectList.h"
#include <windows.h>
#include "CommonStates.h"
#include "gamedata.h"
#include <time.h>
#include "helper.h"
#include "DDSTextureLoader.h"
#include "drawdata.h"
#include "DrawData2D.h"

using namespace DirectX;

extern HWND g_hWnd;

Game::Game(ID3D11Device* _pd3dDevice, HINSTANCE _hInstance) :m_playTime(0), m_myEF(nullptr)
{
	/* initialize random seed: */
	srand(time(NULL));

	m_pKeyboard = nullptr;
	m_pDirectInput = nullptr;

	//set up DirectXTK Effects system
	m_myEF  = new MyEffectFactory(_pd3dDevice);
#if DEBUG
	m_myEF->SetPath(L"Debug\\");
#else
	m_myEF->SetPath(L"Release\\");
#endif

	
	// Create other render resources here
	m_States=new DirectX::CommonStates(_pd3dDevice);

	//Direct Input Stuff
	HRESULT hr = DirectInput8Create(_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pDirectInput, NULL);
	hr = m_pDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboard, NULL);
	hr = m_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = m_pKeyboard->SetCooperativeLevel(g_hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	// initialize the mouse
	hr = m_pDirectInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	hr = m_pMouse->SetCooperativeLevel(g_hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	hr = m_pMouse->SetDataFormat(&c_dfDIMouse);

	m_GD = new GameData();
	m_GD->keyboard = m_keyboardState;
	m_GD->prevKeyboard = m_prevKeyboardState;
	m_GD->mouse = &m_mouse_state;
	m_GD->prevMouse = &m_prevMouseState;
	m_GD->GS = GS_PLAY_MAIN_CAM;


	//create a base camera
	m_cam = new Camera(0.25f * XM_PI, 1920.0f / 1080.0f, 1.0f, 10000.0f, Vector3::Zero, Vector3::UnitY);
	m_cam->SetPos( Vector3(0.0f, 0.0f,1300.0f));
	m_GameObjects.push_back(m_cam);

	/*VBSphere* Sun = new VBSphere(_pd3dDevice, Vector3(0.0f, 0.0f, 0.0f));
	m_GameObjects.push_back(Sun);
	Sun->SetPos(Vector3(0.0f, 0.0f, 0.0f));
	Sun->SetScale(1.0f);*/

	m_Light = new Light(Vector3(0.0f, 0.0f, 0.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Color(1.0f, 1.0f, 1.0f, 1.0f));
	m_GameObjects.push_back(m_Light);

	ParticleSpawner* spawner = new ParticleSpawner(_pd3dDevice);
	m_GameObjects.push_back(spawner);
	m_GD->pSpawner = spawner;

	m_TPSCam = new TPSCamera(0.25f * XM_PI, 1920.0f / 1080.0f, 1.0f, 10000.0f, m_Light, Vector3::UnitY, Vector3(0.0f, 0.0f, 3000.0f));
	m_GameObjects.push_back(m_TPSCam);

	ID3D11DeviceContext* pd3dImmediateContext;
	_pd3dDevice->GetImmediateContext(&pd3dImmediateContext);
	
	// Create DirectXTK spritebatch stuff
	m_DD2D = new DrawData2D();
	m_DD2D->m_Sprites.reset(new SpriteBatch(pd3dImmediateContext));
	m_DD2D->m_Font.reset(new SpriteFont(_pd3dDevice, L"DavesFont.spritefont"));
		
	//create Draw Data
	m_DD = new DrawData();
	m_DD->pd3dImmediateContext = pd3dImmediateContext;
	m_DD->states = m_States;
	m_DD->light = m_Light;

	//initilise the defaults for the VBGOs
	VBGO::Init(_pd3dDevice);
	simulationSpeed = 1;

}

Game::~Game()
{
	VBGO::CleanUp();

	//tidy away Direct Input Stuff
	if (m_pMouse)
	{
		m_pMouse->Unacquire();
		m_pMouse->Release();
	}
	if (m_pKeyboard)
	{
		m_pKeyboard->Unacquire();
		m_pKeyboard->Release();
	}
	if (m_pDirectInput) m_pDirectInput->Release();


	//get rid of the game objects here
	for (list<GameObject *>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		delete (*it);
	}

	m_GameObjects.clear();
	//and the 2D ones
	for (list<GameObject2D *>::iterator it = m_GameObject2Ds.begin(); it != m_GameObject2Ds.end(); it++)
	{
		delete (*it);
	}

	m_GameObject2Ds.clear();

	delete m_States;
	delete m_myEF;
	delete m_GD;
	delete m_DD;

}

bool Game::update()
{
	ReadKeyboard();
	ReadMouse();

	if (m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return false;
	}

	/*if ((m_keyboardState[DIK_SPACE] & 0x80) && !(m_prevKeyboardState[DIK_SPACE] & 0x80))
	{
		if (m_GD->GS == GS_PLAY_MAIN_CAM)
		{
			m_GD->GS = GS_PLAY_TPS_CAM;
		}
		else
		{
			m_GD->GS = GS_PLAY_MAIN_CAM;
		}
	}*/
	if ((m_keyboardState[DIK_PERIOD] & 0x80) && !(m_prevKeyboardState[DIK_PERIOD] & 0x80) && simulationSpeed != 32)
	{
		simulationSpeed *= 2;
	}
	if ((m_keyboardState[DIK_COMMA] & 0x80) && !(m_prevKeyboardState[DIK_COMMA] & 0x80) && simulationSpeed != 1)
	{
		simulationSpeed /= 2;
	}

	//calculate frame time-step dt for passing down to game objects
	DWORD currentTime = GetTickCount();
	m_GD->dt = min((float)(currentTime - m_playTime) / 1000.0f, 0.1f);
	m_playTime = currentTime;


	//update all objects
	for (int i = 0; i < simulationSpeed; i++)
	{
		for (list<GameObject *>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
		{
			(*it)->Tick(m_GD);
		}
	}
	
	for (list<GameObject2D *>::iterator it = m_GameObject2Ds.begin(); it != m_GameObject2Ds.end(); it++)
	{
		(*it)->tick(m_GD);
	}

	return true;
}

void Game::render(ID3D11DeviceContext* _pd3dImmediateContext)
{
	m_DD->pd3dImmediateContext = _pd3dImmediateContext;

	m_DD->cam = m_cam;
	if (m_GD->GS == GS_PLAY_TPS_CAM)
	{
		m_DD->cam = m_TPSCam;
	}

	string simulationSpeedStr = ("Simulation Speed: " + std::to_string(simulationSpeed));
	string currentParticle = ("Current Particle: " + std::to_string(*m_GD->pSpawner->GetCurrentParticle() + 1));
	string currentParticleState;
	if (*m_GD->pSpawner->GetParticleAliveState())
	{
		currentParticleState = ("State: Spawned");
	}
	else
	{
		currentParticleState = ("State: Not Spawned");
	}	
	Vector3 currentParticlesPosition = (*m_GD->pSpawner->GetParticlePosition());
	string currentParticlePosition = ("Position: [X: " + std::to_string(currentParticlesPosition.x) + "  Y: " + std::to_string(currentParticlesPosition.y) + "]");
	Vector3 currentParticlesVelocity = (*m_GD->pSpawner->GetParticleVelocity());
	string currentParticleVelocity = ("Velocity: [X: " + std::to_string(currentParticlesVelocity.x) + "  Y: " + std::to_string(currentParticlesVelocity.y) + "]");
	Color currentParticleColor = (*m_GD->pSpawner->GetParticleColor());
	int currentParticlesMass = (*m_GD->pSpawner->GetParticleMass());
	string currentParticleMass = ("Mass: " + std::to_string(currentParticlesMass));

	VBGO::UpdateConstantBuffer(m_DD);

	//draw all 3D objects
	for (list<GameObject *>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
	{
		(*it)->Draw(m_DD);
	}
	// Draw sprite batch stuff
	m_DD2D->m_Sprites->Begin();
	for (list<GameObject2D *>::iterator it = m_GameObject2Ds.begin(); it != m_GameObject2Ds.end(); it++)
	{
		(*it)->draw(m_DD2D);
	}
	int i = 1;
	xText = 10;
	yText = 15; 
	m_DD2D->m_Font->DrawString(m_DD2D->m_Sprites.get(), Helper::charToWChar("Variable List"), Vector2(xText, 15), Colors::White);
	m_DD2D->m_Font->DrawString(m_DD2D->m_Sprites.get(), Helper::charToWChar(simulationSpeedStr.c_str()), Vector2(xText, 30), Colors::White);
	m_DD2D->m_Font->DrawString(m_DD2D->m_Sprites.get(), Helper::charToWChar(currentParticle.c_str()), Vector2(xText, 45), currentParticleColor);
	m_DD2D->m_Font->DrawString(m_DD2D->m_Sprites.get(), Helper::charToWChar(currentParticleState.c_str()), Vector2(xText, 60), currentParticleColor);
	for (int i = 0; i < 10; i++)
	{
		m_DD2D->m_Font->DrawString(m_DD2D->m_Sprites.get(), Helper::charToWChar(currentParticlePosition.c_str()), Vector2((1920 / 2) + currentParticlesPosition.x + 20, (1080 / 2) - currentParticlesPosition.y + 15), currentParticleColor);
		m_DD2D->m_Font->DrawString(m_DD2D->m_Sprites.get(), Helper::charToWChar(currentParticleVelocity.c_str()), Vector2((1920 / 2) + currentParticlesPosition.x + 20, (1080 / 2) - currentParticlesPosition.y), currentParticleColor);
		m_DD2D->m_Font->DrawString(m_DD2D->m_Sprites.get(), Helper::charToWChar(currentParticleMass.c_str()), Vector2((1920 / 2) + currentParticlesPosition.x + 20, (1080 / 2) - currentParticlesPosition.y - 15), currentParticleColor);
	}
	m_DD2D->m_Sprites->End();

	_pd3dImmediateContext->OMSetDepthStencilState(m_States->DepthDefault(), 0);

}

bool Game::ReadKeyboard()
{
	//copy over old keyboard state
	memcpy(m_prevKeyboardState, m_keyboardState, sizeof(m_keyboardState));

	//clear out previous state
	ZeroMemory(&m_keyboardState, sizeof(m_keyboardState));

	// Read the keyboard device.
	HRESULT hr = m_pKeyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_pKeyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;

}

bool Game::ReadMouse()
{
	memcpy(&m_prevMouseState, &m_mouse_state, sizeof(m_mouse_state));
	//clear out previous state
	ZeroMemory(&m_mouse_state, sizeof(m_mouse_state));

	// Read the Mouse device.
	HRESULT hr = m_pMouse->GetDeviceState(sizeof(m_mouse_state), (LPVOID)&m_mouse_state);
	if (FAILED(hr))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_pMouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;

}
