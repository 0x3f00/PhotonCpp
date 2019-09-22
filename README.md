


# PngToPhoton

Quick and dirty utility I've made to convert PNG files to Anycubic Photon 3d printer file format. It could be useful for PCB production. Source code was ported to C++ from [Photon File Validator] (https://github.com/Photonsters/PhotonFileValidator) project.

# Usage

PngToPhoton.exe input-1440x2560.png > output.photon

To read data from STDIN use '-' as filename.
White and transparent areas of the image are treated as voids.
I insist on checking outoput with Photon File Validator after procedure. Use output at your own risk.