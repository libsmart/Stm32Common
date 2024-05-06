# Create Application Skeleton



## Use board config

You can find some examples in `Board Config Files`



## Add git_rev_macro.py

Add the following at the end of the file `CMakeLists_template.txt`:

```cmake
include(git_rev_macro.cmake)
```



## Add `Application` directory

### In `CMakeLists_template.txt`

Add `Application` to `include_directories`

Add `"Application/*.*"` to `file`



### In `Core/Src/main.c`

Add the following to `USER CODE BEGIN Includes`:

```c
#include "defines.h"
#include "globals.h"
#include "main.hpp"
```

Add the following to `USER CODE BEGIN 2`:

```c
  // Jump to our C++ setup function
  setup();
```

Add the following to `USER CODE BEGIN 3`:

```c
  // Jump to our C++ loop function
  loop();
```

Add the following to `USER CODE BEGIN Error_Handler_Debug`:

```c
  // Jump to our C++ setup function
  errorHandler();
```



## Add Stm32Common library

see [Stm32Common](https://github.com/libsmart/Stm32Common)







