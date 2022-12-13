# prime
tool to help run on nvidia dgpus in laptops on linux

## Requirements

to compile this you need, vulkan, opengl, glew, glfw3.3 or higher.

## Usage

```bash
usage: prime [options]
options:
  -h, --help        print this help message.
  -e, --print-env   print env vars that will be used.
  -p, --power       print state and power management info about the cards.
  -d, --devices     check devices that will be used in opengl/vulkan.
  -c, --create      create local config template that will be used when running prime.

usage: prime <command>
example:
  prime glxgears    will run glxgears with set environment variables to run on dgpu.

```

## Contributing
Pull requests are welcome. bug reports aswell :)
