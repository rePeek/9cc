# 附录 3：使用 Docker 创建开发环境

本附录说明如何使用 Docker 在 macOS 上 build 并运行 Linux 应用程序。

Docker 是提供 Linux 虚拟环境的软件。完整功能的 VM 会模拟 PC 硬件，并在其上运行普通 OS；而 Docker 的不同之处在于，它只把 Linux system call 直接提供给虚拟环境。由于这种差异，与完整虚拟化相比，Docker 具有启动快、整体轻量的优点。Docker 经常用于向云端计算机部署应用程序。

使用 Docker 在 macOS 上开发 Linux 应用程序时，可以考虑下面两种系统构成。

1. 把 Docker 内部环境和外部环境视为完全不同的东西，开发 Linux 应用程序时，所有工作都在 Docker 中进行。

2. 源代码编辑、Git 操作等不依赖平台的普通开发工作在 Docker 外进行，只把 build 和 test 命令放到 Docker 中执行。

前一种构成等同于准备一台独立于 Mac 的 Linux 机器进行开发，所以构成简单；但需要连 editor 等一起准备 Linux 上的开发环境，setup 稍微繁琐。后一种构成并不是在 Docker 中“生活”，所以环境 setup 不需要复杂，但需要区分 Docker 内外，稍微麻烦。

上述两种构成都可以选择，但本书为了避免说明 Linux 环境 setup 步骤，选择后一种构成。因此，只有想在 Linux 环境中运行的命令，才会显式放到 Docker 中执行。

## setup 步骤

为了 setup 使用 Docker 的 Linux 开发环境，首先请下载并安装 Docker Desktop for Mac。之后执行下面的命令，就可以创建名为 `compilerbook` 的 Docker image。

```console
$ docker build -t compilerbook https://www.sigbus.info/compilerbook/Dockerfile
```

“Docker image”或“image”是汇总 Linux 环境所需全部文件和设置的东西。实际启动 Docker image 后得到的东西称为“Docker container”，或简称“container”（与可执行文件和进程的关系相同）。

要创建 container 并在其中执行命令，请把 image 名称和命令作为 `docker run` 命令的参数。下面是在 `compilerbook` container 中执行 `ls /` 命令的例子。

```console
$ docker run --rm compilerbook ls /
bin
root
dev
etc
...
```

container 也可以持续在后台运行，但我们的用法只需要交互式使用，所以通过传入 `--rm` 选项，让 container 在命令结束后也结束。因此，每次输入上面的命令时，都会创建并销毁 container。

## 使用 container 进行 build

要在 container 中执行 `make` 并编译 source file，需要让 container 能看到在 Docker 外部编辑的 source file。

在 `docker run` 中，通过传入 `-v <source>:<dest>` 形式的选项，可以让外部环境的路径 `<source>` 在 Docker 中显示为 `<dest>`。另外，可以用 `-w` 选项指定执行命令时的当前目录。使用这些选项，就能把 source file 所在目录设为当前目录后执行 `make`。

假设 source file 放在 home directory 下名为 `9cc` 的子目录中。要从 container 内对该目录执行 `make test`，请执行下面的命令。

```console
$ docker run --rm -v $HOME/9cc:/9cc -w /9cc compilerbook make test
```

请像上面这样执行 build 和 test 命令。

如果想在 container 中启动 shell 并交互式使用，请像下面这样给 `docker run` 加上 `-it` 选项。

```console
$ docker run --rm -it -v $HOME/9cc:/9cc compilerbook
```

## 向 container 添加新应用程序

按上述步骤创建的 Docker container 中已经安装了一整套开发工具，但有时也会想安装额外应用程序。这里说明方法。

Docker container 是临时存在。即使创建 container 并在其中的 shell 安装应用程序，这些修改也不会写回原 image。Docker 通过这个性质保证应用程序每次都从相同的干净状态启动，但当想修改 image 时，这个性质就会造成麻烦。

如果修改 container 后想把修改写回 image，需要显式执行 `docker commit` 命令。

例如，假设想安装 `curl` 命令。这种情况下，首先像下面这样创建 container。

```console
$ docker run -it compilerbook
```

注意这里没有传入 `--rm` 选项。之后，像下面这样从 container 中的 shell 使用 `apt` 安装 `curl`，并用 `exit` 命令退出 container。

```console
$ sudo apt update
$ sudo apt install -y curl
$ exit
```

执行 `docker run` 时没有加 `--rm` 选项，所以即使从 container shell 中 `exit`，container 也会作为 suspended 状态保留下来。使用 `docker container ls -a` 命令，可以像下面这样显示 suspended container。

```console
$ docker container ls -a
CONTAINER ID        IMAGE              COMMAND              CREATED
a377e570d1da        compilerbook       "/bin/bash"          7 seconds ago
```

应该能看到一个 ID 为 `a377e570d1da` 的 container，它运行的是名为 `compilerbook` 的 image。使用 `docker commit` 命令，可以把 container 写回 image。

```console
$ docker commit a377e570d1da compilerbook
```

通过上述步骤，就可以修改 image。

suspended container 或旧 image 等即使保留下来，也只是稍微占用一些磁盘空间，并没有特别问题；但如果在意，可以执行 `docker system prune` 来删除。
