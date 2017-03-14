# Flappy World

## Building and Running

Building and running Flappy World requires love2d to be installed. The following instructions should result in:

1. love2d SDK installed on your development machine.
2. `love` accessible via command-line.
3. Flappy World project files downloaded to your hard drive.
4. Flappy World built and run on desktop.
5. Flappy World running on iOS client.

First, get the SDK from [love2d](https://love2d.org/). Follow the specific installation instructions for your development machine. Once installed, check whether `love` is available on your command-line:

> DEFINITION: `which N` return the path to command `N`.

```bash
# return path to love
which love
```

Once you've confirmed `love` is available, we can start building and running Flappy World.

So clone this repository and `cd` into the repository folder and run `pdrun`:

```bash
git clone git@gitlab.com:parrottdrive/flappyworld.git
cd flappyworld
./pdrun
```

`pdrun` builds and runs our project in one step, launching `pdrun` the application in a window.

Once you've confirmed the game runs on your desktop, we now look into getting the game to run on your iOS device using Xcode. 

First, you should create the love2d game package the Xcode project depends on. Run `pdpackage` to do so.

Now, open up `ios/platform/xcode/flappyworld.xcodeproj` in Xcode. Then make sure the scheme `flappyworld-ios` is active. Finally, build and deploy to your physical iOS device. Currently, the game fails to run through the iOS simulator.

## Version Requirements

Love SDK and runtime:
```bash
love --version
LOVE 0.10.2 (Super Toast)
```

## Other Requirements

- Physical iOS device.
- Xcode 8
