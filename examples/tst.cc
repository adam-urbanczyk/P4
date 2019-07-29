// Author: Tucker Haydon

#include <cstdlib>

#include "polynomial_solver.h"
#include "gradient.h"

using namespace p4;

int main(int argc, char** argv) {

  // Set up example problem
  const std::vector<double> initial_times = {1.0, 6.0, 11.0};
  const std::vector<NodeEqualityBound> node_equality_bounds = {
    // NodeEqualityBound(dimension_idx, node_idx, derivative_idx, value)
    // Constraining position, velocity, and acceleration of first node to zero
    NodeEqualityBound(0,0,0,0),
    NodeEqualityBound(0,0,1,0),
    NodeEqualityBound(0,0,2,0),

    // Constraining position of second node
    NodeEqualityBound(0,1,0,1),

    // Constraining position, velocity, and acceleration of third node
    NodeEqualityBound(0,2,0,2.5),
    NodeEqualityBound(0,2,1,0),
    NodeEqualityBound(0,2,2,0),
  };
  const std::vector<NodeInequalityBound> node_inequality_bounds = {
  // NodeInequalityBound(dimension_idx, node_idx, derivative_idx, lower, upper)
  };
  const std::vector<SegmentInequalityBound> segment_inequality_bounds = {
    // SegmentInequalityBound(segment_idx, derivative_idx, mapping, value)
    SegmentInequalityBound(0,1,(Eigen::Matrix<double,1,1>() << 1).finished(),1),
    SegmentInequalityBound(0,2,(Eigen::Matrix<double,1,1>() << 1).finished(),1),

    SegmentInequalityBound(1,1,(Eigen::Matrix<double,1,1>() << 1).finished(),1),
    SegmentInequalityBound(1,2,(Eigen::Matrix<double,1,1>() << 1).finished(),1),
  };

  PolynomialSolver::Options solver_options;
  solver_options.num_dimensions   = 1;
  solver_options.polynomial_order = 7;
  solver_options.derivative_order = 3;
  solver_options.continuity_order = 2;
  solver_options.osqp_settings.scaling    = 0;
  solver_options.osqp_settings.polish     = 1;
  solver_options.osqp_settings.warm_start = 0;

  auto solver = std::make_shared<PolynomialSolver>(solver_options);
  solver->Setup(
          initial_times,
          node_equality_bounds,
          node_inequality_bounds,
          segment_inequality_bounds);
  PolynomialSolver::Solution solver_solution = solver->Run(initial_times);

  PolynomialGradient gradient;
  gradient.Run(
      initial_times,
      solver,
      solver_solution);
  return EXIT_SUCCESS;
}
