// Author: Tucker Haydon

#include <cstdlib>

#include <Eigen/Dense>
#include "gnuplot-iostream.h"

#include "polynomial_solver.h"
#include "polynomial_sampler.h"

using namespace mediation_layer;

int main(int argc, char** argv) {

  const std::vector<double> times = {0, 2, 4, 7};

  // Equality bounds paramater order is:
  // 1) Dimension index
  // 2) Node index
  // 3) Derivative index
  // 4) Value
  const std::vector<NodeEqualityBound> equality_bounds = {
    // The first node must constrain position, velocity, and acceleration
    NodeEqualityBound(0,0,0,0),
    NodeEqualityBound(0,0,1,1),
    NodeEqualityBound(0,0,2,0),

    // The second node constrains position
    NodeEqualityBound(0,1,0,2),

    // The third node constrains position
    NodeEqualityBound(0,2,0,8),

    // The fourth node constrains position
    NodeEqualityBound(0,3,0,10),
  };

  const std::vector<SegmentInequalityBound> segment_inequality_bounds = {
    SegmentInequalityBound(0,1,Eigen::Vector3d(1,0,0), 3),
    SegmentInequalityBound(1,1,Eigen::Vector3d(1,0,0), 3),
    SegmentInequalityBound(2,1,Eigen::Vector3d(1,0,0), 3),
  };

  PolynomialSolver::Options solver_options;
  solver_options.num_dimensions = 1;     // 3D
  solver_options.polynomial_order = 6;   // Fit an 7th-order polynomial
  solver_options.continuity_order = 1;   // Require continuity to the 4th order
  solver_options.derivative_order = 2;   // Minimize the 4th order (snap)
  solver_options.polish = true;          // Polish the solution

  PolynomialSolver solver(solver_options);
  const PolynomialPath path
    = solver.Run(times, equality_bounds,{},segment_inequality_bounds);

  { // Plot position profiles
    PolynomialSampler::Options sampler_options;
    sampler_options.frequency = 100;
    sampler_options.derivative_order = 0;

    PolynomialSampler sampler(sampler_options);
    Eigen::MatrixXd samples = sampler.Run(times, path);

    std::vector<double> t_hist, x_hist, y_hist, z_hist;
    for(size_t time_idx = 0; time_idx < samples.cols(); ++time_idx) {
      t_hist.push_back(samples(0,time_idx));
      x_hist.push_back(samples(1,time_idx));
    }

    Gnuplot gp;
    gp << "plot '-' using 1:2 with lines title 'X-Position'";
    gp << std::endl;
    gp.send1d(boost::make_tuple(t_hist, x_hist));
    gp << "set grid" << std::endl;
    gp << "replot" << std::endl;
  }

  // Sampling and Plotting
  { // Plot acceleration profiles
    PolynomialSampler::Options sampler_options;
    sampler_options.frequency = 100;
    sampler_options.derivative_order = 2;

    PolynomialSampler sampler(sampler_options);
    Eigen::MatrixXd samples = sampler.Run(times, path);

    std::vector<double> t_hist, x_hist, y_hist, z_hist;
    for(size_t time_idx = 0; time_idx < samples.cols(); ++time_idx) {
      t_hist.push_back(samples(0,time_idx));
      x_hist.push_back(samples(1,time_idx));
    }

    Gnuplot gp;
    gp << "plot '-' using 1:2 with lines title 'X-Acceleration'";
    gp << std::endl;
    gp.send1d(boost::make_tuple(t_hist, x_hist));
    gp << "set grid" << std::endl;
    gp << "replot" << std::endl;
  }

  { // Plot velocity profiles
    PolynomialSampler::Options sampler_options;
    sampler_options.frequency = 100;
    sampler_options.derivative_order = 1;

    PolynomialSampler sampler(sampler_options);
    Eigen::MatrixXd samples = sampler.Run(times, path);

    std::vector<double> t_hist, x_hist, y_hist, z_hist;
    for(size_t time_idx = 0; time_idx < samples.cols(); ++time_idx) {
      t_hist.push_back(samples(0,time_idx));
      x_hist.push_back(samples(1,time_idx));
    }

    Gnuplot gp;
    gp << "plot '-' using 1:2 with lines title 'X-Velocity'";
    gp << std::endl;
    gp.send1d(boost::make_tuple(t_hist, x_hist));
    gp << "set grid" << std::endl;
    gp << "replot" << std::endl;
  }





  return EXIT_SUCCESS;
}