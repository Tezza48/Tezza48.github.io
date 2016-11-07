---
layout: post
title: "Final Project Proposal"
date: "2016-10-31 22:12:57 +0000"
---

___

_This is a working draft of my dissertation proposal, I realise it would be useful to have a formal introduction to this project, as such, I'm posting it._

___

## Designing and Programming a DirectX 12 Rendering Engine in C++

### Brief outline of work

Over the course of this final project it is my goal to learn the DirectX 11 API (DX11) then, with this knowledge and my experience with C++, create a Rendering engine suitable for use in a game engine. DirectX is available only on the Windows and Xbox platforms, as I own an Xbox One, it seems fitting that I should use DirectX as this will allow me to test the engine in a console environment. I am confident in using C++ and would like to use this project as an opportunity to enhance my skills with it by iterating and optimising my code as I learn more features of the language.


### Methodology

To keep the project in an appropriate scope, I have devised three tiers of features I would like to implement, ranging from:

 - Core essential features that are the minimum expectation of the project.
 - Recommended features that would improve the product, yet not damage if not implemented.
 - Advanced features that will be either “quality of life” or more complex capabilities such as improved shaders or better portability.

As a minimum I will be able to create a render of the Sponza scene (Dabrovic, 2002) using a diffuse shader and the provided diffuse map. Directional lights that cast shadows will be used to light the scene and I will be able to set an ambient light source. Finally, I will be importing meshes using the FBX file format (Autodesk.com, 2016), the textures will be imported as TGAs and I will import light information from a text file to avoid hard coding them.

The additional features for the next tier I would like to implement are as follows:

 - An improved shader that will make use of normal and Specular data using the respective maps, this functionality will bring the shading up to the previous generation standard that I have been using in my 3D Modelling throughout the last two years.
 - To add point lights to the engine to allow a greater depth of lighting possibilities
 - Adding to this I would like to transition from using text files to importing the lights along with meshes using FBX’s light exporting capability. It will also be useful to implement a scene graph system allowing me to both load different scenes in and out and transform objects in engine.
 - Lastly I would like to be able to move the camera around the scene either to predetermined locations or with user input.


Finally, should I manage to successfully implement all of the recommended features above, I will consider adding one or two of these “Pie in the Sky” features.

 - Adding OpenGL or Vulcan support to the engine will allow it to run on non-windows systems such as Mac/Linux, this portability will make the engine available on nearly every platform. Similarly, using the newer but lower level DirectX 12 API (DX12) would allow me to learn more about the interaction between the CPU and GPU.
 - For ease of use it would be nice to be able to use more file types for assets so I would like to be able to import JPEG, PNG and BMP.
 -  For meshes it would be nice to have the ability to import OBJ/Wavefront files as this is another common file format used.


I would like to further add to the shading capabilities by implementing a Physically Based Rendering (PBR) workflow, this is the newest industry standard for rendering and game engines. Having this will bring the visuals I can produce closer to the current generation of game rendering engines such as Unity and UE4.

Most rendering engines provide the user with tools that can let them edit scenes by adding or removing objects, I would like to implement this functionality. In addition, I would like to be able to save these scenes out to either a text or a binary file that can be loaded as and when it’s needed. This sort of functionality will mean that I will no longer rely on using 3D modelling software to create scenes.

Finally, I would like to add a basic animation system, allowing objects to be transformed without relying on hard coding their movements. This will allow me to easily move the camera around the scene and create interesting compositions.


## Rationale

I have spent time looking through Programming vacancies in Game studios and I have noticed a theme of companies either asking for experience in using C++ or knowledge of 3D maths. For this reason I decided taking on a project involving C++ and low level libraries for rendering at its core, I feel this will greatly improve my employability after graduation.

Additionally, the advanced features I have planned consist mostly of portability and quality features. The hope being that, in the future, I will be able to use this rendering engine as a framework similar to Monogame to create my own games with and further contribute to, with the possibility of it becoming a flexible yet powerful Game Engine that I can add to my portfolio.

Initially it was my plan to use DX12 as the starting API, however, after trying my best to understand it, I currently have very little experience with graphics programming and it is more sensible for me to learn the higher level and more established DX11.

Regarding the job vacancies, I have listed, I searched through job openings at major companies that I saw as being where I would like to see my career going in the future. Two major points I took away from these were that primarily the applicants were required to know a variety of programming languages including C++ among various others that I already have experience with; secondly many of these ask for experience with rendering APIs and some requiring having programmed rendering/game engines.

