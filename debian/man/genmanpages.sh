#!/bin/bash

txt2man -d "${CHANGELOG_DATE}" -t YOSYS-SMTBMC -s 1 yosys-smtbmc.txt > yosys-smtbmc.1
