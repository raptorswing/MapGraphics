# MapGraphics

A tile-based "slippy map" library written in/for C++/Qt. BSD licensed (see LICENSE). It's meant to enable C++/Qt developers to easily add nice, interactive maps to their applications.

## Building
Its only dependency is Qt. If you're on a system with recent Qt libraries and a good C++ compiler, this should build.

If you're using QtCreator, just open the .pro file and build it. If you're on a command line, run qmake on the .pro file, then make.

If you're using Visual Studio or Eclipse or some other C++ IDE, google around for instructions on building Qt projects with your IDE.

## Examples

The included test app provides a simple test/example application that shows a map with tiles loaded dynamically from OpenStreetMap
(note: if you use the TestApp code as more than a dev/test example, *please* use your own tile server and don't abuse OSM's tile server).

The TestApp also renders a 5km radius red circle to demonstrate that objects can be added on top of the map. The circle can be clicked, dragged, and deleted.
More complicated object interactions are possible, but not demonstrated in this TestApp.
See https://github.com/raptorswing/FlightPlanner as an example of an application that provides more complicated interactions with objects on the map.
