/*
	Fireflies demo by
	David Mimnagh B00254398 & Michael Fowler B00226883
	this is a demo showing our AGP project 
	that combines the particles and multiple lights 
	from our first hand in
*/

// Windows specific: Uncomment the following line to open a console window for debug output
#if _DEBUG
#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
#endif

#include "rt3d.h"
#include "rt3dObjLoader.h"
#include "particleArray.h"
#include "LightManager.h"
#include "lightClass.h"
#include "SoundManager.h"
#include "FireFlyManager.h"
#include "BloomManager.h"
#include "NormalMappingManager.h"
#include "SkyBox.h"
#include "common.h"


using namespace std;

#define DEG_TO_RADIAN 0.017453293


// Globals don't worry we are fixing that ;-)

// Globals for the meshes
GLuint meshIndexCount = 0;
GLuint toonIndexCount = 0;
GLuint meshObjects[3];

// Global shaders
GLuint textureProgram;
GLuint shaderProgram;

// Global class objects
SkyBox skyBox;
FireFlyManager fireFlies;
BloomManager bloom;
NormalMappingManager bumpMapping;
LightManager spotLight;
vector <LightManager::spotLightStruct> spotLightStructure;

// Channel for the ambient background sound
HCHANNEL main_channel_;

//Globals for the projection and rotation
glm::mat4 projection(1.0);
GLfloat r = 0.0f;

//Globals for the Camera
glm::vec3 eye(-6.3f, 1.0f, 43.2f);
glm::vec3 at(0.0f, 1.0f, -1.0f);
glm::vec3 up(0.0f, 1.0f, 0.0f);

stack<glm::mat4> mvStack;

// TEXTURE STUFF
GLuint textures[4];
char startInput; bool notAsked = true, drawSkybox = true;


rt3d::materialStruct material0 = {
	{ 0.0f, 0.0f, 0.0f, 1.0f }, // ambient
	{ 0.3f, 0.3f, 0.3f, 1.0f }, // diffuse
	{ 0.1f, 0.1f, 0.1f, 1.0f }, // specular
	20.0f  // shininess
};

// Set up rendering context
SDL_Window * setupRC(SDL_GLContext &context) {
	SDL_Window * window;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) // Initialize video
		rt3d::exitFatalError("Unable to initialize SDL");

	// Request an OpenGL 3.0 context.

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);  // double buffering on
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8); // 8 bit alpha buffering
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4); // Turn on x4 multi-sampling anti-aliasing (MSAA)
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);	// 8 bit alpha buffering

												// Create 800x600 window
	window = SDL_CreateWindow("FireFlies", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!window) // Check window was created OK
		rt3d::exitFatalError("Unable to create window");

	context = SDL_GL_CreateContext(window); // Create opengl context and attach to window
	SDL_GL_SetSwapInterval(1); // set swap buffers to sync with monitor's vertical refresh rate
	return window;
}

void init(void) //init Function for the program
{
	main_channel_ = BASS_StreamCreateFile(FALSE, "Sounds/forest_ambience.mp3", 0, 0, BASS_SAMPLE_LOOP); // initialise the ambient background sound
	BASS_ChannelSetAttribute(main_channel_, BASS_ATTRIB_VOL, 0.4f); // set the channel and its attributes
	BASS_ChannelPlay(main_channel_, FALSE); // start playing the channel
	
	//Variables needed for the polygons with world objects
	vector<GLfloat> verts;
	vector<GLfloat> norms;
	vector<GLfloat> tex_coords;
	vector<GLuint> indices;
	vector<GLfloat> tangents;

	//calling the initialise of the class objects 
	skyBox.init();
	fireFlies.bufferinit();
	fireFlies.init();

	for (auto i = 0; i < 1; ++i) // push back 1 spotlight into the vector 
		spotLightStructure.push_back(LightManager::spotLightStruct());

	rt3d::loadObj("cube.obj", verts, norms, tex_coords, indices); // load the cube object
	meshIndexCount = indices.size(); // set the index count for the mesh equal to the indices size

	meshObjects[0] = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), tex_coords.data(), meshIndexCount, indices.data()); // create a mesh from the data

	shaderProgram = rt3d::initShaders("Shaders/phong-tex.vert", "Shaders/phong-tex.frag"); // Set up the shaders

	// initialise BumpMap data
	bumpMapping.init(meshObjects[0], verts, norms, tex_coords, indices, tangents);

	verts.clear(); norms.clear(); tex_coords.clear(); indices.clear(); // clear the vertices for the next world object
	rt3d::loadObj("bunny-5000.obj", verts, norms, tex_coords, indices);
	toonIndexCount = indices.size();
	meshObjects[2] = rt3d::createMesh(verts.size() / 3, verts.data(), nullptr, norms.data(), nullptr, toonIndexCount, indices.data());

	glEnable(GL_DEPTH_TEST); // enable depth test
	glEnable(GL_BLEND); // enable blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // blend Alphas

	bloom.init(); // initialise the bloom class object

				// Load textures
	textures[1] = rt3d::loadBitmap("Images/tree_texture2.bmp");
	textures[2] = rt3d::loadBitmap("Images/grass_texture.bmp");
	textures[3] = rt3d::loadBitmap("Images/tree_texture-normalmapping2.bmp");


	for (auto j = 0; j < 4; ++j) // For all the individual floats within the vec 4
	{
		GLfloat amb[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; // create the ambient for the spotlight
		spotLightStructure[0].ambient[j] = amb[j];  //set the struct ambient to the newly created ambient float[4]

		GLfloat dif[4] = { 4.0f, 4.0f, 4.0f, 1.0f };// create the diffuse for the spotlight
		spotLightStructure[0].diffuse[j] = dif[j];//set the struct diffuse to the newly created ambient float[4]

		GLfloat spe[4] = { 1.0f, 1.0f, 1.0f, 1.0f };// create the specular for the spotlight
		spotLightStructure[0].specular[j] = spe[j];//set the struct specular to the newly created ambient float[4]

		GLfloat position[4] = { -10.0f, 5.0f, -10.0f, 1.0f };// create the position for the spotlight
		spotLightStructure[0].position[j] = position[j];//set the struct position to the newly created ambient float[4]

		auto spotRadius = 0.5f; // radius for the spotlight
		spotLightStructure[0].radius = spotRadius;

		auto spotCutOff = 6.0f; // cutOff for the spotlight
		spotLightStructure[0].cutoff = spotCutOff;
	}
}

