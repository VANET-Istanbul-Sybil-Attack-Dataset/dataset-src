# Running The Simulation

## The easy way

requiremets:

* Docker compatible Linux distro. Tested on Ubuntu 22.04.

* docker and docker-compose.
Ubuntu 22.04 docker installation:
```
  apt-get install docker.io docker-compose
```

installation steps:

1. clone this repository

2. edit docker-compose.yaml file according to your system

3. run docker-compose.yaml file

```
  docker-compose up
```

## The hard way

requiremets:

* OMNeT++ & Sumo compatible Linux distro. Tested on Ubuntu 20.04.

installation steps:

1. install OMNeT++ 5.6.2

```
  detailed instructions available in docker/Dockerfile
```

2. install Sumo 1.8.0

```
  detailed instructions available in docker/Dockerfile
```

3. clone this repository


4. build project

```
  cd ~/project
  export MODE=relase
  ./configure
  make -j$(nproc)
```

5. run simulation

```
  python3 sumo-launchd.py -vv sumo
  cd ./maps/istanbul-v1/
  bash run_custom.sh
```
