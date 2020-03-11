# Object Tracking

Tracking a selected area (tracking_roi.cpp) or a hardcoded area for the provided video (tracking.cpp) using SURF detector and FLANN matchers.

## Setup

To setup the project on your local machine:

1. Click on `Fork`.
2. Go to your fork and `clone` the project to your local machine.
3. `git clone https://github.com/master-coro/cv-object-tracking`
4. Make sure you installed opencv. Clone opencv and opencv-contrib, make sure you modify the cmake list to change nonfree sources to ON, then configure cmake and build. Tutorials easily found online.

## Run

To run the project:
1. Cd into the root of the project `cd path/cv-object-tracking
2. Create a build directory and cd into it `mkdir build && cd build`
3. Configure CMake `cmake ..`
4. Build `make`
5. Run `./tracking` or `./tracking_custom` or `./tracking_previous`

## Contribute

To contribute to the project:

1. Choose any open issue from [here](https://github.com/master-coro/cv-object-tracking/issues). 
2. Comment on the issue: `Can I work on this?` and get assigned.
3. Make changes to your fork and send a PR.

To create a PR:

Follow the given link to make a successful and valid PR: https://help.github.com/articles/creating-a-pull-request/

To send a PR, follow these rules carefully,**otherwise your PR will be closed**:

1. Make PR title in this format: `Fixes #IssueNo : Name of Issue`

For any doubts related to the issues, i.e., to understand the issue better etc, comment down your queries on the respective 
