# Quark Runtime

![Discord](https://img.shields.io/discord/835714549545828352?style=flat-square)](https://discord.gg/xNkretH7sD)

This is a **very** experimental replacement for XUL runner, with the goal of providing a method for creating apps built around the Gecko toolkit.

## Roadmap

- [ ] Internal testing
- [ ] Basic docs and examples
- [ ] Code modules for common tasks
- [ ] Auto generated docs for mozilla internals
- [ ] Auto generated changelogs for mozilla internals
- [ ] Example applications and distribution best practices

## Building from source

You will need nodejs and pnpm installed. Then clone the repo and run the following:

```bash
pnpm install
```

If you have never built a copy of Firefox (or any other modern gekco app) on your computer, you will need to run:

```bash
pnpm bootstrap
```

Then build it using the following:

```bash
pnpm build
```

And run it using:

```bash
cd engine
./mach run -- --app ../example/application.ini
```

## Credits

- [Alexander Vincent](https://github.com/ajvincent/motherhen) - For the beginning of this
- [Gluon](https://github.com/pulse-browser/gluon) and its predecessor [Melon](https://github.com/dothq/melon)
