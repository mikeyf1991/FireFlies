#include "particleArray.h"

particleArray::particleArray(const int n, bool c) : numParticles(n) // constructor for the Particle class
{
	if (numParticles <= 0) // trap invalid input
		return;

	created = c;
	// creating the postions and colours in memory
	positions = new glm::vec3[numParticles];
	colours = new glm::vec3[numParticles];

	//initializing the time related variables
	c_time = 0;
	n_time = 0;
	l_time = 0;
	d_time = 0;
	
	for (auto i = 0; i < numParticles; i++)	//co-ordinate initialization
	{
		positions[i].x = 0;	positions[i].y = 0; positions[i].z = 0; 
	}

	for (auto i = 0; i < numParticles; i++)	// colour initiliasation
	{
		colours[i] = glm::vec3(1.0f);
	}
	setCurrentLight(0); // setting the current light to zero
}


void particleArray::initsound() // function for initialising the sound
{
	fireflySound = BASS_SampleLoad(FALSE, "Sounds/fly_sound.wav", 0, 0, 1, BASS_SAMPLE_LOOP); // loading the sound
	fly_channel_ = BASS_SampleGetChannel(fireflySound, FALSE); // setting the channel with the sound
	BASS_ChannelSetAttribute(fly_channel_, BASS_ATTRIB_VOL, 0.4f); // setting the channel attributes

	BASS_Set3DFactors(1.0, 1.0, 1.0); // setting the initial 3d factors

	BASS_Apply3D(); // apply the change
}

void particleArray::init() // initialising the particle array class
{
	initsound(); // calling the init sound function

	glEnable(GL_POINT_SPRITE); // enable GL point sprite to draw particles
	// Initialise VAO and VBO
	glGenVertexArrays(1, vao); // generate the Vertex arrays
	glGenBuffers(2, vbo); // generate the 2 buffers

	glBindVertexArray(vao[0]); // bind VAO 0 as current object Then Position data in attribute index 0, 3 floats per vertex
							  
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // bind VBO for positions

	glBufferData(GL_ARRAY_BUFFER, numParticles * 3 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);

	for (auto i = 0; i < numParticles; i++) 
		glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * i, 3 * sizeof(GLfloat), glm::value_ptr(positions[i]));
	

	glEnableVertexAttribArray(0);     // Enable attribute index 0
	glVertexAttribPointer(static_cast<GLuint>(0), 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Colours data in attribute 1, 3 floats per vertex
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // bind VBO for colours
	glBufferData(GL_ARRAY_BUFFER, numParticles * 3 * sizeof(GLfloat), nullptr, GL_DYNAMIC_DRAW);

	for (auto i = 0; i < numParticles; i++) 
		glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * i, 3 * sizeof(GLfloat), glm::value_ptr(colours[i]));
	
	glVertexAttribPointer(static_cast<GLuint>(1), 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);    // Enable attribute index 1

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void particleArray::clockTimerRandomColour() // clock timer function for the Randomisation of colours
{
	this_timeRC = clock(); // setting the clock

	time_counterRC += static_cast<double>(this_timeRC - last_timeRC); // incrementing

	last_timeRC = this_timeRC; // last time equal to this current time

	if (time_counterRC > static_cast<double>(FIREFLY_RANDOM_TIMER * CLOCKS_PER_SEC)) // if the count is greater than that of the set time defined ( in this case 5 seconds)
	{
		if (getDoRandimsing() == true) // re-randomise the colours
		{
			time_counterRC = 0; // reset the counter
			setRandomised(true); // set the particles value to true stating that they have been randomised
		}
	}
}

