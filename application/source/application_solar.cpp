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

#define M_PI  3.14159265358979323846   // pi
#define NUMBER_OF_STARS 1000
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

float random(float min, float max) {
	return (max - min) * (float(rand() % 100) / 100) + min;
}

ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
 ,planet_object{}
 ,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 16.0f})}
 ,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
{
  initializeData();
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
	
	//render the sun
	glm::fmat4 model_matrix = rotateAndTranslate({}, root);
	renderEachPlanet(root.getDist(), model_matrix, root.getSize(), root.color);

	// loop through children of sun
 	list<Node> children = root.getChildrenList();

	for (auto each : children) {
		// set the model matrix for the planets
		model_matrix = rotateAndTranslate(glm::mat4(1.0), each);
		//check if planet has any sattelites
		if (each.getChildrenList().size() > 0) {
			// planet has satelite and has to rotate around planet
			for (auto eachChild : each.getChildrenList()) {
				// set new model matrix to rotate around the parents local transform model matrix and render
				glm::fmat4 model_matrix2 = rotateAndTranslate(model_matrix, eachChild);
				renderEachPlanet(eachChild.getDist(), model_matrix2, eachChild.getSize(), each.color);
			}
		}
		renderEachPlanet(each.getDist(), model_matrix, each.getSize(), each.color);
	}


	// render stars
	glUseProgram(m_shaders.at("star").handle);
	// bind the VAO to draw
	glBindVertexArray(star_object.vertex_AO);
	// draw bound vertex array using bound shader
	glDrawArrays(GL_POINTS, 0, NUMBER_OF_STARS);

	for (auto each : children) {
		// render orbits
		glUseProgram(m_shaders.at("orbit").handle);

		// bind the VAO to draw
		glBindVertexArray(orbit_object.vertex_AO);
		glUniformMatrix4fv(m_shaders.at("orbit").u_locs.at("ModelViewMatrix"),
			1, GL_FALSE, glm::value_ptr(glm::fmat4{}));
		glDrawArrays(GL_LINE_LOOP, 1 * 100, 100);
	}

}

glm::mat4 ApplicationSolar::rotateAndTranslate(glm::mat4 model_matrix, Node node) const{
	model_matrix = glm::rotate(model_matrix, float(glfwGetTime() * node.rotationSpeed), glm::fvec3{ 0.0f, 0.1f, 0.0f });
	model_matrix = glm::translate(model_matrix, node.getDist());
	model_matrix = glm::scale(model_matrix, glm::fvec3{ node.getSize(), node.getSize(), node.getSize() });
	return model_matrix;
}

