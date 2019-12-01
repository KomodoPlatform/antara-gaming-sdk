local function enter()
    print("enter game scene")
end

local function leave()
    print("leave game scene")
end

local function on_key_released(evt)
    print("key released: " .. evt.key)
end

local function on_key_pressed(evt)
    print("key pressed: " .. evt.key)
end

local function update()
    print("game scene update")
end

return {
    enter = enter,
    leave = leave,
    update = update,
    on_key_released = on_key_released,
    on_key_pressed = on_key_pressed,
    scene_active = true
}