void particleArray::clockTimerScared() // clock function for the scared AI 
{
	this_timeS = clock(); // starting the clock
	glm::vec4 previousColour; // vec4 for holding the previous colour of the particles
	for (auto j = 0; j < 4; ++j) // for loop to set the hold colour and then set the new colour to white
	{
		previousColour[j] = lStruct.diffuse[j];
		lStruct.diffuse[j] = 1.0f;
	}
	time_counterS += static_cast<double>(this_timeS - last_timeS); // incrementing

	last_timeS = this_timeS; // last time is equal to new time

	if (time_counterS >static_cast<double>(FIREFLY_SCARED_TIMER * CLOCKS_PER_SEC)) // checking to see if the counter is above the time (this case its set to check being above 3 seconds)
	{
		time_counterS = 0; // reset the counters
		time_counterRC = 0; // reset the Randomise colour counter for all
		setScared(false); // the fireflies no longer are scared

		for (auto j = 0; j < 4; ++j) // set the diffuse back to the previous before being scared
			lStruct.diffuse[j] = previousColour[j];
	}
}

void particleArray::moveX(int movementTime)
{
	//Random X direction--------------------------------------------
	if (counterX == movementTime)
	{
		rx = ((std::rand() % 100) - 50);  // goal speed

		if (rx == 0)
			rx = ((std::rand() % 100) - 50); // the random value is 0, re-randomise

		counterX = 0;
		olddx = dx; // dx = current speed, olddx = old speed
	}
	else
	{
		++counterX; // incrementing
		if (rx > 0) // if the speed is greated than 0, then
		{
			if (dx < rx) // if the current speed is  less that goal
			{
				dx += (abs(rx) + abs(olddx)) / movementTime; // move in positive
			}
			else
			{

				dx -= (abs(rx) + abs(olddx)) / movementTime; // move in negative
			}
		}
		else
		{
			if (dx < rx) // if it is less than goal
			{
				dx += (abs(rx) + abs(olddx)) / movementTime; // move positive
			}
			else
			{
				dx -= (abs(rx) + abs(olddx)) / movementTime; // move negative
			}
		}
	}
	//Simple boundry checks
	if (lStruct.position[0] <= -46.1)
		dx *= -1;
	if (lStruct.position[0] >= 29.1)
		dx *= -1;

	lStruct.position[0] += dx / 6000; // moving position positively in relation to current speed
}

void particleArray::moveY(int movementTime)
{
	//Random Y direction--------------------------------------------
	if (counterY == movementTime)
	{
		ry = ((std::rand() % 100) - 50); // goal speed

		if (ry == 0)
		{
			ry = ((std::rand() % 100) - 50); // the random value is 0, re-randomise

		}
		counterY = 0;
		olddy = dy;// dy = current speed, olddy = old speed
	}
	else
	{
		++counterY;
		// incrementing
		if (ry > 0) // if the speed is greated than 0, then
		{
			if (dy < ry) // if current less than goal
			{

				dy += (abs(ry) + abs(olddy)) / movementTime; // move in positive
			}
			else
			{

				dy -= (abs(ry) + abs(olddy)) / movementTime;// move in negative

			}
		}
		else
		{
			if (dy < ry)  // if current less than goal
			{
				dy += (abs(ry) + abs(olddy)) / movementTime;// move in positive
			}
			else
			{
				dy -= (abs(ry) + abs(olddy)) / movementTime;// move in negative
			}
		}
	}

	if (lStruct.position[1] <= -1.1)
		dy *= -1;
	if (lStruct.position[1] >= 18.1)
		dy *= -1;


	lStruct.position[1] += dy / 7000;// moving position positively in relation to current speed
}

