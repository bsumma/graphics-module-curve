//
//  Curve.hpp
//  de_casteljau
//
//  Created by Brian Summa on 8/18/20.
//

#ifndef Curve_h
#define Curve_h

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "common/Angel.h"

using namespace Angel;


class Curve{
public:
  Curve(unsigned int curve_steps, GLint program): program(program),
  curve_vao(0), curve_buffer(0), handle_vao(0), handle_buffer(0){
    control_points.resize(13);
    control_points[0] = vec2(-1.8, 0.0);
    control_points[1] = vec2(-1.5, -0.4);
    control_points[2] = vec2(-1.2, -0.5);
    control_points[3] = vec2(-0.9, 0.0);
    control_points[4] = vec2(-0.6, 0.5);
    control_points[5] = vec2(-0.3, 0.5);
    control_points[6] = vec2(0.0, 0.0);
    control_points[7] = vec2(0.3, -0.5);
    control_points[8] = vec2(0.6, -0.2);
    control_points[9] = vec2(0.9, 0.0);
    control_points[10] = vec2(1.2, 0.2);
    control_points[11] = vec2(1.5, 0.4);
    control_points[12] = vec2(1.8, 0.0);

    
    init();

  };
  
  void init(){
    initCircle();
    updateHandles();
    updateCurve();
  }
  
  void draw(mat4 P){
    drawCurve(P);
    drawHandles(P);
    drawControlPoints(P);
  }
  
  int test_click(vec2 p){
    for(unsigned int i=0; i < control_points.size(); i++){
      if(length(p-control_points[i]) < 0.025){
        return i;
      }
    }
    return -1;
  }
  
  void updateControlPoint(unsigned int idx, vec2 new_position);
  
private:
  std::vector < vec2 > control_points;
  
  GLuint program;
  GLuint circle_vao, circle_buffer;
  unsigned int circle_point_count;
  
  GLuint curve_vao, curve_buffer;
  unsigned int curve_point_count;
  
  GLuint handle_vao, handle_buffer;
  unsigned int handle_point_count;

    
  void initCircle();
  
  void updateCurve();
  void updateHandles();
  
  void drawControlPoints(mat4 P);
  
  void drawCircle(mat4 P, double radius=0.025, vec2 position=vec2(0.0,0.0), vec4 color=vec4(1.0,0.0,0.0,1.0));
  void drawCurve(mat4 P, vec4 color=vec4(1.0,0.0,0.0,1.0));
  void drawHandles(mat4 P, vec4 color=vec4(0.5,0.5,0.5,1.0));
  
  std::vector < vec2 > DeCasteljau(vec2 p0, vec2 p1, vec2 p2, vec2 p3);
  
};

#endif /* Curve_h */
