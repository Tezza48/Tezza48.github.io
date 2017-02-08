---
layout: post
title: "Let there be light"
date: "2017-01-09 00:30:57 +0000"
categories: Dissertation
comments: true
---
_I must begin by apologising for the lack of updates over the last two months, You may have seen on my youtube account and facebook that i am also working on a game with a few colleagues as a part of another module here at University. My attention was somewhat taken hostage by this other project as the weeks were drawing close to our final presentations for the year. I have been working hard on getting that project up and running which has left this project to be rather neglected. As a side note, I am aiming to write about this other project here and document it's progress in the upcomming semester so keep an eye open for that._


Over the christmas break, I decided to continue following _3D Game Programming with DirectX 11_ (3D Game Programming) and strayed away from the milestones that I had set out: implementing Textures and importing FBX meshes. I had had started to add textures at the end of november using _RasterTek_ again however I kept running into issues that i believe mostly came down to me not understanding the code so i eventually gave up on that branch and read into the next chapter in _3D Game Programming:_ Lighting.

# Meshes
As i hadn't yet implemented Mesh file importing I needed to hard code meshes to render, additionally, the vertices now needed to have normals for directional, point and spotlights, as such the Vertex struct now has an `XMFLOAT3 normal` field instead of colour. This is reflected in the chader's header.

I wrote two cubes for testing the lights, a "regular" cube with normals facing away from the faces, prodicing hard faces or sharp corners; and a "soft" cube with the normals all facing radially, this better shows off the effect of point lights and such by loosely immitating a sphere, albeit a very low poly sphere.

Here's a render from 3DS Max of what i hoped to achieve:

![Cube Demonstration]({{ site.url }}\img\dissertation\Lighting-Cube-Types.png){:width="960px"}
![Cube Demonstration TEMP](\img\dissertation\Lighting-Cube-Types.png){:width="960px"}

As you can see the flat shaded "Hard" cube does not show off the point light very effectively.

# The Shader

The lights are described in "LightHelper.h" i can include this in any future shaders that i write and save on having to tweak them in multiple places if i adjust the actual shaders. They are reflected Just like the Vertex struct in their own hlsl header.

I will assume that you know what ambient, directional, point and spot lights are so iIwill not go into detatil about them, they are definedand explaned in detail in _3D Game Programming_ allong with the various equasions that you need to compute them correctally.

The bare minimum code required to compute these lights is written in the book so my code is almost, if not completely identical when it comes to the individual functions; At this stage i am still learning the functions that HLSL provides and as i start to implement textures for colour maps and normal maps etc, i can see that i have the knowledge to continue writing them on my own.

When uploading resources to the GPU for use in Shaders, we use Constant Buffers (cbuffer), which are blocks of memory that we can change with the CPU. I was using one for telling the shader where the object I want to render should be with the `WorldViewProj` matrix, this is changed on a per object basis as the position of each object is most likely different, however updating this cbuffer can be an expensive operation to perform and if we also need to have lighting data available, this only needs to be updated on a per frame basis to be more efficent.

I had been struggling with getting a second buffer working and felt like i was getting nowhere so while optimisation wasn't a priority, to save my sanity I Added the Lights to the existing per object buffer. Once i had gotten all of the lights working i went back to the problem of needing multiple cbuffers, I discovered that the size of a cbuffer needs to be a multiple of 16 bytes; I should have realised this much earlier as _3D Game Programming_ mentions it and it would have saved me lots of time.

Directional Lights.
Light spinning around the cube.
      Had been decomposing the Eye's position from matrix after transposing it for uploading.
Added HLSL light helper
      in HLSL you can `#include<filename>` just like in C++
      made use of this later.

Another Small fix that i implemented during this time was

Replacing Vertex colours with UV data. (Ready for Texturing)

"soft Cube" normals pointing radially to make specular lighting easier to see.

Successfully Implementing a per-frame constant buffer.
      Issue was caused by needing to specify the correct register in the shader.
      cbuffers need to have a size that's a multiple of 16 bytes.
