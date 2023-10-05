# Order statistic self-balancing tree
[This data structure](src/order_statistic_tree) supports the following operations:
* adding an integer key;
* getting the i-th smallest element (k-th order statistic); 
* finding the number of elements lower than a given j.

Tree balancing allows to process all requests in logarithmic time.

As an example of use, a [small wrapper](src/cli) has been implemented in the form of a command line interface. 
Each request is submitted to the input as follows:
* key insertion - k i, where i is an integer value;
* k-th order statistic - m k, where k is an integer value;
* number of elements lower than a given j - n j, where j is an integer value.

[GoogleTest](https://github.com/google/googletest) was used for testing:
* [tests for CLI](test/cli);
* [tests for order statistic tree](test/order_statistic_tree).

## Compile and run
```
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

build/src/cli/cli_order_statistic_tree_bootstrap # to run CLI
build/test/cli/cli_order_statistic_tree_test # to run CLI tests
build/test/order_statistic_tree/order_statistic_tree_test # to run OrderStatisticTree module tests
```
