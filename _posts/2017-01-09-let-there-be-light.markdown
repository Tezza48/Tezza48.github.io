---
layout: post
title: "Let there be light"
date: "2017-03-12 10:30:00 +0000"
categories: Dissertation
comments: true
---

_I must begin by apologising for the lack of updates. You may have seen on my YouTube account and Facebook that I am also working on a game as a part of another module here at University. My attention was somewhat taken hostage by these other project as the weeks were drawing close to our final presentations for the year. I have been working hard on getting that project up and running which has left this project to be rather neglected. As a side note, I am aiming to write about other project here and document their progress in the future so keep an eye open for that._

---

Over the Christmas break, I decided to continue following _3D Game Programming with DirectX 11_ (3D Game Programming) and strayed away from the milestones that I had set out: implementing Textures and importing FBX meshes. I had had started to add textures at the end of November using _RasterTek_ again however I kept running into issues that I believe mostly came down to me not understanding the code so I eventually gave up on that branch and read into the next chapter in _3D Game Programming:_ Lighting.

# Meshes
As I hadn't yet implemented Mesh file importing I needed to hard code meshes to render, additionally, the vertices now needed to have normals for directional, point and spotlights, as such the Vertex struct now has an `XMFLOAT3 normal` field instead of colour. This is reflected in the shader's header.

I wrote two cubes for testing the lights, a "regular" cube with normals facing away from the faces, producing hard faces or sharp corners; and a "soft" cube with the normals all facing radially, this better shows off the effect of point lights and such by loosely imitating a sphere, albeit a very low poly sphere.

Here's a render from 3DS Max of what I hoped to achieve:

![Cube Demonstration](\img\dissertation\Lighting-Cube-Types.png){:width="960px"}

As you can see the flat shaded "Hard" cube does not show off the point light very effectively.

# The Shader

The lights are described in "LightHelper.h" I can include this in any future shaders that I write and save on having to tweak them in multiple places if I adjust the actual shaders. They are reflected Just like the Vertex struct in their own HLSL header.

I will assume that you know what ambient, directional, point and spot lights are so I will not go into detail about them, they are explained in detail in _3D Game Programming_ along with the various equations that you need to compute them correctly.

The bare minimum code required to compute these lights is written in the book so my code is almost identical when it comes to the individual functions; At this stage I am still learning the functions that HLSL provides and as I start to implement textures for colour maps and normal maps etc., I can see that I have the knowledge to continue writing them on my own.

At this point, the Directional light works and both the point light and spot light have issues with them that I am not happy with. There is a problem with attenuation the point and spot lights which means that the intensity of the light must be 0 - 255 and not 0 - 1, this caused many hours of debugging as any values below about 10 are so dim that they are almost impossible to see. Additionally light attenuation does not necessarily make it fade out to the radius of the light, this means that either the light will not have faded leaving a sharp "edge" to it or it will fade long before it has reached the edge, I can live with this for now and just fiddle with the attenuation until it's correct (I know this is the point of the attenuation however I would rather it working at 1 and not having to tweak it just to get it working).


Both of these scenes have the same attenuation on the light but the range is greater in the right image.

![Attenuation Problem](\img\dissertation\Lighting-Point-Light-Attenuation.png){:width="960px"}

# Buffers

When uploading resources to the GPU for use in Shaders, we use Constant Buffers (cbuffer), which are blocks of memory that we can change with the CPU. I was using one for telling the shader where the object I want to render should be with the `WorldViewProj` matrix, this is changed on a per object basis as the position of each object is most likely different, however updating this cbuffer can be an expensive operation to perform and if we also need to have lighting data available, this only needs to be updated on a per frame basis to be more efficient. For this reason, I Use a buffer for per frame data such as lights; a per object buffer with positions and other data that is unique to individual objects; in the future, a per material buffer will be implemented so that all objects using the same material are rendered at the same time, this is an important optimisation as not only changing materials all the time is an obvious waste of time but uploading Textures is costly.

I had been struggling with getting a second buffer working and felt like I was getting nowhere so while optimisation wasn't a priority, to save my sanity I added the Lights to the existing per object buffer. Once I had gotten all of the lights working I went back to the problem of needing multiple cbuffers, I discovered that the size of a cbuffer needs to be a multiple of 16 bytes; I should have realised this much earlier as _3D Game Programming_ mentions it and it would have saved me lots of time.

# Finally

Here are some renders. (These were made after implementing the FBX importer)

![Crate on a pallet](\img\dissertation\Lighting-Crate-Pallet.png){:width="960px"}

![Utah Teapot](\img\dissertation\Lighting-Teapot.png){:width="960px"}

![Sponza](\img\dissertation\Lighting-Sponza.png){:width="960px"}

# Additionally...

I discovered that HLSL shaders allow you to include other files with an `#include` pre-processor directive like in C++, _3D Game Programming_ uses this for the lights helper, to avoid rewriting structs for individual shaders I have a sort of header specifically for the "Lit" shader.
