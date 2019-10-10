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
   * - :doc:`antara::config<config>`
     - Modules containing game configuration and utilities to load configuration
     - nlohmann_json_
     - |:no_entry_sign:|
     - |:no_entry_sign:|
     - Roman Sztergbaum
     -
   * - :doc:`antara::core<core>`
     - Modules containing core utilities such as assets path, safe headers, etc...
     - EnTT_ :raw-html:`<br />` refl-cpp_ :raw-html:`<br />`
     - |:no_entry_sign:|
     - |:no_entry_sign:|
     - Roman Sztergbaum
     -
   * - :doc:`antara::input<input>`
     - Modules containing enums representing keyboard, mouse, etc...
     - |:no_entry_sign:|
     - |:white_check_mark:|
     - |:no_entry_sign:|
     - Roman Sztergbaum
     -
   * - :doc:`antara::event<event>`
     - Module Containing all the common events that's will be used in games such as (key_pressed, key_release, etc...)
     - EnTT_ :raw-html:`<br />` doom_meta_ :raw-html:`<br />` refl-cpp_ :raw-html:`<br />`:doc:`antara::input<input>` :doc:`antara::core<core>`
     - |:no_entry_sign:|
     - |:no_entry_sign:|
     - Roman Sztergbaum
     -
   * - :doc:`antara::timer<timer>`
     - Module allowing us the manipulation of the game frame or retrieve the fixed delta time
     - |:no_entry_sign:|
     - |:no_entry_sign:|
     - |:no_entry_sign:|
     - Roman Sztergbaum
     -
   * - :doc:`antara::geometry<geometry>`
     - Module Containing all the common geometry components (circle, square, rect...)
     - :doc:`antara::event<event>`
     - |:no_entry_sign:|
     - |:no_entry_sign:|
     - Roman Sztergbaum
     -
   * - :doc:`antara::graphics<graphics>`
     - Module Containing all the common graphics components (color, layer, ...)
     - :doc:`antara::event<event>`
     - |:no_entry_sign:|
     - |:no_entry_sign:|
     - Roman Sztergbaum
     -
   * - :doc:`antara::ecs<ecs>`
     - Module allowing us the manipulations of systems such as adding, updating, disabling, or removing systems (Contains a system manager, and system abtract class)
     - EnTT_ :raw-html:`<br />` strong_type_ :raw-html:`<br />` expected_ :raw-html:`<br />` range_v3_ :raw-html:`<br />` doom_meta_ :doc:`antara::timer<timer>` :doc:`antara::event<event>` :doc:`antara::core<core>`
     - |:no_entry_sign:|
     - |:no_entry_sign:|
     - Roman Sztergbaum
     -
   * - :doc:`antara::scenes<scenes>`
     - Module allowing us the manipulation of scenes, possibility of adding, removing and changing a scene (Contains a Scene System Manager and an Abstract Scene class)
     - :doc:`antara::ecs<ecs>`
     - |:no_entry_sign:|
     - |:no_entry_sign:|
     - Roman Sztergbaum
     -
   * - :doc:`antara::world<world>`
     - Module allowing the creation of application just by inheriting a simple class, contains all the necessary for the development of your video game
     -  :doc:`antara::config<config>` :doc:`antara::core<core>` :doc:`antara::ecs<ecs>`
     - |:no_entry_sign:|
     - |:no_entry_sign:|
     - Roman Sztergbaum
     -
   * - :doc:`antara::lua<lua>`
     - System allowing the manipulation of lua scripts, attaching script to entities, creating of lua scripted systems.
     - Lua_ :raw-html:`<br />` sol2_ :raw-html:`<br />` :doc:`antara::ecs<ecs>` :doc:`antara::core<core>`
     - |:no_entry_sign:|
     - |:white_check_mark:|
     - Roman Sztergbaum
     -
   * - :doc:`antara::box2d<box2d>`
     - System allowing us to use box2d physics library and some usefull wrapper.
     - Box2D_  :raw-html:`<br />` :doc:`antara::ecs<ecs>` :doc:`antara::timer<timer>` :doc:`antara::core<core>`
     - |:no_entry_sign:|
     - |:no_entry_sign:|
     - Roman Sztergbaum Tolga Ay
     -
   * - :doc:`antara::sfml<sfml>`
     - Systems Wrapping sfml stuffs such as audio, drawing loop, input
     - SFML_ :raw-html:`<br />` :doc:`antara::config<config>` :doc:`antara::ecs<ecs>` :doc:`antara::scenes<scenes>` optional(:doc:`antara::lua<lua>`)
     - |:no_entry_sign:|
     - |:no_entry_sign:|
     - Roman Sztergbaum Tolga Ay
     -

.. _nlohmann_json: https://github.com/nlohmann/json
.. _refl-cpp: https://github.com/veselink1/refl-cpp
.. _range_v3: https://github.com/ericniebler/range-v3
.. _expected: https://github.com/TartanLlama/expected
.. _strong_type: https://github.com/doom/strong_type
.. _doom_meta: https://github.com/doom/meta
.. _EnTT: https://github.com/skypjack/entt
.. _Box2D: https://github.com/erincatto/Box2D
.. _Lua: https://github.com/lua/lua
.. _sol2: https://github.com/ThePhD/sol2
.. _SFML: https://github.com/SFML/SFML