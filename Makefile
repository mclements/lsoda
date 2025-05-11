R = R-devel

check:
	$(R) CMD build .
	$(R) CMD check --as-cran lsoda_`awk '/Version:/ {print $$2}' DESCRIPTION`.tar.gz

build:
	$(R) CMD build .

install:
	$(R) CMD INSTALL .
