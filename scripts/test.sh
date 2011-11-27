#!/bin/bash

runcurl() {
    {
        time curl http://localhost:1337?n=40
    }&
}

runcurl
runcurl
runcurl
