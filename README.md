# prime
tool to help run on nvidia dgpus in laptops on linux

## Requirements

to compile this you need, vulkan, opengl, glew, glfw3.3 or higher.

## Usage

```bash
To run an application on the NVIDIA gpu.
Use: prime <application>

To run various options prime-run handles.
usage: prime [options]
options:
  -h, --help        print this help message.
  -e, --print-env   print env vars that will be used.
  -p, --power       print state and power management info about the cards.
  -c, --check       check devices that will be used in opengl/vulkan.

```

## Contributing
Pull requests are welcome. bug reports aswell :)