void ApplicationSolar::renderEachPlanet(glm::fvec3 distanceFromOrigin, glm::fmat4 model_matrix, double size, glm::vec3 color) const{

	// bind shader to upload uniforms
	glUseProgram(m_shaders.at("planet").handle);

	glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
		1, GL_FALSE, glm::value_ptr(model_matrix));

	// extra matrix for normal transformation to keep them orthogonal to surface
	glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
	glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
		1, GL_FALSE, glm::value_ptr(normal_matrix));
	
	// get uniforms in shader program to set the Color
	GLint locationColor = glGetUniformLocation(m_shaders.at("planet").handle, "Color");
	glUniform3fv(locationColor, 1, glm::value_ptr(color));

	// get uniforms in shader program to set the vertex of light source
	GLint locationLightSource = glGetUniformLocation(m_shaders.at("planet").handle, "lightSource");
	glUniform3f(locationLightSource, 0.0, 0.0, 0.0);

	// get uniforms in shader program to set the DiffuseColor
	GLint locationDiffuseColor = glGetUniformLocation(m_shaders.at("planet").handle, "diffuseColor");
	glUniform3f(locationDiffuseColor, 0.3, 0.4, 0.0);

	// get uniforms in shader program to set the SpeculativeColor 
	GLint locationSpeculativeColor = glGetUniformLocation(m_shaders.at("planet").handle, "speculativeColor");
	glUniform3f(locationSpeculativeColor, 1.0, 1.0, 1.0);

	// get uniforms in shader program to set the Alpha value in the equation
	GLint locationshininess = glGetUniformLocation(m_shaders.at("planet").handle, "shininess");
	glUniform1f(locationshininess, 4.0f);

	// get uniforms in shader program to set the Rho value
	GLint locationrho = glGetUniformLocation(m_shaders.at("planet").handle, "rho");
	glUniform1f(locationrho, 0.5f);

	// get uniforms in shader program to set the camera Position
	GLint locationCameraPos = glGetUniformLocation(m_shaders.at("planet").handle, "cameraPos");
	glUniform3f(locationCameraPos, 0.0, 0.0, 16.0);

	// get uniforms in shader program to set the light intensity from the pointLight Node
	GLint locationlightIntensity = glGetUniformLocation(m_shaders.at("planet").handle, "lightIntensity");
	glUniform1f(locationlightIntensity, this->pointLight->lightIntensity);

	// get uniforms in shader program to set the light color from the pointLight Node
	GLint locationlightColor = glGetUniformLocation(m_shaders.at("planet").handle, "lightColor");
	glUniform3fv(locationlightColor,1, glm::value_ptr(pointLight->lightColor));

	// get uniforms in shader program to switch between modes
	GLint locationModeSwitch = glGetUniformLocation(m_shaders.at("planet").handle, "modeSwitch");
	glUniform1f(locationModeSwitch, modeSwitch);

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
  // upload matrix to gpu for planets
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
  // upload matrix to gpu for stars
  glUniformMatrix4fv(m_shaders.at("star").u_locs.at("ModelViewMatrix"),
	  1, GL_FALSE, glm::value_ptr(view_matrix));
  // upload matrix to gpu for orbits
  glUniformMatrix4fv(m_shaders.at("orbit").u_locs.at("ModelViewMatrix"),
	  1, GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::uploadProjection() {
  // upload matrix to gpu for stars
  
  glUniformMatrix4fv(m_shaders.at("star").u_locs.at("ProjectionMatrix"),
	  1, GL_FALSE, glm::value_ptr(m_view_projection));
  // upload matrix to gpu for planets
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
	  1, GL_FALSE, glm::value_ptr(m_view_projection));
  // upload matrix to gpu for orbits
  glUniformMatrix4fv(m_shaders.at("orbit").u_locs.at("ProjectionMatrix"),
	  1, GL_FALSE, glm::value_ptr(m_view_projection));
}

// update uniform locations
void ApplicationSolar::uploadUniforms() { 
	// bind shader to which to upload unforms
	glUseProgram(m_shaders.at("planet").handle);
	// upload uniform values to new locations
	uploadView();
	uploadProjection();

	// bind shader to which to upload unforms for stars
	glUseProgram(m_shaders.at("star").handle);
	// upload uniform values to new locations for stars
	uploadView();
	uploadProjection();

	// bind shader to which to upload unforms for orbits
	glUseProgram(m_shaders.at("orbit").handle);
	// upload uniform values to new locations for orbits
	uploadView();
	uploadProjection();
}

///////////////////////////// intialisation functions /////////////////////////

