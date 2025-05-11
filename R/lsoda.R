#' @description 
#' To learn more about `libsoda-cxx` visit the [website](https://github.com/dilawar/libsoda-cxx).
#'
#' @name lsoda
#' @useDynLib lsoda
#' @importFrom Rcpp evalCpp
#' @keywords internal
"_PACKAGE"

#' Ordinary differential equation solver using lsoda
#' @param y vector of initial state values
#' @param times vector of times -- including the start time
#' @param times vector of times -- including the start time
#' @param parms list or vector of parameters that are pass to func
#' @param func R function with signature function(t,y,parms,...) that returns a
#'  list. The first list element is a vector for dy/dt. The second list
#'  elements, if it exists, is a vector of result calculations to be retained.
#' @param rtol double for the relative tolerance
#' @param atol double for the absolute tolerance
#' @param ... other parameters that are passed to func
#' @return a matrix for times in the first column and the state andd results values in the other columns.
#' @examples
#'  times = c(0,0.4*10^(0:10))
#'  y = c(1,0,0)
#'  func = function(t,y,parms,b=-0.04E0) {
#'      ydot = rep(0,3)
#'      ydot[1] = parms$a * y[2] * y[3] + b * y[1]
#'      ydot[3] = 3.0E7 * y[2] * y[2]
#'      ydot[2] = -1.0 * (ydot[1] + ydot[3])
#'      list(ydot, sum(y))
#'  }
#'  lsoda::ode(y, times, func, parms=list(a=1.0E4), rtol=1e-8, atol=1e-8)
#' @export
ode = function(y, times, func, parms, rtol=1e-6, atol=1e-6, ...) {
    lsoda::ode_cpp(y, times, func = function(t,y) func(t,y,parms, ...),
                   rtol=rtol, atol=atol)
}
