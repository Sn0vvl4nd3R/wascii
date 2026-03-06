# WASCII (Webcam to ASCII)

A lightweight, real-time C++ program that captures video from your webcam and renders it as ASCII art directly into your terminal.

The renderer dynamically adjusts to your terminal's window size, applies an unsharp mask to keep facial features and edges distinct, and runs efficiently at a targeted 30 FPS using a Look-Up Table (LUT) for fast pixel-to-character mapping.

## Features

* **Real-Time Rendering:** Capped at an efficient 30 FPS to balance smooth video with low CPU usage.
* **Dynamic Resizing:** Automatically detects terminal window changes and adjusts the output resolution on the fly to fit your screen perfectly.
* **Aspect Ratio Correction:** Accounts for standard terminal font aspect ratios (roughly 2:1) to prevent the image from looking horizontally stretched.
* **Image Enhancement:** Applies a lightweight unsharp mask (using Gaussian blur and image weighting) to emphasize edges and details in the ASCII output.
* **Custom Palettes:** Choose from 6 different predefined ASCII character palettes (ranging from simple to dense) via command-line arguments.

## Prerequisites

To compile and run this project, you will need:

* A C++ compiler supporting C++17 (`clang++` is set as default in the Makefile).
* **OpenCV 4** (for webcam capture and image processing).
* `pkg-config` (used by the Makefile to locate OpenCV flags and libraries).
* A POSIX-compliant operating system (Linux or macOS) due to the use of `<sys/ioctl.h>` and `<unistd.h>` for terminal control.

## Building the Project

Building is straightforward thanks to the included Makefile. Simply open your terminal in the project directory and run:

`make`

To clean up the compiled binary, you can run:

`make clean`

## Usage

Run the compiled executable from your terminal:

`./wascii [palette_index]`

### Available Palettes

You can pass an integer from `0` to `5` as an argument to select different character palettes. If no argument is provided, it defaults to palette `3` (balanced).

* **0:** `classic` (Sparse and clean)
* **1:** `classic-8` (Slightly more contrast)
* **2:** `soft` (Smooth gradients)
* **3:** `balanced` *(Default)* (Great for general faces and lighting)
* **4:** `sharp` (High contrast characters)
* **5:** `dense` (Uses the widest variety of characters for maximum detail)

**Example:**
To run the program with the most detailed character set:
`./wascii 5`
