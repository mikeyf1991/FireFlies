#include "FireFlyManager.h"



FireFlyManager::FireFlyManager(): particleShaderProgram(0), main_channel_(0), LightBuffer(0), particleTexture(0), startInput(0) // initialize variables
{
	particleStruct.resize(LIGHT_COUNT); // set the particleStruct vector to the size of the Light count

	retrievedInput = false;

	notAsked = true;
}

void FireFlyManager::bufferinit()
{
	glGenBuffers(1, &LightBuffer); //Generate the Buffers initially
	glBindBuffer(GL_UNIFORM_BUFFER, LightBuffer); // Binding the light buffer
	glBufferData(GL_UNIFORM_BUFFER, sizeof(particleArray::fireflyStructure) * LIGHT_COUNT, nullptr, GL_DYNAMIC_DRAW);//Getting the buffer data
}

void FireFlyManager::init()
{

	for (auto i = 0; i < LIGHT_COUNT; ++i)
		particles.push_back(new particleArray(1, false)); // push a new particle object into the vector for the amount up til lthe light count

	if (notAsked == true) // function of checking if the user has been asked about randomising the lights on a timer
	{
		notAsked = false;
		std::cout << " Would you like the lights to randomise on a timer? Y/N" << std::endl;
		std::cin.get(startInput);
		for (auto& Particles : particles)
		{
			if (startInput == 'Y' || startInput == 'y')
				Particles->setDoRandomising(true);
			else
				Particles->setDoRandomising(false);
		}
	}
	particleShaderProgram = rt3d::initShaders("Shaders/particle.vert", "Shaders/particle.frag"); // initialising the shader for the particle
	for (auto& Particles : particles)
		Particles->init(); // initializing the particles

	particleTexture = rt3d::loadBitmap("Images/smoke1.bmp"); // loading the particle texture

	for (auto i = 0; i < LIGHT_COUNT; ++i) // For all the lights 
	{
		GLfloat randposition[4] = { static_cast<float>(((std::rand() % 80 ) - 50), static_cast<float>((std::rand() % 60 ) / 20), static_cast<float>((std::rand() % 80 ) - 50), 1.0f) }; // generating a random value of the particle position
		particles[i]->setPosition(randposition);//setting the particle position to the randomly generated one
		for (auto j = 0; j < 4; ++j) // For all the individual floats within the vec 4
		{
			particles[i]->lStruct.ambient[j] = particles[i]->getAmbient(j);  //set the ambient for the struct
			particles[i]->lStruct.diffuse[j] = particles[i]->getDiffuse(j); // set the diffuse for the struct
			particles[i]->lStruct.specular[j] = particles[i]->getSpecular(j); // set the specular for the struct

			particles[i]->setAmbientControl(&particles[i]->lStruct.ambient[j]); // set the particle class ambient
			particles[i]->setDiffuseControl(&particles[i]->lStruct.specular[j]); // set the particle class diffuse
			particles[i]->setSpecularControl(&particles[i]->lStruct.specular[j]); // set the particle class specular
		}

	}
}

void FireFlyManager::getInput() // function for getting the input for more freedom with light selection
{
	std::string num = nullptr;
	auto number = 0;
	std::cout << "Enter light # to change: ";
	std::getline(std::cin, num);
	number = std::stoi(num);
	for (auto& Particles : particles)
	{
		Particles->setCurrentLight(0);
		if (number <= LIGHT_COUNT && number >= 0)
			Particles->setCurrentLight(number);
		retrievedInput = true;
		break;

	}
	std::cout << "Light No: " << number << std::endl;
}

void FireFlyManager::randomiseLightColours() // function for randomising the light colours
{
	for (auto& Particles : particles)
	{
		glm::vec4 temp;
		for (auto j = 0; j < 4; ++j) // For all the individual floats within the vec 4
		{
			temp[j] = static_cast<GLfloat>(static_cast <float> (rand()) / static_cast <float> (RAND_MAX)); // random number between 0.0 and 1.0
			Particles->lStruct.diffuse[j] = temp[j]; // set the diffuse to the randomly generated value
		}
		temp[3] = 1.0f; // setting the alpha to 1
		Particles->lStruct.diffuse[3] = temp[3]; // setting the struct diffuse alpha to the temp alpha
	}
}

