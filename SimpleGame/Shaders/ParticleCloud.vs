#version 330

// Attribute
in vec3 a_Position;
in vec3 a_Velocity;
in float a_StartTime;
in float a_LifeTime;
in float a_Amp;
in float a_Period;
in float a_Value;

uniform float u_Time = 0.f;
uniform float u_Period = 2.f;

const vec3 c_StartPos = vec3(-1.0f, 0, 0);
const vec3 c_Velocity = vec3(2.0f, 0, 0);
const vec3 c_ParaVelocity = vec3(2.0f, 2.0f, 0);
const vec2 c_2DGravity = vec2(0, -0.9f);
const float c_PI = 3.14159265359f;
const float c_Gravity = 9.8f;

void Basic()
{
	vec4 newPosition = vec4(a_Position, 1.f);
	gl_Position = newPosition;
}

void Velocity()
{
	vec4 newPosition = vec4(a_Position,1);
	float t = u_Time - a_StartTime;

	if( t> 0)
	{
		t =  a_LifeTime* fract(t / a_LifeTime);
	    newPosition.xy = newPosition.xy + a_Velocity.xy * t;

	}
	else
	{
	     newPosition.x = 10000000;
	}

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

void SinShape() // 로켓부스터처럼 사용
{
	vec4 newPosition = vec4(a_Position,1);
	float t = u_Time - a_StartTime;
	float amp = a_Amp; // 폭
	float period = a_Period; // 주기 

	if( t > 0)
	{
		t =  a_LifeTime* fract(t / a_LifeTime);
		float tt = t*t;
		float value = a_Value * 2.0 * c_PI;
		float x = cos(value);
		float y = sin(value);
		newPosition.xy = newPosition.xy + vec2(x,y);

		vec2 newVel = a_Velocity.xy + c_2DGravity * t;
		vec2 newDir = vec2(-newVel.y, newVel.x);
		newDir = normalize(newDir);
	    newPosition.xy = newPosition.xy + a_Velocity.xy * t + 0.5* c_2DGravity * tt;
		newPosition.xy = newPosition.xy + newDir*(t*0.1)*amp*sin(period*t*c_PI);

	}
	else
	{
	     newPosition.x = 10000000;
	}

	gl_Position = newPosition;
}


void main()
{
	//Line();
	//Circle();
	//Parabola();
	//Triangle();	// 시험에 낼 것임 직접 만들어 볼 것
	//Basic();
	//Velocity();
	SinShape();
}
