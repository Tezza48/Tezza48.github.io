---
layout: post
title: "Specular Maps"
date: "2017-05-05 05:06:42 +0100"
category: Dissertation
comments: true
---

# Specular Maps

Specular maps are used in 3D Rendering and Games to specify what parts of a material are shiny or more reflective, as the Games industry has moved more towards Physically Based Rendering (PBR) which uses both roughness maps and reflectivity maps for more physically accurate results, however as a beginner I have decided to stick with what I know and implement the more basic model, leaving PBR as a feature to be added way into the future.

Adding a specular map to the shader was very simple. Similarly, to how the Diffuse texture is multiplied with the diffuse value of the light, we can control the specular intensity with a texture in the same way. To add the specular map to my shader, I added another texture to the pixel shader named `gTexSpec` which I assigned to the second texture register and updated it's sub-resource in the same way as I did with the diffuse map, and using 1 as the first parameter, as I’m using slot 1 for the specular map.

To demonstrate the effect of the specular map, I have rendered a plane with a charcoal material from Textures.com with and without the spec map.

![Specular map comparison](\img\dissertation\Specular-Mapping-comparison.PNG)

This milestone took far less time to complete than I had anticipated back in October when I wrote my proposal as I had already learned everything I needed when I added in the support for diffuse textures. Had I realized this i could have completed this task at the same time similar to how I completed my lighting milestones, saving time and freeing up mode space in the advanced milestones for more tasks.

# Reference

Textures.com. (n.d.). _Charred Wood_. [online] Available at: https://www.textures.com/download/3dscans0106/128771 [Accessed 5 May 2017].
