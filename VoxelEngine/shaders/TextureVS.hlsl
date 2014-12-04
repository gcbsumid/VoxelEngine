// Globals
cbuffer MatrixBuffer {
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

// Typedefs input and outputs
struct VertexInputType {
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
};

struct PixelInputType {
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
};

// Shader 
PixelInputType main(VertexInputType input) {
    PixelInputType output;

    // change position to be vector 4 
    input.positon.w = 1.0f;

    // calculate position of vertex
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.tex = input.tex;

	return output;
}