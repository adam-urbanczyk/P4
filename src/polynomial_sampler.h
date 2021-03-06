// Author: Tucker Haydon

#pragma once

#include <Eigen/Dense>
#include <vector>

#include "polynomial_solver.h"

namespace p4 {
  // Class that samples the piecewise polynomial solution.
  class PolynomialSampler {
    public:
      // Options to configure the sampler with
      struct Options {
        // Frequency of samples per second
        double frequency = 20;

        // Order of the derivative to sample. 0 = pos, 1 = vel, ...
        size_t derivative_order = 0;

        Options(){}
      };

      // Constructor
      PolynomialSampler(const Options& options = Options())
        : options_(options) {}

      // Run the sampler. Requires a vector of times corresponding to the
      // polynomial waypoints, and the piecewise polynomial solution. Returns an
      // Eigen matrix containing samples. Rows indicate the dimension, cols
      // indicate the sample index. The first element in each column is the
      // sample time.
      Eigen::MatrixXd Run(
          const std::vector<double>& times, 
          const PolynomialSolver::Solution& solution);

    private:
      Options options_;

  };
}
