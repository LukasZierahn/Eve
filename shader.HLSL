cbuffer ConstantBuffer : register(b0)
{
	matrix worldPos;
	matrix worldRot;
	matrix view;
	matrix projection;
}

cbuffer ModelBuffer : register(b1)
{
	matrix scale;
	matrix position;
	matrix rotation;
	float4 dnaInducedColour;
}

struct VertexInputType
{
	float4 pos : SV_position;
	float2 tex : TEXPOS0;
};

struct VS_OUTPUT
{
	float4 pos : SV_position;
	float2 tex : TEXPOS0;
};


VS_OUTPUT VS(float4 pos : position, float2 tex : TEXPOS)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.pos = mul(pos, scale);

	output.pos = mul(output.pos, rotation);
	output.pos = mul(position, output.pos);

	output.pos = mul(worldPos, output.pos);
	output.pos = mul(output.pos, worldRot);

	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);

	output.tex = tex;
	return output;
}

Texture2D shaderTexture;
SamplerState sampleType;

float4 PS(float4 position : SV_position, float2 tex : TEXPOS) : SV_TARGET
{
	float4 textureColour;

	textureColour = shaderTexture.Sample(sampleType, tex);
	textureColour += dnaInducedColour;

	if (textureColour.x > 1)
	{
		textureColour.x -= 1;
	}
	if (textureColour.y > 1)
	{
		textureColour.y -= 1;
	}
	if (textureColour.z > 1)
	{
		textureColour.z -= 1;
	}

	return textureColour;
}