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


#include "ConnectionGroup.hpp"

#include <iostream>
#include <map>
#include <algorithm> //advance()

#include <SDL/SDL.h>

#include "MutexLocker.hpp"


using namespace std;

namespace inp{
    typedef map<string, inp::Connection*> ConnectionsList;

    unsigned ConnectionGroup::minSocks = 50;

    struct ConnectionGroup::ConnectionGroupImpl{
            //create mutex
            ConnectionGroupImpl() :
                dataAccess(SDL_CreateMutex()),
                refCount(0),
                peakConnections(0),
                maxSocks(minSocks),
                socksInSet(0),
                sSet(NULL)
            {
                sSet = SDLNet_AllocSocketSet(minSocks);
                if( sSet == NULL ){
                    cerr << __FILE__ << __LINE__ << ": ";
                    cerr << "SDLNet_AllocSocketSet: " << SDLNet_GetError() << endl;
                    exit(EXIT_FAILURE);
                }
            }

            //destroy mutex
            ~ConnectionGroupImpl(){
                SDL_LockMutex(dataAccess);
                SDL_DestroyMutex(dataAccess);
                dataAccess = NULL;
                SDLNet_FreeSocketSet(sSet);
                sSet = NULL;
            }

            mutable SDL_mutex *dataAccess;
            unsigned refCount;
            unsigned peakConnections;
            unsigned maxSocks;
            unsigned socksInSet;


            //manages Connections
            ConnectionsList users;

            //manages the Connections' underlying SDL sockets
            SDLNet_SocketSet sSet;
    };

    ConnectionGroup::ConnectionGroup() : pimpl_(new ConnectionGroupImpl){
        ++pimpl_->refCount;
    }

    ConnectionGroup::~ConnectionGroup(){ clean(); }

    void ConnectionGroup::clean(){
        SDL_LockMutex(pimpl_->dataAccess);
        --pimpl_->refCount;
        if( pimpl_->refCount == 0 ){
            //  Last reference to this pimpl
            //  Free resources
            for(    ConnectionsList::iterator itr = pimpl_->users.begin();
                    itr != pimpl_->users.end();
                    ++itr       )
                {
                //  Disconnect Connections
                itr->second->disconnect();
                //  Free Connections' memory
                itr->second->cleanSet();
                delete itr->second;
                itr->second = NULL;
            }
            pimpl_->users.clear();
            //  Destroy Mutex
            SDL_DestroyMutex(pimpl_->dataAccess);
            pimpl_->dataAccess = NULL;
            delete pimpl_;      //  Destroy pimpl;
        } else {
            SDL_UnlockMutex(pimpl_->dataAccess);
        }
        pimpl_ = NULL;
    }

    ConnectionGroup::ConnectionGroup(const ConnectionGroup& src){
        ScopedMutexLock(pimpl_->dataAccess);
        pimpl_ = src.pimpl_;
        ++pimpl_->refCount;
    }

    ConnectionGroup& ConnectionGroup::operator=(ConnectionGroup& rhs){
        if( &rhs != this ){
            ScopedMutexLock(pimpl_->dataAccess);
            clean();
            pimpl_ = rhs.pimpl_;
            ++pimpl_->refCount;
            pimpl_->dataAccess = pimpl_->dataAccess;
        }
        return *this;
    }






    inp::Connection* ConnectionGroup::conAt(unsigned i)const{
        ScopedMutexLock(pimpl_->dataAccess);
        ConnectionsList::iterator itr = pimpl_->users.begin();
        advance(itr, i);
        return itr->second;
    }

    std::string ConnectionGroup::keyAt(unsigned i)const{
        ScopedMutexLock(pimpl_->dataAccess);
        ConnectionsList::iterator itr = pimpl_->users.begin();
        advance(itr, i);
        return itr->first;
    }




    bool ConnectionGroup::empty()const{
        ScopedMutexLock(pimpl_->dataAccess);
        return pimpl_->users.empty();
    }

