/**
 *  @copyright Copyright 2018 The J-PET Framework Authors. All rights reserved.
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may find a copy of the License in the LICENCE file.
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  @file JPetTaskChainExecutor.cpp
 */

#include "./JPetOptionsGenerator/JPetOptionsGeneratorTools.h"
#include "./JPetParamsFactory/JPetParamsFactory.h"
#include "JPetTaskChainExecutor.h"
#include "./JPetLoggerInclude.h"
#include <cassert>
#include <memory>

/**
 * Constructor
 */
JPetTaskChainExecutor::JPetTaskChainExecutor(
  TaskGeneratorChain* taskGeneratorChain,
  int processedFileId,
  const jpet_options_tools::OptsStrAny& opts
): fInputSeqId(processedFileId), fTaskGeneratorChain(taskGeneratorChain)
{
  fParams = jpet_params_factory::generateParams(opts);
  assert(fParams.getParamManager());
  if (taskGeneratorChain) {
    for (auto taskGenerator : *fTaskGeneratorChain) {
      auto task = taskGenerator();
      fTasks.push_back(task);
    }
  } else {
    ERROR("TaskGeneratorChain is null while constructing JPetTaskChainExecutor");
  }
}

/**
 * Destructor
 */
JPetTaskChainExecutor::~JPetTaskChainExecutor()
{
  for (auto& task : fTasks) {
    if (task) {
      delete task;
      task = 0;
    }
  }
}

/**
 * Run with multithreading, using class from CERN ROOT TThread
 */
TThread* JPetTaskChainExecutor::run()
{
  TThread* thread = new TThread(
    std::to_string(fInputSeqId).c_str(), processProxy, (void*) this
  );
  assert(thread);
  thread->Run();
  return thread;
}

/**
 * Main processing method for mode without multithreading. It includes iterating
 * over both tasks and parameters, generating input parameters based on
 * iterated the parameter sets and control parameters produced by the previous task.
 * Each iteration can modify the control parameters from previous tasks.
 * Method includes measurement of execution time, results are then printed out.
 */
bool JPetTaskChainExecutor::process()
{
  JPetTimer timer;
  JPetDataInterface nullDataObject;
  JPetParams controlParams;
  for (auto currentTaskIt = fTasks.begin(); currentTaskIt != fTasks.end(); currentTaskIt++) {
    auto currentTask = *currentTaskIt;
    auto taskName = currentTask->getName();
    auto& currParams = fParams;
    currParams = jpet_params_factory::generateParams(currParams, controlParams);
    jpet_options_tools::printOptionsToLog(
      currParams.getOptions(), std::string("Options for ") + taskName
    );
    timer.startMeasurement();
    INFO(Form("Starting task: %s", taskName.c_str()));
    if (!currentTask->init(currParams)) {
      ERROR("In task initialization");
      return false;
    }
    if (!currentTask->run(nullDataObject)) {
      ERROR("In task run()");
      return false;
    }
    if (!currentTask->terminate(controlParams)) {
      ERROR("In task terminate() ");
      return false;
    }
    timer.stopMeasurement("task " + taskName);
  }
  INFO(timer.getAllMeasuredTimes());
  INFO(timer.getTotalMeasuredTime());
  return true;
}

/**
 * Private method that establishes proxy processing in multithreading mode
 */
void* JPetTaskChainExecutor::processProxy(void* runner)
{
  assert(runner);
  static_cast<JPetTaskChainExecutor*>(runner)->process();
  return 0;
}
