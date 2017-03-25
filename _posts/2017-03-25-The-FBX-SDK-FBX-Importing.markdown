---
layout: post
title: "The FBX SDK - FBX Importing"
date: "2017-03-25 15:27:00 +0000"
categories: Dissertation
comments: true
---

Autodesk's FBX is a common file format used when exporting 3D Model data for use in Games and other 3D applications, as you're reading this I would assume you have already worked with it so I shan’t go into detail about it. I have used it over the last few years with 3DS MAX, Unity 3D and Unreal Engine 4. Autodesk has provided the SDK on their website for free and this is what I have decided I will use to import meshes into the engine. Learning the basics was rather a steep learning curve as the [documentation](http://help.autodesk.com/view/FBX/2017/ENU/) can be rather cryptic or hard to navigate so I would like to use this post as more of a tutorial showing you how I used it and what I did wrong.

# Installation and Setup

The SKD installer can be found on [Autodesk's website](http://www.autodesk.com/products/fbx/overview). On the downloads page, grab the version appropriate for your project; I am using _FBX SDK 2017.1 VS2015_ for windows (vanilla, not UWP). For the sake of simplicity, I left it with the default installation location on my **C** drive

## Linking and Including

To use the SDK, you must include it in your project. Open your project's Properties window > VC++ Directories > Library Directories and Edit the string. Here you must add the location of whichever library is right for your build settings.

![Linker settings](\img\dissertation\FBX-SDK-linking.png){:width="560px"}

Here is the include I use for 32 bit/Debug but you can just repeat the process to add the other libraries for x64 and Release builds.

You must also have the provided _libfbxsdk.dll_ in the build directory for the compiled code to run, At the end of this post there's a little tip to automate this like with the linked libraries.

# The FBX Format

The FBX format uses a node Hierarchy like how a scene in Unity3D is structured. There is a root/scene node which has a tree of other nodes in a hierarchical fashion. Nodes can have certain attributes such as lights meshes and splines among many, here we are interested in Meshes.


## Node Hierarchy

The hierarchy looks roughly like this:

```
Node Hierarchy:

Scene ¬
	Node : Mesh ¬
		Node : Mesh
		Node : Mesh
	Node ¬
		Node : Camera ¬
			Node : Light
```

You can navigate through the tree using the nodes and see what attributes each node contains using Loops or recursion, whichever you are comfortable using. I Iterate through the hierarchy recursively to find all nodes with FBX Meshes attached and add them to an array as I am more comfortable using recursion in this case.

## FBX Mesh

To keep this simple, say we only need to know the positions of the vertices.
we do not care about normals or UV co-ordinates for the time being however using
that data requires a small addition to the loops and the functions are very similar.

FBX uses "Control Points", "Polygons" and "Polygon Vertices"

![Control Points and Polygons](\img\dissertation\FBX-Importing-Control-Points.jpg){:width="960px"}

Vertices are an index of a control point.

___

| Polygon | Vertices |
| --- | :---: |
| 1 | 1, 3, 2 |
| 2 | 2, 3, 4 |
| 3 | 3, 5, 4 |
| 4 | 3, 6, 5 |
| 5 | 1, 6, 3 |

___

A simplified version of the code I use to copy the data is as follows:

* Loop through all polygons
* loop through polygon vertices

```
size_t pgCount = mesh->GetPolygonCount(); // Number of polygons
Vertex * vertices = new Vertex[pgCount * 3]; // Assume only triangles
controlPoints = mesh->GetControlPoints(); // list of position vectors

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

I then construct a mesh and initialize it with this vertex data which is ready to render!

# Resource Management

While working on this milestone I had to iterate my test meshes to fix rotations and scaling; Continually having to copy my textures and meshes into the build folder got very repetitive so after a quick google search and poking around in Preferences I discovered "Build Events". In Visual Studio, you can add commands that can be executed either before, during or after compiling, I added a little command that will copy both my resources and my DLLs into the build directory after the code compiles. This also means I won't forget to drag the updated files into the build folder after compiling leading to time sinks looking for non-existent bugs.

Simply adding the two commands `xcopy /Y /E "$(ProjectDir)res" "$(OutDir)res"` and `xcopy /Y /E "$(ProjectDir)dll\x86\Debug\*" "$(OutDir)"` to the post build events will copy the `\res\` folder and the contents of the `\dll\` folder into the build location. It will do this for both Debug _and_ release.

This little trick is a convenient QOL addition that can be used in all sorts of projects and i9 thought it would be cool to share it if any readers have not seen it before.

# PSA Check Function Parameters

I will finish with a short PSA. Do Not Forget when you change the parameters on a function! A change I made to the Render function on the Lit Shader was that instead of taking the World, View and Projection matrices, it now used the WorldViewProj, combined matrix; Inverse WorldViewProj and the World matrix by itself.

I made the mistake of reverting to the old setup while writing the new on-Render method, all three are still matrices however, understandably nothing was rendering as they completely the wrong things and I lost about 6 hours rewriting and debugging the import code before I found the cause of the issue.

# Thank you

Thank you very much for reading so far and I hope that this little guide will help you.

If you have any questions feel free to write up a comment or contact me at my email (Listed on my [GitHub Profile](https://github.com/Tezza48)).
