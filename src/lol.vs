#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

// Output data ; will be interpolated for each fragment.
out vec2 UV;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V;

void main(){

	// Output position of the vertex, in clip space : MVP * position


   vec4 position =  MVP * vec4(vertexPosition_modelspace,1);
	 //position.z = position.x*position.y;

	gl_Position = position;

	// UV of the vertex. No special space for this one.
	UV = vertexUV;
}
