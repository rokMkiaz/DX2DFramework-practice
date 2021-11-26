struct VertexInput
{
	float4 position : POSITION0;
	float4 color : COLOR0;
};

struct PixelInput
{
	float4 position : SV_POSITION0;
	float4 color : COLOR0;
};

cbuffer TransformBuffer : register(b0)
{
	matrix world; //float 4x4
	matrix view;
	matrix proj;
};

PixelInput VS(VertexInput input)
{
	
	
	PixelInput output;
	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, proj);
	output.color = input.color;

	return
	output;
}

float4 PS(PixelInput input) : SV_Target
{
	return input.color;
}