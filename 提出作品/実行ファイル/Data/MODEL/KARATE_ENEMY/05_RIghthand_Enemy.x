xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 47;
 -17.16279;0.02745;0.00000;,
 -16.02070;0.02745;-4.61649;,
 -16.02070;-3.97060;-2.30825;,
 -17.16279;0.02745;0.00000;,
 -16.02070;-3.97060;2.30825;,
 -17.16279;0.02745;0.00000;,
 -16.02070;0.02745;4.61649;,
 -17.16279;0.02745;0.00000;,
 -16.02070;4.02540;2.30825;,
 -17.16279;0.02745;0.00000;,
 -16.02070;4.02540;-2.30825;,
 -17.16279;0.02745;0.00000;,
 -16.02070;0.02745;-4.61649;,
 -12.90035;0.02745;-7.99600;,
 -12.90035;-6.89734;-3.99800;,
 -12.90035;-6.89734;3.99800;,
 -12.90035;0.02745;7.99600;,
 -12.90035;6.95216;3.99800;,
 -12.90035;6.95216;-3.99800;,
 -12.90035;0.02745;-7.99600;,
 -8.63792;0.02745;-9.23299;,
 -8.63792;-7.96860;-4.61649;,
 -8.63792;-7.96860;4.61650;,
 -8.63792;0.02745;9.23299;,
 -8.63792;8.02343;4.61649;,
 -8.63792;8.02343;-4.61649;,
 -8.63792;0.02745;-9.23299;,
 -4.37551;0.02745;-7.99600;,
 -4.37551;-6.89734;-3.99800;,
 -4.37551;-6.89734;3.99800;,
 -4.37551;0.02745;7.99600;,
 -4.37551;6.95216;3.99800;,
 -4.37551;6.95216;-3.99800;,
 -4.37551;0.02745;-7.99600;,
 -1.25520;0.02745;-4.61650;,
 -1.25520;-3.97060;-2.30825;,
 -1.25520;-3.97060;2.30825;,
 -1.25520;0.02745;4.61650;,
 -1.25520;4.02540;2.30825;,
 -1.25520;4.02540;-2.30825;,
 -1.25520;0.02745;-4.61650;,
 -0.11310;0.02745;0.00000;,
 -0.11310;0.02745;0.00000;,
 -0.11310;0.02745;0.00000;,
 -0.11310;0.02745;0.00000;,
 -0.11310;0.02745;0.00000;,
 -0.11310;0.02745;0.00000;;
 
 36;
 3;0,1,2;,
 3;3,2,4;,
 3;5,4,6;,
 3;7,6,8;,
 3;9,8,10;,
 3;11,10,12;,
 4;1,13,14,2;,
 4;2,14,15,4;,
 4;4,15,16,6;,
 4;6,16,17,8;,
 4;8,17,18,10;,
 4;10,18,19,12;,
 4;13,20,21,14;,
 4;14,21,22,15;,
 4;15,22,23,16;,
 4;16,23,24,17;,
 4;17,24,25,18;,
 4;18,25,26,19;,
 4;20,27,28,21;,
 4;21,28,29,22;,
 4;22,29,30,23;,
 4;23,30,31,24;,
 4;24,31,32,25;,
 4;25,32,33,26;,
 4;27,34,35,28;,
 4;28,35,36,29;,
 4;29,36,37,30;,
 4;30,37,38,31;,
 4;31,38,39,32;,
 4;32,39,40,33;,
 3;34,41,35;,
 3;35,42,36;,
 3;36,43,37;,
 3;37,44,38;,
 3;38,45,39;,
 3;39,46,40;;
 
 MeshMaterialList {
  5;
  36;
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.056800;0.056800;0.056800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.552157;0.432941;0.260392;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.000000;0.088000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.056800;0.056800;0.056800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  32;
  -1.000000;0.000002;0.000000;,
  -0.884164;0.000002;-0.467176;,
  -0.884165;-0.404585;-0.233589;,
  -0.884165;-0.404585;0.233589;,
  -0.884164;0.000002;0.467176;,
  -0.884164;0.404587;0.233588;,
  -0.884164;0.404587;-0.233588;,
  -0.533358;0.000002;-0.845889;,
  -0.533359;-0.732561;-0.422946;,
  -0.533359;-0.732561;0.422946;,
  -0.533359;0.000003;0.845889;,
  -0.533360;0.732561;0.422944;,
  -0.533360;0.732561;-0.422944;,
  0.000001;0.000002;-1.000000;,
  0.000001;-0.866025;-0.500001;,
  0.000001;-0.866025;0.500001;,
  0.000001;0.000002;1.000000;,
  0.000001;0.866026;0.499999;,
  0.000001;0.866026;-0.499999;,
  0.533362;0.000002;-0.845887;,
  0.533362;-0.732559;-0.422945;,
  0.533362;-0.732559;0.422945;,
  0.533362;0.000002;0.845887;,
  0.533363;0.732560;0.422942;,
  0.533363;0.732560;-0.422942;,
  0.884165;0.000002;-0.467174;,
  0.884166;-0.404583;-0.233588;,
  0.884166;-0.404583;0.233588;,
  0.884165;0.000002;0.467174;,
  0.884165;0.404586;0.233586;,
  0.884165;0.404586;-0.233586;,
  1.000000;0.000002;0.000000;;
  36;
  3;0,1,2;,
  3;0,2,3;,
  3;0,3,4;,
  3;0,4,5;,
  3;0,5,6;,
  3;0,6,1;,
  4;1,7,8,2;,
  4;2,8,9,3;,
  4;3,9,10,4;,
  4;4,10,11,5;,
  4;5,11,12,6;,
  4;6,12,7,1;,
  4;7,13,14,8;,
  4;8,14,15,9;,
  4;9,15,16,10;,
  4;10,16,17,11;,
  4;11,17,18,12;,
  4;12,18,13,7;,
  4;13,19,20,14;,
  4;14,20,21,15;,
  4;15,21,22,16;,
  4;16,22,23,17;,
  4;17,23,24,18;,
  4;18,24,19,13;,
  4;19,25,26,20;,
  4;20,26,27,21;,
  4;21,27,28,22;,
  4;22,28,29,23;,
  4;23,29,30,24;,
  4;24,30,25,19;,
  3;25,31,26;,
  3;26,31,27;,
  3;27,31,28;,
  3;28,31,29;,
  3;29,31,30;,
  3;30,31,25;;
 }
 MeshTextureCoords {
  47;
  0.083330;0.000000;,
  0.000000;0.166670;,
  0.166670;0.166670;,
  0.250000;0.000000;,
  0.333330;0.166670;,
  0.416670;0.000000;,
  0.500000;0.166670;,
  0.583330;0.000000;,
  0.666670;0.166670;,
  0.750000;0.000000;,
  0.833330;0.166670;,
  0.916670;0.000000;,
  1.000000;0.166670;,
  0.000000;0.333330;,
  0.166670;0.333330;,
  0.333330;0.333330;,
  0.500000;0.333330;,
  0.666670;0.333330;,
  0.833330;0.333330;,
  1.000000;0.333330;,
  0.000000;0.500000;,
  0.166670;0.500000;,
  0.333330;0.500000;,
  0.500000;0.500000;,
  0.666670;0.500000;,
  0.833330;0.500000;,
  1.000000;0.500000;,
  0.000000;0.666670;,
  0.166670;0.666670;,
  0.333330;0.666670;,
  0.500000;0.666670;,
  0.666670;0.666670;,
  0.833330;0.666670;,
  1.000000;0.666670;,
  0.000000;0.833330;,
  0.166670;0.833330;,
  0.333330;0.833330;,
  0.500000;0.833330;,
  0.666670;0.833330;,
  0.833330;0.833330;,
  1.000000;0.833330;,
  0.083330;1.000000;,
  0.250000;1.000000;,
  0.416670;1.000000;,
  0.583330;1.000000;,
  0.750000;1.000000;,
  0.916670;1.000000;;
 }
}
