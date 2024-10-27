# Automatic Darts Scoring

<img src="Assets/Logo.png" width="600" title="hover text">

Automatic darts scoring using a singular camera.
Scoring is done using OpenCV to detect darts, positions are sent over a network to a GUI allowing tracking of games.

## How To Use

- Set up a camera facing the front of the dartboard.
- Run `pip install -r requirements.txt` to install required packages.
- Run `mt_server.py` to do darts detection.
- Run `Client.exe`
- Connect to python server by inputting the IP address of the device into the top of the GUI

## How To Build

- Download a Release from the Github
  OR
- Build from source
  - Built using CMake and VCPKG
    - Required Packages:
      - Boost
      - Hello-Imgui
      - nlohmann-json
