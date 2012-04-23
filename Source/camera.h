/*
	Copyright (c) 2011, T. Kroes <t.kroes@tudelft.nl>
	All rights reserved.

	Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

	- Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
	- Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
	- Neither the name of the TU Delft nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
	
	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "vector.h"
#include "color.h"

namespace ExposureRender
{

struct Camera
{
	HOST Camera()
	{
	}

	HOST Camera(const Camera& Other)
	{
		*this = Other;
	}

	HOST ~Camera()
	{
	}
	
	HOST Camera& Camera::operator = (const Camera& Other)
	{
		this->FilmSize			= Other.FilmSize;
		this->Pos				= Other.Pos;
		this->Target			= Other.Target;
		this->Up				= Other.Up;
		this->N					= Other.N;
		this->U					= Other.U;
		this->V					= Other.V;
		this->FocalDistance		= Other.FocalDistance;
		this->ApertureSize		= Other.ApertureSize;
		this->ClipNear			= Other.ClipNear;
		this->ClipFar			= Other.ClipFar;
		this->Screen[0][0]		= Other.Screen[0][0];
		this->Screen[0][1]		= Other.Screen[0][1];
		this->Screen[1][0]		= Other.Screen[1][0];
		this->Screen[1][1]		= Other.Screen[1][1];
		this->InvScreen[0]		= Other.InvScreen[0];
		this->InvScreen[1]		= Other.InvScreen[1];
		this->Exposure			= Other.Exposure;
		this->InvExposure		= Other.InvExposure;
		this->Gamma				= Other.Gamma;
		this->InvGamma			= Other.InvGamma;
		this->FOV				= Other.FOV;
		
		this->Update();

		return *this;
	}

	HOST void Update()
	{
		this->InvExposure	= this->Exposure == 0.0f ? 0.0f : 1.0f / this->Exposure;
		this->InvGamma		= this->Gamma == 0.0f ? 0.0f : 1.0f / this->Gamma;
		
		this->N = Normalize(this->Target - this->Pos);
		this->U = Normalize(Cross(this->N, this->Up));
		this->V = Normalize(Cross(this->N, this->U));

		if (this->FocalDistance == -1.0f)
			this->FocalDistance = (this->Target - this->Pos).Length();

		float Scale = 0.0f;

		Scale = tanf((0.5f * this->FOV / RAD_F));

		const float AspectRatio = (float)this->FilmSize[1] / (float)this->FilmSize[0];

		if (AspectRatio > 1.0f)
		{
			this->Screen[0][0] = -Scale;
			this->Screen[0][1] = Scale;
			this->Screen[1][0] = -Scale * AspectRatio;
			this->Screen[1][1] = Scale * AspectRatio;
		}
		else
		{
			this->Screen[0][0] = -Scale / AspectRatio;
			this->Screen[0][1] = Scale / AspectRatio;
			this->Screen[1][0] = -Scale;
			this->Screen[1][1] = Scale;
		}

		this->InvScreen[0] = (this->Screen[0][1] - this->Screen[0][0]) / (float)this->FilmSize[0];
		this->InvScreen[1] = (this->Screen[1][1] - this->Screen[1][0]) / (float)this->FilmSize[1];
	}

	Vec2i	FilmSize;
	Vec3f	Pos;
	Vec3f	Target;
	Vec3f	Up;
	Vec3f	N;
	Vec3f	U;
	Vec3f	V;
	float	FocalDistance;
	float	ApertureSize;
	float	ClipNear;
	float	ClipFar;
	float	Screen[2][2];
	float	InvScreen[2];
	float	Exposure;
	float	InvExposure;
	float	Gamma;
	float	InvGamma;
	float	FOV;
};

}

/*
// sample N-gon
// FIXME: this could use concentric sampling
float lensSides = 6.0f;
float lensRotationRadians = 0.0f;
float lensY = CS.LensUV[0] * lensSides;
float side = (int)lensY;
float offs = (float) lensY - side;
float dist = (float) sqrtf(CS.LensUV[1]);
float a0 = (float) (side * PI_F * 2.0f / lensSides + lensRotationRadians);
float a1 = (float) ((side + 1.0f) * PI_F * 2.0f / lensSides + lensRotationRadians);
float eyeX = (float) ((cos(a0) * (1.0f - offs) + cos(a1) * offs) * dist);
float eyeY = (float) ((sin(a0) * (1.0f - offs) + sin(a1) * offs) * dist);
eyeX *= gpTracer->Camera.ApertureSize;
eyeY *= gpTracer->Camera.ApertureSize;

const Vec2f LensUV(eyeX, eyeY);// = gpTracer->Camera.ApertureSize * ConcentricSampleDisk(CS.LensUV);

const Vec3f LI = ToVec3f(gpTracer->Camera.U) * LensUV[0] + ToVec3f(gpTracer->Camera.V) * LensUV[1];

Rc.O += LI;
Rc.D = Normalize(Rc.D * gpTracer->Camera.FocalDistance - LI);
*/