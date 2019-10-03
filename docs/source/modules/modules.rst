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
   * - :doc:`antara::timer<timer>`
     - Module allowing us the manipulation of the game frame or retrieve the fixed delta time
     - |:no_entry_sign:|
     - |:no_entry_sign:|
     - |:no_entry_sign:|
     - Roman Sztergbaum
     -
   * - :doc:`antara::ecs<ecs>`
     - Module allowing us the manipulations of systems such as adding, updating, disabling, or removing systems (Contains a system manager, and system abtract class)
     - EnTT_ :raw-html:`<br />` strong_type_ :raw-html:`<br />` expected_ :raw-html:`<br />` range_v3_ :raw-html:`<br />` doom_meta_ :doc:`antara::timer<timer>` antara::event antara::core
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
     - antara::config antara::core :doc:`antara::ecs<ecs>`
     - |:no_entry_sign:|
     - |:no_entry_sign:|
     - Roman Sztergbaum
     -
   * - :doc:`antara::lua<lua>`
     - System allowing the manipulation of lua scripts, attaching script to entities, creating of lua scripted systems.
     - Lua_ :raw-html:`<br />` sol2_ :raw-html:`<br />` :doc:`antara::ecs<ecs>` antara::core
     - |:no_entry_sign:|
     - |:white_check_mark:|
     - Roman Sztergbaum
     -
   * - :doc:`antara::box2d<box2d>`
     - System allowing us to use box2d physics library and some usefull wrapper.
     - Box2D_  :raw-html:`<br />` :doc:`antara::ecs<ecs>` :doc:`antara::timer<timer>` antara::core
     - |:no_entry_sign:|
     - |:no_entry_sign:|
     - Roman Sztergbaum Tolga Ay
     -
   * - :doc:`antara::sfml<sfml>`
     - Systems Wrapping sfml stuffs such as audio, drawing loop, input
     - SFML_ :raw-html:`<br />` antara::config :doc:`antara::ecs<ecs>` :doc:`antara::scenes<scenes>` optional(:doc:`antara::lua<lua>`)
     - |:no_entry_sign:|
     - |:no_entry_sign:|
     - Roman Sztergbaum Tolga Ay
     -

.. _range_v3: https://github.com/ericniebler/range-v3
.. _expected: https://github.com/TartanLlama/expected
.. _strong_type: https://github.com/doom/strong_type
.. _doom_meta: https://github.com/doom/meta
.. _EnTT: https://github.com/skypjack/entt
.. _Box2D: https://github.com/erincatto/Box2D
.. _Lua: https://github.com/lua/lua
.. _sol2: https://github.com/ThePhD/sol2
.. _SFML: https://github.com/SFML/SFML