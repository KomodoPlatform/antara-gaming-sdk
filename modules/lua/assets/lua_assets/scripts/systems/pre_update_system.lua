function fake_functor(id)
    print("hello from functor id is: " .. id)
end

function __constructor__()
    local table_type = {entt.entity_registry:layer_1_id(),
                        entt.entity_registry:layer_2_id() }
    entt.entity_registry:for_each_runtime(table_type, fake_functor)
    for i = 1, 10
    do
        local id = entt.entity_registry:create()
        entt.entity_registry:add_layer_1_component(id)
    end
end

function __destructor__()
    print("destructor pre_example_system")
end

function on_key_pressed(evt)
    print("we in")
    print("shift: " .. (evt.shift and 'true' or 'false'))
    print("system: " .. (evt.system and 'true' or 'false'))
    print("alt: " .. (evt.alt and 'true' or 'false'))
    print("ctrl: " .. (evt.control and 'true' or 'false'))
    print("evt keycode: " .. evt.keycode)
    assert(evt.keycode == antara.keyboard.space)
end

function on_key_released(evt)
end

function simple_functor(entity_id)
    print("entity_id: " .. entity_id)
end

function internal_update()
    print("pre_example_system update")
    entt.entity_registry:for_each_entities_which_have_layer_1_component(simple_functor)
end

pre_update_system_table = {
    update = internal_update,
    on_construct = __constructor__,
    on_destruct = __destructor__,
    on_key_pressed = on_key_pressed,
    on_key_released = on_key_released,
    system_type = antara.system_type.pre_update
}