--
-- Created by IntelliJ IDEA.
-- User: roman
-- Date: 15/10/2019
-- Time: 09:04
-- To change this template use File | Settings | File Templates.
--

function enum(names, offset)
    offset = offset or 1
    local objects = {}
    local size = 0
    for idr, name in pairs(names) do
        local id = idr + offset - 1
        local obj = {
            id = id, -- id
            idr = idr, -- 1-based relative id, without offset being added
            name = name -- name of the object
        }
        objects[name] = obj
        objects[id] = obj
        size = size + 1
    end
    objects.idstart = offset -- start of the id range being used
    objects.idend = offset + size - 1 -- end of the id range being used
    objects.size = size
    objects.all = function()
        local list = {}
        for _, name in pairs(names) do
            add(list, objects[name])
        end
        local i = 0
        return function() i = i + 1 if i <= #list then return list[i] end end
    end
    return objects
end
