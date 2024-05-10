# mobilepixels-linux-driver

Since MobilePixels products don't officially support linux, this is an unofficial, community driver to make them work on USB-A connections.

This has been tested on Trio Max 2023 version that shows in `lsusb` up as `ID 0711:560b Magic Control Technology Corp. T6 USB Station`

The initial .run source file was obtained by asking [MCT](https://www.mct.com.tw) for it, as they are the manufacturers of EVDI chip used in this product.

I don't know anything about C, but even I can tell that the code... isn't very mature. So this is a repo to keep track of improvements. PR's are welcome.

# Installation

First off, get the latest EVDI driver:

https://www.reddit.com/r/pop_os/comments/18hbbf7/comment/kd5uqyw/

Clone this repo somewhere

```
git clone git@github.com:rfxDarth/mobilepixels-linux-driver.git
cd mobilepixels-linux-driver/evdi_t6_1
make 
```

Then run using 

```
sudo ./T6evdi
```

You can install it as a permanent service using

```
sudo make install
```

But beware that I have not tested it yet. My screen setup is already shaky with 3 GPU's in Xorg and I wouldn't trust this code not to brick my display manager. Therefore, there's no further documentation for now.
