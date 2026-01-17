// ０番のテクスチャスロットを使用する
Texture2D Texture : register(t0);

// ０番のサンプラスロットを使用する
SamplerState Sampler : register(s0);

cbuffer ConstantBuffer : register(b0)
{
    matrix viewProjMat;
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
	// スロット０
    float4 pos : POSITION;
    float2 uv : TEXUV;
	
	// スロット１
    float4x4 worldMat : INST_MATRIX; // wvp = (World * View * Projection)
    float4 color : INST_COLOR;
    float flipX : INST_FLIPX;
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

VS_OUTPUT VS_New(VS_INPUT input)
{
    VS_OUTPUT output;
    output.pos = mul(mul(input.pos, input.worldMat), viewProjMat);
    output.uv.x = input.uv.x + input.flipX * (1.0 - 2.0 * input.uv.x);
    output.uv.y = input.uv.y;
	
    output.color = input.color;
	
    return output;
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

float4 PS_New(VS_OUTPUT input) : SV_Target0
{
    float4 color = Texture.Sample(Sampler, input.uv);

    return color * input.color;
}

float4 PS_New_Color(VS_OUTPUT input) : SV_Target0
{
    return input.color;
}