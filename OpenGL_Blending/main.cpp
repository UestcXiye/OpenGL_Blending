/*
ʵ�飺���

��������ʱ���ܰ�����
���̰���Z �������
���̰���X �رջ��
*/

#include <iostream>
#include <string>
#include <map>

#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"// �����
#include "Shader.h"// ��ɫ����
#include "Texture.h"

using namespace std;

// ���ڴ�С�����Ļص�����(�����ڴ�С�ı�ʱ���ӿ�ҲҪ�ı�)
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// �����ƻص�
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
// ���ֿ��ƻص�
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
// ���̿��ƻص�
void processInput(GLFWwindow *window);

// ��Ļ����
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

Camera camera(glm::vec3(1.0f, 0.0f, 5.0f)); //�����λ��
float lastX = (float)SCR_WIDTH / 2.0, lastY = (float)SCR_HEIGHT / 2.0; // ��������ʼλ��Ϊ��Ļ����
bool firstMouse = true;

float deltaTime = 0.0f; // ��ǰ֡����һ֡��ʱ���
float lastFrame = 0.0f; // ��һ֡��ʱ��

int main()
{
	// ---------------------��ʼ��--------------------------
	// glfw��ʼ�������õ�GL�汾Ϊ3.3���İ汾
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// ����GL����
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Blending", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// ָ��������Ϊ��ǰ����
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // ���ع�꣬���ͣ���ڴ�����

	// ��ʼ��glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// ��Ȳ���
	glEnable(GL_DEPTH_TEST);

	// �������
	glEnable(GL_BLEND);
	//���û�ϵ�Դ��Ŀ������
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// ��ɫ��
	Shader shader("res/shader/blending.vs", "res/shader/blending.fs");

	// ��������
	float cubeVertices[] = {

		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f
	};
	// ��������
	float planeVertices[] = {
		5.0f, -0.5f,  5.0f,  20.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 20.0f,

		5.0f, -0.5f,  5.0f,  20.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 20.0f,
		5.0f, -0.5f, -5.0f,  20.0f, 20.0f
	};
	// ��������
	float transparentVertices[] = {
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  0.0f
	};

	// ���� VAO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	// �ذ� VAO
	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	// ͸��window VAO
	unsigned int transparentVAO, transparentVBO;
	glGenVertexArrays(1, &transparentVAO);
	glGenBuffers(1, &transparentVBO);
	glBindVertexArray(transparentVAO);
	glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glBindVertexArray(0);

	//����
	unsigned int cubeTexture = Texture::LoadTextureFromFile("res/texture/cube.jpg");
	unsigned int floorTexture = Texture::LoadTextureFromFile("res/texture/TexturesCom_WoodFine0032_2_seamless_S.jpg");
	unsigned int transparentTexture = Texture::LoadTextureFromFile("res/texture/window.png");

	// ͸��window��λ��
	vector<glm::vec3> windows
	{
		glm::vec3(-1.5f, 0.0f, -0.48f),
		glm::vec3(2.5f, 0.0f, 0.51f),
		glm::vec3(0.0f, 0.0f, 0.7f),
		glm::vec3(-0.3f, 0.0f, -2.3f),
		glm::vec3(0.5f, 0.0f, -0.6f)
	};

	shader.Use();
	shader.SetInt("texture1", 0);

	lastFrame = glfwGetTime();
	while (!glfwWindowShouldClose(window))
	{
		// ��ÿһ֡�м�����µ�deltaTime
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// �������
		processInput(window);

		//���������λ�������������λ������֮��ľ���
		//map���Զ����ݼ�ֵ(Key)������ֵ��������ֻҪ������������е�λ�ã��������ľ�����Ϊ�������Ǿͻ��Զ����ݾ���ֵ������
		std::map<float, glm::vec3> sorted;
		for (unsigned int i = 0; i < windows.size(); i++)
		{
			float distance = glm::length(camera.Position - windows[i]);
			sorted[distance] = windows[i];
		}

		// �����ɫ����Ȼ���
		glClearColor(0.0f, 57.0f / 255, 92.0f / 255, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 model(1.0f);
		shader.SetMat4("projection", projection);
		shader.SetMat4("view", view);
		// ��������
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture);
		model = glm::translate(model, glm::vec3(-1.5f, 0.0f, -1.0f));
		shader.SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(3.0f, 0.0f, 0.0f));
		shader.SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		// ���Ƶذ�
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture);
		model = glm::mat4(1.0f);
		shader.SetMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// ����͸��window
		glBindVertexArray(transparentVAO);
		glBindTexture(GL_TEXTURE_2D, transparentTexture);
		glBindVertexArray(transparentVAO);
		glBindTexture(GL_TEXTURE_2D, transparentTexture);
		//���򣨴�Զ��������map�л�ȡֵ��֮����ƶ�Ӧ��window
		for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, it->second);
			shader.SetMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &planeVBO);

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		// �������
		glEnable(GL_BLEND);
		//���û�ϵ�Դ��Ŀ������
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		// �رջ��
		glDisable(GL_BLEND);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}