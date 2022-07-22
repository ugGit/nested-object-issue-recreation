# Too Many Initializers for Object

Clone the repo:

```
git clone git@github.com:ugGit/nested-object-issue-recreation.git
```

## Requirements
The issue has been encountered in an environment using the following modules:

* nvhpc/22.3
* nvhpc/22.5

## Information about the setup
This project contains the minimal required modules and code to recreate the issue encountered.

## What is this about?
nvc++ has a hard time instantiating an object, which contains itself another object holding only a function, followed by an array.

Sometimes code says more than words:
```cpp
struct transformation_faker{
  int point_to_global(int local){
    return local;
  }
};

struct cell_module {
    transformation_faker placement = {};
    int range0[2];
};
```

## Run the code
```
nvc++ main.cpp -o main && ./main
```

Expected output:
```
NVC++-S-0073-Too many initializers for is_the_evil (main.cpp: 21)
NVC++/x86-64 Linux 22.3-0: compilation completed with severe errors
```

## Observations
The code compiles fine with clang (13) and g++ (12). 

An issue based on the same error message has been discussed over a decade ago here: 
https://forums.developer.nvidia.com/t/pgc-s-0073-too-many-initializers/131291.
In summary, they suggested that it might be the other compilers that are interpreting the C++ standard differently.
So, it might be the case here as well.

Anyway, the following list resumes the changes made that allow a successful compilation with nvc++:

* not having the attribute `placement` of type `transformation_faker` in `cell_module`:
  ```cpp
  struct cell_module {
    int placement = {};
    int range0[2];
  };
  ```

* not having an array after the attribute `placement` in `cell_module`:
  ```cpp
  struct cell_module {
    transformation_faker placement = {};
    int range0;
  };

* switching the order of the attributes in `cell_module`:
  ```cpp
  struct cell_module {
    int range0[2];
    transformation_faker placement = {};
  };
  ```

* adding another attribute to `transformation_faker`:
  ```cpp
  struct transformation_faker{
    int something = 0; // can also be after the point_to_global function
    int point_to_global(int local){
      return local;
    }
  };
  ```

* changing `auto` to `cell_module`:
  ```cpp
  cell_module test = cell_module{};
  ```
