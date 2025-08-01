#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <streambuf>
#include <sstream>
#include <math.h>
#include <vector>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext.hpp"
#include "glm/gtx/string_cast.hpp"

#include "Shader.hpp"
#include "Entity.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "ObjLoader.hpp"
#include "Texture.hpp"

#define DEBUG

volatile int screen_width = 800, screen_height = 800;

unsigned int depthMapFBO = 0;
unsigned int depthMap = 0;
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

unsigned int worldFBO = 0;
unsigned int world = 0;
unsigned int worldRBO = 0;


static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // glViewport(0, 0, width, height);
	screen_width = width;
	screen_height = height;
	if(!world && !worldRBO) {
		
	}
}

void APIENTRY glDebugOutput(GLenum source, 
                            GLenum type, 
                            unsigned int id, 
                            GLenum severity, 
                            GLsizei length, 
                            const char *message, 
                            const void *userParam)
{
    // ignore non-significant error/warning codes
    if(id == 131169 || id == 131185 || id == 131218 || id == 131204) return; 

    std::cout << "---------------" << std::endl;
    std::cout << "Debug message (" << id << "): " <<  message << std::endl;

    switch (source)
    {
        case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
        case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
    } std::cout << std::endl;

    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break; 
        case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
        case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
        case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
        case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
        case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
    } std::cout << std::endl;
    
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
        case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
        case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
        case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
    } std::cout << std::endl;
    std::cout << std::endl;
}