//initialize the scene graph nodes
void ApplicationSolar::initializeData() {
	scene_graph = new SceneGraph;

	//VBO for star
	for (int i = 0; i < NUMBER_OF_STARS; i++) {
		//Position x,y,z
		star_buffer.push_back(random(-50, 50));
		star_buffer.push_back(random(-50, 50));
		star_buffer.push_back(random(-50, 50));
		//Color RGB
		star_buffer.push_back(random(0, 1));
		star_buffer.push_back(random(0, 1));
		star_buffer.push_back(random(0, 1));
	}
	
	//VBO for orbits
	// circumference 2PiR
	float increment = 2.0f *3.14 / 100;
	//orbits for planets
	Node root = scene_graph->getRoot();
	list<Node> children = root.getChildrenList();
	int i = 0;
	for (auto each : children) {
		float radius = i * each.getDist()[2];
		i++;
		for (float rad = 0.0f; rad < 2.0f * 3.14; rad += increment) {
		//vertices x,y,z
			orbit_buffer.push_back(radius * cos(rad)); 
			orbit_buffer.push_back(0.0f);
			orbit_buffer.push_back(radius * sin(rad));
		 //color RGB
			orbit_buffer.push_back(1.0);
			orbit_buffer.push_back(1.0);
			orbit_buffer.push_back(1.0);
		}
	}

	// define light color and light intensity for the window
	pointLight = new PointLightNode;
	// light color set to white
	pointLight->lightColor = glm::vec3(1.0, 1.0, 1.0);
	// light intensity set to a float value
	pointLight->lightIntensity = 1.0f;
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
  m_shaders.at("planet").u_locs["Color"] = -1;
  m_shaders.at("planet").u_locs["lightSource"] = -1;
  m_shaders.at("planet").u_locs["diffuseColor"] = -1;
  m_shaders.at("planet").u_locs["speculativeColor"] = -1;
  m_shaders.at("planet").u_locs["shininess"] = -1;
  m_shaders.at("planet").u_locs["cameraPos"] = -1;
  m_shaders.at("planet").u_locs["rho"] = -1;
  m_shaders.at("planet").u_locs["lightIntensity"] = -1;
  m_shaders.at("planet").u_locs["lightColor"] = -1;
  m_shaders.at("planet").u_locs["modeSwitch"] = -1;
   
  m_shaders.emplace("star", shader_program{ {{GL_VERTEX_SHADER,m_resource_path + "shaders/vao.vert"},
										   {GL_FRAGMENT_SHADER, m_resource_path + "shaders/vao.frag"}} });
  // request uniform locations for shader program
  m_shaders.at("star").u_locs["ProjectionMatrix"] = -1;
  m_shaders.at("star").u_locs["ModelViewMatrix"] = -1;

  m_shaders.emplace("orbit", shader_program{ {{GL_VERTEX_SHADER,m_resource_path + "shaders/vao.vert"},
										   {GL_FRAGMENT_SHADER, m_resource_path + "shaders/vao.frag"}} });
  // request uniform locations for shader program
  m_shaders.at("orbit").u_locs["ProjectionMatrix"] = -1;
  m_shaders.at("orbit").u_locs["ModelViewMatrix"] = -1;
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

  //====================vertex specifications for stars====================
  model star_model = { star_buffer,model::POSITION  };

  // generate vertex array object
  glGenVertexArrays(1, &star_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(star_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &star_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, star_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * star_model.data.size(), star_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, 0);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(sizeof(float) * 3));

  // generate generic buffer
  glGenBuffers(1, &star_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, star_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * star_model.indices.size(), star_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  star_object.draw_mode = GL_POINT;
  // transfer number of indices to model object 
  star_object.num_elements = GLsizei(star_model.indices.size());

  //====================vertex specifications for orbits====================
  model orbit_model = { orbit_buffer,model::POSITION | model::NORMAL };

  // generate vertex array object
  glGenVertexArrays(1, &orbit_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(orbit_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &orbit_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, orbit_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * orbit_model.data.size(), orbit_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, orbit_model.vertex_bytes, orbit_model.offsets[model::POSITION]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, orbit_model.vertex_bytes, orbit_model.offsets[model::NORMAL]);

  // generate generic buffer
  glGenBuffers(1, &orbit_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, orbit_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * orbit_model.indices.size(), orbit_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  orbit_object.draw_mode = GL_LINE_LOOP;
  // transfer number of indices to model object 
  orbit_object.num_elements = GLsizei(orbit_model.indices.size());
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

  if (key == GLFW_KEY_1 && (action == GLFW_PRESS)) {
	  modeSwitch = 1.0;
	  uploadView();
  }

  if (key == GLFW_KEY_2 && (action == GLFW_PRESS)) {
	  modeSwitch = 2.0;
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
