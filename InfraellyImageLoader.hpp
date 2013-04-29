/*-----------------------------------------------------------------------------\
|                         ____                                                 |
|      __                / __ \                                /\     /\       |
|     /_/               / /  \/                               / /    / /       |
|     __    ____       / /_    ____     ____       ____      / /    / /        |
|    / /   / __ \     / ___\  / __ \   / __ \     / __ \    / /    / /   /\  /\|
|   / /   / /  \ \   / /     / /  \/  / /  \ \   / ____/   / /    / /   / / / /|
|  / /_  / /   / /  / /     / /       \ \__/ /_  \  \___  / /_   / /_   \ \/ / |
|  \__/  \/    \/   \/      \/         \______/   \____/  \__/   \__/    \  /  |
|                                                                        / /   |
| ______________________________________________________________________/ /    |
|/   ____________________________________________________________________/     |
|\__/                                                                          |
|                                                                              |
|                                                                              |
|   Infraelly MMORPG                                                           |
|   Copyright (C) 2007-2010 Tony Huynh aka insanepotato                        |
|                                                                              |
|   Visit: http://sourceforge.net/projects/infraelly/                          |
|                                                                              |
|   License: (LGPL)                                                            |
|   This is free software; you can redistribute it and/or                      |
|   modify it under the terms of the GNU Library General Public                |
|   License as published by the Free Software Foundation; either               |
|   version 2 of the License, or (at your option) any later version.           |
|                                                                              |
|   This is distributed in the hope that it will be useful,                    |
|   but WITHOUT ANY WARRANTY; without even the implied warranty of             |
|   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          |
|   Library General Public License for more details.                           |
|                                                                              |
|   You should have received a copy of the GNU Library General Public          |
|   License along with this library; if not, write to the Free                 |
|   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA  |
|                                                                              |
|   Tony Huynh                                                                 |
|   tony.huynh1991@gmail.com                                                   |
|                                                                              |
L-----------------------------------------------------------------------------*/


#ifndef INFRAELLYIMAGELOADER_HPP
#define INFRAELLYIMAGELOADER_HPP


#include <SDL/SDL.h>

#include "guichan.hpp"
#include "guichan/sdl.hpp"

#include "guichan/sdl/sdlimage.hpp"
#include "guichan/sdl/sdlimageloader.hpp"

#include "Tileset.hpp"
#include "Tile.hpp"


class InfraellyImageLoader : public gcn::SDLImageLoader{
    public:
        virtual gcn::Image* load(SDL_Surface *surface, bool freeSurface = false);
        virtual gcn::Image* load(SDL_RWops *sdlRWOP, bool freeRWOP = false, bool convertToDisplayFormat = true);
        virtual gcn::Image* load(unsigned char *buffer, long bufferLength, bool convertToDisplayFormat = true);

        virtual gcn::Image* load(const Tileset *sourceTileset);
        virtual gcn::Image* load(const Tileset &sourceTileset);
        virtual gcn::Image* load(const Tile* sourceTile);
        virtual gcn::Image* load(const Tile& sourceTile);

    private:
};


#endif
