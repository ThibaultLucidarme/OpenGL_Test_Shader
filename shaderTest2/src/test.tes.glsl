#version 410 core
// http://ogldev.atspace.co.uk/www/tutorial30/tutorial30.html

layout(triangles, equal_spacing, ccw) in;
/*
    triangles this is the domain the PG will work on. The other two options are quads and isolines.
    equal_spacing means that the triangle edges will be subdivided into segments with equal lengths (according to the TLs). You can also use fractional_even_spacing or fractional_odd_spacing that provide a smoother transition between the lengths of the segments whenever the TL crosses an even or odd integer. For example, if you use fractional_odd_spacing and the TL is 5.1 it means there will be 2 very short segments and 5 longer segments. As the TL grows towards 7 all the segments become closer in length. When the TL hits 7 two new very short segments are created. fractional_even_spacing is the same with even integer TLs.
    ccw means that the PG will emit triangles in counter-clockwise order (you can also use cw for clockwise order). You may be wondering why we are doing that while our front facing triangles in clockwise order. The reason is that the model I supplied with this tutorial (quad2.obj) was generated by Blender in counter clockwise order. I could also have specified the Assimp flag 'aiProcess_FlipWindingOrder' when loading the model and use 'cw' here. I simply didn't want to change 'mesh.cpp' at this point. The bottom line is that whatever you do, make sure you are consistent.

Note that you can also specify each configuration item with its own layout keyword. The scheme above simply saves some space. 
*/

uniform mat4 gVP;
uniform sampler2D gDisplacementMap;
uniform float gDispFactor;

in vec3 WorldPos_ES_in[];
in vec2 TexCoord_ES_in[];
in vec3 Normal_ES_in[];

out vec3 WorldPos_FS_in;
out vec2 TexCoord_FS_in;
out vec3 Normal_FS_in;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
} 

void main()
{
    // Interpolate the attributes of the output vertex using the barycentric coordinates
    TexCoord_FS_in = interpolate2D(TexCoord_ES_in[0], TexCoord_ES_in[1], TexCoord_ES_in[2]);
    Normal_FS_in = interpolate3D(Normal_ES_in[0], Normal_ES_in[1], Normal_ES_in[2]);
    Normal_FS_in = normalize(Normal_FS_in);
    WorldPos_FS_in = interpolate3D(WorldPos_ES_in[0], WorldPos_ES_in[1], WorldPos_ES_in[2]);

    // Displace the vertex along the normal
    float Displacement = texture(gDisplacementMap, TexCoord_FS_in.xy).x;
    WorldPos_FS_in += Normal_FS_in * Displacement * gDispFactor;
    gl_Position = gVP * vec4(WorldPos_FS_in, 1.0);
} 