void particleArray::moveZ(int movementTime)
{


	//Random Z direction--------------------------------------------
	if (counterZ == movementTime)
	{
		rz = ((std::rand() % 100) - 50);

		if (rz == 0)
			rz = ((std::rand() % 100) - 50);
		counterZ = 0;
		olddx = dx;
	}
	else
	{
		++counterZ;
		if (rz > 0)
		{
			if (dz < rz)// if current less than goal
			{
				dz += (abs(rz) + abs(olddz)) / movementTime; // move in positive
			}
			else
			{
				dz -= (abs(rz) + abs(olddz)) / movementTime; // move in negative
			}
		}
		else
		{
			if (dz < rz)// if current less than goal
			{
				dz += (abs(rz) + abs(olddz)) / movementTime; // move in positive

			}
			else
			{
				dz -= (abs(rz) + abs(olddz)) / movementTime; // move in negative
			}
		}
	}
	if (lStruct.position[2] <= -46.1)
		dz *= -1;
	if (lStruct.position[2] >= 26.1)
		dz *= -1;


	lStruct.position[2] += dz / 6000;// moving position positively in relation to current speed
}

void particleArray::fireFlyAI()
{
	
	if (getScared() == true) // if the fireflies are scared
	{
		time_counterRC = 0; // set the timer for colouring the particles to 0
		clockTimerScared(); // start the scared timer
	}
	else if (getScared() == false)
	{
		clockTimerRandomColour(); // calls the random colour function
		moveX(500); // moves the random X direction
		moveY(500);// moves the random Y direction
		moveZ(500);// moves the random Z direction
	}

}

void particleArray::boundryCheckX(float posX)
{
	if (posX >= lStruct.position[0] - 5.0 && posX <= lStruct.position[0] + 5.0) // check if the camera is within the boundry on the X
	{
		touchingX = true; // if it is, set this variable to true
	}
	else
		touchingX = false; // if not, false
}

void particleArray::boundryCheckY(float posY)
{
	if (posY >= lStruct.position[1] - 5.0 && posY <= lStruct.position[1] + 5.0) // check if the camera is within the boundry on the Y
	{
		touchingY = true; // if it is, set this variable to true
	}
	else
		touchingY = false; // if not, false
}

void particleArray::boundryCheckZ(float posZ)
{
	if (posZ >= lStruct.position[2] - 5.0 && posZ <= lStruct.position[2] + 5.0) // check if the camera is within the boundry on the Z
	{
		touchingZ = true; // if it is, set this variable to true
	}
	else
		touchingZ = false; // if not, false
}

void particleArray::checkTouching(float x, float y, float z)
{
	//check all of the boundries
	boundryCheckX(x);
	boundryCheckY(y);
	boundryCheckZ(z);

	if (touchingX && touchingY && touchingZ) // if they are all true, then 
	{
		setScared(true); // set the firefly to be scared
	}
}

void particleArray::moveLight(float speed, bool x, bool y, bool z, bool neg_x, bool neg_y, bool neg_z)
{
	//Movement for positive directions on each axis
	if (x) lStruct.position[0] += speed;
	if (y) lStruct.position[1] += speed;
	if (z) lStruct.position[2] += speed;
	//Movement for negative directions on each axis
	if (neg_x) lStruct.position[0] -= speed;
	if (neg_y) lStruct.position[1] -= speed;
	if (neg_z) lStruct.position[2] -= speed;
}

void particleArray::clearColour()
{
	setRandomised(false); // set the randomised value to false
	setDoRandomising(false); // set the value for doing randomising to false
	
	//set all the diffuse values to gray and the alpha to 1
	lStruct.diffuse[0] = 0.1f;
	lStruct.diffuse[1] = 0.1f;
	lStruct.diffuse[2] = 0.1f;
	lStruct.diffuse[3] = 1.0f;
}

void particleArray::update() // update function for the Particles
{
	fireFlyAI(); // calling the firefly AI function
	
	//incrementing all of the timers being used in the functions
	n_time += 0.33f;
	d_time = (n_time - l_time);
	life_time -= d_time;
	c_time += d_time;
	l_time = n_time;

	//setting the positions to the lights positions
	positions->x = lStruct.position[0];
	positions->y = lStruct.position[1];
	positions->z = lStruct.position[2];

	//setting the colours of the particles to the diffuse values of the lights
	colours->r = lStruct.diffuse[0];
	colours->g = lStruct.diffuse[1];
	colours->b = lStruct.diffuse[2];

	l_time = n_time; // setting last time to = new time
	
}

