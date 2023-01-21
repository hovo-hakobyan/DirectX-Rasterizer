// -----------------------------------------------------
// 
// 
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Wrap; //or Mirror, Clamp, Border
    AddressV = Wrap; //or Mirror, Clamp, Border
};


// -----------------------------------------------------
// Global variables
// -----------------------------------------------------

float4x4 gWorldViewProj : WorldViewProjection;
Texture2D gDiffuseMap : DiffuseMap;

// -----------------------------------------------------
// Input/Output structs
// -----------------------------------------------------
struct VS_INPUT
{
    float3 Position :   POSITION;
    float2 Uv       :   TEXCOORD;
};

struct VS_OUTPUT
{
    float4 Position :   SV_POSITION;
    float2 Uv       :   TEXCOORD;
};

// -----------------------------------------------------
// Vertex Shader
// -----------------------------------------------------
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Position = mul(float4(input.Position,1.f), gWorldViewProj);
    output.Uv = input.Uv;
    return output;
}

// -----------------------------------------------------
// Pixel Shader
// -----------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET
{
    return gDiffuseMap.Sample(samPoint,input.Uv);
}

// -----------------------------------------------------
// Technique 
// -----------------------------------------------------
technique11 DefaultTechnique
{
    pass P0
    {
        SetVertexShader( CompileShader(vs_5_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_5_0, PS()));
    }
}

