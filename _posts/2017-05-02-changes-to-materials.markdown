---
layout: post
title: "Changes to Materials"
date: "2017-05-02 00:06:40 +0100"
categories: Dissertation
comments: true
---

At one point, each mesh had a pointer to a material however for some reason I had changed this and have been using a material pointer in `D3D11App`, I have now reached the point where the meshes that I am using may well require different meshes and eventually these will be imported with the FBX SDK, for this reason I added the pointer back into the mesh class and removed it from `D3D11App`.

Additionally, materials now have a pointer to their diffuse texture and I am no longer manually swapping the texture from `mTextures`. This means that my `UpdateMaterial` method now only requires the device context and a material. This is a far more sensible way of dealing with materials and textures as the imported textures are always being used with a material anyway.

When I start using bigger and bigger scenes that contain many materials I will be able to sort my renderables by the material they are using. Updating the GPU's resources is a costly procedure so by grouping all meshes by material I will only need to update that data the minimum number of times necessary as I will only have to update the resources once we have finished rendering one material. Reducing the number of times update the GPU will, in theory, lead to increased performance.

In the future, as I begin to add support for more texture types in the material, I would like to have a reference to both a texture for each map as well as an alternative Colour like how material in Unity 3D work. This will allow me to set a colour for that slot if I do not intend to use a texture there, and choosing whether to use it depending on whether the texture is set.
