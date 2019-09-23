--
-- Created by IntelliJ IDEA.
-- User: roman
-- Date: 23/09/2019
-- Time: 09:28
-- To change this template use File | Settings | File Templates.
--

local function enter()
    print("enter another test scene")
end

local function leave()
    print("leave another test scene")
end

local function on_key_released(evt)
    print("another key released: " .. evt.key)
end

local function on_key_pressed(evt)
    print("another key pressed: " .. evt.key)
end

local function update()
    print("another test scene update")
end

return {
    enter = enter,
    leave = leave,
    update = update,
    on_key_released = on_key_released,
    on_key_pressed = on_key_pressed,
    scene_active = false
}