// 3Dモデル描画用のスタンダードなシェーダー

Texture2D Texture : register(t0);
SamplerState Sampler : register(s0);

// カメラ関連のバッファ
cbuffer ConstantBuffer : register(b1)
{
    matrix viewProjMat;
};

struct VS_INPUT
{
    // 頂点データ
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    
    // インスタンスデータ (ワールド行列、カラー)
    float4x4 worldMat : INST_MATRIX;
    float4 color : INST_COLOR;
};

struct VS_OUTPUT
{
    float4 pos : SV_Position;
    float3 normal : NORMAL;
    float2 uv : TEXCOORD;
    float4 color : COLOR;
};

// 頂点シェーダー
VS_OUTPUT VS_Main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    // モデル座標 -> ワールド座標 -> クリッピング座標
    float4 worldPos = mul(float4(input.pos, 1.0f), input.worldMat);
    output.pos = mul(worldPos, viewProjMat);
    
    // 行列無視
    //output.pos = float4(input.pos, 1);
    
    // 法線の変換 (拡大縮小を考慮して正規化)
    output.normal = normalize(mul(input.normal, (float3x3)input.worldMat));
    
    output.uv = input.uv;
    output.color = input.color;
    
    return output;
}

// ピクセルシェーダー
float4 PS_Main(VS_OUTPUT input) : SV_Target0
{
    return float4(1, 1, 1, 1);
    
    // 簡易的なランバート照明
    float3 lightDir = normalize(float3(1.0f, -1.0f, 1.0f));
    float ambient = 0.4f;
    float diffuse = saturate(dot(input.normal, -lightDir)) * (1.0f - ambient);
    float3 lighting = ambient + diffuse;

    // テクスチャサンプリングとライティングの適用
    float4 texColor = Texture.Sample(Sampler, input.uv);
    
    // 透明度が0の場合は破棄（必要に応じて）
    if(texColor.a < 0.01f) discard;
    
    return texColor * input.color * float4(lighting, 1.0f);
}
