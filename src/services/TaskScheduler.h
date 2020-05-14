#pragma once

#include <TaskScheduler.h>
#include "./Logger.h"

Scheduler runner;

void initTaskScheduler(){
    runner.init();
}