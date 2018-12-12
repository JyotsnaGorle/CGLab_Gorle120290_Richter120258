#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP
#include <math.h>
#include <string.h>
#include "application.hpp"
#include "model.hpp"
#include "structs.hpp"
#include "GeometryNode.h"
#include "SceneGraph.h"
#include "Node.h"
#include "PointLightNode.h"

// gpu representation of model
class ApplicationSolar : public Application {
 public:
	 SceneGraph *scene_graph;

  // allocate and initialize objects
  ApplicationSolar(std::string const& resource_path);
  // free allocated objects
  ~ApplicationSolar();

  // react to key input
  void keyCallback(int key, int action, int mods);
  //handle delta mouse movement input
  void mouseCallback(double pos_x, double pos_y);
  //handle resizing
  void resizeCallback(unsigned width, unsigned height);

  // draw all objects
  void render() const;

 protected:
  void initializeData();
  void initializeTextures();
  void initializeShaderPrograms();
  void initializeGeometry();
  // update uniform values
  void uploadUniforms();
  // upload projection matrix
  void uploadProjection();
  // upload view matrix
  void uploadView();
  void renderPlanet();
  void applyTexture(int i) const;
  void renderEachPlanet(glm::fvec3 distanceFromOrigin, glm::mat4 model_matrix, double size, glm::vec3 color, int textureIndex) const;
  glm::mat4 rotateAndTranslate(glm::mat4 model_matrix, Node node) const;
  void bindGeometry(model model, model_object object);
  void loadTextureForEachObject(string fileName, int textureObjectindex);
  void loadTextureForSkybox();
  void renderSkyBox() const;
  void initializeCustomFrameBuffer();

  // point light node 
  PointLightNode *pointLight;

  // cpu representation of model
  model_object planet_object;
  model_object star_object;
  model_object orbit_object;

  std::vector<float> star_buffer;
  std::vector<float> orbit_buffer;
  
  // camera transform matrix
  glm::fmat4 m_view_transform;
  // camera projection matrix
  glm::fmat4 m_view_projection;

  glm::vec3 cameraPos = glm::fvec3{ 0.0f, 0.0f, 16.0f };
  glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 cameraback = glm::vec3(0.0f, 0.0f, 1.0f);
  glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 cameradown = glm::vec3(0.0f, -1.0f, 0.0f);
  glm::vec3 cameraLeft = glm::vec3(-1.0f, 0.0f, 0.0f);
  glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
  GLfloat cameraSpeed = 0.000001f;
  bool isMouseUp = true;
  float mouseX = 0;
  float mouseY = 0;
  float modeSwitch = 1.0;
  texture_object texture_object_planets[11];
  texture_object texture_object_skybox;

  vector<std::string> skybox_texture_faces =
  {
	"right.jpg",
	"left.jpg",
	"top.jpg",
	"bottom.jpg",
	"front.jpg",
	"back.jpg"
  };

  GLuint fbo_handle = 0;
  GLuint rbo_handle = 0;
};

#endif