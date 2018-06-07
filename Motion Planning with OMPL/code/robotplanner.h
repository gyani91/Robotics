#include <ompl/base/SpaceInformation.h>
#include <ompl/geometric/planners/prm/PRM.h>
#include <ompl/geometric/SimpleSetup.h>
#include <boost/math/constants/constants.hpp>
#include <ompl/base/spaces/SE2StateSpace.h>
#include <ompl/control/ODESolver.h>
#include <ompl/control/spaces/RealVectorControlSpace.h>
#include <ompl/control/SimpleSetup.h>
#include <ompl/control/SpaceInformation.h>
#include <ompl/control/planners/rrt/RRT.h>
#include <ompl/control/planners/kpiece/KPIECE1.h>
#include <ompl/base/State.h>
#include <ompl/base/StateValidityChecker.h>
#include <ompl/base/MotionValidator.h>
#include <ompl/control/Control.h>
#include <ompl/base/ProblemDefinition.h>
#include <ompl/base/objectives/PathLengthOptimizationObjective.h>
#include <ompl/base/OptimizationObjective.h>

#include <ompl/tools/benchmark/Benchmark.h>

#include <ompl/config.h>
#include <functional>
#include <iostream>
#include <fstream>

namespace ob = ompl::base;
namespace og = ompl::geometric;
namespace oc = ompl::control;
namespace ot = ompl::tools;

enum planners { RRT, KPIECE1 };
enum opt_objectives { PATH_LENGTH };

typedef std::function< void(const oc::ODESolver::StateType& q, const oc::Control *ctrl, oc::ODESolver::StateType& qdot)> ODEFunction;

class RobotPlanner
{
public:
	RobotPlanner();

	void ode(const oc::ODESolver::StateType& q, const oc::Control *ctrl, oc::ODESolver::StateType& qdot);

	void postPropagate(const ob::State*, const oc::Control*, const double, ob::State* result);

	void benchmark(oc::SimpleSetupPtr ss);

	void plan(double time);
	void benchmark();

	void setSpaceBounds(double xlow, double xhigh, double ylow, double yhigh);

	void setControlSpaceBounds(double c1low, double c1high, double c2low, double c2high);

	void setObstacles(const ob::StateValidityCheckerFn &svc);
	void setResolution(double resolution);

	void setMovements(ODEFunction ode);

	void setPlanner(planners planner);

	void setStartAndGoal(double sx, double sy, double stheta, double gx, double gy, double gtheta);

	void setOptimizationObjective(opt_objectives opt);
	void setRRTGoalBias(double bias);

private:
	oc::SimpleSetupPtr ss_;
	planners planner_type_;
};