int main() {
 // Initialize GLFW
	glfwInit();

	// Tell GLFW what version of OpenGL we are using 
	// In this case we are using OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Tell GLFW we are using the CORE profile
	// So that means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	#ifdef __APPLE__
	glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
	#endif

	#ifdef DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
	#endif

	// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
	GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "RG2", NULL, NULL);
	// Error check if the window fails to create
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Introduce the window into the current context
	glfwMakeContextCurrent(window);

	

	// VSync
	glfwSwapInterval(1);

	//Load GLAD so it configures OpenGL
	gladLoadGL((GLADloadfunc)glfwGetProcAddress);


	
	std::cout << "[INFO] OpenGL version "
              << glGetString(GL_VERSION)
              << std::endl;

	#ifdef DEBUG
	int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		// initialize debug output 
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
	#endif

	int temp_screen_width, temp_screen_height;
    glfwGetFramebufferSize(window, &temp_screen_width, &temp_screen_height);
	screen_width = temp_screen_width;
	screen_height = temp_screen_height;
    glViewport(0, 0, screen_width, screen_height);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glEnable(GL_DEPTH_TEST);


	// gladLoadGL();
	// Specify the viewport of OpenGL in the Window
	// In this case the viewport goes from x = 0, y = 0, to x = 800, y = 800
	// glViewport(0, 0, screen_width, screen_height);

	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	Shader test("../shaders/default.vert", "../shaders/default.frag");
	Shader shadow_map_shader("../shaders/shadow_map.vert", "../shaders/shadow_map.frag");
	Shader ui_shader("../shaders/ui_default.vert", "../shaders/ui_default.frag");
	Shader depth_shader("../shaders/depth.vert", "../shaders/depth.frag");

	Texture tex = Texture("../assets/cube_tex.png", "ourTexture", 0);
	Texture terrain_tex = Texture("../assets/terrain.png", "ourTexture", 0);
	
	
	{
		// frame buffer stuff
		glGenFramebuffers(1, &depthMapFBO); 
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);

		// Texture for depth map
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
					SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); 

		// Attach the texture as framebuffers depth map
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);  
	}

	{
		// frame buffer stuff
		glGenFramebuffers(1, &worldFBO); 
		glBindFramebuffer(GL_FRAMEBUFFER, worldFBO);

		// Texture for depth map
		glGenTextures(1, &world);
		glBindTexture(GL_TEXTURE_2D, world);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
					screen_width, screen_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Attach the texture as framebuffers depth map
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, world, 0);
		// glDrawBuffer(GL_NONE);
		// glReadBuffer(GL_NONE);

		glGenRenderbuffers(1, &worldRBO);
		glBindRenderbuffer(GL_RENDERBUFFER, worldRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screen_width, screen_height); // use a single renderbuffer object for both a depth AND stencil buffer.
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, worldRBO); // now actually attach it
		// now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}



	Mesh obj = ObjLoader::load("../assets/cube.obj");
	Mesh terrain_mesh = ObjLoader::load("../assets/terrain.obj");
	Texture shadow_map_texture = Texture(depthMap, "shadowMap", 2);
	Texture frame_buffer_texture = Texture(world, "ourTexture", 0);

	Mesh ui_quad = Mesh::Quad(1.0f, 1.0f, {1.0f, 1.0f, 1.0f});	
	Mesh ui_w_quad = Mesh::Quad(1.0f, 1.0f, {1.0f, 1.0f, 1.0f});	

	obj.textures.push_back(tex);
	obj.textures.push_back(shadow_map_texture);
	terrain_mesh.textures.push_back(terrain_tex);
	terrain_mesh.textures.push_back(shadow_map_texture);
	ui_quad.textures.push_back(shadow_map_texture);
	ui_w_quad.textures.push_back(frame_buffer_texture);

	Entity entity1("1", obj);
	Entity entity2("2", obj);
	Entity entity3("3", obj);
	Entity entity_terrain("terrain", terrain_mesh);

	Entity ui_shadow_map_quad("shadow map", ui_quad);
	Entity ui_world_quad("world quad", ui_w_quad);

	

	entity1.setPosition({0.0f, 3.0f, 0.0f});
	entity2.setPosition({5.0f, 0.5f, 3.0f});
	entity3.setPosition({-3.0f, 0.5f, 4.0f});
	entity_terrain.setPosition({0.0f, -3.0f, 0.0f});

	ui_shadow_map_quad.setPosition({0.0f, 0.0f, 0.0f});
	ui_shadow_map_quad.setScale({300.0f, 300.0f, 1.0f});
	ui_world_quad.setPosition({0.0f, 0.0f, 0.0f});
	ui_world_quad.setScale({screen_width, screen_height, 0.0f});


	Camera camera = Camera();

	Camera ui_camera = Camera();
	ui_camera.projection = glm::ortho(0.0f, (float)screen_width, 0.0f, (float)screen_height, 0.1f, 10.0f);
	ui_camera.position = {0.0f, 0.0f, 1.0f};

	Camera sun_camera = Camera();
	sun_camera.projection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 30.0f);

	std::vector<Entity> entities = {
		entity1,
		entity2,
		entity3,
		entity_terrain
	};

	std::vector<Entity*> ui_entities = {
		&ui_world_quad,
		&ui_shadow_map_quad
	};

	bool show_demo_window = true;
	bool show_another_window = true;
	float cam_rotation_y = 4.0f * 3.14f / 6.0f;
	float cam_rotation_x = -3.14f / 4.0f;
	float cam_distance = 15.0f;

	float sun_cam_rotation_y = 3.14f / 4.0f;
	float sun_cam_rotation_x = -3.14f / 4.0f;
	float sun_cam_distance = 10.0f;

	
	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

		// ImGui::ShowDemoWindow(&show_demo_window);

        if (show_another_window) {
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::InputFloat3("camera position", (float*)&camera.position);

			ImGui::Text("WORLD entities");
			for(Entity& entity: entities)
				ImGui::InputFloat3(("entity position " + entity.name).c_str(), (float*)&entity.position);

			ImGui::Text("UI entities");
			for(Entity* entity: ui_entities) {
				ImGui::InputFloat3(("ui_entity position " + entity->name).c_str(), (float*)&entity->position);
				ImGui::InputFloat3(("ui_entity scale " + entity->name).c_str(), (float*)&entity->scale);
			}

			ImGui::Text("Cameras");
			ImGui::DragFloat("Camera rotation y", &cam_rotation_y, 0.01f, 0.0f, 2*3.14f);
			ImGui::DragFloat("Camera rotation x", &cam_rotation_x, 0.01f, -3.14f / 2.0f, 3.14f / 2.0f);
			ImGui::DragFloat("Camera distance", &cam_distance, 0.01f, 0.0f, 20.0f);
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

        ImGui::Render();

		// INPUT STUFF
		float speed = 0.01f;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			cam_rotation_x = glm::clamp(cam_rotation_x - speed, -3.14f / 2.0f, 3.14f / 2.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			cam_rotation_x = glm::clamp(cam_rotation_x + speed, -3.14f / 2.0f, 3.14f / 2.0f);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			cam_rotation_y = glm::clamp(cam_rotation_y - speed, 0.0f, 2*3.14f);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			cam_rotation_y = glm::clamp(cam_rotation_y + speed, 0.0f, 2*3.14f);
		}

		// Clear the depth buffer
		// Specify the color of the background
		// glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// Clean the back buffer and assign the new color to it
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

		camera.position = glm::vec3(glm::toMat4(glm::quat({cam_rotation_x, cam_rotation_y, 0.0f})) * glm::vec4(0.0f, 0.0f, cam_distance, 1.0f));
		camera.rotation = glm::lookAt(camera.position, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});

		sun_camera.position = glm::vec3(glm::toMat4(glm::quat({sun_cam_rotation_x, sun_cam_rotation_y, 0.0f})) * glm::vec4(0.0f, 0.0f, sun_cam_distance, 1.0f));
		sun_camera.rotation = glm::lookAt(sun_camera.position, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});

		// Set shadow map framebuffer
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		// Render shadows
		for(const Entity& entity: entities)
			entity.draw(shadow_map_shader, sun_camera);
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Render the scene
		glViewport(0, 0, screen_width, screen_height);
		glBindFramebuffer(GL_FRAMEBUFFER, worldFBO);

		// Change texture size BEGIN
		glBindTexture(GL_TEXTURE_2D, world);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
						screen_width, screen_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindRenderbuffer(GL_RENDERBUFFER, worldRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screen_width, screen_height); // use a single renderbuffer object for both a depth AND stencil buffer.
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		// Change texture size END

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for(const Entity& entity: entities)
			entity.draw(test, camera, sun_camera);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		ui_camera.projection = glm::ortho(0.0f, (float)screen_width, 0.0f, (float)screen_height, 0.1f, 10.0f);
		ui_world_quad.setScale({screen_width, screen_height, 1.0f});
		glViewport(0, 0, screen_width, screen_height);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ui_shadow_map_quad.draw(depth_shader, ui_camera);
		ui_world_quad.draw(ui_shader, ui_camera);

		// Swap the back buffer with the front buffer
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}


	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}
