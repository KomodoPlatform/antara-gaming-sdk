Tutorial: Antara Gaming Systems
===============================

If you have not read the :doc:`getting started<getting_started>` part yet, I invite you to do it now for the rest of this tutorial.

If you have not read the :doc:`ecs<../modules/ecs>` module documentation yet, I invite you to do it now to understand what we are doing in this tutorial series.

How to create your own system step by step ?
--------------------------------------------

.. include:: basic_setup.rst

Create a system
^^^^^^^^^^^^^^^

Let's create between the ``using namespace`` statement and the definition of the class ``my_world`` a system class who will be a ``pre_update_system``

.. code-block:: cpp

    class pre_concrete_system final : public ecs::pre_update_system<pre_concrete_system>
    {
    public:
        //! Here the constructor can take other additional arguments but the first two are mandatory
        pre_concrete_system(entt::registry &registry, entt::dispatcher &dispatcher) noexcept : system(registry, dispatcher)
        {

        }

        void update() noexcept final
        {
            //! Empty for the moment
        }

        ~pre_concrete_system() noexcept final = default;
    };

Now we can load this system into our world. Place yourself at the body of the constructor of the class ``my_world``.

In order to load the system we will use the function ``create_system`` of the ``system_manager`` class.

.. code-block:: cpp

    my_world() noexcept
    {
        //! Here we don't need to add any parameters for the constructor
        //! because the mandatory parameters are forwarded by default
        this->system_manager_.create_system<pre_concrete_system>();
    }

Now, if you compile your program and start it, you will realize that you are in an infinite loop, that's simply mean your system is running inside the game loop.

But do not panic we have a way to stop our system thanks to the dispatcher_.

We will make sure that after a number of iterations from our system, we will emit a ``quit_game event`` that will be catched by the world and stop the gaming loop.

To do this we will create a counter as a private field of our system and increment it each time the update function is called, arrived at 10 iterations we will emit an event to leave the game

.. code-block:: cpp

    class pre_concrete_system final : public antara::gaming::ecs::pre_update_system<pre_concrete_system>
    {
    public:
        //! Here the constructor can take other additional arguments but the first two are mandatory
        pre_concrete_system(entt::registry &registry, entt::dispatcher &dispatcher) noexcept : system(registry, dispatcher)
        {

        }

        void update() noexcept final
        {
            std::cout << "nb_iteration: " << (++nb_iteration) << "\n";
            if (nb_iteration == 10u) {
                this->dispatcher_.trigger<antara::gaming::event::quit_game>(0);
            }
        }

        ~pre_concrete_system() noexcept final = default;
    private:
        std::size_t nb_iteration{0u};
    };

    REFL_AUTO(type(pre_concrete_system)) //! This line is very important, it's give a static reflection name function to your system, otherwise you will not compile.

.. warning::
        Let's not forget the inclusion of iostream_ header at the top of the file.

Now, if you compile your program and start it you will quit your game after 10 iterations.

Below all the code of this tutorial:

.. literalinclude:: ../../../examples/ecs/system.example.cpp
   :language: cpp

**Congratulations, you have managed to create your own system and add it to your game world !**

.. _dispatcher: https://github.com/skypjack/entt/wiki/Crash-Course:-events,-signals-and-everything-in-between#event-dispatcher

.. _iostream: https://en.cppreference.com/w/cpp/header/iostream