    bool ConnectionGroup::exists(const std::string& key)const{
        ScopedMutexLock(pimpl_->dataAccess);
        if( pimpl_->users.find(key) == pimpl_->users.end() ){
            return false;
        } else {
            return true;
        }
    }

    Connection *ConnectionGroup::getConnection(const std::string& key)const{
        ScopedMutexLock(pimpl_->dataAccess);
        if( pimpl_->users.find(key) != pimpl_->users.end() ){
            return pimpl_->users.find(key)->second;
        } else {
            return NULL;
        }
    }

    unsigned ConnectionGroup::size()const{
        ScopedMutexLock(pimpl_->dataAccess);
        return pimpl_->users.size();
    }

    unsigned ConnectionGroup::sizePeak()const{
        ScopedMutexLock(pimpl_->dataAccess);
        return pimpl_->peakConnections;
    }





    bool ConnectionGroup::addConnection( const std::string& key, Connection* c ){
        ScopedMutexLock(pimpl_->dataAccess);
        //skip if either connection or thread is NULL
        if( c == NULL ){
            return false;
        } else {
            //only add if the key is unique
            if( pimpl_->users.find(key) == pimpl_->users.end() ){
                //make sure theres enough space in the sSet
                if( pimpl_->socksInSet == pimpl_->maxSocks ){
                    //make a set larger than current
                    SDLNet_SocketSet largerSet = SDLNet_AllocSocketSet(pimpl_->maxSocks + 25);
                    if( largerSet == NULL ){
                        cerr << __FILE__ << __LINE__ << ": ";
                        cerr << "SDLNet_AllocSocketSet: " << SDLNet_GetError() << endl;
                    }
                    for( ConnectionsList::iterator itr = pimpl_->users.begin();
                            itr != pimpl_->users.end();
                            ++itr   ){
                            TCPsocket t = itr->second->getSocket();
                        if( SDLNet_TCP_AddSocket(pimpl_->sSet, t) == -1 ){
                            cerr << __FILE__ << __LINE__ << ": ";
                            cerr << "SDLNet_AddSocket: " << SDLNet_GetError() << endl;
                        }
                    }
                    //free old set and associate pointer with the new,larger one
                    SDLNet_FreeSocketSet(pimpl_->sSet);
                    pimpl_->maxSocks += 25;
                    pimpl_->sSet = largerSet;
                }

                //add to list
                pimpl_->users[key] = c;
                //add to sSet
                if( c->setSocketSet( pimpl_->sSet ) == -1 ){
                    cerr << __FILE__ << __LINE__ << ": ";
                    cerr << "SDLNet_AddSocket: " << SDLNet_GetError() << endl;
                } else {
                    ++pimpl_->socksInSet;
                }

                //update peak con
                if( pimpl_->users.size() > pimpl_->peakConnections ){
                    pimpl_->peakConnections = pimpl_->users.size();
                }
                return true;
            } else {
                return false;
            }
        }
    }

    void ConnectionGroup::erase(const std::string& key){
        ScopedMutexLock(pimpl_->dataAccess);
        if( pimpl_->users.find(key) != pimpl_->users.end() ){
            //remove from set
            pimpl_->users[key]->cleanSet();
            --pimpl_->socksInSet;
            //  Disconnect Connections
            pimpl_->users[key]->disconnect();
            //  Free Connections' memory
            if( pimpl_->users[key] != NULL ){
                delete pimpl_->users[key];
                pimpl_->users[key] = NULL;
            }
            //remove entry
            pimpl_->users.erase(key);
        }
    }

    void ConnectionGroup::disconnect(const std::string& key){
        erase(key);
    }

    void ConnectionGroup::remove(const std::string& key){
        ScopedMutexLock(pimpl_->dataAccess);
        //remove from set
        pimpl_->users[key]->cleanSet();
        --pimpl_->socksInSet;
        pimpl_->users[key]->makeSet();
        //remove entry
        pimpl_->users.erase(key);
    }

