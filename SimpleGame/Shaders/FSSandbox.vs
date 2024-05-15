#version 330

in vec3 a_Position;

out vec4 v_Color;

uniform float u_Time = 0.f;

void main()
{
	vec4 newPosition = vec4(a_Position, 1);
	gl_Position = newPosition;
	v_Color = vec4(newPosition.x + 0.5, newPosition.y + 0.5, 0, 1);
}
