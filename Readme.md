## Synopsis

This C++ Library is intended to provide balancing functions for applications.
Current Version 0.3

## Installation

```
$ git clone https://gitlab.com/io.d-libraries/orchestration-api.git
$ make
$ sudo make install
```

## How to Use

Application Usage:
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

Balancing Service Usage:
	
## License

Copyright (c)  2014, 2016 Io. D

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
