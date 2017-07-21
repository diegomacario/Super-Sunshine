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
The camera is specified with this command:
 ```sh
 camera fromx fromy fromz atx aty atz upx upy upz fovy
 ```
Where:
- *__from__* is the point at which the camera is located.
- *__at__* is the point that the camera points to.
- *__up__* is the vector that defines which way is up.
- *__fovy__* is the field of view in the Y direction.

In the animation below, the *__from__* point is rotated along a 45° arc while the *__at__* point remains fixed.

<p align="center">
<img src="https://github.com/diegomacario/Ray-Tracer/blob/master/readme_images/pyramid_on_mars.gif"/>
 <p align="center">
  <em>A pyramid on Mars.</em>
 </p>
</p>

### Geometry
Two geometric primitives are currently supported: *__spheres__* and *__triangles__*. Two doesn't sound like much, but remember you can make any shape with just triangles:

<p align="center">
<img src="https://github.com/diegomacario/Manta-Ray-Tracer/blob/master/readme_images/stanford_dragon.png"/>
 <p align="center">
  <em>This rendering of the Stanford Dragon is made out of 100 thousand triangles (scene description from CSE167x).</em>
 </p>
</p>

A sphere is created using this command:
 ```sh
 sphere centerx centery centerz radius
 ```
Where:
- *__center__* is the point at which the sphere is centered.
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
- *__tri__* is the command used to create a triangle. Its three parameters are the indices of three vertices. The first vertex you create with the *__vertex__* command has index zero, and this value increases by one for each subsequent vertex you create. Note that the indices must be specified in a counterclockwise order so that the normal of the triangle points in the correct direction, and that different triangles can share vertices.

### Transformations
Three basic transformations are currently supported: *__translations__*, *__rotations__* and *__scaling__*. The commands for these transformations are:
 ```sh
 translate x y z
 rotate x y z angle
 scale x y z
 ```
Where:
 - *__translate__* translates a geometric primitive *__x__*, *__y__* and *__z__* units along the X, Y and Z axes, respectively.
 - *__rotate__* rotates a geometric primitive counterclockwise by *__angle__* degrees about the vector defined by *__x__*, *__y__* and *__z__*.
 - *__scale__* scales a geometric primtive by *__x__*, *__y__* and *__z__* units along the X, Y and Z axes, respectively.

Just as in OpenGL, these transformations right multiply the model-view matrix. This means that the last transformation specified is the first one to be applied. For example, if you wanted to:

- Create a sphere of radius 1.5 centered at the origin.
- Scale it by a factor of 2. 
- Rotate it clockwise by 90 degrees about the Y axis.
- Translate it -5 units along the Z axis.

You would write the following:
 ```sh
 translate 0 0 -5
 rotate 0 1 0 -90
 scale 2 2 2
 sphere 0 0 0 1.5
 ```

This order might seem odd at first, but all you have to remember is that the last transformation specified is the first one to be applied. Also keep in mind that the order does matter: rotating and then translating is not the same as translating and then rotating.

Additionally, the commands *__pushTransform__* and *__popTransform__* are also supported to imitate the syntax of old-school OpenGL. To better understand their use and the order in which transformations are applied, consider the following example:
 ```sh
 translate 1 0 0
 
 pushTransform
    rotate 0 1 0 45
    sphere 0 0 0 1
 popTransform
 
 sphere 0 1 0 0.5
 ```
 - The sphere that is inside the push/pop block is centered at (0, 0, 0) and has a radius of 1. The first transformation applied to it is the nearest one moving towards the top. In this case it is a 45° counterclockwise rotation about the Y axis. The next transformation is a translation of 1 unit along the X axis.
 - The sphere that is outside the push/pop block is centered at (0, 1, 0) and has a radius of 0.5. Since it is not inside the push/pop block, the 45° counterclockwise rotation about the Y axis does not apply to it. Its first transformation then ends up being the translation of 1 unit along the X axis.
 
Transformations can be intimidating at first, but play around with them for a while and they will start to make sense!

### Lights
Three types of light sources are currently supported: *__ambient light__*, *__point lights__* and *__directional lights__*.

The command used to set the colour of the ambient light is:
 ```sh
 ambient r g b
 ```
Where:
- The *__RGB__* values, which can range from 0 to 1, define the colour of the ambient light. If this command is not specified in a scene description, the colour of the ambient light defaults to (0.2, 0.2, 0.2).

In theory, ambient light exists at all points in space and is propagated with equal intensity in all directions. Based on this definition, there should only exist one ambient light in a scene. While this is true for this ray-tracer, it still allows something rather unusual: the colour of the ambient light can be modified in between the creation of geometric primitives. In other words, users can use the ambient light to give primitives a base colour. Consider the following scene description, in which I create four spheres and modify the colour of the ambient light before creating each one:
 ```sh
 # Left sphere (green)
ambient 0.2 0.4 0.1
sphere -0.75 0 -2 0.5

# Right sphere (yellow)
ambient 0.7 0.7 0
sphere 0.75 0 -2 0.5

# Bottom sphere (red)
ambient 0.75 0 0
sphere 0 -0.75 -2 0.5

# Top sphere (blue)
ambient 0 0.262 0.344
sphere 0 0.75 -2 0.5
 ```

The resulting image is:
<p align="center">
<img src="https://github.com/diegomacario/Manta-Ray-Tracer/blob/master/readme_images/four_spheres_ambient.png"/>
 <p align="center">
  <em>One ambient light shining four different colours.</em>
 </p>
</p>

This behaviour does not match the real world, but it is very convenient in the context of a ray-tracer. Just remember that settings like the ambient light, the attenuation and the material properties are all maintained by a state machine. Once they are set, they affect all the lights and geometric primitives created afterwards. If you want different lights or primitives to have different appeareances, you need to modify these settings before creating them.

**(Connect flow here)** A point light is placed at a specific location and emits light in all directions. This type of light can be affected by three different types of attenuation: *__constant__*, *__linear__* and *__quadratic__* attenuation. The commands used to create this type of light are:
 ```sh
 attenuation constant linear quadratic
 point posx posy posz r g b
 ```
Where:
- *__attenuation__* is the command used to set the attenuation for point lights. A point light with no attenuation has a *__constant__* coefficient of 1, and *__linear__*/*__quadratic__* coefficients of 0 (these are the default values, just as in OpenGl).
- *__point__* is the command used to create a point light at point *__pos__*. The colour of the emitted light is defined by the *__RGB__* values.

### Materials

## Learning Resources

## Dedication

This last image is for all the citizens of Venezuela, who are currently fighting for their freedom against an oppressive dictatorship.

<p align="center">
<img src="https://github.com/diegomacario/Manta-Ray-Tracer/blob/master/readme_images/bandera.png"/>
</p>
