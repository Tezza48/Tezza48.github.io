---
layout: post
title: "Textured Sponza Render"
date: "2017-04-07 00:08:02 +0100"
---

# Textured Sponza Render

I can import Fbx files and textures.

I wanted to finally render the sponza scene with the correct textures

started by trying to get texture information from the FBX file

spent lots of time not getting enywhere

Decided to manually pair up mesh parts with textures

renamed all meshes in max file to end with the texture they use

attatched all meshes with the same texture together

exported all indivisual meshes and named them according to the texture they use

Converted textures to Targa in GIMP.

textures were 24 bit good excuse to add 24 bit conversion to content manager

how the manager loops to add in the alpha channel
```
for (size_t tex24 = 0, tex32 = 0;
	tex24 < width * height * 3;
	tex24+=3, tex32+=4)
{
	imgData[tex32 + 0] = rawData[tex24 + 2];
	imgData[tex32 + 1] = rawData[tex24 + 1];
	imgData[tex32 + 2] = rawData[tex24 + 0];
	imgData[tex32 + 3] = (char)255;
}
```
figured out you can itterate through more than one variable in a for loop.

fix to targa header li hi endian problem.

issue with Texture dtor, now checking that pointers are not null before deleting.

now holding an array of textures.

Mesh importing now uses FBX built in axis conversion `FbxAxisSystem::DirectX.ConvertScene(scene); `

finally rendering the scene after placing the camera.
