#include "lsoda.h"

void unit() {}

namespace LSODA {

  void lsoda_rfunctor_adaptor(double t, double* y, double* ydot, void* data) {
    using Tuple = std::tuple<Rcpp::Function, size_t, size_t>;
    Tuple* tuple = static_cast<Tuple*>(data);
    Rcpp::Function f = std::get<0>(*tuple);
    size_t neq = std::get<1>(*tuple);
    size_t nout = std::get<2>(*tuple);
    std::vector<double> yv(neq);
    std::copy(y,y+neq,yv.begin());
    Rcpp::List vals = Rcpp::as<Rcpp::List>(f(t,yv));
    std::vector<double> ydotv = Rcpp::as<std::vector<double> >(vals[0]);
    std::copy(ydotv.begin(),ydotv.end(),ydot);
    if (vals.size() > 1 && nout > neq) {
      std::vector<double> yresv = Rcpp::as<std::vector<double> >(vals[1]);
      std::copy(yresv.begin(),yresv.end(),ydot+neq);
    }
  }

} // namespace LSODA

//' Ordinary differential equation solver using lsoda
//' @param y vector of initial state values
//' @param times vector of times -- including the start time
//' @param func R function with signature function(t,y) that returns a
//'  list. The first list element is a vector for dy/dt. The second list
//'  elements, if it exists, is a vector of result calculations to be retained.
//' @param rtol double for the relative tolerance
//' @param atol double for the absolute tolerance
//' @return a matrix for times in the first column and the state andd results values in the other columns.
//' @examples
//'   times = c(0,0.4*10^(0:10))
//'  y = c(1,0,0)
//'  func = function(t,y,...) {
//'      ydot = rep(0,3)
//'      ydot[1] = 1.0E4 * y[2] * y[3] - .04E0 * y[1]
//'      ydot[3] = 3.0E7 * y[2] * y[2]
//'      ydot[2] = -1.0 * (ydot[1] + ydot[3])
//'      list(ydot, sum(y))
//'  }
//'  lsoda::ode(y,times,func, rtol=1e-8, atol=1e-8)
//' @export
// [[Rcpp::export]]
Rcpp::NumericMatrix ode(std::vector<double> y,
			std::vector<double> times,
			Rcpp::Function func,
			double rtol = 1e-6, double atol = 1e-6) {
  using namespace Rcpp;
  using Tuple = std::tuple<Function,size_t,size_t>;
  List vals = as<List>(func(times[0],y));
  size_t nres = (vals.size() > 1) ? (as<std::vector<double> >(vals[1])).size() : 0;
  Tuple pr = std::make_tuple(func, y.size(), y.size()+nres);
  return LSODA::ode(y, times, LSODA::lsoda_rfunctor_adaptor, y.size()+nres,
		    (void*) &pr, rtol, atol);
}
