---
layout: post
title: "Milestone - Drawing a Spinning Cube"
date: "2016-11-17 12:00:00 +0000"
---

Over the past two weeks I have been working towards completing my first programming milestone: _Drawing a cube in DirectX_. This has been a massive learning experience for me; I have learned about DirectX and Direct 3D in general, the rendering pipeline, shaders and many useful standards relating to C++ and OOP.


# Outcome
In short, I have managed to create a cube mesh; draw it using vertex colours and a set of very basic pixel and vertex shaders; and spun it on the y axis for the sake of "why not".
The three main components that make up this program, on top of the nesscsary initialization, ade the Mesh class, Basic shader class and the Camera Class.

The Mesh class handles the Vertex buffer (essentially the list of vertices) and the Index buffer, it also has a matrix for it's World transform. Not only does it take out vertex and index arrays and put them into the respective buffers but it also has a Render methos that Sets them on the GPU ready for our shaders to use.

Our Shader class compiles the shader programs we want to use and Deals with both mapping any constant buffers (variables we can take from System memory and then use on the GPU in shaders) to the GPU and actually drawing the geometry we've sent it.

Finally the camera class contains the view matrix (position and rotation of the viewpoint in the "world") and the projection matrix (the frustum of the camera's view) which we can change to either move the viewpoint or to keep the projection correct as the screen's dimentions change and also `get` to multiply by a mesh's world matrix for drawing.


I have also been trying to use the correct conventions for naming and using variables, i found a very comprehensive collection of these [Naming Conventions](http://geosoft.no/development/cppstyle.html#Naming%20Conventions) online and have been uing it as a guide. The examples that come to mind are:

 * using `m` for class members
 * `p` for pointers
 * `g` for global variables
   * additionally using the global scope operator `::` to emphasize the fact that it's in the global scope.

These are proving very helpful as i can see at a glance what an object is and how i should access it.

# Problems
Moving to DirectX 11 has presented some problems in the way of certain libraries that are used in Books and tutorials are no longer available or i just cannot figure out what i need to be including to get them working.
The worst example of this is the "Effects11" framework. This is a framework that's in the DX SDK from June 2010, it's essentially a convenient way of creating complex effects with shaders however, as DX11 has become somewhat un-supported as it's bigger brother DX12 has been introduced it was more hastle trying to build it and use it than to just figure out how to get the same effect with regular shaders. I will add that this is a bit of a pain as Luna (2012) used effects throughout the book.

To get a basic introduction on how to use shaders, [RasterTek](http://www.rastertek.com/tutdx11s2.html) does provide a very good tutorial/series on using DX11. For the finished build, it was enough to get me started and complete the milestone so i will make sure to keep the site handy for if i hit any more bumps in the road.

# Youtube Video
Finally, what you've been waiting for: Proof!
<iframe width="960" height="540" src="https://www.youtube.com/embed/Pxzs37qwhcw" frameborder="0" allowfullscreen></iframe>

It may not be that impressive compared to what i can do with Monogame or a full game engine such as UE4 but I am pleased with it and what i have learned.