    void ConnectionGroup::removeAll(const std::string& key){
        ScopedMutexLock(pimpl_->dataAccess);
        //free set
        SDLNet_FreeSocketSet(pimpl_->sSet);
        //realoc set
        pimpl_->sSet = SDLNet_AllocSocketSet(minSocks);
        if( pimpl_->sSet == NULL ){
            cerr << __FILE__ << __LINE__ << ": ";
            cerr << "SDLNet_AllocSocketSet: " << SDLNet_GetError() << endl;
            exit(EXIT_FAILURE);
        }
        pimpl_->socksInSet = 0;
        pimpl_->maxSocks = minSocks;
        //remove all entries
        pimpl_->users.clear();
    }



    void ConnectionGroup::disconnectAll(){
        ScopedMutexLock(pimpl_->dataAccess);
        //  tell all threads to exit
        if( !pimpl_->users.empty() ){
            ConnectionsList::iterator itr;
            while( !pimpl_->users.empty() ){
                itr = pimpl_->users.begin();
                //send quick shutdown note
                INFPacket packet;
                packet << DataTypeByte::SERVER_MSG << "Server shutdown.";
                itr->second->send(packet);
                //  Disconnect Connections
                itr->second->disconnect();
                //  Free Connections' memory
                itr->second->cleanSet();
                delete itr->second;
                itr->second = NULL;
                pimpl_->users.erase( itr );
            }
        }
        //remove all entries
        pimpl_->users.clear();
        //clear and re aloc set
                    //free set
        SDLNet_FreeSocketSet(pimpl_->sSet);
        //realoc set
        pimpl_->sSet = SDLNet_AllocSocketSet(minSocks);
        if( pimpl_->sSet == NULL ){
            cerr << __FILE__ << __LINE__ << ": ";
            cerr << "SDLNet_AllocSocketSet: " << SDLNet_GetError() << endl;
            exit(EXIT_FAILURE);
        }
        pimpl_->socksInSet = 0;
        pimpl_->maxSocks = minSocks;
    }




    bool ConnectionGroup::checkSockets(Uint32 timeout){
        ScopedMutexLock(pimpl_->dataAccess);
        return SDLNet_CheckSockets(pimpl_->sSet, timeout);
    }

    bool ConnectionGroup::checkSockets(Uint32 timeout, vector<Connection*>& activeList){
        activeList.clear();
        ScopedMutexLock(pimpl_->dataAccess);
        //put all the active cons into the vector
        if( SDLNet_CheckSockets(pimpl_->sSet, timeout) != -1 ){
            for( ConnectionsList::iterator itr = pimpl_->users.begin();
                 itr != pimpl_->users.end();
                 ++itr ){
                if( SDLNet_SocketReady(itr->second->getSocket()) ){
                    activeList.push_back(itr->second);
                }
            }
            return 1;
        }
        return 0;
    }




    bool ConnectionGroup::sendTo(const std::string& key, const INFPacket& packet){
        ScopedMutexLock(pimpl_->dataAccess);
        if( pimpl_->users.find(key) == pimpl_->users.end() ){
            return false;
        } else {
            return pimpl_->users[key]->send(packet);
        }
    }

    void ConnectionGroup::sendAll(const INFPacket& packet){
        ScopedMutexLock(pimpl_->dataAccess);
        for( ConnectionsList::iterator itr = pimpl_->users.begin();
                itr != pimpl_->users.end();
                ++itr   ){
            itr->second->send(packet);
        }
    }

    void ConnectionGroup::sendAllBar(const inp::INFPacket& packet, const inp::Connection* bar){
        ScopedMutexLock(pimpl_->dataAccess);
        for(    ConnectionsList::iterator itr = pimpl_->users.begin();
                itr != pimpl_->users.end();
                ++itr   ){
            if( itr->second != bar ){
                itr->second->send(packet);
            }
        }
    }

    void ConnectionGroup::sendAllBar(const inp::INFPacket& packet, const std::string& bar){
        ScopedMutexLock(pimpl_->dataAccess);
        for(    ConnectionsList::iterator itr = pimpl_->users.begin();
                itr != pimpl_->users.end();
                ++itr   ){
            if( itr->first != bar ){
                itr->second->send(packet);
            }
        }
    }
}