void FireFlyManager::update(const Uint8 *keys, glm::vec3 eye)
{
	for (auto& Particles : particles) // ability to identify all of the particle objects within the particle class with "Particle" keyword  within block.
	{
		Particles->playSound(eye);//for playing the sounds, passing in the eye co-ordinates 

		//=================================== Handles the firefly selection ===================================
		if (keys[SDL_SCANCODE_1])
		{
			Particles->setCurrentLight(0);
			retrievedInput = false;
		}
		else if (keys[SDL_SCANCODE_2])
		{
			Particles->setCurrentLight(1);
			retrievedInput = false;
		}
		else if (keys[SDL_SCANCODE_3])
		{
			Particles->setCurrentLight(2);
			retrievedInput = false;
		}
		else if (keys[SDL_SCANCODE_4])
		{
			Particles->setCurrentLight(3);
			retrievedInput = false;
		}
		else if (keys[SDL_SCANCODE_5])
		{
			Particles->setCurrentLight(4);
			retrievedInput = false;
		}
		else if (keys[SDL_SCANCODE_6])
		{
			Particles->setCurrentLight(5);
			retrievedInput = false;
		}

		if (keys[SDL_SCANCODE_7])
		{
			Particles->setCurrentLight(6);
			retrievedInput = false;
		}
		//====================================================================================================

		//------------------------------------- Handles the firefly colouring --------------------------------
		if (keys[SDL_SCANCODE_R])
		{
			particles[Particles->getCurrentLight()]->lStruct.diffuse[0] = 1.0f;
			retrievedInput = false;
		}

		if (keys[SDL_SCANCODE_G])
		{
			particles[Particles->getCurrentLight()]->lStruct.diffuse[1] = 1.0f;
			retrievedInput = false;
		}

		if (keys[SDL_SCANCODE_B])
		{
			particles[Particles->getCurrentLight()]->lStruct.diffuse[2] = 1.0f;
			retrievedInput = false;
		}

		if (keys[SDL_SCANCODE_T])
		{
			particles[Particles->getCurrentLight()]->lStruct.diffuse[0] = 0.0f;
			retrievedInput = false;
		}

		if (keys[SDL_SCANCODE_H])
		{
			particles[Particles->getCurrentLight()]->lStruct.diffuse[1] = 0.0f;
			retrievedInput = false;
		}

		if (keys[SDL_SCANCODE_N])
		{
			particles[Particles->getCurrentLight()]->lStruct.diffuse[2] = 0.0f;
			retrievedInput = false;
		}

		if (keys[SDL_SCANCODE_C])
		{
			Particles->clearColour();
			retrievedInput = false;
		}
		if (keys[SDL_SCANCODE_Z])
		{
			Particles->clearColour();
			randomiseLightColours();
		}
		//----------------------------------------------------------------------------------------------------

		//============================== Move the currently selected firefly =================================
		if (keys[SDL_SCANCODE_KP_8])
		{
			particles[Particles->getCurrentLight()]->moveLight(0.05, false, false, false, false, false, true);
		}
		if (keys[SDL_SCANCODE_KP_5])
		{
			particles[Particles->getCurrentLight()]->moveLight(0.05, false, false, true, false, false, false);
		}
		if (keys[SDL_SCANCODE_KP_4])
		{
			particles[Particles->getCurrentLight()]->moveLight(0.05, false, false, false, true, false, false);
		}
		if (keys[SDL_SCANCODE_KP_6])
		{
			particles[Particles->getCurrentLight()]->moveLight(0.05, true, false, false, false, false, false);
		}

		//====================================================================================================

		if (Particles->getRandomised() == true) // calling the randomise timer function for the colours
		{
			randomiseLightColours();
			Particles->setRandomised(false);
		}

		for (auto i = 0; i < particles.size(); i++) // calling the update for all of the particles
		{
			particles[i]->update();
		}

	}

}

void FireFlyManager::draw(glm::mat4 projection, std::stack<glm::mat4> mvStack, glm::vec3 eye, GLuint shaderprogram)
{
	glUseProgram(shaderprogram); // Using the Shader program	
	rt3d::setUniformMatrix4fv(shaderprogram, "projection", glm::value_ptr(projection)); // passing the projection to the shaderprogram
	rt3d::setUniformMatrix4fv(shaderprogram, "modelview", glm::value_ptr(mvStack.top()));// passing the modelview to the shader

	for (auto i = 0; i < LIGHT_COUNT; ++i) // for all of the fireflies
	{
		for (auto j = 0; j < 4; ++j) // For all the individual floats within the vec 4
		{
			particleStruct[i] = particles[i]->lStruct; // set the particleStruct equal to the particle struct data
		}
		particles[i]->checkTouching(eye.x, eye.y, eye.z);//check if the eye is within range of the particle

		particles[i]->lStruct.position[0] = particles[i]->getPositions()->x; // update the position
		particles[i]->lStruct.position[1] = particles[i]->getPositions()->y; // update the position
		particles[i]->lStruct.position[2] = particles[i]->getPositions()->z; // update the position

		particles[i]->drawLighting(shaderprogram, projection, LightBuffer, particleStruct); // draw the lights within the particles
	}

	glUseProgram(particleShaderProgram); // use the particle shader program
	// Update particles
	glDepthMask(0);//disabling depth mask
	// pass modelview & projection as uniforms into shader
	glBindTexture(GL_TEXTURE_2D, particleTexture); // bind the particle texture
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // blend the Alpha
	rt3d::setUniformMatrix4fv(particleShaderProgram, "modelview", glm::value_ptr(mvStack.top()));// passing the modelview to the  particle shader
	rt3d::setUniformMatrix4fv(particleShaderProgram, "projection", glm::value_ptr(projection));// passing the projection to the particle shader
	// Update particles
	for (auto i = 0; i < particles.size(); i++)
	{
		particles[i]->update(); // update all particles
		particles[i]->draw(); // draw all the particles
	}
	glDepthMask(GL_TRUE); // re-enable depth mask
}

FireFlyManager::~FireFlyManager()
{
}
