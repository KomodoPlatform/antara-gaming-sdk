Tutorial: How to do a tic-tac-toe in less than 15 minutes with the gaming SDK ?
===============================================================================

If you have not read the :doc:`getting started<getting_started>` part yet, I invite you to do it now for the rest of this tutorial.

This tutorial is divided into several steps, to help you to follow more simply.

Step 0: setup the executable and the window
-------------------------------------------

For this first step we need a ``CMakeLists.txt`` to be able to create our executable and compile it.

In this ``CMakeLists.txt`` file we will have: the name of the project, the creation of the executable, the link with the SDK, the C++ standard that will be used and the extras modules that we want to use, in our case it will be the module  :doc:`antara::sfml<../modules/sfml>` provided by the **SDK**.

Below is the ``CMakeLists.txt`` file:

.. literalinclude:: ../../../tutorials/tic-tac-toe/step_0/CMakeLists.txt
   :language: cmake

Then we create our input file for the application and call it tic-tac-toe.cpp.

We add an empty main and a return value:

.. code-block:: cpp

    int main()
    {
        return 0;
    }

If you have followed well until then you should have the following tree:

.. code-block:: bash

    .
    ├── CMakeLists.txt
    └── tic-tac-toe.cpp

Before continuing the tutorial check that your program compiles by referring to the build commands available in the tutorial :doc:`getting started<getting_started>`.

Now we are going to need a world representing the world of our game, to do this we are going to need the following header file: ``#include <antara/gaming/world/world.app.hpp>``

And a basic structure that we name ``tic_tac_toe world`` that will inherit from an ``antara::gaming::world::app``.

As well as a using namespace ``antara::gaming`` to make naming easier.

Finally, we declare our new object in the body of the main function and we replace the return value with the return value of our game returned by the ``run`` function of the ``class world::app``

Which gives us the following result:

.. code-block:: cpp

    #include <antara/gaming/world/world.hpp>

    using namespace antara::gaming;

    struct tic_tac_toe_world : world::app
    {
        //! Our game entry point
        tic_tac_toe_world() noexcept = default;
    };

    int main()
    {
        tic_tac_toe_world game;
        return game.run();
    }

If you compile now and start your executable you have an infinite loop and nothing that happens.

The last stage of this first step is to add the graphic side of the application, for that we will need 2 modules: ``antara::gaming::sfml::graphic_system`` and ``antara::gaming::sfml::input::system``.
Who have the following headers, respectively: ``#include <antara/gaming/sfml/graphic.system.hpp>`` and ``#include <antara/gaming/sfml/input.system.hpp>``.

Now in the body of the constructor of our class tic_tac_toe_world we will load the graphic system, then the input system with the window coming from the graphic system.

Which gives us the following result:

.. literalinclude:: ../../../tutorials/tic-tac-toe/step_0/tic-tac-toe.cpp
   :language: cpp

If you compile now you should see a black window open that you can close by pressing the cross:

.. image:: ../../assets/black_window.png

And now, the first step is over. The objectives have been reached: to have a window that opens and can be closed, a basic executable and a CMakeLists.txt to be able to compile our program.

