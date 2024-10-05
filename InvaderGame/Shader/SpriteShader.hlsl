// ０番のテクスチャスロットを使用する
Texture2D Texture : register(t0);

// ０番のサンプラスロットを使用する
SamplerState Sampler : register(s0);

cbuffer ConstantBuffer : register(b0)
{
	matrix wp;
};

struct VSOutput
{
	float4 Pos : SV_Position;
	float2 UV : TEXCOORD0; // UV座標
};

VSOutput VS(float4 pos : POSITION, float2 uv : TEXUV)
{
	VSOutput Out;
	Out.Pos = pos;

	// 頂点のUV座標を何も加工せずそのまま出力
	Out.UV = uv;
	return Out;
}

float4 PS(VSOutput In) : SV_Target0
{
	float4 color = Texture.Sample(Sampler, In.UV);

	if (color.a == 0)
	{
		color = float4(1.0, 1.0, 0, 1.0);
	}

	return color;
}

float4 PS_Color(VSOutput In) : SV_Target0
{
    float4 color = float4(1.0, 1.0, 0, 1.0);
	
    return color;
}