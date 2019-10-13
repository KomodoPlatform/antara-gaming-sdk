/******************************************************************************
 * Copyright © 2013-2019 The Komodo Platform Developers.                      *
 *                                                                            *
 * See the AUTHORS, DEVELOPER-AGREEMENT and LICENSE files at                  *
 * the top-level directory of this distribution for the individual copyright  *
 * holder information and the developer policies on copyright and licensing.  *
 *                                                                            *
 * Unless otherwise agreed in a custom licensing agreement, no part of the    *
 * Komodo Platform software, including this file may be copied, modified,     *
 * propagated or distributed except according to the terms contained in the   *
 * LICENSE file                                                               *
 *                                                                            *
 * Removal or modification of this copyright notice is prohibited.            *
 *                                                                            *
 ******************************************************************************/

#include <cmath>
#include <entt/entity/helper.hpp>
#include <antara/gaming/graphics/component.layer.hpp>
#include <antara/gaming/transform/component.position.hpp>
#include <antara/gaming/geometry/component.vertex.hpp>
#include <antara/gaming/geometry/component.circle.hpp>
#include <antara/gaming/graphics/component.color.hpp>
#include <antara/gaming/graphics/component.canvas.hpp>
#include "tic.tac.toe.factory.hpp"
#include "tic.tac.toe.constants.hpp"
#include "tic.tac.toe.components.hpp"

namespace tictactoe ::example
{
    using namespace antara::gaming;

    const auto grid_thickness = 20.0f;

    entt::entity tic_tac_toe_factory::create_grid_entity(entt::registry &entity_registry) noexcept
    {
        auto [canvas_width, canvas_height] = entity_registry.ctx<graphics::canvas_2d>().canvas.size;
        auto grid_entity = entity_registry.create();

        std::vector<geometry::vertex> lines{8 * 4};
        auto constants = entity_registry.ctx<tic_tac_toe_constants>();
        const auto half_thickness = grid_thickness * 0.5f;
        for (std::size_t counter = 0, i = 0; i <= constants.nb_cells; ++i, counter += 4 * 2) {
            // First and last ones should be a bit inside, otherwise half of it is out of the screen
            auto offset_x = 0.0f;
            auto offset_y = 0.0f;

            if(i == 0) {
                offset_x += half_thickness;
                offset_y += half_thickness;
            }
            else if(i == constants.nb_cells) {
                offset_x -= half_thickness;
                offset_y -= half_thickness;
            }

            // Prepare lines
            // Vertical
            lines[counter].pos = {offset_x + i * constants.cell_width - half_thickness, 0.f};
            lines[counter + 1].pos = {offset_x + i * constants.cell_width + half_thickness, 0.f};
            lines[counter + 2].pos = {offset_x + i * constants.cell_width + half_thickness, canvas_height};
            lines[counter + 3].pos = {offset_x + i * constants.cell_width - half_thickness, canvas_height};

            // Horizontal
            lines[counter + 4].pos = {offset_x + 0, offset_y + i * constants.cell_height - half_thickness};
            lines[counter + 5].pos = {offset_x + canvas_width, offset_y + i * constants.cell_height - half_thickness};
            lines[counter + 6].pos = {offset_x + canvas_width, offset_y + i * constants.cell_height + half_thickness};
            lines[counter + 7].pos = {offset_x + 0, offset_y + i * constants.cell_height + half_thickness};
        }

        entity_registry.assign<geometry::vertex_array>(grid_entity, lines, geometry::vertex_geometry_type::quads);
        entity_registry.assign<entt::tag<"grid"_hs>>(grid_entity);
        entity_registry.assign<entt::tag<"game_scene"_hs>>(grid_entity);
        entity_registry.assign<graphics::layer<0>>(grid_entity);
        return grid_entity;
    }

    entt::entity tic_tac_toe_factory::create_board(entt::registry &entity_registry) noexcept
    {
        auto board_entity = entity_registry.create();
        auto constants = entity_registry.ctx<tic_tac_toe_constants>();
        entity_registry.assign<board_component>(board_entity, constants.nb_cells);
        entity_registry.assign<entt::tag<"game_scene"_hs>>(board_entity);
        return board_entity;
    }

    entt::entity
    tic_tac_toe_factory::create_x(entt::registry &entity_registry, std::size_t row, std::size_t column) noexcept
    {
        auto constants = entity_registry.ctx<tic_tac_toe_constants>();
        const float half_box_side = static_cast<float>(std::fmin(constants.cell_width, constants.cell_height) * 0.25f);
        const float center_x = static_cast<float>(constants.cell_width * 0.5 + column * constants.cell_width);
        const float center_y = static_cast<float>(constants.cell_height * 0.5 + row * constants.cell_height);

        auto x_entity = entity_registry.create();
        entity_registry.assign<cell_position>(x_entity, row, column);
        std::vector<geometry::vertex> lines{2 * 4};

        for (auto&& current_vertex: lines) current_vertex.pixel_color = graphics::magenta;

        const auto half_thickness = grid_thickness * 0.5f;

        // Top-left to Bottom-right
        lines[0].pos = {center_x - half_box_side - half_thickness, center_y - half_box_side};
        lines[1].pos = {center_x - half_box_side + half_thickness, center_y - half_box_side};
        lines[2].pos = {center_x + half_box_side + half_thickness, center_y + half_box_side};
        lines[3].pos = {center_x + half_box_side - half_thickness, center_y + half_box_side};


        // Top-right to Bottom-left
        lines[4].pos = {center_x + half_box_side - half_thickness, center_y - half_box_side};
        lines[5].pos = {center_x + half_box_side + half_thickness, center_y - half_box_side};
        lines[6].pos = {center_x - half_box_side + half_thickness, center_y + half_box_side};
        lines[7].pos = {center_x - half_box_side - half_thickness, center_y + half_box_side};

        entity_registry.assign<geometry::vertex_array>(x_entity, lines, geometry::vertex_geometry_type::quads);
        entity_registry.assign<entt::tag<"game_scene"_hs>>(x_entity);
        entity_registry.assign<entt::tag<"player_x"_hs>>(x_entity);
        entity_registry.assign<graphics::layer<1>>(x_entity);
        return x_entity;
    }

    entt::entity
    tic_tac_toe_factory::create_o(entt::registry &entity_registry, std::size_t row, std::size_t column) noexcept
    {
        auto constants = entity_registry.ctx<tic_tac_toe_constants>();
        const auto half_box_side = static_cast<float>(std::fmin(constants.cell_width, constants.cell_height) * 0.25f);
        const auto center_x = static_cast<float>(constants.cell_width * 0.5 + column * constants.cell_width);
        const auto center_y = static_cast<float>(constants.cell_height * 0.5 + row * constants.cell_height);

        auto o_entity = entity_registry.create();
        entity_registry.assign<graphics::fill_color>(o_entity, graphics::transparent);
        entity_registry.assign<graphics::outline_color>(o_entity, grid_thickness, graphics::cyan);
        entity_registry.assign<geometry::circle>(o_entity, half_box_side);
        entity_registry.assign<transform::position_2d>(o_entity,
                                                       center_x,
                                                       center_y);

        entity_registry.assign<entt::tag<"game_scene"_hs>>(o_entity);
        entity_registry.assign<graphics::layer<1>>(o_entity);
        return o_entity;
    }
}