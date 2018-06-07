#include <ompl/control/ODESolver.h>
#include <ompl/control/Control.h>
#include <ompl/control/spaces/RealVectorControlSpace.h>
#include <ompl/base/spaces/SE2StateSpace.h>

#include "robotplanner.h"

namespace ob = ompl::base;
namespace oc = ompl::control;

#define TIMESTEP 10.0

double rad(double deg)
{
	return deg * 3.1415 / 180.0;
}


/* MOVEMENT MODELS */

// controls are simply the global x- and y-velocities
void pointMovements(
    const oc::ODESolver::StateType& q, 
    const oc::Control *ctrl, 
    oc::ODESolver::StateType& qdot)
{
    const double *u = 
        ctrl->as<oc::RealVectorControlSpace::ControlType>()->values;

    // zero out qdot
    qdot.resize(q.size(), 0);

    qdot[0] = u[0];
    qdot[1] = u[1];
    qdot[2] = 0;
}

// controls for a bicycle: u[0] = fwd speed, u[1] = steer angle
void bicycleMovements(
    const oc::ODESolver::StateType& q, 
    const oc::Control *ctrl, 
    oc::ODESolver::StateType& qdot)
{
    const double *u = 
        ctrl->as<oc::RealVectorControlSpace::ControlType>()->values;

    // zero out qdot
    qdot.resize(q.size(), 0);

    qdot[0] = u[0] * TIMESTEP * cos(q[2]);
    qdot[1] = u[0] * TIMESTEP * sin(q[2]);
    qdot[2] = u[0] * 0.1 * TIMESTEP * tan(u[1]);
}

// controls for a thymio: 
// u[0] = left wheel linear speed, u[1] = right wheel linear speed
void thymioMovements(
    const oc::ODESolver::StateType& q, 
    const oc::Control *ctrl, 
    oc::ODESolver::StateType& qdot)
{
    const double *u = 
        ctrl->as<oc::RealVectorControlSpace::ControlType>()->values;

    // zero out qdot
    qdot.resize(q.size(), 0);

    double l = 20.0;

    double timestep_modifier = 10.0;

    qdot[2] = 0.1 * TIMESTEP * (u[1] - u[0]) / (2 * l);

    double v = (u[0] + u[1]) / 2;

    if (u[1] - u[0] != 0)
    {
    	double R = l * (u[1] + u[0]) / (u[1] - u[0]);

	    double qdot0_loc = R * sin(v / R);
	    double qdot1_loc = R * -cos(v / R);

	    qdot[0] = TIMESTEP * qdot0_loc * cos(q[2]);
	    qdot[1] = TIMESTEP * qdot0_loc * sin(q[2]);
    }
    else {
    	qdot[0] = u[0] * TIMESTEP * cos(q[2]);
	    qdot[1] = u[1] * TIMESTEP * sin(q[2]);
    }
}


/* OBSTACLES (VALID STATE CHECKERS) */

bool boundingBox(const ob::State *state)
{
    const ob::SE2StateSpace::StateType& pos = *state->as<ob::SE2StateSpace::StateType>();

    double x = pos.getX();
    double y = pos.getY();

    return (x >= 20 && x <= 640 && y >= -420 && y <= -20);
}

bool outside_circle(double x, double y, double cx, double cy, double r)
{
	return (x - cx) * (x - cx) + (y - cy) * (y - cy) > r * r;
}

bool outside_rect(double x, double y, double sx, double sy, double ex, double ey)
{
	return !(x >= sx && x <= ex && y >= sy && y <= ey);
}

bool manyObstacles(const ob::State *state)
{
    const ob::SE2StateSpace::StateType& pos = *state->as<ob::SE2StateSpace::StateType>();

    double x = pos.getX();
    double y = pos.getY();

    // also always include bounding box
    return boundingBox(state)
    
    	&& outside_rect(x, y, 100, -200, 200, -100)
    	&& outside_rect(x, y, 300, -300, 400, -200)
    	&& outside_circle(x, y, 150, -350, 50)
    	&& outside_circle(x, y, 550, -150, 50)
    	&& outside_circle(x, y, 500, -300, 50);
    	
    	//&& outside_rect(x, y, 100, -500, 300, -50);
}

bool cornerObstacle(const ob::State *state)
{
    const ob::SE2StateSpace::StateType& pos = *state->as<ob::SE2StateSpace::StateType>();

    double x = pos.getX();
    double y = pos.getY();


    // also always include bounding box
    bool val = boundingBox(state)
    	&& outside_rect(x, y, 300, -300, 700, 0);

    return val;
}

bool parallel_parking(const ob::State *state)
{
    const ob::SE2StateSpace::StateType& pos = *state->as<ob::SE2StateSpace::StateType>();

    double x = pos.getX();
    double y = pos.getY();


    // also always include bounding box
    bool val = boundingBox(state)
    	&& outside_rect(x, y, 597, -210, 700, 0)
    	&& outside_rect(x, y, 597, -500, 700, -240);

    return val;
}



int main()
{
	

	// default:
	// resolution: 0.001
	// planner: RRT
	// optimization: none
	// goal bias: 0.2
	// time: 20.0 s
	/*
	RobotPlanner bicyclePlanner;

	bicyclePlanner.setMovements(bicycleMovements);
	bicyclePlanner.setSpaceBounds(20, 640, -420, -20);
	//bicyclePlanner.setControlSpaceBounds(0, 20, rad(0), rad(30));
	bicyclePlanner.setControlSpaceBounds(-20, 20, rad(20), rad(30));
	bicyclePlanner.setObstacles(boundingBox);
	bicyclePlanner.setResolution(0.001);
	bicyclePlanner.setStartAndGoal(50, -50, 0, 600, -400, 0);
	bicyclePlanner.setPlanner(RRT); // values: RRT or KPIECE1
	
	//bicyclePlanner.setOptimizationObjective(PATH_LENGTH);
	
	//bicyclePlanner.setRRTGoalBias(0.5); // default = 0.05

	bicyclePlanner.plan(60.0);
	*/
	
	
	
	RobotPlanner thymioPlanner;

	thymioPlanner.setMovements(thymioMovements);
	thymioPlanner.setSpaceBounds(20, 640, -420, -20);
	thymioPlanner.setControlSpaceBounds(0, 10, -10, 10);
	thymioPlanner.setObstacles(manyObstacles);
	thymioPlanner.setResolution(0.001);
	thymioPlanner.setStartAndGoal(50, -50, 0, 600, -400, 0);
	//thymioPlanner.setStartAndGoal(50, -50, 0, 600, -225, 0);
	thymioPlanner.setPlanner(RRT); // values: RRT or KPIECE1

	thymioPlanner.plan(20.0);
	
	
}