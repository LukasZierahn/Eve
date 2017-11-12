struct FM_INPUT
{
	float4 Pos : SV_POSITION;
	float3 Norm : TEXCOORD0;
	float2 Tex : TEXCOORD1;
};

[maxvertexcount(12)]
void FM(point FM_INPUT input[1], inout TriangleStream<FM_INPUT> TriStream)
{
	FM_INPUT output = 0;
	TriStream.Append(output);
}