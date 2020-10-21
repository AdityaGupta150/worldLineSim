/*
FOR THE WORLD TO BE DYNAMICALLY GROWING ->
    We need the logica; matrix like structure but list like additions too...  if that's possible

    ACCOMPLISHED -> That is what i wrote the Graph_Mat for !!
*/

#pragma once

#include "log.hpp"
#include "snake.hpp"
#include "world_plot.hpp"

#include <vector>
#include <utility>
#include <unordered_map>

enum class Events{  //for logging puposes
    ENTITY_BIRTH,
    ENTITY_DEATH,
    ENTITY_MOVE,
    ENTITY_GROW,
    NEW_WORLD,
    WORLD_PAUSE,
    // WORLD_RESUME,   //won't be used now
        //BIG_BANG happens only once, so not must to include
};

typedef std::shared_ptr<World> World_Ptr;
class World{
    typedef _coord<int32_t> coord_type;
public:
    coord_type food;    //food can have different points/nutitional values too in 'future'
    _timePoint currentTime;

    void resumeWorld();
    void pauseWorld();
    void ateFood(const Snake*); //which snake ate it, log it, then randomize the food again

    //------constants for this world------//
    int _MAX_NumSnakes = 4;
    int _init_SnakeLength = 2;
    //------                ------//

    const std::pair<uint16_t, uint16_t>& get_curr_bounds() const;
    bool isCellEmpty( const coord_type& ) const;

    // --Just abstracted access to private worldPlot member function, no logic in these of their own-- //
    bool isPathClear( const Graph_Box<_box>* origin, const directionalPath& path ) const;
    directionalPath getShortestPathToFood( const Graph_Box<_box>* origin ) const;
    void getShortestPathToFood( const Graph_Box<_box>* origin, directionalPath& ) const;
    // x-Just abstracted access to private worldPlot member function, no logic in these of their own-x //


    World( const World_Ptr, _timePoint );  //can later be made private

private:
    std::vector<Log> logs;
    State currentState;
    bool simulationRunning;

    std::pair<uint16_t, uint16_t> _WorldDimens; //current dimensions of this world
    std::pair<uint16_t, uint16_t> _curr_BOUNDS; //current `reserved` dimensions of this world

    WorldPlot world_plot;    // @todo - Will be 3D in future

    std::vector< Snake > snakes;

    bool _CheckBounds();    //for checking `need` to increase size
    void _Expand(uint16_t);

    bool _RangeCheck(const coord_type&) const;    //for checking if a coordinate is valid

    void createFood();
    void runNextUnitTime();   //resumes the world, the nextTime period happens in this time
    // void resumeSimulation();
    void stopSimulation();
    World();

    friend class Verse;
    friend class PathFinder;

};