glm::vec3 moveForward(glm::vec3 pos, GLfloat angle, GLfloat d) {
	return glm::vec3(pos.x + d*std::sin(r*DEG_TO_RADIAN), pos.y, pos.z - d*std::cos(r*DEG_TO_RADIAN));
}

glm::vec3 moveRight(glm::vec3 pos, GLfloat angle, GLfloat d) {
	return glm::vec3(pos.x + d*std::cos(r*DEG_TO_RADIAN), pos.y, pos.z + d*std::sin(r*DEG_TO_RADIAN));
}


void update() // update function for the program
{
	const Uint8 *keys = SDL_GetKeyboardState(nullptr);

	fireFlies.update(keys, eye); // calling the update function for the fireflies class object 

					// Camera Movements
	if (keys[SDL_SCANCODE_W]) eye = moveForward(eye, r, 0.1f);
	if (keys[SDL_SCANCODE_S]) eye = moveForward(eye, r, -0.1f);
	if (keys[SDL_SCANCODE_A]) eye = moveRight(eye, r, -0.1f);
	if (keys[SDL_SCANCODE_D]) eye = moveRight(eye, r, 0.1f);
	if (keys[SDL_SCANCODE_COMMA]) r -= 1.0f;
	if (keys[SDL_SCANCODE_PERIOD]) r += 1.0f;
	
				// SpotLight Controller
	if (keys[SDL_SCANCODE_P])
	{
		spotLightStructure[0].diffuse[0] = 0.0f;
		spotLightStructure[0].diffuse[1] = 0.0f;
		spotLightStructure[0].diffuse[2] = 0.0f;
	}
	if (keys[SDL_SCANCODE_O])
	{
		spotLightStructure[0].diffuse[0] = 4.0f;
		spotLightStructure[0].diffuse[1] = 4.0f;
		spotLightStructure[0].diffuse[2] = 4.0f;
	}
	if(keys[SDL_SCANCODE_F3]) drawSkybox = false;
	if (keys[SDL_SCANCODE_F4]) drawSkybox = true;


	if (keys[SDL_SCANCODE_ESCAPE]) exit(1); // exit the program

}

void drawSpotLight() // Draw spotlight function
{
	glm::vec4 LPos(spotLightStructure[0].position[0], spotLightStructure[0].position[1], spotLightStructure[0].position[2], 1.0); // variable for current light position
	auto outLPos = LPos * mvStack.top(); // multiplying it by the view matrix into world space

	for (auto i = 0; i < 4; ++i)
		spotLightStructure[0].position[i] = outLPos[i]; // setting the position to the new world space one

			//Sending through the structure data to the shader
	auto uniformIndex = glGetUniformLocation(shaderProgram, "spambient");
	glUniform4fv(uniformIndex, 1, spotLightStructure[0].ambient);
	uniformIndex = glGetUniformLocation(shaderProgram, "spdiffuse");
	glUniform4fv(uniformIndex, 1, spotLightStructure[0].diffuse);
	uniformIndex = glGetUniformLocation(shaderProgram, "spspecular");
	glUniform4fv(uniformIndex, 1, spotLightStructure[0].specular);
	uniformIndex = glGetUniformLocation(shaderProgram, "spposition");
	glUniform4fv(uniformIndex, 1, spotLightStructure[0].position);
	uniformIndex = glGetUniformLocation(shaderProgram, "spradius");
	glUniform4fv(uniformIndex, 1, &spotLightStructure[0].radius);
	uniformIndex = glGetUniformLocation(shaderProgram, "spcutoff");
	glUniform4fv(uniformIndex, 1, &spotLightStructure[0].cutoff);
}

