#!/usr/bin/env bash

cd build && \
make && make test

lcov --capture --directory src/ --output-file coverage.info
# remove output for external libraries
lcov --remove coverage.info "/usr*" -o coverage.info
lcov --remove coverage.info "lib/*" -o coverage.info
lcov --remove coverage.info "*.h" -o coverage.info
genhtml coverage.info --output-directory out

xdg-open out/index.html
