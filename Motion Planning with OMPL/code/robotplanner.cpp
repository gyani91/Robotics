#include "robotplanner.h"

RobotPlanner::RobotPlanner()
{
	// create state space for robot to move in: (x, y, theta), i.e. SE(2)
    ob::StateSpacePtr se2 = std::make_shared<ob::SE2StateSpace>();

    // create control space on SE(2), with 2 dimensions for control
    oc::ControlSpacePtr cspace = std::make_shared<oc::RealVectorControlSpace>(se2, 2);

    // create simple setup for CONTROL, using control space
    ss_ = std::make_shared<oc::SimpleSetup>(cspace);
}

void RobotPlanner::postPropagate(
	const ob::State* /*state*/, 
    const oc::Control* /*control*/, 
    const double /*duration*/, 
    ob::State* result)
{
    // Normalize orientation value between 0 and 2*pi
    const ob::SO2StateSpace* SO2 = 
        ss_->getStateSpace()->as<ob::SE2StateSpace>()
     ->as<ob::SO2StateSpace>(1);

    auto* so2 = 
        result->as<ob::SE2StateSpace::StateType>()
     ->as<ob::SO2StateSpace::StateType>(1);
     
    SO2->enforceBounds(so2);
 }

void RobotPlanner::setMovements(ODEFunction ode)
{
	oc::ODESolverPtr odeSolver = std::make_shared<oc::ODEBasicSolver<>>(
        // si_ = SpaceInformation object
        ss_->getSpaceInformation(), 
        [=](
            const oc::ODESolver::StateType& q, 
            const oc::Control *ctrl, 
            oc::ODESolver::StateType& qdot)
         {
             ode(q, ctrl, qdot);
         });

    ss_->getSpaceInformation()->setStatePropagator(oc::ODESolver::getStatePropagator(
        odeSolver,
        [=](
            const ob::State* state, 
            const oc::Control* control,
            const double duration, 
            ob::State* result
        ) {
            postPropagate(state, control, duration, result);
        }));
}


void RobotPlanner::setObstacles(const ob::StateValidityCheckerFn &svc)
{
	ss_->setStateValidityChecker(svc);
}

void RobotPlanner::setResolution(double resolution)
{
	ss_->getSpaceInformation()->setStateValidityCheckingResolution(resolution);
}

void RobotPlanner::setSpaceBounds(double xlow, double xhigh, double ylow, double yhigh)
{
	// set the bounds of the x and y of the state space (not theta)
    ob::RealVectorBounds bounds(2);
    // bounds of x
    bounds.setLow(0, xlow);
    bounds.setHigh(0, xhigh);
    // bounds of y
    bounds.setLow(1, ylow);
    bounds.setHigh(1, yhigh);
    ss_->getStateSpace()->as<ob::SE2StateSpace>()->setBounds(bounds);
}

void RobotPlanner::setControlSpaceBounds(double c1low, double c1high, double c2low, double c2high)
{
	// set the bounds of the control space (both dimensions)
    ob::RealVectorBounds cbounds(2);
    cbounds.setLow(0, c1low);
    cbounds.setHigh(0, c1high);
    cbounds.setLow(1, c2low);
    cbounds.setHigh(1, c2high);
    ss_->getControlSpace()->as<oc::RealVectorControlSpace>()->setBounds(cbounds);
}

void RobotPlanner::setPlanner(planners planner)
{
    planner_type_ = planner;
	if (planner == RRT)
	{
		std::shared_ptr<oc::RRT> planner = std::make_shared<oc::RRT>(ss_->getSpaceInformation());
        ss_->setPlanner(planner);
	}
	else if (planner == KPIECE1)
	{
		std::shared_ptr<oc::KPIECE1> planner = std::make_shared<oc::KPIECE1>(ss_->getSpaceInformation());
        ss_->setPlanner(planner);
	}
	else {
		std::cout << "Unknown planner" << std::endl;
		exit(1);
	}
}

void RobotPlanner::setStartAndGoal(double sx, double sy, double stheta, double gx, double gy, double gtheta)
{
	ob::ScopedState<ob::SE2StateSpace> start(ss_->getStateSpace());
    start->setX(sx);
    start->setY(sy);
    start->setYaw(stheta);

    ob::ScopedState<ob::SE2StateSpace> goal(ss_->getStateSpace());
    goal->setX(gx);
    goal->setY(gy);
    goal->setYaw(gtheta);

    ss_->setStartAndGoalStates(start, goal);
}

void RobotPlanner::setOptimizationObjective(opt_objectives opt)
{
    if (opt == PATH_LENGTH)
    {
        ob::OptimizationObjectivePtr opt = std::make_shared<ob::PathLengthOptimizationObjective>(ss_->getSpaceInformation());

        ss_->setOptimizationObjective(opt);
    }
    else
    {
        std::cout << "Unknown optimization objective" << std::endl;
    }
}

void RobotPlanner::setRRTGoalBias(double bias)
{
    if (planner_type_ == RRT)
    {
        ob::PlannerPtr planner = ss_->getPlanner();
        planner->as<oc::RRT>()->setGoalBias(bias);
    }
}

void RobotPlanner::plan(double time)
{
	ob::ProblemDefinitionPtr pd = ss_->getProblemDefinition();
    std::cout << "has optimization objective: " << pd->hasOptimizationObjective() << std::endl;

    if (planner_type_ == RRT)
    {
        ob::PlannerPtr planner = ss_->getPlanner();
        double bias = planner->as<oc::RRT>()->getGoalBias();
        std::cout << "goal bias: " << bias << std::endl;
    }

    ss_->setup();

    ob::PlannerStatus solved = ss_->solve(time);
    if (solved)
    {
        std::cout << "Found solution:" << std::endl;
        og::PathGeometric geompath = ss_->getSolutionPath().asGeometric();
        geompath.interpolate();

        geompath.printAsMatrix(std::cout);
        std::cout << "The above path is also printed to path.txt" << std::endl;

        std::ofstream pathfile;
        pathfile.open("path.txt");
        geompath.printAsMatrix(pathfile);
        pathfile.close();
    }
    else
    {
        std::cout << "No solution found" << std::endl;
    }
}

void RobotPlanner::benchmark()
{
	ot::Benchmark::Request request(20., 10000., 2); // runtime (s), memory (MB), run count

    ot::Benchmark b(*ss_);
    b.addPlanner(std::make_shared<oc::RRT>(ss_->getSpaceInformation()));
    b.addPlanner(std::make_shared<oc::KPIECE1>(ss_->getSpaceInformation()));
    b.benchmark(request);
    b.saveResultsToFile();
}