void drawSceneObjects()
{
	// draw a cube for ground plane
	glBindTexture(GL_TEXTURE_2D, textures[2]);
	mvStack.push(mvStack.top());
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(-10.0f, -3.1f, -10.0f));
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(40.0f, 0.1f, 40.0f));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::setMaterial(shaderProgram, material0);
	rt3d::drawIndexedMesh(meshObjects[0], meshIndexCount, GL_TRIANGLES);
	mvStack.pop();


	// draw the toon shaded bunny
	mvStack.push(mvStack.top());
	mvStack.top() = glm::translate(mvStack.top(), glm::vec3(-10.0f, 0.1f, -10.0f));
	mvStack.top() = glm::scale(mvStack.top(), glm::vec3(20.0, 20.0, 20.0));
	rt3d::setUniformMatrix4fv(shaderProgram, "modelview", glm::value_ptr(mvStack.top()));
	rt3d::setMaterial(shaderProgram, material0);
	rt3d::drawIndexedMesh(meshObjects[2], toonIndexCount, GL_TRIANGLES);
	mvStack.pop();
}

void draw(SDL_Window * window)
{
	glUseProgram(shaderProgram);
	// clear the screen
	bloom.clear();
	projection = glm::perspective(float(60.0f*DEG_TO_RADIAN), 800.0f / 600.0f, 1.0f, 150.0f);

	//auto scale(1.0f); // just to allow easy scaling of complete scene

	glm::mat4 modelview(1.0); // set base position for scene
	mvStack.push(modelview); // push in the modelview

	at = moveForward(eye, r, 1.0f);
	mvStack.top() = glm::lookAt(eye, at, up); // setting the camera to the top of the stack

	// Calling the draw function of the skybox class
	if(drawSkybox)
	skyBox.draw(projection, textures, mvStack, eye, meshObjects[0], meshIndexCount);

	// back to remainder of rendering
	glDepthMask(GL_TRUE); // make sure depth test is on
	glEnable(GL_DEPTH_TEST); // enabling depth test

	//================================SPOTLIGHT================================================================
	glUseProgram(shaderProgram); // Use the shaderProgram
	drawSpotLight();// calling the draw spotLight function
	//=============================================================================================

	drawSceneObjects(); // Draw all the scene objects

	//================================BUMP MAPPING========================================
	bumpMapping.draw(projection, textures, mvStack, spotLightStructure, eye, meshObjects[0], meshIndexCount); // Calling the draw function for the bumpMapping class object
	//======================================================================================================

	//==============================BLOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOM===============================================================	
	bloom.particleFBO(); // Draw the particle(fireflies) FBO'S
	fireFlies.draw(projection, mvStack, eye, shaderProgram); // Draw the Fireflies
	bloom.draw(projection, mvStack, eye, skyBox.getShader(), meshObjects[0]);//Calling the draw function of the bloom class object
	//====================================================================================================================
	
	// remember to use at least one pop operation per push...
	mvStack.pop(); // initial matrix

	glDepthMask(1); // enable depth mask

	SDL_GL_SwapWindow(window); // swap buffers
}

// Program entry point - SDL manages the actual WinMain entry point for us
int main(int argc, char *argv[]) {
	SDL_Window * hWindow; // window handle
	SDL_GLContext glContext; // OpenGL context handle
	hWindow = setupRC(glContext); // Create window and render context 

	BASS_Init(-1, 44100, BASS_DEVICE_3D, nullptr, nullptr); // Initialise BASS
	BASS_SetVolume(0.7f); // set the overall volume for the sound

	// Required on Windows *only* init GLEW to access OpenGL beyond 1.1
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err) { // glewInit failed, something is seriously wrong
		std::cout << "glewInit failed, aborting." << endl;
		exit(1);
	}
	cout << glGetString(GL_VERSION) << endl;

	init(); // calling the init function
	srand(static_cast <unsigned> (time(nullptr)));
	bool running = true; // set running to true
	SDL_Event sdlEvent;  // variable to detect SDL events
	while (running) {	// the event loop
		while (SDL_PollEvent(&sdlEvent)) {
			if (sdlEvent.type == SDL_QUIT)
				running = false;
		}
		update(); // calling the update function
		draw(hWindow); // call the draw function
	}
	BASS_Free(); // stop the BASS music and sounds

				// Clean up
	SDL_GL_DeleteContext(glContext); 
	SDL_DestroyWindow(hWindow);
	SDL_Quit();

	return 0;
}