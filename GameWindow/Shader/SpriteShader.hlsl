// ０番のテクスチャスロットを使用する
Texture2D Texture : register(t0);

// ０番のサンプラスロットを使用する
SamplerState Sampler : register(s0);

cbuffer ConstantBuffer : register(b0)
{
	matrix wp;
};

cbuffer ColorBuffer : register(b1)
{
    float4 color;
}

struct VSOutput
{
	float4 Pos : SV_Position;
	float2 UV : TEXCOORD0; // UV座標
};

struct VS_INPUT
{
    float4 pos : POSITION;
    float2 uv : TEXUV;
	
    float4 color : INSTANCE_COLOR;
};

struct VS_OUTPUT
{
	float4 pos : SV_Position;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};


// 頂点シェーダー
VSOutput VS(float4 pos : POSITION, float2 uv : TEXUV)
{
	VSOutput Out;
	Out.Pos = pos;

	// 頂点のUV座標を何も加工せずそのまま出力
	Out.UV = uv;
	return Out;
}

// 左右反転して描画する頂点シェーダー
VSOutput VS_Flip(float4 pos : POSITION, float2 uv : TEXUV)
{
	VSOutput Out;
	Out.Pos = pos;

	// UV の X を反転して渡す（左右反転）
	Out.UV = float2(1.0 - uv.x, uv.y);
	return Out;
}

float4 PS(VSOutput In) : SV_Target0
{
	float4 color = Texture.Sample(Sampler, In.UV);

	/*if (color.a == 0)
	{
		color = float4(1.0, 1.0, 0, 1.0);
	}*/

	return color;
}

float4 PS_Color(VSOutput In) : SV_Target0
{
    return color;
}