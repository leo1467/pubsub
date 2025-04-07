#!/usr/bin/env bash

DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

cd ${DIR}/Tools/inih/src &&     make clean; make release BUILD_TYPE=static install
cd ${DIR}/Tools/iniTools/src && make clean; make release BUILD_TYPE=static install
cd ${DIR}/src &&                make clean; make release BUILD_TYPE=static install
