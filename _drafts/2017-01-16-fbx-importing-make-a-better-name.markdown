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

To keep this simple, say we only need to know the positions of the vertices.
we do not care about normals or UV co-ords for the time being however using
that data requires a small addidition to the loops and the functions are very similar.

FBX uses "Control Points", "Polygons" and "Polygon Vertices"

![Control Points and Polygons](\img\dissertation\FBX-Importing-Control-Points.jpg){:width="960px"}

Vertices are an index of a control point.

| Polygon | vertices|
| --- | :---: |
| 1 | 1, 3, 2 |
| 2 | 2, 3, 4 |
| 3 | 3, 5, 4 |
| 4 | 3, 6, 5 |
| 5 | 1, 6, 3 |

A simplified version of the code i use to copy the data is as follows:

* Loop through all polygons
* loop through polygon vertices

```
size_t pgCount = mesh->GetPolygonCount(); // Number of polygons
Vertex * vertices = new Vertex[pgCount * 3]; // Assume only triangles
controlPoints = meshe->GetControlPoints(); // list of positoin vectors

// Loop through all polygons
for (size_t currentPg = 0; currentPg < pgCount; currentPg++)
{
      // Loop through the vertices in the polygon
      for (size_t currentV = 0; currentV < 3; currentV++)
      {
            // Get the position of the current vertex in the polygon
            vertices[currentPg * 3 + currentV].pos =
                  controlPoints[mesh->GetPolygonVertex(currentPg, 2 - currentV)];
      }
}
```

# Resource Management

While working on this milestone i had to itterate my test meshes to fix rotations and scaling; Continually having to copy my textures and meshes into the build folder got very repetative so after a quick google search and poking around in Preferences i discovered "Build Events". In visual Studio you can add commands that can be executed either before, during or after compiling, I added a little command that will copy both my resources and my DLLs into the build directory after the code compiles. This also means I won't forget to drag the updated files into the build folder after compiling leading to time sinks looking for nonexistant bugs.

Simply adding the two commands `xcopy /Y /E "$(ProjectDir)res" "$(OutDir)res"` and `xcopy /Y /E "$(ProjectDir)dll\x86\Debug\*" "$(OutDir)"` to the post build events will copy the `\res\` folder and the contents of the `\dll\` folder into the build location. It will do this for both Debug _and_ release.

This little trick is a convenient QOL addition that can be used in all sorts of projects and i9 thought it would be cool to share it in the event that any readers have not seen it before.

# PSA Check Function Parameters

I will finish with a short PSA. Do Not Forget when you change the parameters on a function! A change i made to the Render function on the Lit Shader was that instead of taking the World, View and Projection matrices, it now used the WorldViewProj, combined matrix; Inverse WorldViewProj and the World matrix by itself.

I made the mistake of reverting to the old setup while writing the new on-Render method, All three are still matrices however, understandibly nothing was rendering as they completely the wrong things and i lost about 6 hours rewriting and debugging the import code before i found the cause of the issue.
