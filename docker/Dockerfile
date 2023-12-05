FROM ubuntu:20.04

# Minimal ubuntu-desktop OS
RUN \
  export DEBIAN_FRONTEND=noninteractive && \
  apt-get update && \
  apt-get -y dist-upgrade && \
  apt-get -y install ubuntu-desktop-minimal && \
  apt-get clean

# Libraries & tools for development
RUN \
  export DEBIAN_FRONTEND=noninteractive && \
  apt-get update && \
  apt-get -y install build-essential gcc g++ bison flex perl python python3 qt5-default libqt5opengl5-dev tcl-dev tk-dev libxml2-dev zlib1g-dev default-jre doxygen graphviz && \
  \
  echo "ubuntu 20.04 libwebkitgtk-3.0-0 fix" && \
  echo "deb http://cz.archive.ubuntu.com/ubuntu bionic main universe" >> /etc/apt/sources.list && \
  apt-get update && \
  apt-get -y install libwebkitgtk-3.0-0 && \
  \
  apt-get -y install openscenegraph-plugin-osgearth libosgearth-dev && \
  apt-get -y install openmpi-bin libopenmpi-dev && \
  apt-get -y install libpcap-dev && \
  \
  export OMNET_VERSION=5.6.2 && \
  mkdir -p /tmp/build && cd /tmp/build && \
  wget https://github.com/omnetpp/omnetpp/releases/download/omnetpp-5.6.2/omnetpp-${OMNET_VERSION}-src-linux.tgz && \
  tar xf omnetpp-${OMNET_VERSION}-src-linux.tgz && \
  mv omnetpp-${OMNET_VERSION} /opt/omnetpp-${OMNET_VERSION} && \
  export PATH=$PATH:/opt/omnetpp-${OMNET_VERSION}/bin && \
  cd /opt/omnetpp-${OMNET_VERSION} && \
  ./configure && make -j$(nproc) && \
  rm -rf /tmp/build && \
  \
  export SUMO_VERSION=v1_8_0 && \
  mkdir -p /tmp/build && cd /tmp/build && \
  DEBIAN_FRONTEND=noninteractive apt-get -y install git cmake python3 g++ libxerces-c-dev libfox-1.6-dev libgdal-dev libproj-dev libg\
l2ps-dev && \
  apt-get clean && \
  git clone --recursive https://github.com/eclipse/sumo && \
  export SUMO_HOME="/tmp/build/sumo" && \
  cd ${SUMO_HOME} && git checkout ${SUMO_VERSION} && \
  mkdir ${SUMO_HOME}/build/cmake-build && cd ${SUMO_HOME}/build/cmake-build && \
  cmake ../.. && \
  make -j$(nproc) && \
  make install && \
  rm -rf /tmp/build

# Veins 5.2 project
RUN \
    cd /tmp && \
    wget https://veins.car2x.org/download/veins-5.2.zip && \
    unzip veins-5.2.zip && \
    mv /tmp/veins-*5.2 /opt/veins-5.2 && \
    rm veins-5.2.zip

# Project specific libraries
RUN \
  export DEBIAN_FRONTEND=noninteractive && \
  apt-get update && \
  apt-get -y install rapidjson-dev && \
  apt-get clean

