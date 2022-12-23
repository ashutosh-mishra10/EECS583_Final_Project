#!/bin/bash

/usr/local/bin/opt \
 -polly-process-unprofitable \
 -polly-dependences \
 -analyze \
 -GetStride \
 example.preopt.ll \