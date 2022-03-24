APP_NAME=cs-50x-2022
APP_DIR=/${APP_NAME}/dev
DOCKER_BASE_IMAGE=gcc:11.2.0
COMMAND?=bash

YELLOW=\e[1m\033[33m
COLOR_OFF=\e[0m

remove-containers:
ifneq ($(shell docker ps -a --filter "name=${APP_NAME}" -aq 2> /dev/null | wc -l | bc), 0)
	@echo "${YELLOW}Removing containers${COLOR_OFF}"
	@docker ps -a --filter "name=${APP_NAME}" -aq | xargs docker rm -f
endif

docker-command: remove-containers
	@echo "${YELLOW}Initiating container ${APP_NAME}${COLOR_OFF}"
	@docker run -it -v $(shell pwd):${APP_DIR} -w ${APP_DIR} \
		--name ${APP_NAME} \
		${DOCKER_BASE_IMAGE} bash -c "${COMMAND}"

build:
	@echo "${YELLOW}Building $(file)${COLOR_OFF}"
	@make -s docker-command COMMAND="gcc $(file)"
	@make -s remove-containers
