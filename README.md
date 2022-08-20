# Quark Runtime

This is a **very** experimental replacement for XUL runner, with the goal of providing a method for creating apps built around the Gecko toolkit.

## Roadmap

- [ ] Basic docs and examples
- [ ] Code modules for common tasks
- [ ] Auto generated docs for mozilla internals
- [ ] Auto generated changelogs for mozilla internals

## Building from source

First, create a folder to house your application for development:

```bash
mkdir quark-cleandir
cd quark-cleandir
```

Then clone the firefox source code, either via `git clone` or `hg clone`:

```bash
git clone --depth 1 https://github.com/mozilla/gecko-dev mozilla-central
```

Clone this repository:

```bash
git clone https://github.com/quark-platform/runtime
```

Keep track of the location of `mozilla-central`:

```bash
echo $PWD/mozilla-central > runtime/.moz-central
```

Now, we need to add our app to the gecko source code:

```bash
# On linux add -r to each of the following commands
ln -s runtime mozilla-central/runtime
ln -s runtime/runtime.mozconfig mozilla-central/.mozconfig
```

Apply custom patches:

```bash
cd runtime
./patches.sh import
cd ..
```

Configure and build your application:

```bash
cd mozilla-central
./mach configure
./mach build
```

Then run it:

```bash
./mach run
```

## Credit

- [Alexander Vincent](https://github.com/ajvincent/motherhen) - For the begining of this
