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

    Note that our makefile already runs the necessary `conan` commands for you -- you just need to have it installed.
  - _Other dependencies are automatically managed by conan._
- `frontend`
  - Python 3 (for launching the web server via `python -m http.server`)

## How to run

First, generate the graph of Rio's streets:

```sh
python src/create_data/create_graph_database.py
```

Then, compile the backend:

```sh
make -C src/backend
```

Then, run the backend; this will launch the backend server on `localhost:8080`:

```sh
src/backend/waze-server
```

Then, compile and serve the frontend; this will launch the frontend server on `localhost:8000`:

```sh
make -C src/frontend
```

Finally, go with your browser to [localhost:8000/](http://localhost:8000/).