void particleArray::playSound(glm::vec3 eye) const // play sound function
{
	auto eyePosInWorld = sqrt((eye.x * eye.x) + (eye.y * eye.y) + (eye.z * eye.z)); // eye position in within the world
	auto particlePosInWorld = sqrt((positions->x * positions->x) + (positions->y * positions->y) + (positions->z * positions->z)); // particles position within the world

	// if statements for comparing distance between the two, then altering the volume accordingly
	if (eyePosInWorld - particlePosInWorld <= 2.5f) 
	{
		BASS_ChannelSetAttribute(fly_channel_, BASS_ATTRIB_VOL, 0.4f);
		BASS_ChannelPlay(fly_channel_, FALSE);
	}
	else if (eyePosInWorld - particlePosInWorld <= 5.0f)
	{
		BASS_ChannelSetAttribute(fly_channel_, BASS_ATTRIB_VOL, 0.2f);
		BASS_ChannelPlay(fly_channel_, FALSE);
	}
	else if (eyePosInWorld - particlePosInWorld <= 7.5f)
	{
		BASS_ChannelSetAttribute(fly_channel_, BASS_ATTRIB_VOL, 0.1f);
		BASS_ChannelPlay(fly_channel_, FALSE);
	}
	else
		BASS_ChannelPause(fly_channel_); // if not in range, then stop the sound
	
}

void particleArray::draw(void) // draw function for the particles
{
	glEnable(GL_BLEND); // enable GL_BLEND
	glEnable(GL_PROGRAM_POINT_SIZE); // Enable point size, this allows the scaling depending on distance from the viewer

	glBindVertexArray(vao[0]); // bind VAO 0 as current object particle data may have been updated - so need to resend to GPU

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // bind VBO 0

	for (auto i = 0; i < numParticles; i++)
		glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat)*i, 3 * sizeof(GLfloat), glm::value_ptr(positions[i]));
	
	// Position data in attribute index 0, 3 floats per vertex
	glEnableVertexAttribArray(0);     // Enable attribute index 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);



	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // bind VBO 1

	for (auto i = 0; i < numParticles; i++)
		glBufferSubData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat)*i, 3 * sizeof(GLfloat), glm::value_ptr(colours[i]));

	// Position data in attribute index 0, 3 floats per vertex
	glEnableVertexAttribArray(1);     // Enable attribute index 0
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	// Now draw the particles... as easy as this!
	glDrawArrays(GL_POINTS, 0, numParticles);
	glBindVertexArray(0);
	glDisable(GL_BLEND);

}

void particleArray::drawLighting(GLuint shaderProgram, glm::mat4 projection, GLuint LightBuffer, std::vector<particleArray::fireflyStructure> lStruct) const
{
	GLint activeBlock;	//Int active light buffer block 
	auto b_Index = glGetUniformBlockIndex(shaderProgram, "Lights"); // creating the index for the buffers
	glGetActiveUniformBlockiv(shaderProgram, b_Index, GL_UNIFORM_BLOCK_DATA_SIZE, &activeBlock); // getting the current active buffer
	rt3d::setUniformMatrix4fv(shaderProgram, "projection", glm::value_ptr(projection)); //sending the projection to the shader program
	glBindBuffer(GL_UNIFORM_BUFFER, LightBuffer); // binding the buffer
	glBufferData(GL_UNIFORM_BUFFER, sizeof(particleArray::fireflyStructure) * LIGHT_COUNT, lStruct.data(), GL_DYNAMIC_DRAW); // collecting the new light data
	glBindBufferBase(GL_UNIFORM_BUFFER, b_Index, LightBuffer); // Binding the buffer to the uniform block within the shader
}

particleArray::~particleArray() // destructor for deleting those nast pointers
{
	delete[] positions;
	delete[] colours;
}
