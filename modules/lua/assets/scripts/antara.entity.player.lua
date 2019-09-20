--
-- Created by IntelliJ IDEA.
-- User: roman
-- Date: 20/09/2019
-- Time: 11:27
-- To change this template use File | Settings | File Templates.
--

function update(entity_id)
end

function init(entity_id)
    print("INIT: " .. entity_id)
    return true
end

player_table = {
    on_update = update,
    on_init = init
}

