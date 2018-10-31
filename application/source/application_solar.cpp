#include "application_solar.hpp"
#include "window_handler.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"

#include <glbinding/gl/gl.h>
// use gl definitions from glbinding 
using namespace gl;

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}
 ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 16.0f})}
 ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
{
  initializeSceneGraph();
  initializeGeometry();
  initializeShaderPrograms();
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
}

void ApplicationSolar::render() const {
	Node root = scene_graph->getRoot();
	
	//the sun rotate around the origin since it's dist is set to 0
	glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{ 0.0f, 0.1f, 0.0f });
	//render the sun
	renderEachPlanet(root.getDist(), model_matrix, root.getSize());

	// loop through children of sun
	list<Node> children = root.getChildrenList();

	for (auto each : children) {
		// set the model matrix for the planets
		model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{ 0.0f, 0.1f, 0.0f });
		
		//check if planet has any sattelites
		if (each.getChildrenList().size() > 0) {
			// planet has satelite and has to rotate around planet
			for (auto eachChild : each.getChildrenList()) {
				// set new model matrix to rotate around the parents model matrix
				glm::fmat4 model_matrix2 = glm::rotate(model_matrix, float(glfwGetTime()*0.1f), glm::fvec3{ 0.0f, 0.1f, 0.0f });
				// translate with respect to planet origin hence send model_matrix2
				renderEachPlanet(eachChild.getDist(), model_matrix2, eachChild.getSize());
			}
		}
		renderEachPlanet(each.getDist(), model_matrix, each.getSize());
	}
}


void ApplicationSolar::renderEachPlanet(glm::fvec3 distanceFromOrigin, glm::fmat4 model_matrix, double size) const{

	// bind shader to upload uniforms
	glUseProgram(m_shaders.at("planet").handle);

	model_matrix = glm::translate(model_matrix, distanceFromOrigin);
	model_matrix = glm::scale(model_matrix, glm::fvec3{ size, size, size });
	glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
		1, GL_FALSE, glm::value_ptr(model_matrix));

	// extra matrix for normal transformation to keep them orthogonal to surface
	glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
	glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
		1, GL_FALSE, glm::value_ptr(normal_matrix));

	// bind the VAO to draw
	glBindVertexArray(planet_object.vertex_AO);

	// draw bound vertex array using bound shader
	glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
}

void ApplicationSolar::uploadView() {
	//rotate the view window with given x and y positions
	m_view_transform = glm::rotate(m_view_transform, -mouseX, glm::fvec3{ 0, 1.0f, 0 });
	m_view_transform = glm::rotate(m_view_transform, -mouseY, glm::fvec3{ 1.0f, 0, 0 });
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::uploadProjection() {
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
}

// update uniform locations
void ApplicationSolar::uploadUniforms() { 
  // bind shader to which to upload unforms
  glUseProgram(m_shaders.at("planet").handle);
  // upload uniform values to new locations
  uploadView();
  uploadProjection();
}

///////////////////////////// intialisation functions /////////////////////////

//initialize the scene graph nodes
void ApplicationSolar::initializeSceneGraph() {
	scene_graph = new SceneGraph;
}

// load shader sources
void ApplicationSolar::initializeShaderPrograms() { 
  // store shader program objects in container
  m_shaders.emplace("planet", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/simple.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple.frag"}}});
  // request uniform locations for shader program
  m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
}

// load models
void ApplicationSolar::initializeGeometry() {
	GeometryNode *geometryNode = new GeometryNode;
	geometryNode->setGeometry(model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL));
	model planet_model = geometryNode->getGeometry();

  // generate vertex array object
  glGenVertexArrays(1, &planet_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(planet_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &planet_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, planet_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planet_model.data.size(), planet_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::POSITION]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::NORMAL]);

   // generate generic buffer
  glGenBuffers(1, &planet_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model.indices.size(), planet_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  planet_object.draw_mode = GL_TRIANGLES;
  // transfer number of indices to model object 
  planet_object.num_elements = GLsizei(planet_model.indices.size());
}

///////////////////////////// callback functions for window events ////////////
// handle key input
void ApplicationSolar::keyCallback(int key, int action, int mods) {
  if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
	  // moving camera  to zoom in
	  m_view_transform = glm::translate(m_view_transform, cameraFront);
	  uploadView();
  }
  if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
	  //moving camera to move up the screen
	  m_view_transform = glm::translate(m_view_transform, cameraUp);
	  uploadView();
  }

  if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
	  //moving the camera to focus on left of the screen
	  m_view_transform = glm::translate(m_view_transform, cameraLeft);
	  uploadView();
  }

  if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
	  //moving the camera to focus on right of the screen
	  m_view_transform = glm::translate(m_view_transform, cameraRight);
	  uploadView();
  }

  if (key == GLFW_KEY_Z && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
	  //moving camera to move up the screen
	  m_view_transform = glm::translate(m_view_transform, cameradown);
	  uploadView();
  }

  if (key == GLFW_KEY_F && (action == GLFW_PRESS || action == GLFW_REPEAT))
  {
	  //moving camera to zoom out the screen
	  m_view_transform = glm::translate(m_view_transform, cameraback);
	  uploadView();
  }
}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
  // mouse handling
	if (isMouseUp) { // if mouse is on window
		mouseX += pos_x * cameraSpeed;
		mouseY += pos_y * cameraSpeed;
		uploadView();
	}
	uploadView();
}

//handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height) {
  // recalculate projection matrix for new aspect ration
  m_view_projection = utils::calculate_projection_matrix(float(width) / float(height));
  // upload new projection matrix
  uploadProjection();
}

// exe entry point
int main(int argc, char* argv[]) {
  Application::run<ApplicationSolar>(argc, argv, 3, 2);
}


/*

void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean  constrainPitch = true)
{
xoffset *= this->MouseSensitivity;
yoffset *= this->MouseSensitivity;

this->Yaw   += xoffset;
this->Pitch += yoffset;

if (constrainPitch)
{
if (this->Pitch > 89.0f)
this->Pitch = 89.0f;
if (this->Pitch < -89.0f)
this->Pitch = -89.0f;
}

this->updateCameraVectors();
}

*/
