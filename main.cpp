#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<iostream>
#include <math.h>
#include <ImageLoader/stb_image.h>
#include "Shaders/ShaderHelper.cpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//predefined Functions
void frame_buffer_callback(GLFWwindow*,int,int);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void input(GLFWwindow*);

//global

int  success;
char infoLog[512];

//cube using element Array scheme
float vertices[] = {
	//Top Plane
	// positions          // colors           // texture coords
    -0.5f,  0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // top right
     0.5f,  0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   // bottom right
     0.5f,  0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // bottom left
    -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // top left

	//Bottom plane
	-0.5f, -0.5f, -0.5f,   1.0f, 0.0f, 0.0f,   0.0f, 0.1f,   // top right
	 0.5f, -0.5f, -0.5f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // bottom right
	 0.5f, -0.5f,  0.5f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // top left

};

unsigned int indices[] = {
	//face 1
	0, 1, 2,
	2, 3, 0,
	//face 2
	0, 1, 5,
	5, 4, 0,
	//face 3
	0, 4, 7,
	7, 3, 0,
	//face 4
	1, 5, 6,
	6, 2, 1,
	//face 5
	4, 5, 6,
	6, 7, 4,
	//face 6
	3, 2, 6,
	6, 7, 3
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

//camera position
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

// camera rotation
float lastX = 400, lastY = 300;
float yaw = -90.0f;
float pitch = 0.0f;

//ZOOM
float fov = 45.0f;

//delta
float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

int main(){


	//initializing

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(600, 600, "openGL", NULL,NULL);

	if(window == NULL)
	{
		std::cout<<"creating Windows failed"<<std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	//openGl function loader
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout<<"failed to load glad"<<std::endl;
	}

	glViewport(0,0,600,600);

	//SHADERS:

	Shader shader("Shaders/vertex.glsl","Shaders/fragment.glsl");

	//BUFFERS

	unsigned int VBO,VAO,EBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1,&VBO);
	glGenBuffers(1,&EBO);


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);


	glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1,3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),(void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2,2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),(void*) (6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//TEXTURE

	int width, height, no_of_channel;
	unsigned int texture;
	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D,texture);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	unsigned char* data = stbi_load("assets/wall.jpg", &width, &height, &no_of_channel, 0);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	//TRANSFORMATIONS
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(0.0f,0.0f,0.0f));

	//MODEL MATRIX - WORLD SPACE COORDINATE
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	//VIEW MATRIX - CAMERA OR CLIP SPACE COORDINATE
	//const float radius = 10.0f;

	//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	//float camX = sin(glfwGetTime()) * radius;
	//float camZ = cos(glfwGetTime()) * radius;
	//glm::vec3 cameraPos = glm::vec3(camX, 0.0f, camZ);
	//glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	//glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	//glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

	glm::mat4 view = glm::mat4(1.0f);
	// note that we're translating the scene in the reverse direction of where we want to move
	//view = glm::lookAt(cameraPos,cameraTarget,up);
	//PROJECTION MATRIX
	glm::mat4 projection;

	glfwSetFramebufferSizeCallback(window, frame_buffer_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//disable mouse pointer

	glEnable(GL_DEPTH_TEST);

	while(!glfwWindowShouldClose(window))	{

		//calculate delta
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		input(window);

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //state-setting function
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //state_using function

		//rendering commands
		shader.use();



		//tranforming the object
		float time = (float)glfwGetTime();
		GLint location = glGetUniformLocation(shader.ID,"offset");
		trans = glm::mat4(1.0f);
		trans  = glm::rotate(trans,(-time), glm::vec3(0.0f,0.0f,1.0f));
		glUniformMatrix4fv(location,1,GL_FALSE,glm::value_ptr(trans));

		//pass model matrix
		int modelLoc = glGetUniformLocation(shader.ID, "model");
		//pass view matrix
		int viewLoc = glGetUniformLocation(shader.ID, "view");

		//Camera position

		view = glm::lookAt(cameraPos,cameraPos + cameraFront,cameraUp);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		//pass projection matrix
		int projectionLoc = glGetUniformLocation(shader.ID, "projection");
		projection = glm::perspective(glm::radians(fov), 800.0f / 600.0f, 0.1f, 100.0f);
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glBindTexture(GL_TEXTURE_2D,texture);
		glBindVertexArray(VAO);
		for(unsigned int i = 0; i < 10; i++)
		{
		    glm::mat4 model = glm::mat4(1.0f);
		    model = glm::translate(model, cubePositions[i]);
		    float angle = 20.0f * i;
		    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);
		}
		//glDrawArrays(GL_TRIANGLES,0,3);



		glfwSwapBuffers(window); //swaps the back buffer to front buffer(the visible buffer)
		glfwPollEvents(); //listens all event queue
	}


	 glDeleteVertexArrays(0, &VAO);
	 glDeleteBuffers(1, &VBO);
	 glDeleteBuffers(1,&EBO);
	 shader.deleteProgram();
	glfwTerminate();

	return 1;
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
	lastX = xpos;
	lastY = ypos;

	const float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw   += xoffset;
	pitch += yoffset;

	if(pitch > 89.0f)
	  pitch =  89.0f;
	if(pitch < -89.0f)
	  pitch = -89.0f;

	//angle
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

	cameraFront = glm::normalize(direction);

}

void input(GLFWwindow* window)
{
	if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window,GLFW_KEY_X) == GLFW_PRESS)
		cameraPos -= cameraUp * cameraSpeed;
	if (glfwGetKey(window,GLFW_KEY_Z) == GLFW_PRESS)
		cameraPos += cameraUp * cameraSpeed;

	if (glfwGetKey(window,GLFW_KEY_UP) == GLFW_PRESS)
	    fov -= 2.0f;

	if (glfwGetKey(window,GLFW_KEY_DOWN) == GLFW_PRESS)
	    fov += 2.0f;


	if (fov < 1.0f)
		fov = 1.0f;
	if (fov > 45.0f)
		fov = 45.0f;

}

void frame_buffer_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0,0,width,height);
}
