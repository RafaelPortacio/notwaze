# Final project

## Code organization

Our code is split into three components:

- `create_data` ([`src/create_data`](/src/create_data));

  Creates/Fetches graphs and saves them in a specified JSON format;

- `backend` ([`src/backend`](/src/backend)); and

  Transforms Latitude+Longitude into a node;
  Calculates the shortest path between two nodes;
  Provides an executable to try out the shortest path calculation;
  Provides an executable that serves a REST API;

- `frontend` ([`src/frontend`](/src/frontend)).

  Provides a nice user interface that uses the aforementioned REST API (provided by the `backend`) to calculate the shortest paths;

## Dependencies

- `create_data`
  - `tqdm` (`pip install tqdm` / `conda install -c conda-forge tqdm`)
  - `networkx` (`pip install networkx` / `conda install networkx`)
  - `osmnx` (See https://osmnx.readthedocs.io/en/stable/#installation)
- `backend`
  - [`conan`](https://conan.io/) (for automatic C++ dependency management)

    Note that our makefile already runs the necessary `conan` commands for you -- you just need to have it properly installed.

    **Warning:** If you are using GCC, make sure you run the "Important" box in step 5 of [Conan's getting started guide](https://docs.conan.io/en/latest/getting_started.html), which is:

    ```sh
    conan profile new default --detect  # This will throw a big scary warning, but all it tells you is to run the following command:
    conan profile update settings.compiler.libcxx=libstdc++11 default
    ```

    This tells Conan to use the glibc++11 ABI. If you don't do it, then you'll have linking errors.
  - _Other dependencies are automatically managed by conan._
- `frontend`
  - Python 3 (for launching the web server via `python -m http.server`)

## How to run

Run:

```sh
make
```

This will, if necessary, generate the street graph, and then serve the backend and the frontend.

Once that's done, go with your browser to [localhost:8000/](http://localhost:8000/).
