# Flappy World

## Building and Running

Building and running Flappy World requires love2d to be installed. The following instructions should result in:

1. love2d SDK installed on your development machine
2. `love` accessible via command-line.
3. Flappy World project files downloaded to your hard drive.
4. Flappy World built and run.

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

## Version Requirements

Love SDK and runtime:
```bash
love --version
LOVE 0.10.2 (Super Toast)
```
