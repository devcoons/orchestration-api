###### Version: 0.2-alpha
Heterogeneous Orchestration API 
==================


  - [Synopsis](#synopsis)
  - [Requirements](#requirements)
  - [Installation](#installation)
  - [API](#api)
  - [Usage](#usage)
  - [Contributing](#contributing)
  - [License](#license)


## Synopsis

This Project is a linux shared library for Linux. It provides a proper API for both application and service of the orchestration mechanism. 

## Requirements

[ NONE ]

## Installation

Download the source code and copy both header and source files to your project's folder.
```
$ download as zip
$ cd "Source Code"/
$ sudo make
$ sudo make install
```

## API

Client's API
```
Client(pid_t);
~Client();
void execute(void*);
void disconnect(void);
void setPriority(int);
void setProfiling(int);
void setAppName(char *);
void connect(string, int);
double getCurrentMs(void);
double getAverageMs(void);
IndividualPolicyType getPolicy(void);
void setPolicy(IndividualPolicyType);
void setGoalMs(double, double, double);
void registerImplementation(ImplementationKernel, ImplementationType);
void deregisterImplementation(ImplementationKernel, ImplementationType);
```
Service API
```
Server();
~Server();
void execute(void);
void initialize(void);
void beginServer(int,int);
```

## Usage

Make sure you have included the header file.
```
#include <orchestration/orchestration.h>
```
Create some Kernel Functions
```
void CPUKernel1(void * arg)
{
...
...
}
void CPUKernel2(void * arg)
{
...
...
}
```
Declare the proper λ-functions for each kernel
```
ImplementationKernel cpu1Impl = [&](void * arg) -> void 
{
	CPUKernel1(arg);
};
ImplementationKernel cpu2Impl = [&](void * arg) -> void 
{
	CPUKernel2(arg);
};
```
Initialize an Orchestration Client Object
```
Orchestration::Client TConnector(getpid());
```
Connect to the orchestration service
```
TConnector.connect("127.0.0.1", orchPort);
```
Configure some settings
```
TConnector.setAppName(appName);
TConnector.setPolicy(IndividualPolicyType::Balanced);
TConnector.setGoalMs(goal, lowestGoal, highestGoal);
TConnector.setPriority(priority);
```
Register the λ-functions
```
TConnector.registerImplementation(cpu1Impl, ARMA57);
TConnector.registerImplementation(cpu2Impl, ARMA53);
```
Set the profiling 
```
TConnector.setProfiling(profiling);
```
Execute the application
```
...
TConnector.execute(arg);
...
```
Close the connection
```
TConnector.disconnect();
```

## Contributing
One of the easiest ways to contribute is to participate in discussions and discuss issues. You can also contribute by submitting pull requests with code changes. Please log a new issue in the appropriate section [Issues] or by e-mail.
The best way to get your bug fixed is to be as detailed as you can be about the problem.  Providing a minimal project with steps to reproduce the problem is ideal. 

Here are questions you can answer before you file a bug to make sure you're not missing any important information.

1. Did you read the documentation?
2. Did you include the command you executed in the issue?
3. What are the EXACT steps to reproduce this problem?
4. What package versions are you using?

## License

The MIT License (MIT)

Copyright (c) 2016 Io. D

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
