# Manta-Ray-Tracer

A recursive ray-tracer with a built-in scene parser for easily generating beautiful images.

## About

This ray-tracer was written as a final project for Ravi Ramamoorthi's fantastic [course](https://www.edx.org/course/computer-graphics-uc-san-diegox-cse167x-2) on computer graphics. It was built from scratch with a strong focus on making the C++ code clear and organized. In its final form, the project consists of:

- A recursive ray-tracer.
- A scene parser that can be used to generate images from text files.
- A linear algebra API for performing operations with points, vectors, normals and affine transformation matrices.

The only external library used is the [FreeImage](https://www.edx.org/course/computer-graphics-uc-san-diegox-cse167x-2) library (it is used to generate PNG images with the RGB values produced by the ray-tracer).

## Description

When I started working on this project, I thought the most challenging part of it would be to translate the theory behind a ray-tracer into well-written code. Once I finished it, however, I found that using a ray-tracer is a lot more challenging that actually implementing one! Just consider some of the things that need to be specified to generate an image:

- A camera (including its position, orientation and field of view).
- Geometric shapes (including their positions, dimensions and the transformations that act on them).
- Light sources (including their positions, directions, colours and attenuations).
- Material properties.

Keeping track of all these values can become quite complex, specially as the number of geometric shapes and light sources increases. On top of that, the whole process is slowed down significantly if you have to recompile your ray-tracer whenever you make changes to a scene. That is why it is essential to have a scene parser! With this tool a ray-tracer can take a scene description written with simple commands, such as this one:

And turn it into this:

A scene parser makes it a lot easier to play with a ray-tracer, and it also allows users to generate animations by writing scripts. It is hard to believe how such a simple feature can enable users to produce such stunning results:

## Features

All the commands supported by the scene parser follow this format:
 ```sh
 command parameter1 parameter2 ...
 ```
Where each parameter is separated by at least one space.

### Camera
The camera is specified as follows:
 ```sh
 camera fromx fromy fromz atx aty atz upx upy upz fovy
 ```
Where:
- *__from__* is the point at which the camera is located.
- *__at__* is the point that the camera point to.
- *__up__* is the vector that defines which way is up.
- *__fovy__* is the field of view in the Y direction.

In the animation below the *__from__* point is rotated along a 45° arc while leaving the *__at__* point fixed:

<p align="center">
<img src="https://github.com/diegomacario/Ray-Tracer/blob/master/readme_images/pyramid_on_mars.gif"/>
 <p align="center">
  <em>A pyramid on Mars.</em>
 </p>
</p>

### Geometry
The ray-tracer currently supports two geometric primitives: spheres and triangles. They don't sound like much, but remember you can make any shape with just triangles:

<p align="center">
<img src="https://github.com/diegomacario/Manta-Ray-Tracer/blob/master/readme_images/stanford_dragon.png"/>
 <p align="center">
  <em>This rendering of the Stanford Dragon is made up of 100 thousand triangles (scene description from CSE167x).</em>
 </p>
</p>

A sphere is created using this command:
 ```sh
 sphere centerx centery centerz radius
 ```
Where:
- *__center__* is the point at which the center of the sphere is located.
- *__radius__* is the radius of the sphere.

In the case of a triangle, it is created as follows:
 ```sh
 maxverts num
 vertex posx posy posz
 vertex posx posy posz
 vertex posx posy posz
 tri index1 index2 index3
 ```
Where:
- *__maxverts__* is the command used to define the maximum number of vertices (*__num__*) that can be used in a scene description.
- *__vertex__* is the command used to create a single vertex at point *__pos__*.
- *__tri__* is the command used to create a triangle. Its three parameters are the indices of three vertices. The first vertex you create with the *__vertex__* command has index zero, and this value increases by one for each subsequent vertex you create. Note that the indices must be specified in a counter-clockwise order so that the normal of the triangle points in the correct direction, and that multiple triangles can share vertices.

### Transformations

### Lights

### Materials

## Learning Resources
