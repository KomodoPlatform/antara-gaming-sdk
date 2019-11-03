Tutorial: Flappy Bird
=====================

If you have not read the :doc:`getting started<getting_started>` part yet, Please read it before reading this one.

This tutorial is divided into multiple steps to make it easier to follow.

Step 1: Setup the executable, window and the game scene
-------------------------------------------------------

First, create a folder called ``flappy-bird`` for your project, and create another folder called ``assets`` inside. Inside ``assets`` folder. Make another folder inside named ``textures`` and place ``player.png`` inside. Then create one more folder called ``fonts`` and add ``sansation.ttf`` inside.

You should have the following tree:

.. code-block:: bash

    ./flappy-bird
    └── assets
        ├── fonts
        │   └── sansation.ttf
        └── textures
            └── player.png

Then create a text file and save it as ``CMakeLists.txt``.

In this ``CMakeLists.txt`` file we will have: name of the project, creation of the executable, link with the SDK, moving of the assets, C++ standard that will be used and extra modules that we want to use.

Below is the ``CMakeLists.txt`` file:

.. literalinclude:: ../../../tutorials/flappy-bird/step_1/CMakeLists.txt
   :language: cmake

Then we create our input file for the application and call it ``flappy-bird.cpp``.

We add an empty main function:

.. code-block:: cpp

    int main() {
        return 0;
    }

If you did everything correctly so far, you should have the following tree:

.. code-block:: bash

    ./flappy-bird
    ├── assets
    │   ├── fonts
    │   │   └── sansation.ttf
    │   └── textures
    │       └── player.png
    ├── CMakeLists.txt
    └── flappy-bird.cpp

Before continuing the tutorial, make sure that you have installed the required dependencies and your program compiles with the build commands available in the tutorial :doc:`getting started<getting_started>`.

Now we need a world representing the world of our game, to do this we use the following header file: ``#include <antara/gaming/world/world.app.hpp>``

And a basic structure that we name ``flappy_bird_world``. It will inherit from ``antara::gaming::world::app`` class.

And use the namespace ``antara::gaming`` and ``std::string_literals`` to make things easier. 

Finally, we declare our new object in the body of the main function and we replace the return value with the return value of our game returned by the ``run`` function of the ``class world::app``.

It gives us the following result:

.. code-block:: cpp

    #include <antara/gaming/world/world.hpp>

    // For convenience
    using namespace antara::gaming;
    using namespace std::string_literals;

    struct flappy_bird_world : world::app {
        // Game entry point
        flappy_bird_world() noexcept = default;
    };

    int main() {
        // Declare the world
        flappy_bird_world game;

        // Run the game
        return game.run();
    }

If you compile now and run your executable, you have an infinite loop and nothing will happen.

The last stage of this step one is to add the graphics side of the application, for that we will need two modules: ``antara::gaming::sfml::graphic_system`` and ``antara::gaming::sfml::input::system`` which have these following headers, respectively: ``#include <antara/gaming/sfml/graphic.system.hpp>`` and ``#include <antara/gaming/sfml/input.system.hpp>``.

Now in the body of the constructor of our class ``flappy_bird_world`` we will load the graphic system, then we will initialize input system with the window coming from the loaded graphic system.

.. code-block:: cpp

    // Game entry point
    flappy_bird_world() noexcept {
        // Load the graphical system
        auto &graphic_system = system_manager_.create_system<sfml::graphic_system>();

        // Load the input system with the window from the graphical system
        system_manager_.create_system<sfml::input_system>(graphic_system.get_window());
    }

If you compile and run now, you should see a black window open. You can close by pressing the close button of the window:

.. image:: ../../assets/black_window.png

And now, the setup part is over. We have a ``CMakeLists.txt`` to be able to compile our program into a basic executable which can create the game window.

We will create a game scene using the scene manager. In order to do so we need to include the header file ``#include <antara/gaming/scenes/scene.manager.hpp>`` and load the scenes manager system into the system manager.

.. code-block:: cpp

    // Game entry point
    struct flappy_bird_world : world::app {
        //! Our game entry point
        flappy_bird_world() noexcept {
            // Load the graphical system
            auto &graphic_system = system_manager_.create_system<sfml::graphic_system>();

            // Load the input system with the window from the graphical system
            system_manager_.create_system<sfml::input_system>(graphic_system.get_window());

            // Load the scenes manager
            auto &scene_manager = system_manager_.create_system<scenes::manager>();
        }
    };


Now we are going to create the ``game_scene`` class that inherits from the ``base_scene`` class. This class will be the entry point of our game scene.

