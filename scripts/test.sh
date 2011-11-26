#!/bin/bash

runcurl() {
    {
        time curl http://localhost:1337
    }&
}

runcurl
runcurl
runcurl
