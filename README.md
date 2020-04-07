# fty-service-status-systemd
Fty-service-status plugin for systemd notification

## How to build
```bash
mkdir build
cd build
cmake ..
make
make doc # to create doxygen documentation at the root of the project
```

## Description

This plugin is a wrapper on "sd_notify" [see man page here](https://www.freedesktop.org/software/systemd/man/sd_notify.html)

When Operating Status is set at "InService" we notify systemd with a "READY=1" and when the Operating Status is set to "Stopping" or "Shutting Down" we notify systemd with "STOPPING=1". This allow systemd to syncronize process in a better way.

As an extra status for the service, for all Operating Status and Health States, we notify systemd with the "STATUS=Operating Status \<X\>, Health State \<Y\> "

By default the plugin is install here "$(PREFIX_LIBEXECDIR)/service_status_plugins/libfty-service-status-systemd.so"

The error codes return by the functions of the plugin are the one of "sd_notify" [see man page here](https://www.freedesktop.org/software/systemd/man/sd_notify.html)
