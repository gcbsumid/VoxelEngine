// globals
Texture2D shaderTexture;
SamplerState SampleType;

// light buffer
cbuffer LightBuffer {
    float4 diffuseColor;
    float3 lightDirection;
    float padding;
};

// typedefs
struct PixelInputType {
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;


    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    textureColor = shaderTexture.Sample(SampleType, input.tex);

    // invert the light direction for calculations
    lightDir = -lightDirection;

    // Calculate the amount of light on this pixel
    lightIntensity = saturate(dot(input.normal, lightDir));

    // determine amount of diffuse color based on the diffuse color combined with the light intensity
    color = saturate(diffuseColor * lightIntensity);
    color = color * textureColor;
    //color = float4(input.normal, 1.0f);

    return color;
}