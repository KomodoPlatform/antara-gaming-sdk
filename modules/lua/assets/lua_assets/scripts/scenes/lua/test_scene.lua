--
-- Created by IntelliJ IDEA.
-- User: roman
-- Date: 23/09/2019
-- Time: 09:28
-- To change this template use File | Settings | File Templates.
--

local function enter()
    print("enter test scene")
end

local function leave()
    print("leave test scene")
end

local function on_key_released(evt)
    print("key released: " .. evt.key)
end

local function on_key_pressed(evt)
    print("key pressed: " .. evt.key)
    print("assert key pressed is space")
    assert(evt.key == antara.keyboard.space)
end

local function update()
    print("test scene update")
end

return {
    enter = enter,
    leave = leave,
    update = update,
    on_key_released = on_key_released,
    on_key_pressed = on_key_pressed,
    scene_active = true
}

