# TDMH Visualizer

This program helps visualize logs. A graph with the nodes put on the background image is displayed. 
When moving the cursor inside the file, the graph is updated to show the strong (blue solid) and weak (green dotted) links up to that moment.
If the file is really long, press `Ctrl+f` to bring up a line number selection box to jump to the specified line.

If Stat mode is selected then the availabilities (timed and untimed) will be shown on the text window. Selecting a line from one of the two will display on the graph the arcs for each non-zero availability link.
A line with little space between dashes means closer to 100%, a line with more space between dashes means closer to 0%.

## Compiling
The project has been created with QT Creator. 

In alternative to that in the repository is a .sh that generates the makefile using qmake. QT 5.12.6 is required.

If libs are not detected or error on parse, make sure that qmake is using the right qt installation:

`qtchooser -install qt5.12.6 <pathtoqmake>`

e.g. `qtchooser -install qt5.12.6 /opt/Qt/5.12.6/gcc_64/bin/qmake`

then `export Q_SELECT=qt5.12.6`

Run .sh with `debug` argument to generate a debug build

## Running
The program accepts as first argument the path to the configuration file, a commented example configuration file is in the repository

### Configuration
The visualizer operates in 3 different modes: 
* Batch reads and processes a file
* Real Time reads a file and then remains active scanning for new lines
* Stat availabilities on the given log


If `BATCHFIRST` is specified in the configuration file, the program processes the entire log and then updates the gui. 
If not specifies it updates the gui every time the bottom of the scroll bar is reached, so that the log can be inspected while it is being loaded

## Usage
TODO
