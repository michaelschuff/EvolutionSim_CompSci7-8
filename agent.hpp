#ifndef agent_hpp
#define agent_hpp

#include "vector3.hpp"

class agent {
public:
    vector3 position;
    vector3 velocity;

    static int nextID;
    int id; //individual agent id
    //vector<int> sight (vector<fish>&, int); //see the fish around them and determine which ones are in a distance of them

protected:
    agent(vector3 pos, vector3 vel) : position(pos), velocity(vel), id(nextID) {
        nextID++;
    } //agent constructor takes in two vector3s to create instance of agent
};

/*
vector<int> agent::sight(vector<fish> &fishList, int maxDist) {

    vector<int> fishWork;

    for (int f = 0; f < fishList.size(); f++) {

        //use 3D Pythagorean theorem to find the radius value
        float actualRadius = (fishList[f].position - position).magnitude();

        //if it's less than r add that fish's id to the list
        if (actualRadius <= maxDist) {
            fishWork.push_back(fishList[f].id);
        }
    }

    return fishWork;
}
*/
#endif /* agent_hpp */
