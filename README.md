# Ray-Tracer

A recursive ray-tracer with a built-in scene parser for easily generating beautiful images.

<p align="center">
  <img src="https://github.com/diegomacario/Ray-Tracer/blob/master/readme_images/pyramid_on_mars.gif"/>
</p>

## About

This ray-tracer was written as a final project for Ravi Ramamoorthi's fantastic [course](https://www.edx.org/course/computer-graphics-uc-san-diegox-cse167x-2) on computer graphics. It was built from scratch with a strong focus on making the C++ code clear and organized.

The only external library used is the [FreeImage Library](https://www.edx.org/course/computer-graphics-uc-san-diegox-cse167x-2).

## Description

When I started working on this project, I thought the most challenging aspect of it would be to translate the theory behind a ray-tracer into well-written code. Once I finished it, however, I found that using a ray-tracer is a lot more challenging that actually implementing one! Just consider some of the things that need to be specified to generate an image:

- A camera (including its position, orientation and field of view).
- Geometric shapes (including their positions, dimensions and the transformations that act on them).
- Light sources (including their positions, directions, colours and attenuations).
- Material properties.

Keeping track of all these values can become quite complex, specially as the number of geometric shapes and light sources increases. On top of that, the whole process is slowed down significantly if you have to recompile your ray-tracer whenever you make changes to a scene. That is why it is essential to have a scene parser! With this tool a ray-tracer can take a scene description written with simple commands, such as this one:

And turn it into this:

A scene parser makes it a lot easier to play with a ray-tracer, and it also allows users to generate animations by writing scripts. It is hard to believe how such a simple feature can enable users to produce such stunning results:

## Learning Resources
