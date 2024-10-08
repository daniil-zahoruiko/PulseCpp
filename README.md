# PulseCpp

<p>
  <img src="https://img.shields.io/badge/License-MIT-red?style=flat-square"/>
  <img src="https://img.shields.io/badge/Made_with-C++-blue?style=flat-square"/>
  <img src="https://img.shields.io/badge/Protocol-HTTP_1.1-yellow?style=flat-square"/>
</p>

## Description
PulseCpp is a web framework for C++. It allows you to create simple API endpoints and run your own server while leveraging the high performance of C++.

## Requirements

In order to be able to use this library, you will need the following:
- CMake 3.5 or newer
- C++11 compiler or newer

## Usage

### Installation
In order to add the library to your project, you have 2 options:
1. (Recommended) Add this repository as a submodule in your project by running:

   `git submodule add https://github.com/daniil-zahoruiko/PulseCpp`.

   After that, you need to include it in your root CMake by adding this line:

   `add subdirectory PulseCpp`

   Note that this assumes that the root CMake is in one directory above the added submodule. You will need to change the path if your project structure is different.
2. You could also clone the repo and run `cmake --build .` in it, which will generate a binary of the library that you can include in your linker.
### Basic Code

Firstly, you need to include the header:
```c++
#include "pulse/app.hpp"
```

After that, you need to instantiate an App object:
```c++
App app = App();
```
In order to add endpoints, add this:
```c++
app.register_endpoint("/test/url", &handler)
```
Note that all handlers must follow the following signature: `Response <method_name>(Request)`

After you add all endpoints, you are ready to launch your server:
```c++
app.start("<port>")
```

You can see an example in the examples directory.
