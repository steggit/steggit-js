#!/bin/bash

if [ -z "$1" ]; then
  echo "Error: Docker image parameter is required"
  echo "Usage: $0 <docker-image>"
  exit 1
fi

DOCKER_IMAGE="$1"

echo "Using Docker image: $DOCKER_IMAGE"

docker run --rm \
  -v "$(git rev-parse --show-toplevel)":/app \
  -w /app \
  --entrypoint bash "$DOCKER_IMAGE" -c "
  dnf install -y gcc gcc-c++ make libpng-devel libjpeg-turbo-devel python && \
  npm install -g corepack && yarn install && \
  cd packages/node && yarn prebuild
"