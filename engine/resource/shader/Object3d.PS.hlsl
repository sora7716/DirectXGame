#include "Object3d.hlsli"

struct Material
{
    float4 color;
    int enableLighring;
};

//平行光源
struct DirectionalLight
{
    float4 color;//ライトの色
    float3 direction;//ライトの向き
    float intensity;//輝度
};

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{
    float4 textureColor = gTexture.Sample(gSampler, input.texcoord);
    PixelShaderOutput output;
    if (gMaterial.enableLighring != 0)//Lightingする場合
    {
        float cos = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));
        output.color = gMaterial.color * textureColor * gDirectionalLight.color * cos * gDirectionalLight.intensity;
    }
    else
    {
        output.color = gMaterial.color * textureColor;
    }
        return output;
}