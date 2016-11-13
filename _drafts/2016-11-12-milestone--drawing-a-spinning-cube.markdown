---
layout: post
title: "Milestone - Drawing a Spinning Cube"
date: "2016-11-4 12:00:00 +0000"
---

# Intro
Over the past two weeks I have been working towards completing my first programming milestone: _Drawing a cube in DirectX_. This has been a massive learning experience for me; I have learned about DirectX and Direct 3D in general, the rendering pipeline, shaders and many useful standards relating to C++ and OOP.


# Outcome
 * Spinning Cube
  * Basic Mesh & Camera class
  * Vertex Colour shader
  * [Naming Conventions](http://geosoft.no/development/cppstyle.html#Naming%20Conventions) i will be sticking to
   * name prefixes:
     * `m` = class member, eg `mSwapChain`
     * `p` = pointer, eg `pDXGIFactory`
     * `g` = global variable, eg `gd3dDevice`, but i'm not currently using any
       * note i'm also accessing global variables with the global scope operator `::`
     * I'll be trying my best to keep to them to make my code more easy to read and presentable.


# Problems
 * Starting with DX12
  * covered in last post
 * Effects11 framework
  * Used in (Luna, F 2012), didnt work
  * Workaround using [RasterTek](http://www.rastertek.com/tutdx11s2.html)


# Youtube Video
<iframe width="560" height="315" src="https://www.youtube.com/embed/Pxzs37qwhcw" frameborder="0" allowfullscreen></iframe>

# Conclusion
