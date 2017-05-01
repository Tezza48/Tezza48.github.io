---
layout: post
title: "Textured Sponza Render"
date: "2017-04-11 00:00:00 +0100"
categories: Dissertation
comments: true
---

# Textured Sponza Render

In it's current state, the Rendering engine can take in an FBX ans use it's Mesh data and load either 24 or 32 bit TARGA files to be used as textures. at this point I want to finally get a textured render of the Sponza scene running and the only thing stopping that happening happening is not being able to get texture data fmrom my FBX file so for this installment i decided to take take the long and tedeous route of separating the meshes and importing all the parts one by one.

Originally I had intended on working with the FBX sdk to figure out how it's materials and textures work, however after a few long hours wasted and going round in circles I had been getting nowhere.

The solution I eventually landed on involves the following process. (Sadly I did not get any screenshots during the process)

Firstly, I matched up the meshes in the Max file with the textures they are currently using, i did this by dragging on the texture i thought was correct and using trial and error, then adding the texture name to the end of the object name. In my knowledge of 3DS Max, there is no way to populate the _Material Editor_ with materials in the scene so this was the only way I knew how. It was an arduous task.

Secondly, I attatched all of the objects that were using the same texture together. This was easy to do using _Select by Name_ in max and selecting all of the objects with the same texture suffix that I set in the first step, then attatching them in the _Editable Poly: Modify_ tab.

Finally I exported all of the meshes, that had been joined by texture, as FBX files with the texture name as the Suffix.

![Exported Meshes](\img\dissertation\Textured-Sponza-exported-files.PNG)

		 * exported all indivisual meshes and named them according to the texture they use

After all of this, all that was left wasadding the various lines of code to the start function of the program to load the meshes and their textures into their arrays (in the same order) and then adjust the rendering code to change the texture inside the for loop. Of course not forgetting to place the all important camera at a cool and jaunty angle and we are Finally ready to get a Render (or two).

## Renders

![Daytime Render](\img\dissertation\Textured-Sponza-daytime.PNG){:width="960px"}

![Nighttime Render](\img\dissertation\Textured-Sponza-nighttime.PNG){:width="960px"}

---

## Important Lessons

A major discovery i have made is why people check that a pointer is not NULL before deleting, if for some resason a destructor has been called before an object has been fully initialized, there is a chance that some pointers may be still uninitialized, throwing an error. I will at some point go through and add these checks to all of my classes just in case.
```
if(pObject)
{
	delete pObject;
	pObject = nullptr;
}
```

Additionally i changed `mTexture` to be an array of textures, in the future, i may use either a list or dictionary and have texture index referanced in the materials however for this render i only needed the simple array.

Finally, I removed the need for my axis conversion function and the backwards polygon reading inside of the `LoadFBX` method by using FBX's `FbxAxisSystem::DirectX.ConvertScene(scene)` function. This tells FBX to read all of the axes using the 'y up', right hand axis system as opposed to whatever system it has been exported with. (This does make me wonder why when exporting to FBX into Unity3D, that does not perform the same conversion as i have always had to rotate my mesh's pivots.)
