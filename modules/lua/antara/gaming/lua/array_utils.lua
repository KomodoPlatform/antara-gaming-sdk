--
-- Created by IntelliJ IDEA.
-- User: roman
-- Date: 15/10/2019
-- Time: 09:05
-- To change this template use File | Settings | File Templates.
--

function new_array(size, obj)
    obj = obj or nil
    local t = {}
    for i = 1, size do
        if type(obj) == "number" then
            t[i] = obj
        elseif obj ~= nil and obj.new ~= nil then
            t[i] = obj.new()
        end
    end
    return t
end

