--
-- Created by IntelliJ IDEA.
-- User: roman
-- Date: 23/09/2019
-- Time: 08:54
-- To change this template use File | Settings | File Templates.
--

local scenes_table = {}
local current_scene

local function get_file_extension(file)
    print("analyzing file: " .. file)
    return file:match("^.+(%..+)$")
end

function get_file_name(file)
    print("get_file_name of file: " .. file)
    return file:match("^.+/(.+)$")
end

local function __constructor__()
    print("scene manager constructor")
    local path_entries, filenames = antara:get_all_scripts_scenes()
    for i, v in ipairs(path_entries) do
        if (get_file_extension(filenames[i]) == ".lua") then
            local scene_name = string.gsub(filenames[i], '.lua$', '')
            print("loading scene: " .. scene_name)
            scenes_table[scene_name] = dofile(path_entries[i])
            if scenes_table[scene_name].scene_active == true then
                current_scene = scenes_table[scene_name]
                current_scene.enter()
            end
        end
    end
end

local function __destructor__()
    print("scene manager destructor")
    if current_scene ~= nil and current_scene.leave ~= nil then
        current_scene.leave()
    else
        print("current scene doesn't have leave callback")
    end
end

local function on_key_pressed(evt)
    if current_scene ~= nil and current_scene.on_key_pressed ~= nil then
        current_scene.on_key_pressed(evt)
    else
        print("current scene doesn't have on_key_pressed callback")
    end
end

local function on_key_released(evt)
    if current_scene ~= nil and current_scene.on_key_released ~= nil then
        current_scene.on_key_released(evt)
    else
        print("current scene doesn't have on_key_released callback")
    end
end

local function update()
    if current_scene ~= nil and current_scene.update ~= nil then
        current_scene.update()
    end
end

local function change_scene(scene_name)
    print("scene_name invoked: " .. scene_name)
    if scenes_table[scene_name] ~= nil then
        current_scene.scene_active = false
        current_scene.leave()
        current_scene = scenes_table[scene_name]
        current_scene.scene_active = true
        current_scene.enter()
    end
end

--noinspection GlobalCreationOutsideO
scenes_system_table = {
    update = update,
    on_construct = __constructor__,
    on_destruct = __destructor__,
    on_key_pressed = on_key_pressed,
    on_key_released = on_key_released,
    on_change_scene = change_scene,
    system_type = antara.system_type.logic_update
}