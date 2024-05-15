#version 330

in vec3 a_Position;
in vec3 a_Velocity;
in float a_StartTime;
in float a_LifeTime;
in float a_Amplitude;
in float a_Period;
in float a_Value;
in vec4 a_Color;

out vec4 v_Color;

uniform float u_Time = 0.f;
uniform float u_Period = 2.f;
uniform vec2 u_Acc = vec2(0.f, 0.f); 
uniform vec2 u_AttractPos = vec2(0.f, 0.f); 

const vec3 c_StartPos = vec3(-1.0f, 0, 0);
const vec3 c_Velocity = vec3(2.0f, 0, 0);
const vec3 c_ParaVelocity = vec3(2.0f, 2.0f, 0);
const vec2 c_2DGravity = vec2(0, -1.5f);
const float c_PI = 3.14159265359f;
const vec4 c_BgColor = vec4(0.0f, 0.3f, 0.3f, 1.0f);

void Basic()
{
	vec4 newPosition = vec4(a_Position, 1.f);
	gl_Position = newPosition;
	v_Color = a_Color;
}

void Velocity()
{
	vec4 newPosition = vec4(a_Position, 1.f);
	float t = u_Time - a_StartTime;

	if(t > 0)
	{
		t = a_LifeTime * fract(t / a_LifeTime);
		float attractValue = fract(t / a_LifeTime);
		float tt = t * t;
		
		newPosition.xy += a_Velocity.xy * t + 0.5f * (c_2DGravity + u_Acc) * tt;
		newPosition.xy = mix(newPosition.xy, u_AttractPos, attractValue);	// mix(a, b, t): a와 b를 t비율로 섞어줌
		gl_Position = newPosition;
	}
	else
	{
		gl_Position = vec4(-10, -10, -10, 1);
	}

	v_Color = a_Color;
}

void Line()
{
	float newTime = abs(fract(u_Time / u_Period) - 0.5f) * 2.f;
	vec4 newPosition;
	newPosition.xyz = (c_StartPos + a_Position) + c_Velocity * newTime;
	newPosition.w = 1.f;
	gl_Position = newPosition;
	v_Color = a_Color;
}

void Circle()
{
	float newTime = fract(u_Time / u_Period) * 2.f * c_PI;
	vec2 trans = vec2(cos(newTime), sin(newTime));
	vec4 newPosition;
	newPosition.xy = a_Position.xy + trans;
	newPosition.zw = vec2(0, 1);
	gl_Position = newPosition;
	v_Color = a_Color;
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
	v_Color = a_Color;
}

void CircleShape()
{
	vec4 newPosition = vec4(a_Position, 1.f);
	float t = u_Time - a_StartTime;
	float amplitude = a_Amplitude;
	float period = a_Period;

	if(t > 0)
	{
		t = a_LifeTime * fract(t / a_LifeTime);
		float tt = t * t;

		float value = a_Value * 2.f * c_PI;
		float x = cos(value);
		float y = sin(value);

		newPosition.xy += vec2(x, y);

		vec2 newVel = vec2(a_Velocity.x, a_Velocity.y + c_2DGravity.y * t);
		vec2 newDir = vec2(-newVel.y, newVel.x);	// 진행 방향에 수직인 벡터
		newDir = normalize(newDir);

		newPosition.y += a_Velocity.y * t + 0.5f * c_2DGravity.y * tt;
		newPosition.x += newDir.x * (0.1f * t * amplitude) * cos(t * c_PI * period);
		newPosition.y += newDir.y * (0.1f * t * amplitude) * sin(t * c_PI * period);

		gl_Position = newPosition;
	}
	else
	{
		gl_Position = vec4(-10, -10, -10, 1);
	}

	v_Color = a_Color;
}

void CircleShapeCycle()
{
	vec4 newPosition = vec4(a_Position, 1.f);
	float t = u_Time - a_StartTime;
	float amplitude = a_Amplitude;
	float period = a_Period;

	if(t > 0)
	{
		t = a_LifeTime * fract(t / a_LifeTime);
		float tt = t * t;

		float value = a_StartTime * 2.f * c_PI;
		float x = cos(value);
		float y = sin(value);

		newPosition.xy += vec2(x, y);

		vec2 newVel = vec2(a_Velocity.x, a_Velocity.y + c_2DGravity.y * t);
		vec2 newDir = vec2(-newVel.y, newVel.x);	// 진행 방향에 수직인 벡터
		newDir = normalize(newDir);

		newPosition.y += a_Velocity.y * t + 0.5f * c_2DGravity.y * tt;
		newPosition.x += newDir.x * (0.1f * t * amplitude) * cos(t * c_PI * period);
		newPosition.y += newDir.y * (0.1f * t * amplitude) * sin(t * c_PI * period);

		gl_Position = newPosition;
	}
	else
	{
		gl_Position = vec4(-10, -10, -10, 1);
	}

	v_Color = a_Color;
}

void HeartShapeCycle()
{
	vec4 newPosition = vec4(a_Position, 1.f);
	float t = u_Time - a_StartTime;
	float amplitude = a_Amplitude;
	float period = a_Period;

	if(t > 0)
	{
		t = a_LifeTime * fract(t / a_LifeTime);
		float particleAlpha = 1.f - t / a_LifeTime;
		float tt = t * t;

		float value = a_StartTime * 2.f * c_PI;
		float x = 16 * pow(sin(value), 3);
		float y = 13 * cos(value) - 5 * cos(2 * value) - 2 * cos(3 * value) - cos(4 * value);
		x /= 20;
		y /= 20;

		newPosition.xy += vec2(x, y);

		vec2 newVel = vec2(a_Velocity.x, a_Velocity.y + c_2DGravity.y * t);
		vec2 newDir = vec2(-newVel.y, newVel.x);	// 진행 방향에 수직인 벡터
		newDir = normalize(newDir);

		newPosition.y += a_Velocity.y * t + 0.5f * c_2DGravity.y * tt;
		newPosition.x += newDir.x * (0.1f * t * amplitude) * cos(t * c_PI * period);
		newPosition.y += newDir.y * (0.1f * t * amplitude) * sin(t * c_PI * period);

		v_Color = vec4(a_Color.rgb, particleAlpha);
		gl_Position = newPosition;
	}
	else
	{
		gl_Position = vec4(-10, -10, -10, 1);
		v_Color = vec4(0, 0, 0, 0);
	}
}

void main()
{
	//Line();
	//Circle();
	//Parabola();
	//Triangle();	// 시험에 낼 것임 직접 만들어 볼 것
	//Basic();
	//Velocity();
	//CircleShape();
	//CircleShapeCycle();
	HeartShapeCycle();

	
}