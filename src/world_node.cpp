#include "world.hpp"
#include "world_node.hpp"
#include "display/display.hpp"
#include "adapter.hpp"

void World_Node::capture_state(){
    this->states.push_back(State(this));
}

const StatePlusPlus World_Node::return_data(){
    return {
        states.back(),
        this->_id
    };
}

// this is to be provided to the user to edit how the `current` state should be, this option creates a new worldLine for future
// this returns `current state`
const State& World_Node::return_state(){
    return states[states.size() - 1];   // or simply states.back(), but leave so as to have better understanding of the two overloads
}

// this is to be provided to the user to edit how the `current` state should be, this option creates a new worldLine for future
// this returns `past state` time t_back back into the past
const State& World_Node::return_state(_timePoint t_back){   // this only `returns` the current state of 
    if( t_back < states.size() ){ // @caution - Can be concurrently accessing the size and in other thread pushing back state to it
        if( !parent_node ){
            return parent_node->return_state(t_back - states.size());
        } else{
            if( t_back == 0 ){
                throw std::invalid_argument("You are going 0 seconds before the BIG BANG, ie. TO THE BIG BANG, Sorry Can;t handle that now :-) ");
            }
            throw std::invalid_argument("Can't go to even before the BIG BANG. Soooorrry for that :D ");
        }
    }

    return this->states[states.size() - 1 - t_back];    // we are seeing from the reverse, thats why we did -t_back, and we had to -1, since what we receive is 1-based index type something (think logically by drawing a stack of states at t = t` for all t` belonging to {0,..., latest(ie. states.size())})
}

// @START_HERE -> LAST TIME I WAS COMPLETETING IMPLEMENTING WORLD_TREE, WITH WORLD_NODE TAKING CHARGE OF MANAGING ALL SIMULATION RELATED TASK OF THE WORLD IT HOLDS A POINTER TO, during doing the latter, i also created the functions return_state and handle_pause, do read the notes given for handle_pause later on
// actual on screen handling of pause to be done by verse, as well as the display, and then calling return_state(), then asking user to modify it, and then that modified state should be passed to this handle_pause()
void World_Node::handle_pause(State that_state){    // the time of pause to be deduced from the time of current world
    // @note - stop_simulation should be called before this handle_pause, likely by the Verse itself, because this function will actually just do the work while the actual world has been `kind of` paused (but at same time it should keep running, or pause and resume when created the new node that continues with this world itself)
    this->paused_time = this->current_time;

    // @todo - Cr5eating a new world here too, and also a node with this same world
    this->left_node = new World_Node(this->tree, this, this->paused_time, true); //this will be almost the copy of this node, with the same world pointer, just starting with an empty states vector, and that it's paused_time will also not be there (ie. by default 0, meaning the world_node has an active world currently running)

}

    // should be on a different thread than main thread
void World_Node::start_state_management(){
    while( this->world->_shared_concurrent_data.is_world_running() ){
        ++this->current_time;
        this->capture_state();   // can PAUSE then capture, for i am preferring some wrong coords in state, rather than stopping and starting threads again and again
        this->adapter->update();

        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(statics::UNIT_TIME * 1000)));
    }
}

void World_Node::stop_WorldSimulation(){
    this->world->_shared_concurrent_data.reset_world_running();
}

/**This will pause the threads for the time being, and update the time, and states vector, then create a new node with the same World* pointer and the world_id */
void World_Node::pauseWorld(){
    // @todo @most_important
}

const World_Ptr World_Node::get_world() const{
    return this->world;
}

const _timePoint World_Node::getCurrentTime() const
{
    return this->current_time;
}

const dimen_t World_Node::get_world_order() const{
    return this->world->getWorldDimen();
}

const coord& World_Node::get_exact_dimen() const {
    return this->world->get_dimensions();
}

// @note - Be sure you have ALL respective arguments as taken by the World class constructor, since the node itself will need them to construct a new world
World_Node::World_Node( World_Tree* tree, World_Node* parent_node, _timePoint t, bool is_continued) : 
    continued_world(is_continued),
    current_time(t)
{
    if( is_continued ){
        this->world = new World( parent_node->world, t );	// starting from parent_node->world
    }else if(parent_node != nullptr)
    {
        this->world = parent_node->world;
    }
    else
    {
        // ie. the root node... create a new world
        this->world = new World();
    }

    this->paused_time = current_time;  //initially

    this->tree = tree;
    this->world_id = this->world->_id;

    this->adapter = tree->access_disp_manager()->add_node_adapter(this);
    std::thread( &World_Node::start_state_management, this ).detach();
}

World_Node::~World_Node(){
    if( !this->continued_world )
        delete this->world;
}
