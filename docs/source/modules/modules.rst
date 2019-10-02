Antara Gaming Modules
=====================

.. role:: raw-html(raw)
    :format: html

.. list-table:: Modules Tables
   :header-rows: 1
   :align: center

   * - Name
     - Description
     - Dependencies
     - Header-Only
     - Contains Script
     - Authors
     - Contributors
   * - antara::config
     -
     -
     - |:no_entry_sign:|
     -
     -
     -
   * - antara::core
     -
     -
     - |:no_entry_sign:|
     -
     -
     -
   * - antara::input
     -
     -
     - |:white_check_mark:|
     -
     -
     -
   * - antara::event
     -
     -
     - |:no_entry_sign:|
     -
     -
     -
   * - antara::timer
     -
     -
     - |:no_entry_sign:|
     -
     -
     -
   * - antara::ecs
     -
     -
     - |:no_entry_sign:|
     -
     -
     -
   * - :doc:`antara::scenes<scenes>`
     - Module allowing us the manipulation of scenes, possibility of adding, removing and changing a scene (Contains a Scene System Manager and an Abstract Scene class)
     - antara::ecs
     - |:no_entry_sign:|
     - |:no_entry_sign:|
     - Roman Sztergbaum
     -
   * - :doc:`antara::world<world>`
     - Module allowing the creation of application just by inheriting a simple class, contains all the necessary for the development of your video game
     - antara::config antara::core antara::ecs
     - |:no_entry_sign:|
     - |:no_entry_sign:|
     - Roman Sztergbaum
     -
   * - :doc:`antara::lua<lua>`
     - System allowing the manipulation of lua scripts, attaching script to entities, creating of lua scripted systems.
     - Lua_ :raw-html:`<br />` sol2_ :raw-html:`<br />` antara::ecs antara::core
     - |:no_entry_sign:|
     - |:white_check_mark:|
     - Roman Sztergbaum
     -
   * - :doc:`antara::box2d<box2d>`
     - System allowing us to use box2d physics library and some usefull wrapper.
     - Box2D_  :raw-html:`<br />` antara::ecs antara::timer antara::core
     - |:no_entry_sign:|
     - |:no_entry_sign:|
     - Roman Sztergbaum Tolg Ay
     -
   * - antara::sfml
     -
     - SFML_
     - |:no_entry_sign:|
     - |:no_entry_sign:|
     - Roman Sztergbaum Tolg Ay
     -

.. _Box2D: https://github.com/erincatto/Box2D
.. _Lua: https://github.com/lua/lua
.. _sol2: https://github.com/ThePhD/sol2
.. _SFML: https://github.com/SFML/SFML