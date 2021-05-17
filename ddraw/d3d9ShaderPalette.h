//
// Generated by Microsoft (R) HLSL Shader Compiler 10.1
//
// Parameters:
//
//   sampler2D PaletteTex;
//   sampler2D SurfaceTex;
//
//
// Registers:
//
//   Name         Reg   Size
//   ------------ ----- ----
//   SurfaceTex   s0       1
//   PaletteTex   s1       1
//

/*
    ps_2_0
    def c0, 0.99609375, 0.001953125, 0, 0
    dcl t0.xy
    dcl_2d s0
    dcl_2d s1
    texld r0, t0, s0
    mad r0.x, r0.x, c0.x, c0.y
    mov r0.y, c0.z
    texld r0, r0, s1
    mov oC0, r0
*/

// approximately 5 instruction slots used (2 texture, 3 arithmetic)

// fxc.exe /Tps_2_0 shader.hlsl /Fhshader.h
/*
uniform sampler2D SurfaceTex;
uniform sampler2D PaletteTex;

float4 main(float2 texCoords : TEXCOORD) : COLOR
{
    float pIndex = tex2D(SurfaceTex, texCoords).r;
    return tex2D(PaletteTex, float2(pIndex * (255./256) + (0.5/256), 0));
}
*/

const BYTE PalettePixelShaderSrc[] =
{
      0,   2, 255, 255, 254, 255, 
     44,   0,  67,  84,  65,  66, 
     28,   0,   0,   0, 131,   0, 
      0,   0,   0,   2, 255, 255, 
      2,   0,   0,   0,  28,   0, 
      0,   0,   0,   1,   0,   0, 
    124,   0,   0,   0,  68,   0, 
      0,   0,   3,   0,   1,   0, 
      1,   0,   0,   0,  80,   0, 
      0,   0,   0,   0,   0,   0, 
     96,   0,   0,   0,   3,   0, 
      0,   0,   1,   0,   0,   0, 
    108,   0,   0,   0,   0,   0, 
      0,   0,  80,  97, 108, 101, 
    116, 116, 101,  84, 101, 120, 
      0, 171,   4,   0,  12,   0, 
      1,   0,   1,   0,   1,   0, 
      0,   0,   0,   0,   0,   0, 
     83, 117, 114, 102,  97,  99, 
    101,  84, 101, 120,   0, 171, 
      4,   0,  12,   0,   1,   0, 
      1,   0,   1,   0,   0,   0, 
      0,   0,   0,   0, 112, 115, 
     95,  50,  95,  48,   0,  77, 
    105,  99, 114, 111, 115, 111, 
    102, 116,  32,  40,  82,  41, 
     32,  72,  76,  83,  76,  32, 
     83, 104,  97, 100, 101, 114, 
     32,  67, 111, 109, 112, 105, 
    108, 101, 114,  32,  49,  48, 
     46,  49,   0, 171,  81,   0, 
      0,   5,   0,   0,  15, 160, 
      0,   0, 127,  63,   0,   0, 
      0,  59,   0,   0,   0,   0, 
      0,   0,   0,   0,  31,   0, 
      0,   2,   0,   0,   0, 128, 
      0,   0,   3, 176,  31,   0, 
      0,   2,   0,   0,   0, 144, 
      0,   8,  15, 160,  31,   0, 
      0,   2,   0,   0,   0, 144, 
      1,   8,  15, 160,  66,   0, 
      0,   3,   0,   0,  15, 128, 
      0,   0, 228, 176,   0,   8, 
    228, 160,   4,   0,   0,   4, 
      0,   0,   1, 128,   0,   0, 
      0, 128,   0,   0,   0, 160, 
      0,   0,  85, 160,   1,   0, 
      0,   2,   0,   0,   2, 128, 
      0,   0, 170, 160,  66,   0, 
      0,   3,   0,   0,  15, 128, 
      0,   0, 228, 128,   1,   8, 
    228, 160,   1,   0,   0,   2, 
      0,   8,  15, 128,   0,   0, 
    228, 128, 255, 255,   0,   0
};
