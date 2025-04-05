#include "lsoda.h"

void unit() {}

// [[Rcpp::export]]
Rcpp::NumericMatrix ode(Rcpp::Function f, std::vector<double> y,
			std::vector<double> ts,
			double rtol, double atol) {
  using Pair = std::pair<Rcpp::Function,int>;
  Pair pr = std::make_pair(f,y.size());
  return LSODA::ode(LSODA::lsoda_rfunctor_adaptor, y, ts, (void*) &pr, rtol, atol);
}
  
