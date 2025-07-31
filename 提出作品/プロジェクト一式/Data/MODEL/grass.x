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
 22;
 -10.04542;-1.29706;3.60261;,
 -9.64749;15.82376;3.21938;,
 0.19897;15.51855;-0.19161;,
 -0.19881;-1.60226;0.19161;,
 3.61503;15.65969;9.65691;,
 3.21725;-1.46113;10.04012;,
 -9.98862;-1.29531;3.63014;,
 -0.22634;-1.59784;0.24826;,
 0.17159;15.52219;-0.13497;,
 -9.59085;15.82471;3.24692;,
 3.16061;-1.45797;10.01260;,
 3.55854;15.66205;9.62921;,
 9.64764;-1.90732;-3.21938;,
 10.04542;15.21334;-3.60261;,
 -3.21709;15.37742;-10.04012;,
 -3.61487;-1.74325;-9.65691;,
 9.59100;-1.90416;-3.24692;,
 -0.17128;-1.60164;0.13497;,
 0.22650;15.51839;-0.24825;,
 9.98893;15.21587;-3.63014;,
 -3.55822;-1.74151;-9.62936;,
 -3.16045;15.37852;-10.01259;;
 
 16;
 3;0,1,2;,
 3;2,3,0;,
 3;3,2,4;,
 3;4,5,3;,
 3;6,7,8;,
 3;8,9,6;,
 3;7,10,11;,
 3;11,8,7;,
 3;12,13,2;,
 3;2,3,12;,
 3;3,2,14;,
 3;14,15,3;,
 3;16,17,18;,
 3;18,19,16;,
 3;17,20,21;,
 3;21,18,17;;
 
 MeshMaterialList {
  1;
  16;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   100.000000;
   1.000000;1.000000;1.000000;;
   0.800000;0.800000;0.800000;;
   TextureFilename {
    "Data\\TEXTURE\\gltf_embedded_8.png";
   }
  }
 }
 MeshNormals {
  24;
  -0.327676;-0.013531;-0.944693;,
  -0.327678;-0.013529;-0.944692;,
  -0.327674;-0.013532;-0.944694;,
  0.944505;-0.029268;-0.327192;,
  0.944504;-0.029268;-0.327192;,
  0.327680;0.013531;0.944692;,
  0.327683;0.013530;0.944691;,
  0.327678;0.013533;0.944693;,
  -0.944501;0.029280;0.327202;,
  -0.944502;0.029281;0.327199;,
  0.327679;0.013532;0.944692;,
  0.327679;0.013532;0.944692;,
  -0.944504;0.029268;0.327192;,
  -0.944504;0.029268;0.327192;,
  -0.327680;-0.013531;-0.944692;,
  -0.327683;-0.013533;-0.944691;,
  -0.327678;-0.013530;-0.944693;,
  0.944502;-0.029269;-0.327199;,
  0.944502;-0.029269;-0.327200;,
  0.944505;-0.029268;-0.327192;,
  -0.944500;0.029278;0.327204;,
  0.327679;0.013532;0.944692;,
  -0.944504;0.029268;0.327192;,
  0.944502;-0.029269;-0.327199;;
  16;
  3;0,1,0;,
  3;0,2,0;,
  3;3,19,3;,
  3;3,4,3;,
  3;5,6,5;,
  3;5,7,5;,
  3;8,9,8;,
  3;8,20,8;,
  3;11,10,11;,
  3;11,21,11;,
  3;12,22,12;,
  3;12,13,12;,
  3;14,15,14;,
  3;14,16,14;,
  3;17,18,17;,
  3;17,23,17;;
 }
 MeshTextureCoords {
  22;
  0.991819;0.000000;,
  0.991819;0.913454;,
  0.494341;0.913454;,
  0.494341;0.000000;,
  -0.003136;0.913454;,
  -0.003136;0.000000;,
  -0.003136;0.000000;,
  0.494341;0.000000;,
  0.494341;0.913454;,
  -0.003136;0.913454;,
  0.991819;0.000000;,
  0.991819;0.913454;,
  0.991819;0.000000;,
  0.991819;0.913454;,
  -0.003136;0.913454;,
  -0.003136;0.000000;,
  -0.003136;0.000000;,
  0.494341;0.000000;,
  0.494341;0.913454;,
  -0.003136;0.913454;,
  0.991819;0.000000;,
  0.991819;0.913454;;
 }
}
