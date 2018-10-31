#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include "application.hpp"
#include "model.hpp"
#include "structs.hpp"
#include "GeometryNode.h"
#include "SceneGraph.h"
#include "Node.h"

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
  void initializeSceneGraph();
  void initializeShaderPrograms();
  void initializeGeometry();
  // update uniform values
  void uploadUniforms();
  // upload projection matrix
  void uploadProjection();
  // upload view matrix
  void uploadView();
  void renderPlanet();
  void renderEachPlanet(glm::fvec3 distanceFromOrigin, glm::mat4 model_matrix, double size) const;
  glm::mat4 rotateAndTranslate(glm::mat4 model_matrix, Node node) const;

  // cpu representation of model
  model_object planet_object;
  
  // camera transform matrix
  glm::fmat4 m_view_transform;
  // camera projection matrix
  glm::fmat4 m_view_projection;

  glm::vec3 cameraPos = glm::vec3(0.0f, 15.0f, 3.0f);
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
};

#endif