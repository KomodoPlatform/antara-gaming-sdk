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

#pragma once

#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>

namespace antara::gaming::sdl
{
    class timer
    {
      public:
        // Initializes variables
        timer()
        {
            // Initialize the variables
            m_start_ticks  = 0;
            m_paused_ticks = 0;

            m_paused  = false;
            m_started = false;
        }

        // The various clock actions
        void
        start()
        {
            // Start the timer
            m_started = true;

            // Unpause the timer
            m_paused = false;

            // Get the current clock time
            m_start_ticks  = SDL_GetTicks();
            m_paused_ticks = 0;
        }

        void
        stop()
        {
            // Stop the timer
            m_started = false;

            // Unpause the timer
            m_paused = false;

            // Clear tick variables
            m_start_ticks  = 0;
            m_paused_ticks = 0;
        }

        void
        pause()
        {
            // If the timer is running and isn't already paused
            if (m_started && !m_paused)
            {
                // Pause the timer
                m_paused = true;

                // Calculate the paused ticks
                m_paused_ticks = SDL_GetTicks() - m_start_ticks;
                m_start_ticks  = 0;
            }
        }

        void
        unpause()
        {
            // If the timer is running and paused
            if (m_started && m_paused)
            {
                // Unpause the timer
                m_paused = false;

                // Reset the starting ticks
                m_start_ticks = SDL_GetTicks() - m_paused_ticks;

                // Reset the paused ticks
                m_paused_ticks = 0;
            }
        }

        // Gets the timer's time
        Uint32
        getTicks()
        {
            // The actual timer time
            Uint32 time = 0;

            // If the timer is running
            if (m_started)
            {
                // If the timer is paused
                if (m_paused)
                {
                    // Return the number of ticks when the timer was paused
                    time = m_paused_ticks;
                }
                else
                {
                    // Return the current time minus the start time
                    time = SDL_GetTicks() - m_start_ticks;
                }
            }

            return time;
        }

        // Checks the status of the timer
        bool
        isStarted()
        {
            // Timer is running and paused or unpaused
            return m_started;
        }

        bool
        isPaused()
        {
            // Timer is running and paused
            return m_paused && m_started;
        }

      private:
        // The clock time when the timer started
        Uint32 m_start_ticks;

        // The ticks stored when the timer was paused
        Uint32 m_paused_ticks;

        // The timer status
        bool m_paused;
        bool m_started;
    };
} // namespace antara::gaming::sdl
