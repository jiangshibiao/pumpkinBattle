# pumpkinBattle

[A detailed Chinese report for this project](https://github.com/jiangshibiao/pumpkinBattle/blob/master/ChineseReport/report.md)

## Project Introduction
+ This is the final project for my Computer Graphics course in Zhejiang University.
+ It's an **interesting game** designed by C++ and OpenGL. The Player are surrounded by walls in the dark, feeling the fear of **Pumpkin Monster** approaching. Just run and jump between the walls! Blow up the walls and kill all Pumpkin Monsters for surviving!

## How to play?
+ Build up a new VS project and add files from [Resources](https://github.com/jiangshibiao/pumpkinBattle/tree/master/Resources) 
+ Use the codes in [pumpkinBattle](https://github.com/jiangshibiao/pumpkinBattle/tree/master/pumpkinBattle) to play!

## Player Instructions
+ Use the *space* to jump (You can only jump twice at a time) and use *WASD* to move.
+ Use the *mouse* to control your perspective. 
+ There are several **pumpkin** around you when you start the game. Each pumpkin will wander ramdomly, but the closer you near him, the more possibility that he walks to you. 
+ There's a **grenade sign** when you get close to the wall. Click down the *right button* to blow up random pieces of wall.
+ After you click down the *left button*, you can shoot a **bullet**. Each pumpkin will disapper after being hit 5 times.
+ You can not move if caught by the moster. Then please shoot him as quickly as possible and  press the *enter* to reborn.
+ Press the *P* on the keyboard to get a screenshot.

## Brief Technology Introduction
+ Shadow
	- Know the principle of shadow and light
	- Control the dynamic light
	- Control the real-time shadow according to the light
+ Collision
	- Use the knowledge of 3D-geometry to describe the collision.
	- Implement it in many situations: bullet and pumpkin monsters, explosive and walls, people and bricks...
+ Models
	- Master the structure of the model
	- Import some models from the Internet
+ Texture
	- Build the texture by programs
	- Use the textures from the Internet and attach them on the objects
	- Use the skybox
+ Others
	- Print real-time words on the screen
	- Gravity system

## Some Screenshots
+ Game interface
![](https://github.com/jiangshibiao/pumpkinBattle/blob/master/ChineseReport/screenshot0.bmp)
+ The sky
![](https://github.com/jiangshibiao/pumpkinBattle/blob/master/ChineseReport/skybox1.png)
+ Walls, Props and Shadow
![](https://github.com/jiangshibiao/pumpkinBattle/blob/master/ChineseReport/planet.png)
+ After the contruction of walls
![](https://github.com/jiangshibiao/pumpkinBattle/blob/master/ChineseReport/screenshot1.bmp)
+ The flying bullet
![](https://github.com/jiangshibiao/pumpkinBattle/blob/master/ChineseReport/bullet.bmp)
+ The pumpkin monsters
![](https://github.com/jiangshibiao/pumpkinBattle/blob/master/ChineseReport/fkwyh2.bmp)
+ When the pumpkin monster is hit...
![](https://github.com/jiangshibiao/pumpkinBattle/blob/master/ChineseReport/shot1.bmp)
![](https://github.com/jiangshibiao/pumpkinBattle/blob/master/ChineseReport/shot0.bmp)
