#### Current Version
```
0.3:alpha
```
## Synopsis
```
This C++ Library is intended to provide balancing functions for applications.
```
## Requirements
```
[ Empty ]
```
## Installation
```
$ git clone https://gitlab.com/io.d-libraries/orchestration-api.git
$ make
$ sudo make install
```
## Usage
```
Orchestrator::Client TConnector(getpid());
TConnector.connect("127.0.0.1", orchPort);
TConnector.setAppName(appName);
TConnector.setPolicy(IndividualPolicyType::); // Balanced or Restricted
TConnector.setGoalMs(goal, lowestGoal, highestGoal);
TConnector.setPriority(priority);
TConnector.registerImplementation(cpu1Impl, ARMA57);
TConnector.registerImplementation(cpu2Impl, ARMA53);
TConnector.setProfiling(profiling);
..
..
TConnector.execute((void*)&args);	
..
..
```
## Example
```
[ Empty ]
```
## License
```
Please check the LICENSE.md file
```