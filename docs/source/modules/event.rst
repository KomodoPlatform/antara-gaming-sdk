:orphan:

antara::event module documentation
==================================

This module contains all the utility events you can find in a game, below the list of events provided by the SDK for you:

.. list-table:: Modules Tables
   :header-rows: 1
   :align: center

   * - Name
     - Description
   * - key_pressed
     - This event is triggered when you press a key on your keyboard, (it's blocking)
   * - key_released
     - This event is triggered when you release a key on your keyboard, (it's blocking)
   * - quit_game
     - Exit the current game with a given return value of the program
   * - window_resized
     - This internally managed event is fired when the window is resize
   * - mouse_moved
     - This event is triggered when you move the mouse
   * - mouse_button_pressed
     - This event is triggered when you press a button of your mouse or trackpad
   * - mouse_button_released
     - This event is triggered when you release a button of your mouse or trackpad

How do the events works ?
--------------------------

Internally we use the EnTT_ library which has an event dispatcher, The documentation can be found here_.

.. _EnTT: https://github.com/skypjack/entt
.. _here: https://github.com/skypjack/entt/wiki/Crash-Course:-events,-signals-and-everything-in-between#event-dispatcher

Tutorial Related to Events in Antara SDK
-----------------------------------------

:doc:`Click here to access events tutorials<../tutorials/event>`

API antara::event
-------------------------

:doc:`Click here to access antara::event API<../api/event>`