___

## References

Autodesk.com. (2016). FBX - Overview. [online] Available at: http://www.autodesk.com/products/fbx/overview [Accessed 11 Oct. 2016].

I will be using the FBX file format to import mesh data to the engine.

Dabrovic, M. (2002). Sponza. [3D Model].
Marko Dabrovic is the creator of the Sponza 3D model that I will be using as a test bench for my engine.

Foster, G. (n.d.). Understanding and Implementing Scene Graphs. [online] Archive.gamedev.net. Available at: http://archive.gamedev.net/archive/reference/programming/features/scenegraph/index.html [Accessed 11 Oct. 2016].
This article demonstrates how a scene graph works in a 3D environment, I will use this as a starting point when designing the one I will use.

Lippman, S., Lajoie, J. and Moo, B. (2013). C++ primer. Upper Saddle River, NJ: Addison-Wesley.
Luna, F. (n.d.). Introduction to 3D game programming with DirectX® 12.

McGuire, (2011). Meshes. [online] Computer Graphics Archive. Available at: http://graphics.cs.williams.edu/data/meshes.xml [Accessed 5 Oct. 2016].
McGuire has provided the download of Dabrovic’s Sponza scene and has also included his own normal maps which will prove useful.

Msdn.microsoft.com. (2016). Getting Started with Direct3D (Windows). [online] Available at: https://msdn.microsoft.com/en-us/library/windows/desktop/hh769064(v=vs.85).aspx#What_is_D3D [Accessed 11 Oct. 2016].

Paulbourke.net. (n.d.). Object Files (.obj). [online] Available at: http://paulbourke.net/dataformats/obj/ [Accessed 11 Oct. 2016].
This page provides an in depth look at the OBJ format, it is a very simple and readable format so I should have no problem writing a class to load them.

Russell, J. (2015). Basic Theory of Physically-Based Rendering. [online] Marmoset. Available at: https://www.marmoset.co/toolbag/learn/pbr-theory [Accessed 5 Oct. 2016].
Russell provides a detailed look at PBR and explains how each map type affects the overall material.

Stroustrup, B. (2014). Programming.

Weinman, B. (2014). C++ Essential Training. [online] Lynda.com - from LinkedIn. Available at: https://www.lynda.com/C-tutorials/C-Essential-Training/182674-2.html [Accessed 13 Oct. 2016].
I went through this course to learn the basics of C++.

___

## Job Vacancies

Programming - Quebec City, Quebec. [online] Available at: https://activision.jibeapply.com/activision/jobs/PRO0001DP/Quebec-City-Quebec-Programmers-ANI00007L?lang=en-US [Accessed 24 Oct. 2016].

GamesIndustry.biz. (2016). Core Tech Programmer – PC and Console Games - UK. [online] Available at: http://www.gamesindustry.biz/jobs/opm/uk-and-europe/core-tech-programmer-andndash--pc-and-console-games-uk-id87739 [Accessed 23 Oct. 2016].

GamesIndustry.biz. (2016). Graduate Programmer – PC and Console Games Studio - UK. [online] Available at: http://www.gamesindustry.biz/jobs/opm/uk-and-europe/graduate-programmer-andndash--pc-and-console-games-studio-uk-id87752 [Accessed 23 Oct. 2016].

GamesIndustry.biz. (2016). Graphics Engine Programmer – PC and Console Games - UK. [online] Available at: http://www.gamesindustry.biz/jobs/opm/uk-and-europe/graphics-engine-programmer-andndash--pc-and-console-games-uk-id87737 [Accessed 23 Oct. 2016].

Guerrilla-games.com. (2016). Tools Programmer. [online] Available at: https://www.guerrilla-games.com/join/senior-tools-programmer [Accessed 24 Oct. 2016].

Jobs.zenimax.com. (2016). ZeniMax Careers. [online] Available at: https://jobs.zenimax.com/requisitions/view/76 [Accessed 24 Oct. 2016].

Jobs.zenimax.com. (2016). ZeniMax Careers. [online] Available at: https://jobs.zenimax.com/requisitions/view/907 [Accessed 24 Oct. 2016].

___

_I may make this post a standalone page once it has been finalised and submitted, along with the milestones (which will be posted shortly.)_
