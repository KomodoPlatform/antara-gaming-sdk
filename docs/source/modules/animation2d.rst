:orphan:

antara::animation2d module documentation
========================================

How does the 2D animation system work ?
---------------------------------------

The animation system works in a very simple way based on **spritesheet** with fixed offset.

You just need to specify the number of column and row in your **spritesheet** and the algorithm will automatically detect the size of a frame.

You can then select the entire **spritesheet** as a single animation and name it or on the contrary take several parts to cut them into different animations.

Once your animation is loaded in memory thanks to the animation system, you can attach animation components to your entities by indicating the id of the animation that you want to attach.

The animation system loop will update your animation at the speed you have chosen when assigning the component.

Tutorial Related to Animations
------------------------------

:doc:`Click here to access animations tutorials<../tutorials/animation2d>`