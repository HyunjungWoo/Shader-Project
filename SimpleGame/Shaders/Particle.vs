#version 330

in vec3 a_Position;

uniform float u_Time = 0.f;
uniform float u_Period = 2.f;

const vec3 c_StartPos = vec3(-1.0f, 0, 0);
const vec3 c_Velocity = vec3(2.0f, 0, 0);
const vec3 c_ParaVelocity = vec3(2.0f, 2.0f, 0);
const vec2 c_2DGravity = vec2(0, -9.8f);
const float c_PI = 3.14159265359f;
const float c_Gravity = 9.8f;

void Basic()
{
	vec4 newPosition = vec4(a_Position, 1.f);
	gl_Position = newPosition;
}

void Line()
{
	float newTime = abs(fract(u_Time / u_Period) - 0.5f) * 2.f;
	vec4 newPosition;
	newPosition.xyz = (c_StartPos + a_Position) + c_Velocity * newTime;
	newPosition.w = 1.f;
	gl_Position = newPosition;
}

void Circle()
{
	float newTime = fract(u_Time / u_Period) * 2.f * c_PI;
	vec2 trans = vec2(cos(newTime), sin(newTime));
	vec4 newPosition;
	newPosition.xy = a_Position.xy + trans;
	newPosition.zw = vec2(0, 1);
	gl_Position = newPosition;
}

void Parabola()
{
	float newTime = fract(u_Time / u_Period);
	float t = newTime;
	float tt = t * t;
	vec4 newPosition;
	float transX = (a_Position.x + c_StartPos.x) 
					+ c_ParaVelocity.x * t 
					+ 0.5f * c_2DGravity.x * tt;
	float transY = (a_Position.y + c_StartPos.y) 
					+ c_ParaVelocity.y * t 
					+ 0.5f * c_2DGravity.y * tt;
	newPosition.xy = vec2(transX, transY);
	newPosition.zw = vec2(0, 1);
	gl_Position = newPosition;
}

void main()
{
	//Line();
	//Circle();
	//Parabola();
	//Triangle();	// 시험에 낼 것임 직접 만들어 볼 것
	Basic();
}
