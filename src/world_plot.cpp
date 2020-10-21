#include "world_plot.hpp"
#include "world.hpp"

bool WorldPlot::isPathClear( const Graph_Box<_box>* origin, const directionalPath& path ) const{
    const Graph_Box<_box> *temp{ origin };

    for ( uint32_t i = 0; i<path.size(); ++i ){
        temp = temp->get_adj_box(path[i]);
        if( ! temp->getData().entities.empty() )    return false;
    }
    
    return true;
}

WorldPlot::WorldPlot(const World* world): Square_Matrix(statics::init_Bound), parent_world(world){
    
}

void WorldPlot::expand(){   //expands one unit on each side
    this->__temp.free_space_ratio = this->parent_world->getFreeSpace())/this->getOrder();
    if( __temp.time_since_speed_updated >= 10 ){
        --__temp.expansion_speed;
        __temp.time_since_speed_updated = 0;

            // @future [Oct22] - The below does the speed resetting part, change it to allow negative expansions
        if(__temp.expansion_speed <= 0) __temp.expansion_speed = statics::init_expansion_speed;
    }

    if ( (this->__temp.free_space_ratio > statics::max_free_space ){
        // @log - world doesn't need to expand since reached max_free_space
        return;
    }else if ( this->__temp.free_space_ratio < statics::min_free_space )
    {
        ++__temp.time_since_speed_update;
        ++__temp.expansion_speed;   // @log increasing the expansion speed
    }

    this->__expand_n_units(__temp.expansion_speed);

}

void WorldPlot::__expand_n_units(int8_t n){    //to be used when there's rate
    this->resize(this->getOrder() + n);
}

int32_t WorldPlot::getCurrentOrder(){  //size
    return this->getOrder();
}

directionalPath WorldPlot::getShortestPathToFood(const Graph_Box<_box>* origin) const{
    directionalPath shortest_path;
    this->getShortestPathToFood(std::forward(origin), shortest_path);

    return shortest_path;
}

void WorldPlot::getShortestPathToFood(const Graph_Box<_box>* origin, directionalPath& old_path) const{
    old_path.clear();   //the old path will be cleared !! If you wish to be more optimised, call WorldPlot::isPathClear() before calling this function

    // @todo - Apply the BFS here
    // @note - snake class expects the path to be like a stack, ie. the first move, will be the last one, which will be popped, then move_on

    return {};
}
