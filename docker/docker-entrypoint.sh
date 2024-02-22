#!/bin/bash
if [ ! -z "$(ls -A /tmp/veins-output)" ]; then
    echo "ERROR: output folder not empty"
    exit 1
fi

if [ "$ENABLE_KSM" == "1" ]; then
    rm -rf /tmp/ksm
    mkdir /tmp/ksm
    cd /tmp/ksm
    pwd
    git clone https://github.com/unbrice/ksm_preload.git
    ls
    cd /tmp/ksm/ksm_preload
    cmake .
    make install
    export LD_PRELOAD=/usr/local/share/ksm_preload/libksm_preload.so
fi
echo "KSM LD_PRELOAD enabled"

VEINS_ROOT="/opt/omnetpp-5.6.2"
export PATH="$PATH":"$VEINS_ROOT/bin"

cd /project
export MODE=relase
./configure
make -j$(nproc)

# run sumo
cd /opt/veins-5.2/
python3 sumo-launchd.py -vv sumo 2>&1 >> /tmp/veins-output/sumo.log &

# run veins
# cd /project/maps/istanbul-v1/

# copy source config and map config to out directory
mkdir /tmp/veins-output/config
cd /project
env > /tmp/veins-output/config/env.txt
cp src/veins/modules/application/istanbul/IstanbulVeinsApp.ned /tmp/veins-output/config/
cp -r /project/maps/${DATASET_MAP}/ /tmp/veins-output/config/map

veins_run() {
    ../../bin/veins_run -r 0 -m -u Cmdenv -n ../veins:../../src/veins -l ../../src/veins omnetpp.ini | tee /tmp/veins-output/log.txt
    out_path=$(ls -d /tmp/veins-output/*/dataset_*)
    mv $out_path /tmp/veins-output/data
}


cd /project/maps/${DATASET_MAP}/
# bash /project/docker/run_custom.sh
veins_run
