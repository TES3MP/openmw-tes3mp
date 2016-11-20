#ifndef OPENMW_WORLDEVENT_HPP
#define OPENMW_WORLDEVENT_HPP

#include <components/esm/loadcell.hpp>
#include <components/esm/cellref.hpp>
#include <RakNetTypes.h>

namespace mwmp
{
    class WorldEvent
    {
    public:

        WorldEvent(RakNet::RakNetGUID guid) : guid(guid)
        {

        }

        WorldEvent()
        {

        }

        RakNet::RakNetGUID guid;

        ESM::Cell cell;
        ESM::CellRef cellRef;
        ESM::Position pos;
        
        int count;
        int state;
        int lockLevel;
        float scale;

        std::string filename;
        bool allowSkipping;

        std::string animGroup;
        int animMode;

        int index;
        int shortVal;
        float floatVal;
        std::string varName;
    };
}

#endif //OPENMW_WORLDEVENT_HPP
