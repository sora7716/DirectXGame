#include "Object3d.hlsli"

struct Material {
    float4 color;
    int enableLighring;
    float4x4 uvTransform;
};

//平行光源
struct DirectionalLight {
    float4 color; //ライトの色
    float3 direction; //ライトの向き
    float intensity; //輝度
};

ConstantBuffer<Material> gMaterial : register(b0);
Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);

struct PixelShaderOutput {
    float4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input) {
    float4 transformedUV = mul(float4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
    float4 textureColor = gTexture.Sample(gSampler, transformedUV.xy);
    PixelShaderOutput output;
    //Lightingする場合
    if (gMaterial.enableLighring != 0) {
        //lambert
        // float cos = saturate(dot(normalize(input.normal), -gDirectionalLight.direction));
        //half lambert
        float NdotL = dot(normalize(input.normal), -gDirectionalLight.direction);
        float cos = pow(NdotL * 0.5f + 0.5f, 2.0f);
        output.color.rgb = gMaterial.color.rgb * textureColor.rgb * gDirectionalLight.color.rgb * cos * gDirectionalLight.intensity;
        output.color.a = gMaterial.color.a * textureColor.a;
    } else {
        output.color = gMaterial.color * textureColor;
    }
    //textureのα値が0.5f以下の時にPixelを棄却
    if (textureColor.a <= 0.5) {
        discard;
    }
    //textureのα値の0の時にPixelを棄却
    if (textureColor.a == 0.0) {
        discard;
    }
    //output.colorのα値が0の時にPixelを棄却
    if (output.color.a == 0.0) {
        discard;
    }
    return output;
}