---
layout: post
title: "Changes to Materials"
date: "2017-05-02 00:06:40 +0100"
categories: Dissertation
comments: true
---

At one point each mesh had a pointer to a material however for some reason I had changed this and have been using a material pointer in `D3D11App`, I have now reached the point where the meshes that I am using may well require different meshes and eventually these will be imported with the FBX SDK, for this reason I added the pointer back into the mesh class and removed it from `D3D11App`.

Additionally, materials now have a pointer to their diffuse texture and I am no longer manually swapping the texture from `mTextures`. This means that my `UpdateMaterial` method now only requires the device context and a material. This is a far more sensible way of dealing with materials and textures as the imported textures are always being used with a material anyway.

In the future, as I begin to add support for more texture types in the material, I would like to have a referance to both a texture for each map as well as an alternative Colour similar to how material in Unity 3D work. This will allow me to set a colour for that slot if I do not intend to use a texture there, and choosing whether to use it depending on whether the texture is set.
