#include "lsoda.h"

void unit() {}

//' Ordinary differential equation solver using lsoda
//' @param f R function with signature function(t,y)
//' @param y vector of initial state values
//' @param times vector of times -- including the start time
//' @param rtol double for the relative tolerance
//' @param atol double for the absolute tolerance
//' @return a matrix for times in the first column and the state values in the other columns. 
//' @export
// [[Rcpp::export]]
Rcpp::NumericMatrix ode(std::vector<double> y,
			std::vector<double> times,
			Rcpp::Function func,
			double rtol = 1e-6, double atol = 1e-6) {
  using Pair = std::pair<Rcpp::Function,int>;
  Pair pr = std::make_pair(func, y.size());
  return LSODA::ode(y, times, LSODA::lsoda_rfunctor_adaptor, (void*) &pr, rtol, atol);
}
  
