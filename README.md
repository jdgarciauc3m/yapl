# yapl

**YAPL** (_Yet Another Parallel Library_) is a small experimental prototype to
illustrate some ideas about parallel libraries. The library is currently a
header-only library.

The source-tree contains:

* **include** Header files of the library.
* **unit_test** Some (incomplete) unit tests using gtest.

To build the library you can use cmake:

1. In the root of *yapl* create a *build* directory (*yapl/build*).

    mkdir yapl/build

2. Generate the *Makefiles*:

    cmake ..

3. Run *make*

    make

4. Run the unit tests

    bin/yapl_unit_tests
