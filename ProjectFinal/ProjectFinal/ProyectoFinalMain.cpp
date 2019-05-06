/*
	LABORATORIO DE COMPUTACIÓN GRÁFICA E INTERACCIÓN HUMANO COMPUTADORA, GRUPO 01
	PROYECTO FINAL
	06 Mayo 2019
	MARCO ANTONIO MORENO GUERRA

	Este proyecto consiste en la recreación de la fachada y el cuarto con algunos de sus elementos
	de la caricatura "Cuentos de la Calle Broca". Para ello se emplea OpenGL v3.3, así como otras
	bibliotecas complementarias para permitir el control del teclado, mouse, generación de ventanas,
	entre otros.
*/

#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

#include "Window.h"
#include "Mesh_texturizado.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"

#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

#include "Model.h"
#include "Skybox.h"
#include "SpotLight.h"

const float toRadians = 3.14159265f / 180.0f;
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
Camera camera;

// Texturas
Texture floorTexture;
Texture floorInteriorTexture;
Texture floorCalleTexture;
Texture paredTexture;
Texture maderaVentanaTexture;
Texture maderaVentana1Texture;
Texture decoracionBlackTexture;
Texture madera;
Texture desktop;
Texture regCash;

// Materiales
Material Material_brillante;
Material Material_opaco;

// Luz direccional
DirectionalLight mainLight;

// Modelos
Model Mesa;
Model Silla;
Model Tubo;
Model Tomate;
Model Box;
Model LargeBox;
Model Lata;
Model Latas;
Model Radio;
Model Pineaple;
Model Tv;

// Skybox de entorno
Skybox skybox;

// Iniciación de valores para obtener el tiempo entre Frames 
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