The concrete class must override several functions such as update, ``scene_name``.
Flappy Bird is a game that needs an update for each tick, so we will fill the update function later.
For the ``scene_name`` function we'll just return the name of the scene.

.. code-block:: cpp

    // Game Scene
    class game_scene final : public scenes::base_scene {
    public:
        game_scene(entt::registry &registry) noexcept : base_scene(registry) {
        }

        // Scene name
        std::string scene_name() noexcept final {
            return "game_scene";
        }

    private:
        // Update the game every tick
        void update() noexcept final {
        }
    };

Now we are going to load our game scene into the ``scene_manager`` using the ``change_scene`` member function

.. code-block:: cpp

    struct flappy_bird_world : world::app
    {
        //! Our game entry point
        flappy_bird_world() noexcept
        {
            //! Load the graphical system
            auto &graphic_system = system_manager_.create_system<sfml::graphic_system>();

            //! Load the input system with the window from the graphical system
            system_manager_.create_system<sfml::input_system>(graphic_system.get_window());

            //! Load the scenes manager
            auto &scene_manager = system_manager_.create_system<scenes::manager>();

            // Change the current_scene to "game_scene" by pushing it.
            scene_manager.change_scene(std::make_unique<game_scene>(entity_registry_), true);
        }
    };

We will also use a sprite for the bird, so we need the ``sfml::resources_system``. In order to do so we need to include the header file ``#include <antara/gaming/sfml/resources.manager.hpp>`` and load it in the world constructor.

.. code-block:: cpp

    // Game world
    struct flappy_bird_world : world::app {
        // Game entry point
        flappy_bird_world() noexcept {
            // Load the graphical system
            auto &graphic_system = system_manager_.create_system<sfml::graphic_system>();

            // Load the resources system
            entity_registry_.set<sfml::resources_system>(entity_registry_);

            // Load the input system with the window from the graphical system
            system_manager_.create_system<sfml::input_system>(graphic_system.get_window());

            // Load the scenes manager
            auto &scene_manager = system_manager_.create_system<scenes::manager>();

            // Change the current_scene to "game_scene" by pushing it.
            scene_manager.change_scene(std::make_unique<game_scene>(entity_registry_), true);
        }
    };

If you compile now you should still see the black window from the previous step, but we are now in our game scene.

.. image:: ../../assets/black_window.png

.. note::

    The scene system is very handy to organize multiple screens of the game: **introduction scene**, **game scene**, **end-of-game scene**, etc.

Step 1 is complete, here is the full code.

.. literalinclude:: ../../../tutorials/flappy-bird/step_1/flappy-bird.cpp
   :language: cpp


Step 2: Creation of Pipes
-------------------------

At this step, we will add the pipes which Flappy Bird dies when it touches them. At the image below, you'll see two pipes with a gap between them. We will call this a ``column``. 

.. image:: ../../assets/fb_column.png

Let's start with the constant values that we will use. We will keep them in a struct. There are many of them:

.. code-block:: cpp

    // Constants
    struct flappy_bird_constants {
        // Pipes
        const float gap_height{265.f};
        const float column_start_distance{700.f};
        const float column_min{0.2f};
        const float column_max{0.8f};
        const float column_thickness{100.f};
        const float column_distance{400.f};
        const std::size_t column_count{6};
        const float pipe_cap_extra_width{10.f};
        const float pipe_cap_height{50.f};
        const graphics::color pipe_color{92, 181, 61};
        const graphics::outline_color pipe_outline_color{2.0f, graphics::color{76, 47, 61}};
    };

Then we will add this to the ``registry`` in the ``game_scene`` constructor.

.. code-block:: cpp

    // Game Scene
    class game_scene final : public scenes::base_scene {
    public:
        game_scene(entt::registry &registry) noexcept : base_scene(registry) {
            // Set the constants that will be used in the program
            registry.set<flappy_bird_constants>();
        }

Now let's make a struct which will represent a single ``pipe``. Instead of using a sprite, we will make graphics with basic shapes. For example, a pipe has two parts as you see in the image above: ``body`` and ``cap``. Body is being the long part of the pipe and the cap is the tip of it. Both will be green rectangle entities but with different sizes. We also prepare a destroy function which will destroy ``body`` and ``cap`` entities.

.. code-block:: cpp

    // A Flappy Bird column which has two pipes
    struct pipe {
        entt::entity body{entt::null};
        entt::entity cap{entt::null};

        // Destroy pipe
        void destroy(entt::registry &registry) {
            registry.destroy(body);
            registry.destroy(cap);
        }
    };

    