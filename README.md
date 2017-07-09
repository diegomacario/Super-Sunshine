<!---
Image of beautiful header
--->

# Ray-Tracer

A recursive ray-tracer with a built-in scene parser for easily generating beautiful images.

<!---
<p align="center">
  <img src="https://github.com/diegomacario/Ray-Tracer/blob/master/readme_images/flower.gif"/>
  <p align="center">
    <em>You can make lots of cool things by squashing a few spheres and putting them together!</em>
  </p>
</p>
--->

## About

This ray-tracer was written as a final project for Ravi Ramamoorthi's fantastic [course](https://www.edx.org/course/computer-graphics-uc-san-diegox-cse167x-2) on computer graphics. It was built from scratch with a strong focus on making the C++ code clear and organized.

The only external library used is the [FreeImage Library](https://www.edx.org/course/computer-graphics-uc-san-diegox-cse167x-2).

## Description

Let's consider the things that one needs to specify to generate an image with a ray-tracer:

- The geometric shapes. This includes their locations, sizes and the transformations that act on them.
- The light sources. This includes their locations, directions, colours and attenuations.
- The material properties. These control the colour of the geometric shapes, their shininess and how they respond to the light sources.

Keeping track of all these values can become quite complex, specially as the number of geometric shapes and light sources increases. On top of that, the whole process is slowed down significantly if you have to recompile your ray-tracer whenever you make changes to a scene.

To solve these issues, this project focused on the development of a ray-tracer with a built-in scene parser. The scene parser simplifies the process of generating an image. The entire process can be summarized as follows:

- Describe a scene in a text file using simple commands.
- Feed the file into the ray-tracer by specifying its name in the command-line.
- The scene parser reads the file and feeds all the relevant values into the ray-tracer
- The ray-tracer generates an image and spits it out

When this is done, the scene parser reads the file and feeds all the relevant values into the ray-tracer. The ray-tracer then uses those values to generate an image and spits it out. In its final form, this project can take a scene description such as this:

*** Scene Description ***

And use it to generate this image:

*** Image ***

The fact that a scene can be described in a text file also makes it easy to write scripts that can generate animations, such as the ones presented above.

## Features