void CreateObjects() 
{
	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	 0.0f,  0.0f,	0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f, -10.0f,	10.0f,  0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f,  10.0f, 	 0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		 10.0f, 0.0f,  10.0f,	10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj1);

}

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,
		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20
	};

	GLfloat cubo_vertices[] = {
		// FRONT FACE
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.27f,  0.35f,		0.0f,	0.0f,	-1.0f,
		 0.5f, -0.5f,  0.5f,	0.48f,	0.35f,		0.0f,	0.0f,	-1.0f,
		 0.5f,  0.5f,  0.5f,	0.48f,	0.64f,		0.0f,	0.0f,	-1.0f,
		-0.5f,  0.5f,  0.5f,	0.27f,	0.64f,		0.0f,	0.0f,	-1.0f,
		// RIGHT FACE
		//x		y		z		S		T
		 0.5f, -0.5f,  0.5f,    0.52f,  0.35f,	   -1.0f,	0.0f,	 0.0f,
		 0.5f, -0.5f, -0.5f,	0.73f,	0.35f,	   -1.0f,	0.0f,	 0.0f,
		 0.5f,  0.5f, -0.5f,	0.73f,	0.64f,	   -1.0f,	0.0f,	 0.0f,
		 0.5f,  0.5f,  0.5f,    0.52f,	0.64f,	   -1.0f,	0.0f,	 0.0f,
		// BACK FACE
		//x		y		z		S		T
		-0.5f, -0.5f, -0.5f,	0.77f,	0.35f,		0.0f,	0.0f,	 1.0f,
		 0.5f, -0.5f, -0.5f,	0.98f,	0.35f,		0.0f,	0.0f,	 1.0f,
		 0.5f,  0.5f, -0.5f,	0.98f,	0.64f,		0.0f,	0.0f,	 1.0f,
		-0.5f,  0.5f, -0.5f,	0.77f,	0.64f,		0.0f,	0.0f,	 1.0f,
		// LEFT FACE
		//x		y		z		S		T
		-0.5f, -0.5f, -0.5f,	0.0f,	0.35f,		1.0f,	0.0f,	 0.0f,
		-0.5f, -0.5f,  0.5f,	0.23f,  0.35f,		1.0f,	0.0f,	 0.0f,
		-0.5f,  0.5f,  0.5f,	0.23f,	0.64f,		1.0f,	0.0f,	 0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,	0.64f,		1.0f,	0.0f,	 0.0f,
		// BOTTOM FACE
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.27f,	0.02f,		0.0f,	1.0f,	 0.0f,
		 0.5f, -0.5f,  0.5f,	0.48f,  0.02f,		0.0f,	1.0f,	 0.0f,
		 0.5f, -0.5f, -0.5f,	0.48f,	0.31f,		0.0f,	1.0f,	 0.0f,
		-0.5f, -0.5f, -0.5f,	0.27f,	0.31f,		0.0f,	1.0f,	 0.0f,
		// UP FACE
		//x		y		z		S		T
		-0.5f,  0.5f,  0.5f,	0.27f,	0.68f,		0.0f,	-1.0f,	 0.0f,
		 0.5f,  0.5f,  0.5f,	0.48f,  0.68f,		0.0f,	-1.0f,	 0.0f,
		 0.5f,  0.5f, -0.5f,	0.48f,	0.98f,		0.0f,	-1.0f,	 0.0f,
		-0.5f,  0.5f, -0.5f,	0.27f,	0.98f,		0.0f,	-1.0f,	 0.0f
	};
	
	Mesh *cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(cubo);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main() 
{
	mainWindow = Window(1366, 768);
	mainWindow.Initialise();

	CreateObjects();
	CrearCubo();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f), -10.0f, 10.0f, 5.0f, 0.5f);

	// Definición de Texturas
	floorTexture = Texture("Textures/suelo.png");
	floorTexture.LoadTextureA();

	floorInteriorTexture = Texture("Textures/sueloInt.png");
	floorInteriorTexture.LoadTextureA();

	floorCalleTexture = Texture("Textures/sueloCalle.png");
	floorCalleTexture.LoadTextureA();

	paredTexture = Texture("Textures/pared.png");
	paredTexture.LoadTextureA();
	
	maderaVentanaTexture = Texture("Textures/maderaVentana.png");
	maderaVentanaTexture.LoadTextureA();
	
	maderaVentana1Texture = Texture("Textures/maderaVentana1.png");
	maderaVentana1Texture.LoadTextureA();
	
	decoracionBlackTexture = Texture("Textures/decoracion.png");
	decoracionBlackTexture.LoadTextureA();
	
	madera = Texture("Textures/madera.png");
	madera.LoadTextureA();

	desktop = Texture("Textures/desktopColor.png");
	desktop.LoadTextureA();

	regCash = Texture("Textures/regCash.png");
	regCash.LoadTextureA();
	

	// Definición de Modelos
	Material_brillante = Material(0.001f, 256);
	Material_opaco = Material(1.0f, 4);

	Mesa = Model();
	Mesa.LoadModel("Models/mesa.obj");

	Silla = Model();
	Silla.LoadModel("Models/silla.obj");

	Tubo = Model();
	Tubo.LoadModel("Models/metalPipe.obj");

	Tomate = Model();
	Tomate.LoadModel("Models/Tomato.obj");

	Box = Model();
	Box.LoadModel("Models/box.obj");

	LargeBox = Model();
	LargeBox.LoadModel("Models/largeBox.obj");

	Lata = Model();
	Lata.LoadModel("Models/lata.obj");

	Latas = Model();
	Latas.LoadModel("Models/latas.obj");

	Radio = Model();
	Radio.LoadModel("Models/Radio.obj");

	Pineaple = Model();
	Pineaple.LoadModel("Models/10200_Pineapple_v1-L2.obj");

	Tv = Model();
	Tv.LoadModel("Models/tv.obj");


	// Luz direccional superior para iluminado del escenario
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 0.5f, 0.5f, 0.0f, -1.0f, -1.0f);

	// Definición de texturas del skybox
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/met_ft.tga");
	skyboxFaces.push_back("Textures/Skybox/met_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/met_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/met_up.tga");
	skyboxFaces.push_back("Textures/Skybox/met_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/met_lf.tga");
	skybox = Skybox(skyboxFaces);

	// Proyección
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 300.0f);

	// Loop mientras no se cierre la ventana (detención de ejecución)
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime; 
		lastTime = now;

		// Recibe eventos del usuario
		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		
		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		shaderList[0].SetDirectionalLight(&mainLight);
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		glm::mat4 model(1.0);

		// Definición del suelo (no de Skybox)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.8f, 1.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		floorTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.65f, -1.745f, -5.0f));
		model = glm::scale(model, glm::vec3(0.635f, 1.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		floorInteriorTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.775f, 10.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		floorCalleTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();

		// Calle lateral derecha
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.0f, -1.775f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		floorCalleTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();

		// Calle lateral izquierda
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, -1.775f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 1.0f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		floorCalleTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////// RENDERIZACIÓN DE CUBOS PARA CONTRUCCIÓN DE FACHADA ////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/*****************EDIFICIO IZQ PLANTA BAJA***********/
		// Pared edificio izquierdo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-11.125f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.1f, 3.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-19.425f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(6.0f, 3.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.75f, 1.35f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 0.8f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.75f, -1.30f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 0.9f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Pared lateral izquierdo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-22.375f, 0.0f, -5.0f));
		model = glm::scale(model, glm::vec3(0.1f, 3.5f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Pared trasera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-14.725f, 0.0f, -10.0f));
		model = glm::scale(model, glm::vec3(15.4f, 3.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Detalles de separación de edificios
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.05f, 0.0f, 0.05f));
		model = glm::scale(model, glm::vec3(0.05f, 3.5f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Ventana edif izq
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.80f, 0.05f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 1.8f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.8f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.8f, 0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.8f, -0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.8f, -0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Puerta
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-11.125f, -0.2f, 0.05f));
		model = glm::scale(model, glm::vec3(1.5f, 3.0f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		madera.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.6f, 0.0f, 0.1f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();
		/*****************EDIFICIO IZQ PLANTA BAJA***********/



		/*****************EDIFICIO IZQ PLANTA ALTA***********/
		// Pared edificio izquierdo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-11.125f, 3.56f, 0.0f));
		model = glm::scale(model, glm::vec3(8.1f, 3.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-19.425f, 3.56f, 0.0f));
		model = glm::scale(model, glm::vec3(6.0f, 3.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.75f, 4.91f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 0.8f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.75f, 2.26f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 0.9f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Pared lateral izquierdo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-22.375f, 3.56f, -5.0f));
		model = glm::scale(model, glm::vec3(0.1f, 3.5f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Pared lateral izquierdo delimitador con cuarto principal (centro)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.0f, 3.56f, -5.0f));
		model = glm::scale(model, glm::vec3(0.1f, 3.5f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Pared trasera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-14.725f, 3.56f, -10.0f));
		model = glm::scale(model, glm::vec3(15.4f, 3.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Detalles de separación de edificios
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.05f, 3.56f, 0.05f));
		model = glm::scale(model, glm::vec3(0.05f, 3.5f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Ventana edif izq superior
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.80f, 3.61f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 1.8f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.8f, 3.61f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.8f, 3.86f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.8f, 3.26f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-15.8f, 3.06f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();
		/*****************EDIFICIO IZQ PLANTA ALTA***********/


		/*****************EDIFICIO DER PLANTA ALTA***********/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.0f, 3.56f, 0.0f));
		model = glm::scale(model, glm::vec3(6.0f, 3.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 3.56f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 3.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.6f, 4.91f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 0.8f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.6f, 2.26f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 0.9f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.25f, 3.56f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 3.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.5f, 3.56f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 3.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.60f, 4.91f, 0.0f));
		model = glm::scale(model, glm::vec3(2.8f, 0.8f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		//Detalles de decoración de ventana
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.625f, 3.61f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 1.8f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.625f, 4.06f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.625f, 3.86f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.625f, 3.26f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.625f, 3.06f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Pared trasera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.6f, 3.56f, -10.0f));
		model = glm::scale(model, glm::vec3(12.9f, 3.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Pared lateral derecho
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.8f, 3.56f, -5.0f));
		model = glm::scale(model, glm::vec3(0.1f, 3.5f, 10.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();
		/*****************EDIFICIO DERECHA PLANTA ALTA***********/



		// Detalles de separación de edificios horizontalmente
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.29f, 1.78f, 0.0f));
		model = glm::scale(model, glm::vec3(28.29f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.29f, 1.78f, -10.0f));
		model = glm::scale(model, glm::vec3(28.29f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.0f, 1.78f, -5.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.05f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-22.375f, 1.78f, -5.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.05f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.8f, 1.78f, -5.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.05f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// TECHOS
		//Coordenadas Centro(-0.6,-5.0)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.6f, 1.78f, -5.0f));
		model = glm::scale(model, glm::vec3(12.8f, 0.05f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		floorTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		//Coordenadas Centro(-0.6,-5.0)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.6f, 5.0f, -5.0f));
		model = glm::scale(model, glm::vec3(12.8f, 0.05f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		//Coordenadas Centro(-14.6875,-5.0)
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-14.6875f, 1.78f, -5.0f));
		model = glm::scale(model, glm::vec3(15.375f, 0.05f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		floorTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-14.6875f, 5.0f, -5.0f));
		model = glm::scale(model, glm::vec3(15.375f, 0.05f, 10.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();


		/*****************EDIFICIO DERECHA PLANTA BAJA (PRINCIPAL)***********/
		// Pared principal de fachada
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 3.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 3.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.6f, 1.35f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 0.8f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.6f, -1.30f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 0.9f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.25f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 3.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.5f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 3.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.60f, 1.35f, 0.0f));
		model = glm::scale(model, glm::vec3(2.8f, 0.8f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Pared del costado izquierdo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.0f, 0.0f, -5.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.1f, 3.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Pared trasera
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.6f, 0.0f, -10.0f));
		model = glm::scale(model, glm::vec3(12.9f, 3.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Paredes del costado derecho
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.8f, 0.0f, -0.95f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 3.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.8f, 1.35f, -2.55f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 0.8f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.8f, -1.30f, -2.55f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 0.9f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.8f, 0.0f, -4.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 3.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.8f, 0.0f, -6.075f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.75f, 3.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.8f, 0.0f, -9.075f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.75f, 3.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.8f, 1.35f, -7.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 0.8f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.8f, -1.30f, -7.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 0.9f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();


		// Paredes Internas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, 0.0f, -5.0f));
		model = glm::scale(model, glm::vec3(4.0f, 3.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.8f, 0.0f, -5.0f));
		model = glm::scale(model, glm::vec3(6.0f, 3.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.60f, 1.35f, -5.0f));
		model = glm::scale(model, glm::vec3(2.8f, 0.8f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Ventana 1 lateral frente
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.0f, -1.85f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 1.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.0f, -0.95f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 1.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.85f, -1.40f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.1f, 0.2f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, -0.75f, -1.40f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.1f, 0.2f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, -0.55f, -1.40f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, -0.4f, -1.40f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, -0.25f, -1.40f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, -0.10f, -1.40f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.05f, -1.40f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.20f, -1.40f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.35f, -1.40f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.50f, -1.40f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.65f, -1.40f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Ventana 2 lateral fondo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.0f, -4.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 1.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.0f, -3.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 1.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.85f, -3.75f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.1f, 0.2f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, -0.75f, -3.75f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.1f, 0.2f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, -0.55f, -3.75f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, -0.4f, -3.75f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, -0.25f, -3.75f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, -0.10f, -3.75f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.05f, -3.75f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.20f, -3.75f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.35f, -3.75f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.50f, -3.75f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.65f, -3.75f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();


		///////////////////////////////////////////////////////////////////////////////////////////////
		// Ventana 1 lateral fondo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.0f, -6.85f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 1.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.0f, -5.95f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 1.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.85f, -6.40f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.1f, 0.2f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, -0.75f, -6.40f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.1f, 0.2f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, -0.55f, -6.40f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, -0.4f, -6.40f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, -0.25f, -6.40f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, -0.10f, -6.40f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.05f, -6.40f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.20f, -6.40f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.35f, -6.40f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.50f, -6.40f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.65f, -6.40f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();
		
		// Ventana 2 lateral fondo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.0f, -9.2f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 1.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.0f, -8.3f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 1.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.85f, -8.75f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.1f, 0.2f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, -0.75f, -8.75f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.1f, 0.2f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, -0.55f, -8.75f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, -0.4f, -8.75f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, -0.25f, -8.75f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, -0.10f, -8.75f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.05f, -8.75f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.20f, -8.75f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.35f, -8.75f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.50f, -8.75f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.90f, 0.65f, -8.75f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();
		///////////////////////////////////////////////////////////////////////////////////////////////

		// Detalles de decoración de ventana lateral derecha frente
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.80f, 0.05f, -2.575f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 1.8f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.8f, 0.5f, -2.575f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.8f, 0.3f, -2.575f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.8f, -0.3f, -2.575f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.8f, -0.5f, -2.575f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.85f, 1.25f, -2.575f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.025f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.85f, 1.1f, -2.33f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.025f, 0.275f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.85f, 1.1f, -2.82f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.025f, 0.275f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.85f, 1.07f, -1.95f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.025f, 0.22f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.85f, 1.07f, -3.20f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.025f, 0.22f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, 10 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(5.85f, 0.60f, -3.17f));
		model = glm::scale(model, glm::vec3(0.05f, 0.025f, 0.40f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, -10 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(5.85f, 1.5f, -1.9f));
		model = glm::scale(model, glm::vec3(0.05f, 0.025f, 0.40f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Detalles de decoración de ventana lateral derecha fondo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.80f, 0.05f, -7.575f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 1.8f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.8f, 0.5f, -7.575f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.8f, 0.3f, -7.575f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.8f, -0.3f, -7.575f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.8f, -0.5f, -7.575f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.85f, 1.25f, -7.575f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.025f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.85f, 1.1f, -7.33f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.025f, 0.275f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.85f, 1.1f, -7.82f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.025f, 0.275f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.85f, 1.07f, -6.95f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.025f, 0.22f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.85f, 1.07f, -8.20f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.025f, 0.22f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();


		model = glm::mat4(1.0);
		model = glm::rotate(model, 10 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(5.85f, -0.25f, -8.1f));
		model = glm::scale(model, glm::vec3(0.05f, 0.025f, 0.40f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, -10 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(5.85f, 2.36f, -6.84f));
		model = glm::scale(model, glm::vec3(0.05f, 0.025f, 0.40f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();


		// Construcción de puertas de madera verdes de ventana 1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.90f, 0.0f, 0.1f));
		model = glm::scale(model, glm::vec3(0.2f, 1.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.1f));
		model = glm::scale(model, glm::vec3(0.2f, 1.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.45f, 0.85f, 0.1f));
		model = glm::scale(model, glm::vec3(1.1f, 0.2f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.45f, -0.75f, 0.1f));
		model = glm::scale(model, glm::vec3(1.1f, 0.2f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.45f, -0.55f, 0.1f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.45f, -0.4f, 0.1f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.45f, -0.25f, 0.1f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.45f, -0.1f, 0.1f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.45f, 0.05f, 0.1f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.45f, 0.2f, 0.1f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.45f, 0.35f, 0.1f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.45f, 0.5f, 0.1f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.45f, 0.65f, 0.1f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Construcción de puertas de madera verdes de ventana 2
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.25f, 0.0f, 0.1f));
		model = glm::scale(model, glm::vec3(0.2f, 1.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.35f, 0.0f, 0.1f));
		model = glm::scale(model, glm::vec3(0.2f, 1.5f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.80f, 0.85f, 0.1f));
		model = glm::scale(model, glm::vec3(1.1f, 0.2f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.80f, -0.75f, 0.1f));
		model = glm::scale(model, glm::vec3(1.1f, 0.2f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.80f, -0.55f, 0.1f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.80f, -0.4f, 0.1f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.80f, -0.25f, 0.1f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.80f, -0.1f, 0.1f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.80f, 0.05f, 0.1f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.80f, 0.2f, 0.1f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.80f, 0.35f, 0.1f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.80f, 0.5f, 0.1f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.80f, 0.65f, 0.1f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.1f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Detalles de decoración de ventana frontal
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.625f, 0.05f, 0.0f));
		model = glm::scale(model, glm::vec3(0.05f, 1.8f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.625f, 0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.625f, 0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.625f, -0.3f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.625f, -0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.25f, 0.05f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.625f, 1.25f, 0.05f));
		model = glm::scale(model, glm::vec3(0.5f, 0.025f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.39f, 1.1f, 0.05f));
		model = glm::scale(model, glm::vec3(0.025f, 0.275f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.86f, 1.1f, 0.05f));
		model = glm::scale(model, glm::vec3(0.025f, 0.275f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.3f, 1.07f, 0.05f));
		model = glm::scale(model, glm::vec3(0.025f, 0.22f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.975f, 1.07f, 0.05f));
		model = glm::scale(model, glm::vec3(0.025f, 0.22f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, 10 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(2.25f, 0.75f, 0.05f));
		model = glm::scale(model, glm::vec3(0.45f, 0.025f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, -10 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.96f, 1.32f, 0.05f));
		model = glm::scale(model, glm::vec3(0.45f, 0.025f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		
		// Toldo exterior
		model = glm::mat4(1.0);
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-1.25f, 1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.05f, 1.70f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.25f, 0.70f, 1.45f));
		model = glm::scale(model, glm::vec3(0.5f, 0.35f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-1.75f, 1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.05f, 1.70f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.75f, 0.70f, 1.45f));
		model = glm::scale(model, glm::vec3(0.5f, 0.35f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-2.25f, 1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.05f, 1.70f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.25f, 0.70f, 1.45f));
		model = glm::scale(model, glm::vec3(0.5f, 0.35f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-2.75f, 1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.05f, 1.70f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.75f, 0.70f, 1.45f));
		model = glm::scale(model, glm::vec3(0.5f, 0.35f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-3.25f, 1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.05f, 1.70f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.25f, 0.70f, 1.45f));
		model = glm::scale(model, glm::vec3(0.5f, 0.35f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-3.75f, 1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.05f, 1.70f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.75f, 0.70f, 1.45f));
		model = glm::scale(model, glm::vec3(0.5f, 0.35f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-4.25f, 1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.05f, 1.70f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.25f, 0.70f, 1.45f));
		model = glm::scale(model, glm::vec3(0.5f, 0.35f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-4.75f, 1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.05f, 1.70f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.75f, 0.70f, 1.45f));
		model = glm::scale(model, glm::vec3(0.5f, 0.35f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-5.25f, 1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.05f, 1.70f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.25f, 0.70f, 1.45f));
		model = glm::scale(model, glm::vec3(0.5f, 0.35f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-5.75f, 1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.05f, 1.70f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.75f, 0.70f, 1.45f));
		model = glm::scale(model, glm::vec3(0.5f, 0.35f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-6.25f, 1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.05f, 1.70f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.25f, 0.70f, 1.45f));
		model = glm::scale(model, glm::vec3(0.5f, 0.35f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentanaTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-6.75f, 1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.05f, 1.70f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.75f, 0.70f, 1.45f));
		model = glm::scale(model, glm::vec3(0.5f, 0.35f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		maderaVentana1Texture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Generación de elementos auxiliares
		// Tabla externa
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, -0.8f, 1.0f));
		model = glm::scale(model, glm::vec3(3.0f, 0.05f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		madera.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Tabla interna/Repisas
		// Repisa derecha superior
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.5f, 0.05f, -4.55f));
		model = glm::scale(model, glm::vec3(5.0f, 0.05f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		madera.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Repisa derecha inferior
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.5f, 0.8f, -4.55f));
		model = glm::scale(model, glm::vec3(5.0f, 0.05f, 0.7f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		madera.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Repisa izq inferior
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.5f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(1.0f, 0.05f, 4.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		madera.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Repisa izq superior
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.5f, 0.75f, -2.5f));
		model = glm::scale(model, glm::vec3(1.0f, 0.05f, 4.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		madera.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();


		// Mostrador/Aparador
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.5f, -0.95f, -2.5f));
		model = glm::scale(model, glm::vec3(4.0f, 1.15f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		desktop.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.5f, -0.35f, -2.5f));
		model = glm::scale(model, glm::vec3(4.3f, 0.05f, 1.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		desktop.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.25f, -1.62f, -2.15f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		desktop.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.25f, -1.62f, -2.85f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		desktop.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.2f, -1.62f, -2.15f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		desktop.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.2f, -1.62f, -2.85f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		desktop.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Caja registradora 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.75f, 0.10f, -2.25f));
		model = glm::scale(model, glm::vec3(0.7f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		regCash.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.75f, -0.21f, -2.26f));
		model = glm::scale(model, glm::vec3(0.6f, 0.11f, 0.49f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		floorTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.75f, -0.3f, -2.25f));
		model = glm::scale(model, glm::vec3(0.7f, 0.05f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		regCash.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.425f, -0.212f, -2.25f));
		model = glm::scale(model, glm::vec3(0.05f, 0.126f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		regCash.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.75f, -0.212f, -2.0125f));
		model = glm::scale(model, glm::vec3(0.65f, 0.125f, 0.025f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		regCash.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.075f, -0.212f, -2.25f));
		model = glm::scale(model, glm::vec3(0.05f, 0.126f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		regCash.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.75f, 0.45f, -2.25f));
		model = glm::scale(model, glm::vec3(0.1f, 0.2f, 0.075f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		regCash.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.75f, 0.10f, -2.6f));
		model = glm::rotate(model, -50 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		regCash.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.75f, -0.05f, -2.6f));
		model = glm::scale(model, glm::vec3(0.7f, 0.1f, 0.2f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		regCash.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.75f, 0.65f, -2.25f));
		model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		regCash.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.75f, 0.65f, -2.20f));
		model = glm::scale(model, glm::vec3(0.25f, 0.15f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.75f, 0.65f, -2.30f));
		model = glm::scale(model, glm::vec3(0.25f, 0.15f, 0.01f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		decoracionBlackTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		// Botones de la Caja registradora
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, 0.2125f, -2.55f));
		model = glm::rotate(model, -50 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, 0.0875f, -2.65f));
		model = glm::rotate(model, -50 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.0f, -0.03f, -2.725f));
		model = glm::rotate(model, -50 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.85f, 0.2125f, -2.55f));
		model = glm::rotate(model, -50 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.85f, 0.0875f, -2.65f));
		model = glm::rotate(model, -50 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.85f, -0.03f, -2.725f));
		model = glm::rotate(model, -50 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.7f, 0.2125f, -2.55f));
		model = glm::rotate(model, -50 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.7f, 0.0875f, -2.65f));
		model = glm::rotate(model, -50 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.7f, -0.03f, -2.725f));
		model = glm::rotate(model, -50 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.5f, 0.2125f, -2.55f));
		model = glm::rotate(model, -50 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.5f, 0.0875f, -2.65f));
		model = glm::rotate(model, -50 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		paredTexture.UseTexture(); // Textura que se cargara en el cubo
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();


		

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////// CARGA DE MODELOS 3D /////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Modelo de la mesa
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.5f, -1.25f, -7.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Mesa.RenderModel();

		// Modelo de la mesa exterior
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.25f, -1.5f, 0.8f));
		model = glm::scale(model, glm::vec3(0.006f, 0.006f, 0.01f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Mesa.RenderModel();

		// Modelos de las sillas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.2f, -1.05f, -8.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.07f, 0.09f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Silla.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.8f, -1.05f, -6.0f));
		model = glm::scale(model, glm::vec3(0.09f, 0.07f, 0.09f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Silla.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-2.0f, -1.05f, -7.35f));
		model = glm::scale(model, glm::vec3(0.09f, 0.07f, 0.09f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Silla.RenderModel();

		// Modelos de tubos de soporte para puesto de verduras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.75f, -0.4f, 1.25f));
		model = glm::scale(model, glm::vec3(0.09f, 1.4f, 0.09f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tubo.RenderModel();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.25f, -0.4f, 1.25f));
		model = glm::scale(model, glm::vec3(0.09f, 1.4f, 0.09f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tubo.RenderModel();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.75f, -0.1f, 0.25f));
		model = glm::scale(model, glm::vec3(0.09f, 1.7f, 0.09f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tubo.RenderModel();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.25f, -0.1f, 0.25f));
		model = glm::scale(model, glm::vec3(0.09f, 1.7f, 0.09f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tubo.RenderModel();
		
		// Modelo de tomates
		// Abajo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.10f, -0.75f, 1.80f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.10f, -0.75f, 1.60f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.10f, -0.75f, 1.40f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.10f, -0.75f, 1.20f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.30f, -0.75f, 1.80f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.30f, -0.75f, 1.60f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.30f, -0.75f, 1.40f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.30f, -0.75f, 1.20f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		//Arriba
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.10f, -0.40f, 0.30f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.10f, -0.5f, 0.45f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.10f, -0.60f, 0.60f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.10f, -0.67f, 0.775f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.30f, -0.40f, 0.30f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.30f, -0.5f, 0.45f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.30f, -0.60f, 0.60f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.30f, -0.67f, 0.775f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		//////////////////
		// Abajo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.90f, -0.75f, 1.80f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.90f, -0.75f, 1.60f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.90f, -0.75f, 1.40f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.90f, -0.75f, 1.20f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.10f, -0.75f, 1.80f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.10f, -0.75f, 1.60f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.10f, -0.75f, 1.40f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.10f, -0.75f, 1.20f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		//Arriba
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.90f, -0.40f, 0.30f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.90f, -0.5f, 0.45f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.90f, -0.60f, 0.60f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.90f, -0.67f, 0.775f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.10f, -0.40f, 0.30f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.10f, -0.5f, 0.45f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.10f, -0.60f, 0.60f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.10f, -0.67f, 0.775f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		////////////////////////////
		// Abajo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.70f, -0.75f, 1.80f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.70f, -0.75f, 1.60f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.70f, -0.75f, 1.40f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.70f, -0.75f, 1.20f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.90f, -0.75f, 1.80f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.90f, -0.75f, 1.60f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.90f, -0.75f, 1.40f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.90f, -0.75f, 1.20f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		//Arriba
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.70f, -0.40f, 0.30f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.70f, -0.5f, 0.45f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.70f, -0.60f, 0.60f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.70f, -0.67f, 0.775f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.90f, -0.40f, 0.30f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.90f, -0.5f, 0.45f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.90f, -0.60f, 0.60f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.10f, -0.67f, 0.775f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();
		
		// Tomates de caja de verduras de la mesita
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.25f, -1.175f, 0.55f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.25f, -1.175f, 0.75f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.25f, -1.175f, 0.95f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.25f, -1.175f, 1.15f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.05f, -1.175f, 0.55f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.05f, -1.175f, 0.75f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.05f, -1.175f, 0.95f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.05f, -1.175f, 1.15f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		//////////////////////
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.55f, -1.175f, 0.55f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.55f, -1.175f, 0.75f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.55f, -1.175f, 0.95f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.55f, -1.175f, 1.15f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.75f, -1.175f, 0.55f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.75f, -1.175f, 0.75f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.75f, -1.175f, 0.95f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.75f, -1.175f, 1.15f));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tomate.RenderModel();
		
		// Modelo de Cajas de verduras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.20f, -0.75f, 1.5f));
		model = glm::scale(model, glm::vec3(0.008f, 0.008f, 0.008f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Box.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-4.20f, -0.55f, 0.5f));
		model = glm::scale(model, glm::vec3(0.008f, 0.008f, 0.008f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Box.RenderModel();
		
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, -0.75f, 1.5f));
		model = glm::scale(model, glm::vec3(0.008f, 0.008f, 0.008f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Box.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.0f, -0.55f, 0.5f));
		model = glm::scale(model, glm::vec3(0.008f, 0.008f, 0.008f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Box.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.8f, -0.75f, 1.5f));
		model = glm::scale(model, glm::vec3(0.008f, 0.008f, 0.008f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Box.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-5.8f, -0.55f, 0.5f));
		model = glm::scale(model, glm::vec3(0.008f, 0.008f, 0.008f));
		model = glm::rotate(model, 30 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Box.RenderModel();

		// Modelos de cajas de verduras de la mesita
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.15f, -1.175f, 0.85f));
		model = glm::scale(model, glm::vec3(0.008f, 0.008f, 0.008f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Box.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.65f, -1.175f, 0.85f));
		model = glm::scale(model, glm::vec3(0.008f, 0.008f, 0.008f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Box.RenderModel();

		// Modelos de cajas largas e interiores del local
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.2f, -1.45f, -4.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		LargeBox.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.2f, -0.85f, -4.5f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		LargeBox.RenderModel();

		// Modelo de lata de refresco
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.25f, 0.845f, -4.5f));
		model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Lata.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.65f, 0.845f, -4.5f));
		model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Lata.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.85f, 0.845f, -4.5f));
		model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Lata.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.4f, 0.1f, -4.5f));
		model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Lata.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.25f, 0.1f, -4.5f));
		model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Lata.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.7f, 0.845f, -4.5f));
		model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Lata.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.75f, 0.845f, -4.5f));
		model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Lata.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.5f, 0.8f, -3.5f));
		model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Lata.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.5f, 0.05f, -2.5f));
		model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Lata.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.5f, 0.05f, -1.25f));
		model = glm::scale(model, glm::vec3(0.035f, 0.035f, 0.035f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Lata.RenderModel();

		// Modelo de latas de aluminio
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.75f, 0.75f, -2.5f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Latas.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.75f, 0.75f, -3.5f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Latas.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.75f, 0.0f, -2.5f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Latas.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.75f, 0.0f, -1.5f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Latas.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.75f, 0.0f, -5.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Latas.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-7.75f, 0.0f, -4.35f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Latas.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.5f, 0.8f, -5.25f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Latas.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, 0.05f, -5.75f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Latas.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.7f, 0.05f, -5.75f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Latas.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.5f, 0.05f, -5.7f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Latas.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(4.0f, 0.05f, -5.7f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Latas.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, 0.8f, -5.7f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Latas.RenderModel();

		// Modelo de Piña
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.5f, 0.75f, -4.0f));
		model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pineaple.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.5f, 0.75f, -1.0f));
		model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pineaple.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.5f, 0.0f, -3.0f));
		model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pineaple.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, 0.8f, -4.5f));
		model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pineaple.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.25f, 0.8f, -4.5f));
		model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pineaple.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(3.75f, 0.05f, -4.5f));
		model = glm::scale(model, glm::vec3(0.025f, 0.025f, 0.025f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Pineaple.RenderModel();

		// Modelo de TV
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, -1.225f, -9.5f));
		model = glm::scale(model, glm::vec3(0.25f, 0.25f, 0.25f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Tv.RenderModel();
		
		// Modelo de Radio
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.2f, 0.05f, -4.4f));
		model = glm::scale(model, glm::vec3(0.11f, 0.11f, 0.11f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		Radio.RenderModel();

		
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}