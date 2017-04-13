---
layout: post
title: "Targa: 24 bit support"
date: "2017-04-10 18:38:34 +0100"
---

# Targa: 24 bit Texture Support

At this point in the project i really just want to have a render of the Sponza scene with all of it's txtures applied as it's now close to the end of the project and i am getting excited to see the whole thing come together

I ran into a problem with the provided textures which were all in JPEG format, which the engine cannot import, and i could only get GIMP to esport to TARGA as 24 bit which was not supported. I chose the sensible choice and added the support for using 24 bit Targas.

## The method

Initially i had to convert the textures to .tga in GIMP which took far longer than i had anticipated, were it in Photoshop i could have set up a simple macro however i rarely use GIMP so that was not sopething i know how to do so i had to do it the old fashioned way and manually export each file.

Then i started on modifying the importer in my content manager. The first port of call was removing the if statement that threw an error if the texture's bit depth was not 32, i replaced this with a switch block and placed the existing colour revering loop under the case: 32.

To make the data usable as a texture in DirectX i had to add alpha data to the texture, therefore inserting a new byte after every three existing ones. To do this i created a new array of `char`s of size width * height * 4, which i fill up using the data from the file, simultaneously swapping the blue and red channels as i do with the 32 bit files.

this ended up being very simple to do as i just iterate through a variable for the index in the 24 bit texture and the 32 bit texture at the same time adding 3 or 4 respectively, realizing i could do this in a for loop was helpful and i am sure i will be able to use it elsewhere in similar situations. i then just fill in the new larger array using the 3 existing bytes and setting the 4th byte to 255, completely opaque.

The loop is as follows:

```
for (size_t tex24 = 0, tex32 = 0;
    tex24 < width * height * 3;
    tex24+=3, tex32+=4)
{
    imgData[tex32 + 0] = rawData[tex24 + 2];
    imgData[tex32 + 1] = rawData[tex24 + 1];
    imgData[tex32 + 2] = rawData[tex24 + 0];
    imgData[tex32 + 3] = (char)255;
}
```

## Side note Bugfix

I was running into an error that meant that if textures were a certain size then the width or height would end up being negative, this was due to the header not being unsigned and me negating the low bytes. To fix this i cast the individual parts to `unsigned chars` before performing the bitwise maths, this uses up more lines and involves 4 temporary variables but it makes it a lot clearer what is happening and fixes the bug.

```
unsigned char widthLo = buffer[12];
unsigned char widthHi = buffer[13];
unsigned char heightLo = buffer[14];
unsigned char heightHi = buffer[15];

/* unsigned chars */

header.width = widthHi << 8 | widthLo;
header.height = heightHi << 8 | heightLo;
```
