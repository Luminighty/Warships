#include "MyApp.h"

#include <math.h>
#include <vector>

#include <array>
#include <list>
#include <tuple>
#include <imgui/imgui.h>
#include "includes/GLUtils.hpp"

#include "SphereMesh.h"
#include "PlaneMesh.h"
#include "PlayerShip.h"
#include "EnemyShip.h"
#include "Water.h"
#include "HillMesh.h"
#include "CannonBottomMesh.h"
#include "CannonTopMesh.h"

std::vector<GameObject*> CMyApp::gameObjects = {};
std::vector<GameObject*> CMyApp::toDestroy = {};
std::vector<GameObject*> CMyApp::toAdd = {};

CMyApp::CMyApp(void)
{
	camera.SetView(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

CMyApp::~CMyApp(void)
{
	std::cout << "dtor!\n";
}

void CMyApp::InitMeshes()
{
	MyMesh::meshes["ship"] = new ObjMesh("assets/ship.obj");
	MyMesh::meshes["sphere"] = new SphereMesh();
	MyMesh::meshes["plane"] = new PlaneMesh();
	MyMesh::meshes["hill"] = new HillMesh(5);
	MyMesh::meshes["cannonbottom"] = new CannonBottomMesh(8);
	MyMesh::meshes["cannontop"] = new CannonTopMesh();

	for (std::map<const char*, MyMesh*>::iterator it = MyMesh::meshes.begin(); it !=MyMesh::meshes.end(); ++it)
	{
		it->second->init();
	}
}

void CMyApp::InitSkyBox()
{
	m_SkyboxPos.BufferData(
		std::vector<glm::vec3>{
		// hátsó lap
		glm::vec3(-1, -1, -1),
		glm::vec3(1, -1, -1),
		glm::vec3(1, 1, -1),
		glm::vec3(-1, 1, -1),
		// elülsõ lap
		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(-1, 1, 1),
	}
	);

	// és a primitíveket alkotó csúcspontok indexei (az elõzõ tömbökbõl) - triangle list-el való kirajzolásra felkészülve
	m_SkyboxIndices.BufferData(
		std::vector<int>{
			// hátsó lap
			0, 1, 2,
			2, 3, 0,
			// elülsõ lap
			4, 6, 5,
			6, 4, 7,
			// bal
			0, 3, 4,
			4, 3, 7,
			// jobb
			1, 5, 2,
			5, 6, 2,
			// alsó
			1, 0, 4,
			1, 4, 5,
			// felsõ
			3, 2, 6,
			3, 6, 7,
	}
	);

	// geometria VAO-ban való regisztrálása
	m_SkyboxVao.Init(
		{
			{ CreateAttribute<0, glm::vec3, 0, sizeof(glm::vec3)>, m_SkyboxPos },
		}, m_SkyboxIndices
	);

	// skybox texture
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glGenTextures(1, &m_skyboxTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	TextureFromFileAttach("assets/xpos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	TextureFromFileAttach("assets/xneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	TextureFromFileAttach("assets/ypos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	TextureFromFileAttach("assets/yneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	TextureFromFileAttach("assets/zpos.png", GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	TextureFromFileAttach("assets/zneg.png", GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}

void CMyApp::InitShaders()
{

	AddShader("default", "myVert.vert", "myFrag.frag");
	AddShader("water", "waterVert.vert", "waterFrag.frag");
	//m_program = CMyApp::programs["default"];
	// shader program rövid létrehozása, egyetlen függvényhívással a fenti három:
	m_programSkybox.Init(
		{
			{ GL_VERTEX_SHADER, "skybox.vert" },
			{ GL_FRAGMENT_SHADER, "skybox.frag" }
		},
		{
			{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
		}
	);
}

void CMyApp::AddShader(const char* title, const char* vert, const char* frag)
{
	ProgramObject* program = new ProgramObject();
	program->Init(
		{
			{ GL_VERTEX_SHADER, vert },
			{ GL_FRAGMENT_SHADER, frag }
		},
		{
			{ 0, "vs_in_pos" },				// VAO 0-as csatorna menjen a vs_in_pos-ba
			{ 1, "vs_in_norm" },			// VAO 1-as csatorna menjen a vs_in_norm-ba
			{ 2, "vs_in_tex" }				// VAO 2-as csatorna menjen a vs_in_tex-ba
		}
	);
	GameManager::programs[title] = program;
}

void CMyApp::AddTexture(const char* title, const char* fileName)
{
	Texture2D* texture = new Texture2D();
	texture->FromFile(fileName);
	GameManager::textures[title] = texture;
}

void CMyApp::AddGameObject(GameObject* obj)
{
	toAdd.push_back(obj);
}

bool CMyApp::Init()
{
	// törlési szín legyen kékes
	glClearColor(0.125f, 0.25f, 0.5f, 1.0f);

	glEnable(GL_CULL_FACE); // kapcsoljuk be a hatrafele nezo lapok eldobasat
	glEnable(GL_DEPTH_TEST); // mélységi teszt bekapcsolása (takarás)

	InitMeshes();
	InitShaders();
	InitSkyBox();

	// egyéb textúrák betöltése
	AddTexture("wood", "assets/wood.jpg");
	AddTexture("water", "assets/water2.jpg");
	AddTexture("marron", "assets/marron.jpg");
	AddTexture("ship_texture", "assets/ship_texture.jpg");
	AddTexture("ship_empty", "assets/ship_empty.jpg");

	// kamera
	camera.SetProj(45.0f, 640.0f / 480.0f, 0.01f, 1000.0f);
	GameManager::viewProj = camera.GetViewProj();
	//GameManager::m_matProj = glm::perspective(45.0f, 640 / 480.0f, 1.0f, 1000.0f);
	

	InitGameObjects();
	camera.SetFollowObject(player);


	return true;
}

void CMyApp::InitGameObjects()
{
	int waterCount = GameManager::mapSize;
	for (int i = -waterCount; i < waterCount; i++)
	for(int j = -waterCount; j < waterCount; j++)
		gameObjects.push_back(new Water(i,j));

	gameObjects.push_back(new EnemyShip({10, 0, 30}));
	player = new PlayerShip();
	gameObjects.push_back(player);
}

void CMyApp::Clean()
{
	glDeleteTextures(1, &m_skyboxTexture);

	//delete m_mesh;
}

void CMyApp::Update()
{
	static Uint32 last_time = SDL_GetTicks();
	float delta_time = (SDL_GetTicks() - last_time) / 1000.0f;
	GameObject::deltaTime = delta_time;
	//std::cout << delta_time << std::endl;
	camera.Update(delta_time);

	if (!isPaused)
	for each (GameObject* gameObject in gameObjects)
	{
		gameObject->Update();
	}
	ClearGameObjects();
	AddGameObjects();

	last_time = SDL_GetTicks();
	Input::Update();
}
void CMyApp::AddGameObjects()
{
	for (unsigned int i = 0; i < toAdd.size(); i++) {
		gameObjects.push_back(toAdd[i]);
	}
	toAdd.clear();
}

void CMyApp::ClearGameObjects()
{
	for (unsigned int i = 0; i < toDestroy.size(); i++) {
		for (unsigned int j = 0; j < gameObjects.size(); j++) {
			if (gameObjects[j] == toDestroy[i]) {
				GameObject* obj = gameObjects[j];
				gameObjects[j] = gameObjects[gameObjects.size() - 1];
				gameObjects.pop_back();
				if (obj == camera.GetFollowObject())
					camera.SetFollowObject(nullptr);

				obj->~GameObject();
				//delete obj;
			}
		}
	}
	toDestroy.clear();
}

void CMyApp::RenderSkyBox()
{
	// skybox
	// mentsük el az elõzõ Z-test eredményt, azaz azt a relációt, ami alapján update-eljük a pixelt.
	GLint prevDepthFnc;
	glGetIntegerv(GL_DEPTH_FUNC, &prevDepthFnc);

	// most kisebb-egyenlõt használjunk, mert mindent kitolunk a távoli vágósíkokra
	glDepthFunc(GL_LEQUAL);

	m_SkyboxVao.Bind();
	m_programSkybox.Use();
	m_programSkybox.SetUniform("MVP", camera.GetViewProj() * glm::translate(camera.GetEye()));

	// cube map textúra beállítása 0-ás mintavételezõre és annak a shaderre beállítása
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture);
	glUniform1i(m_programSkybox.GetLocation("skyboxTexture"), 0);
	// az elõzõ három sor <=> m_programSkybox.SetCubeTexture("skyboxTexture", 0, m_skyboxTexture);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
	m_programSkybox.Unuse();

	// végül állítsuk vissza
	glDepthFunc(prevDepthFnc);
}

void CMyApp::Render()
{
	// töröljük a frampuffert (GL_COLOR_BUFFER_BIT) és a mélységi Z puffert (GL_DEPTH_BUFFER_BIT)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GameManager::viewProj = camera.GetViewProj();

	for each (GameObject* gameObject in gameObjects)
	{
		gameObject->Render();
	}

	RenderSkyBox();

	//ImGui Testwindow
	//ImGui::ShowTestWindow();
}

void CMyApp::KeyboardDown(SDL_KeyboardEvent& key)
{
	if (key.keysym.sym == SDLK_SPACE)
		camera.SetFollowObject((camera.GetFollowObject() == nullptr) ? player : nullptr);
	if (key.keysym.sym == SDLK_0)
		isPaused = !isPaused;
	camera.KeyboardDown(key);
	Input::KeyboardDown(key);
}

void CMyApp::KeyboardUp(SDL_KeyboardEvent& key)
{
	camera.KeyboardUp(key);
	Input::KeyboardUp(key);
}

void CMyApp::MouseMove(SDL_MouseMotionEvent& mouse)
{
	camera.MouseMove(mouse);
	//player->MouseMove(mouse);
}

void CMyApp::MouseDown(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseUp(SDL_MouseButtonEvent& mouse)
{
}

void CMyApp::MouseWheel(SDL_MouseWheelEvent& wheel)
{
	camera.MouseWheel(wheel);
	//player->MouseWheel(wheel);
}

// a két paraméterbe az új ablakméret szélessége (_w) és magassága (_h) található
void CMyApp::Resize(int _w, int _h)
{

	glViewport(0, 0, _w, _h);
	camera.Resize(_w, _h);

	/*
	GameManager::m_matProj = glm::perspective(45.0f,		// 90 fokos nyilasszog
		_w / (float)_h,	// ablakmereteknek megfelelo nezeti arany
		0.01f,			// kozeli vagosik
		100.0f);		// tavoli vagosik
		*/
}

void CMyApp::DestroyGameObject(GameObject* obj)
{
	toDestroy.push_back(obj);
}
