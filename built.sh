#!/bin/bash
g++ -IElement -IScene -c Scene/Scene.cpp -o obj/Scene.o
g++ -IElement -c Element/Element.cpp -o obj/Element.o
g++ -IUtils -IScene -c Utils/manager.cpp -o obj/manager.o
g++ -IScene -IUtils -c main.cpp -o obj/main.o
g++ obj/Element.o obj/Scene.o obj/manager.o obj/main.o -L/usr/lib/x86_64-linux-gnu -lGL -lX11 -o apps/simulator
