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

Step 1: The Game Scene, The Grid, Game constants
------------------------------------------------

For this second step our goal is to draw the grid of the tick toe.

The grid will look like this:

.. image:: ../../assets/tictactoe.grid.jpg

To do this, we will create a game scene thanks to the scene manager, so in order we will include the header file ``#include <antara/gaming/scenes/scene.manager.hpp>`` and load the scenes manager system into the system manager.

.. code-block:: cpp

    struct tic_tac_toe_world : world::app
    {
        //! Our game entry point
        tic_tac_toe_world() noexcept
        {
            //! Here we load our graphical system
            auto &graphic_system = system_manager_.create_system<sfml::graphic_system>();

            //! Here we load our input system with the window from the graphical system
            system_manager_.create_system<sfml::input_system>(graphic_system.get_window());

            //! Here we load the scenes manager
            auto &scene_manager = system_manager_.create_system<scenes::manager>();
        }
    };


Now we are going to create the game_scene class that inherits from the base_scene class. This class will be the entry point of our game scene.

The concrete class must override several functions such as updade, scene_name, and the destructor.
We will not use the update function because the tictactoe is not a game that needs an update for each frame, so we will leave the function empty.
For the scene_name function we'll just return the name of the scene.

.. code-block:: cpp

    class game_scene final : public scenes::base_scene
    {
    public:
        game_scene(entt::registry &entity_registry) noexcept : base_scene(entity_registry)
        {}

        //! This function will not be used, because tic tac toe doesn't need an update every frame.
        void update() noexcept final
        {}

        //! our scene name
        std::string scene_name() noexcept final
        {
            return "game_scene";
        }

        ~game_scene() noexcept final
        {}
    private:
    };

Now we are going to load our game scene into the scene_manager using the change_scene member function

.. code-block:: cpp

    struct tic_tac_toe_world : world::app
    {
        //! Our game entry point
        tic_tac_toe_world() noexcept
        {
            //! Here we load our graphical system
            auto &graphic_system = system_manager_.create_system<sfml::graphic_system>();

            //! Here we load our input system with the window from the graphical system
            system_manager_.create_system<sfml::input_system>(graphic_system.get_window());

            //! Here we load the scenes manager
            auto &scene_manager = system_manager_.create_system<scenes::manager>();

            //! Here we change the current_scene to "game_scene" by pushing it.
            scene_manager.change_scene(std::make_unique<game_scene>(entity_registry_), true);
        }
    };

If you compile now you should still see the black window from the previous step, but we are now in our game scene.

.. image:: ../../assets/black_window.png

.. note::

    The scene system is very handy when you want to organize your game with different screens, **introduction scene**, **game scene**, **end-of-game scene**, etc.

Now we are going to need several constant data essential for the tick-to-toe, the size of a cell in width, in height, the number of cells per line, the thickness of our grid.

For the size of the cells we will use the current size of our canvas divided by the number of cells per line to obtain the size of a cell.

So we create a structure tic_tac_toe_constants that will contain these different information.

Then we save it in the entity registry to be able to access from anywhere in the program.

.. code-block:: cpp

    struct tic_tac_toe_constants
    {
        tic_tac_toe_constants(std::size_t nb_cells_per_row_, std::size_t width_, std::size_t height_) noexcept :
                nb_cells_per_row(nb_cells_per_row_),
                cell_width(width_ / nb_cells_per_row),
                cell_height(height_ / nb_cells_per_row)
        {
        }

        const std::size_t nb_cells_per_row;
        const std::size_t cell_width;
        const std::size_t cell_height;
        const float grid_thickness{20.0f};
    };

In the constructor of the gaming scene:

.. code-block:: cpp

    game_scene(entt::registry &entity_registry) noexcept : base_scene(entity_registry)
    {
        //! Here we retrieve canvas information
        auto[canvas_width, canvas_height] = entity_registry_.ctx<graphics::canvas_2d>().canvas.size.to<math::vec2u>();

        //! Here i set the constants that will be used in the program
        entity_registry_.set<tic_tac_toe_constants>(3ull, canvas_width, canvas_height);
    }