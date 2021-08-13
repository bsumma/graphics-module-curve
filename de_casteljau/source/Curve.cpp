//
//  Curve.cpp
//  de_casteljau
//
//  Created by Brian Summa on 8/18/20.
//

#include "Curve.h"

void Curve::updateCurve(){
  curve_point_count = 0;
  std::vector < vec2 > tempHolder;
 
  //TODO
  //Populate the tempHolder vector with all curve points
  //Do this by calling the DeCasteljau function below
  
  if(curve_vao == 0){ glGenVertexArrays( 1, &curve_vao ); }
  glBindVertexArray(curve_vao);
  if(curve_buffer == 0){ glGenBuffers( 1, &curve_buffer ); }
  glBindBuffer( GL_ARRAY_BUFFER, curve_buffer );

  if (curve_point_count > 0 && tempHolder.size() > 0) {
    glBufferData( GL_ARRAY_BUFFER, sizeof(vec2)*curve_point_count, &tempHolder[0], GL_STATIC_DRAW );
  }
  
  glEnableVertexAttribArray(glGetAttribLocation(program, "vPosition"));
  glVertexAttribPointer( glGetAttribLocation(program, "vPosition"), 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

  glBindVertexArray(0);
}

std::vector < vec2 > Curve::DeCasteljau(vec2 p0, vec2 p1, vec2 p2, vec2 p3){
  std::vector < vec2 > curve;
  unsigned int steps = 99;  //how many discrete curve parameter increments to do

  //TODO:
  //Populate the curve vector with the curve defined by the 4 input control
  //points

  return curve;
}


void Curve::updateControlPoint(unsigned int idx, vec2 new_position){
  vec2 delta = new_position - control_points[idx];
  control_points[idx] = new_position;
  if(idx%3 == 0){
    if(idx > 0){
      control_points[idx-1] += delta;
    }
    if(idx < control_points.size()-1){
      control_points[idx+1] += delta;
    }
  }else if(idx > 1 && idx < control_points.size()-2){
    if(idx > 1 && (idx-2)%3 == 0){
      vec2 offset = control_points[idx+1] - control_points[idx];
      control_points[idx+2] = control_points[idx+1] + offset;
    }
    
    if(idx > 3 && (idx-4)%3 == 0){
      vec2 offset = control_points[idx-1] - control_points[idx];
      control_points[idx-2] = control_points[idx-1] + offset;
    }

  }
  updateHandles();
  updateCurve();
}

void Curve::initCircle(){
  std::vector < vec2 > circle;
  double steps = 99;
  
  circle.push_back(vec2(0,0));
  for(double theta = 0; theta <= 2*M_PI; theta+=(2*M_PI/steps)){
    circle.push_back(vec2(cos(theta), sin(theta)));
  }
  
  circle_point_count = circle.size();
  
  glGenVertexArrays( 1, &circle_vao );
  glBindVertexArray( circle_vao );
  
  glGenBuffers( 1, &circle_buffer );
  glBindBuffer( GL_ARRAY_BUFFER, circle_buffer );

  glBufferData( GL_ARRAY_BUFFER, sizeof(vec2)*circle_point_count, &circle[0], GL_STATIC_DRAW );
  
  GLuint vpos_location   = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(vpos_location);
  glVertexAttribPointer( vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  
  glBindVertexArray(0);

  
}


void Curve::updateHandles(){
  std::vector < vec2 > handles;
  
  for(unsigned int i=0; i < control_points.size()-1; i++){
    if((i+2)%3!=0){
      handles.push_back(control_points[i]);
      handles.push_back(control_points[i+1]);
    }
  }
  handle_point_count = handles.size();

  if(handle_vao == 0){ glGenVertexArrays( 1, &handle_vao ); }
  glBindVertexArray(handle_vao);
  if(handle_buffer == 0){
    glGenBuffers( 1, &handle_buffer );
  }
  glBindBuffer( GL_ARRAY_BUFFER, handle_buffer );

  glBufferData( GL_ARRAY_BUFFER, sizeof(vec2)*handle_point_count, &handles[0], GL_STATIC_DRAW );

  glEnableVertexAttribArray(glGetAttribLocation(program, "vPosition"));
  glVertexAttribPointer( glGetAttribLocation(program, "vPosition"), 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

  glBindVertexArray(0);
}

void Curve::drawControlPoints(mat4 P){
  for(unsigned int i=0; i < control_points.size(); i++){
    if(i%3 == 0){
      drawCircle(P, 0.025, control_points[i], vec4(1.0,0.0,0.0,1.0));
    }else{
      drawCircle(P, 0.015, control_points[i], vec4(0.0,0.0,0.0,1.0));
    }
  }
}

void Curve::drawCircle(mat4 P, double radius, vec2 position, vec4 color){
  
  glBindVertexArray( circle_vao );
  glUniform4fv( glGetUniformLocation(program, "Color"), 1, color);
  glUniformMatrix4fv( glGetUniformLocation( program, "ModelView" ),
                     1, GL_TRUE, P*Translate(position.x,  position.y, 0)*Scale(radius, radius, 1.0));
  glDrawArrays(GL_TRIANGLE_FAN, 0, circle_point_count);
  glBindVertexArray(0);

}

void Curve::drawCurve(mat4 P, vec4 color){
  
  glBindVertexArray( curve_vao );
  glUniform4fv( glGetUniformLocation(program, "Color"), 1, color);
  glUniformMatrix4fv( glGetUniformLocation( program, "ModelView" ),
                     1, GL_TRUE, P);
  glDrawArrays(GL_LINE_STRIP, 0, curve_point_count);
  glBindVertexArray(0);

}

void Curve::drawHandles(mat4 P, vec4 color){
  
  glBindVertexArray( handle_vao );
  glUniform4fv( glGetUniformLocation(program, "Color"), 1, color);
  glUniformMatrix4fv( glGetUniformLocation( program, "ModelView" ),
                     1, GL_TRUE, P);
  glDrawArrays(GL_LINES, 0, handle_point_count);
  glBindVertexArray(0);

}

