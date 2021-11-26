struct VertexInput
{
	float4 position : POSITION0;
	float2 uv : TEXCOORD0;
};

struct PixelInput
{
	float4 position : SV_POSITION0;
	float2 uv : TEXCOORD0;
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
	output.uv = input.uv;

	return
	output;
}

Texture2D source_texture1 : register(t0);
Texture2D source_texture2 : register(t1);

SamplerState samp : register(s0);

float4 PS(PixelInput input) : SV_Target
{
	//float4 color = 0.0f;
	
	//if (input.uv.x < 1.0f)
	//	color = source_texture1.Sample(samp, input.uv);
	//else
	//	color = source_texture2.Sample(samp, float2(input.uv.x - 1.0f, input.uv.y));
	
	float4 color = source_texture1.Sample(samp, input.uv);
	
	//clip(color.a - 0.9f);
	
	//if (color.a < 0.1f)
	//	discard;
	
		return color;

}