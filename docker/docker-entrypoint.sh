#!/bin/bash

VEINS_ROOT="/opt/omnetpp-5.6.2"
export PATH="$PATH":"$VEINS_ROOT/bin"

cd /project
export MODE=relase
./configure
make -j$(nproc)

# run sumo
cd /opt/veins-5.2/
python3 sumo-launchd.py -vv sumo 2>&1 >> /tmp/sumo.log &

# run veins
cd /project/maps/istanbul-v1/
bash run_custom.sh
