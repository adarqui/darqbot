MongoDB C Driver Documentation
==============================

Overview
--------

The MongoDB C Driver is a 10gen-supported driver for MongoDB.
It's written in pure C. The goal is to be super strict for ultimate
portability, no dependencies, and generic embeddability.

The driver is still considered alpha but is undergoing active
development. Support for replica sets was just added in v0.3.1.
The API was completely revamped in v0.4.

:doc:`tutorial`
    An overview of the driver's API.

:doc:`building`
    How to build the driver from source.

:doc:`bson`
    How to work with BSON objects.

:doc:`connections`
    How connect to single nodes and to replica sets.

`API Docs <api/index.html>`_
    Doxygen-generated API docs.

`Source code <http://github.com/mongodb/mongo-c-driver>`_
    The source code is hosted on GitHub.

.. toctree::
   :maxdepth: 2

   tutorial
   building
   bson
   connections
