---
layout: post
title: "Tidying Things Up"
date: "2017-05-05 03:16:20 +0100"
categoty: Dissertation
comments: true;
---

The last few commits started out as the beginning of creating the scene manager, however as I began to move code around i realised that my D3D11App class was far too complicated, it's purpose was originally only to deal with DirectX matters but as time has passed it slowly turned into what can only be described as spaghetti code. Thereforei decided it would be a far better decision to _tidy up_ the code and try to keep all of my classes only handling  what they should and having them make sense from an OOP perspective.

An important change I have made is that i have renamed my `D3D11App` class to `D3D11Graphics`and this class now deals only with initializing D3D and Drawing. All of the remaining functionality that `D3D11App` had has been moved to the new `NoobieEngine` class (Noobie being the name I have settled on calling the Engine) and the Win32 code like the window creation and message handling is now Dealt with in the `Source.cpp` file with the entry point of the program.

My plan with this change is to not only make it easier for the programmer to use the engine when they add their own content to render but to also allow me to easily implement different rendering APIs like DirectX 12 or OpenGL as per my _Advanced_ features list, where I would only have to add a new Graphics class and slightly modify the `NoobyEngine` class to accomodate the variations different APIs will bring without changing the workflow os actually programming the engine. In it's current state, if the user wishes to add meshes, move objects or in any way modify the scene, all of this can be done within this new class without having to go anywhere near unrelated code.
