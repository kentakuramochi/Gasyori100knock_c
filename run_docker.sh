#!/bin/bash -eu

image_name=g100knock
if [ "$(docker images | grep ${image_name})" == "" ]; then
    docker build -t ${image_name} .
fi

container_name=g100knock_work
docker run -u $(id -u $USER):$(id -g $USER) \
    -v /etc/group:/etc/group:ro \
    -v /etc/passwd:/etc/passwd:ro \
    -v $(pwd):/workspace \
    --rm \
    --name ${container_name} \
    -it ${image_name} /bin/bash
