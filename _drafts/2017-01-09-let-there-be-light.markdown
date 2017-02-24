---
layout: post
title: "Let there be light"
date: "2017-01-09 00:30:57 +0000"
categories: Dissertation
comments: true
---
_I must begin by apologising for the lack of updates. You may have seen on my youtube account and facebook that I am also working on a game as a part of another module here at University. My attention was somewhat taken hostage by this other project as the weeks were drawing close to our final presentations for the year. I have been working hard on getting that project up and running which has left this project to be rather neglected. As a side note, I am aiming to write about other project here and document their progress in the future so keep an eye open for that._


Over the christmas break, I decided to continue following _3D Game Programming with DirectX 11_ (3D Game Programming) and strayed away from the milestones that I had set out: implementing Textures and importing FBX meshes. I had had started to add textures at the end of november using _RasterTek_ again however I kept running into issues that i believe mostly came down to me not understanding the code so i eventually gave up on that branch and read into the next chapter in _3D Game Programming:_ Lighting.

# Meshes
As i hadn't yet implemented Mesh file importing I needed to hard code meshes to render, additionally, the vertices now needed to have normals for directional, point and spotlights, as such the Vertex struct now has an `XMFLOAT3 normal` field instead of colour. This is reflected in the chader's header.

I wrote two cubes for testing the lights, a "regular" cube with normals facing away from the faces, prodicing hard faces or sharp corners; and a "soft" cube with the normals all facing radially, this better shows off the effect of point lights and such by loosely immitating a sphere, albeit a very low poly sphere.

Here's a render from 3DS Max of what i hoped to achieve:

![Cube Demonstration]({{ site.url }}\img\dissertation\Lighting-Cube-Types.png){:width="960px"}

As you can see the flat shaded "Hard" cube does not show off the point light very effectively.

# The Shader

The lights are described in "LightHelper.h" i can include this in any future shaders that i write and save on having to tweak them in multiple places if i adjust the actual shaders. They are reflected Just like the Vertex struct in their own hlsl header.

I will assume that you know what ambient, directional, point and spot lights are so I will not go into detatil about them, they are explaned in detail in _3D Game Programming_ allong with the various equasions that you need to compute them correctally.

The bare minimum code required to compute these lights is written in the book so my code is almost identical when it comes to the individual functions; At this stage i am still learning the functions that HLSL provides and as i start to implement textures for colour maps and normal maps etc, i can see that i have the knowledge to continue writing them on my own.

At this point, the Directional light works and both the point light and spot light have issues with them that I am not happy with. There is a problem with attenuation the point and spot lights which means that the intensity of the light must be 0 - 255 and not 0 - 1, this caused many hours of debugging as any values below about 10 are so dim that they are almost impossible to see. Additionally light attenuation does not nesscserally make it fade out to the radius of the light, this means that either the light will not have faded leaving a sharp "edge" to it or it will fade long before it has reached the edge, I can live with this for now and just fiddle with the attenuation until it's correct (I know this is the point of the attenuation however i would rather it working at 1 and not having to tweak it just to get it working).


Both of these scenes have the same attenuation on the light but the range is greater in the right image.

![Attenuation Problem]({{ site.url }}\img\dissertation\Lighting-Point-Light-Attenuation.png){:width="960px"}

# Buffers

When uploading resources to the GPU for use in Shaders, we use Constant Buffers (cbuffer), which are blocks of memory that we can change with the CPU. I was using one for telling the shader where the object I want to render should be with the `WorldViewProj` matrix, this is changed on a per object basis as the position of each object is most likely different, however updating this cbuffer can be an expensive operation to perform and if we also need to have lighting data available, this only needs to be updated on a per frame basis to be more efficent. For this reason I Use a buffer for per frame data such as lights; a per object buffer with positions and other data that is unique to individual objects; in the future, a per material buffer will be implemented so that all objects using the same material are rendered at the same time, this is an important optimisation as not only changing materials all the time is an obvious waste of time but uploading Textures is costly.

I had been struggling with getting a second buffer working and felt like i was getting nowhere so while optimisation wasn't a priority, to save my sanity I added the Lights to the existing per object buffer. Once i had gotten all of the lights working i went back to the problem of needing multiple cbuffers, I discovered that the size of a cbuffer needs to be a multiple of 16 bytes; I should have realised this much earlier as _3D Game Programming_ mentions it and it would have saved me lots of time.

# Finally

Here are some renders. (These were made after implementing the FBX importer)

![Crate on a pallet]({{ site.url }}\img\dissertation\Lighting-Crate-Pallet.png){:width="960px"}

![Utah Teapot]({{ site.url }}\img\dissertation\Lighting-Teapot.png){:width="960px"}

![Sponza]({{ site.url }}\img\dissertation\Lighting-Sponza.png){:width="960px"}

# Additionally...

I discovered that HLSL shaders allow you to include other files with an `#include` preprocessor directive like in c++, _3D Game Programming_ uses this for the lights helper, To avoid rewriting structs for individual shaders I have a sort of header specifically for the "Lit" shader.
