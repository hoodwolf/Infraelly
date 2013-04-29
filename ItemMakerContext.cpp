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


#include "ItemMakerContext.hpp"
#include "ItemMakerGui.hpp"


ItemMakerContext::ItemMakerContext() :
    gui(new ItemMakerGui(screen))
{
    // Set window caption
    SDL_WM_SetCaption("Forge (Infraelly Item Editor)", NULL);
    //set to wait (instead of poll) for events
    poll_events = true;

    run();
}

ItemMakerContext::~ItemMakerContext(){
    delete gui;
    gui = NULL;
}


void ItemMakerContext::events(SDL_Event &event){
    if( (event.type == SDL_VIDEORESIZE) || (event.key.keysym.sym == SDLK_F4) ){
        // Window has been resized
        gui->resetPositions();
    }
    gui->pushInput(event);
}

void ItemMakerContext::input(Uint8 *keys){
}

void ItemMakerContext::logic(){
    // let giuchan do its thang!
    gui->logic();
}

void ItemMakerContext::draw(){
    //clear
    SDL_FillRect(screen,NULL,SDL_MapRGB(screen->format,255,255,255));
    //let guichan draw on the screen =]
    gui->draw();
}
