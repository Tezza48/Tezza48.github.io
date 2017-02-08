---
layout: post
title: "The FBX SDK - FBX Importing"
date: "2017-01-16 02:17:40 +0000"
categories: Dissertation
comments: true
---

The FBX SDK has really in depth documentation. With really lazy examples.

Autodesk's FBX is a common file format used when exporting 3D Model data for use in Games and other 3D applications, I have used it over the last few years with 3DS MAX, Unity 3D and Unreal Engine 4. Additionally Autodesh has provided an SDK on their website which is both convenient and simple to use (once you get to know it). As i had a few issues with it, this post will be more similar to a tutorial in the hopes that i can be of assistance to anyone else havving trouble getting started.

# Installation and Setup

Linking and including

FBX Format
      Node hierarchy
      Scene node
            Varoius other nodes with Mesh, light or other attributes

FBX mesh format

Say we only need to know the positions of the vertices, we do not care about normals or UV co-ords for the time being, we have our mesh class needing only Vertex positions and vertex indices.
FBX uses "Control Points", "Polygons" and "Polygon Vertices"

![Control Points and Polygons]({{ site.url }}\img\dissertation\FBX-Importing-Control-Points.jpg){:width="960px"}

![Control Points and Polygons](\img\dissertation\FBX-Importing-Control-Points.jpg){:width="960px"}

Demo Control points, Polygons and Polygon Vertices

How i Create a mesh from this.

Using xcopy to copy DLLs and Resources to build

My failure of Changing the shader to use (world, worldviewproj, worldinvtrans), forgetting and using (world, view proj). wasted time debugging and rewriting importing code because of this issue.
