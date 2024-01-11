# NULS2.0 Ledger Application

Ledger Application for several NULS Blockchain Assets: https://nuls.io/

## Prerequisite

### With the docker image builder

The app-builder docker image [from this repository](https://github.com/LedgerHQ/ledger-app-builder) contains all needed tools and library to build and load an application.
You can download it from the ghcr.io docker repository:

```shell
sudo docker pull ghcr.io/ledgerhq/ledger-app-builder/ledger-app-builder-full
```

You can then enter this development environment by executing the following command from the directory of the application `git` repository:

```shell
sudo docker run --rm -ti --user "$(id -u)":"$(id -g)" -v "$(realpath .):/app" ghcr.io/ledgerhq/ledger-app-builder/ledger-app-builder-full
```

The application's code will be available from inside the docker container, you can proceed to the following compilation steps to build your app.



## Compilation and load

```shell
make DEBUG=1  # compile optionally with PRINTF
make load     # load the app on the Nano using ledgerblue
```

## Documentation

High level documentation such as [APDU](doc/APDU.md), [commands](doc/COMMANDS.md) and [transaction serialization](doc/TRANSACTION.md) are included in developer documentation which can be generated with [doxygen](https://www.doxygen.nl)


## Contribute to Ledger Application
Click Star and Fork to start contributing improvements to this module.
Hope more contributors can submit improvement suggestions and bug reports here.
Issues: https://github.com/nuls-io/app-nuls/issues


## Contribute to NULS
We are committed to making blockchain technology simpler and our slogan is "NULS Making It Easier to Innovate".

Get to know NULS developers
https://nuls.io/developer

You are welcome to contribute to NULS! We sincerely invite developers with rich experience in the blockchain field to join the NULS technology community.
https://nuls.io/community

Documentation：https://docs.nuls.io

NULS Brand Assets: https://nuls.io/brand-assets



## License

NULS is released under the [MIT](http://opensource.org/licenses/MIT) license.
Modules added in the future may be release under different license, will specified in the module library path.

## Community

- Website: https://nuls.io
- Twitter: https://twitter.com/nuls
- Discord:https://discord.gg/aRCwbj47WN
- Telegram: https://t.me/Nulsio)
- Medium: https://nuls.medium.com
- Forum: https://forum.nuls.io
- GitHub: https://github.com/nuls-io

####  
