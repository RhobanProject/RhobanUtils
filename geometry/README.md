Geometry
========

Description
-----------

This part of Rhoban's Math repository contains code concerning geometry, in
contrast to graphics part, all objects presented here aren't meaned to be drawn.
They exists only as mathematic object, allowing to compute easily some
mathematic functions like dotProduct or projection of a Point on a line.

Warnings
--------
This part has been developed with the purpose of developing a trajectory module.
Certain parts which should ideally be more generic have been implementended in a
specific way (ParametricLine as example) and the API is not exhaustive at all.

Building
--------
Those sources are build as part of the Math library, the `Sources.cmake` file
contains all the sources built in this part.

Tests
-----
Some testing have been done about functionalities of the geometry module. All
the testing code is presented in the folder test at the root of Math.