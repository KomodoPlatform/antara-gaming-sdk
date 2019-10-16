Tutorial: How to do a tic-tac-toe in less than 15 minutes with the gaming SDK ?
===============================================================================

This tutorial is divided into several steps, to help you to follow more simply.

Step 0: setup the executable and the window
-------------------------------------------

For this first step we need a CMakeLists.txt to be able to create our executable and compile it.

In this CMakeLists.txt file we will have: the name of the project, the creation of the executable, the link with the SDK, the C++ standard that will be used and the extras modules that we want to use, in our case it will be the module **SFML** provided by the **SDK**.

Below is the CMakeLists.txt file:

.. literalinclude:: ../../../tutorials/tic-tac-toe/step_0/CMakeLists.txt
   :language: cmake

