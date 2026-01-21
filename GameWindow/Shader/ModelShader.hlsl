// ０番のテクスチャスロットを使用する
Texture2D Texture : register(t0);

// ０番のサンプラスロットを使用する
SamplerState Sampler : register(s0);

cbuffer ConstantBuffer : register(b0)
{
    matrix viewProjMat;
};


struct VS_INPUT
{
	// スロット０
    float4 pos : POSITION;
    float2 uv : TEXUV;
	
	// スロット１
    float4x4 worldMat : INST_MATRIX; // wvp = (World * View * Projection)
    float4 color : INST_COLOR;
};

struct VS_OUTPUT
{
	float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};


VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = mul(mul(input.pos, input.worldMat), viewProjMat);
    output.uv.x = input.uv.x;
    output.uv.y = input.uv.y;
	
    output.color = input.color;
	
    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target0
{
    float4 color = Texture.Sample(Sampler, input.uv);

    return color * input.color;
}

float4 PS_Color(VS_OUTPUT input) : SV_Target0
{
    return input.color;
}