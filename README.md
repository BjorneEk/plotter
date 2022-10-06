# Plotter
---
a basic graph plotting software that reads in 3d coordinates from a file and displays them onto the screen.

![](https://github.com/BjorneEk/plotter/blob/main/images/Plotter.png?raw=true)
the graph in the picture is the result of a neural network implemented using my [Neural Network Library](https://github.com/BjorneEk/NL-nerual-library) solving a XOR gate

---

## **Usage**
```
$ ./plotter <filename.csv>
$ ./plotter <filename.csv> -d ,
$ ./plotter <filename.csv> --height 1000 --width 2000
$ ./plotter --help
```
---

## **Controls**
| Key              | Action                  |
|:----------------:|:-----------------------:|
|ESCAPE            |   show/hide settings    |
|MOUSE             |       translate         |
|MOUSE + SHIFT     |        rotate           |
| scroll wheel     |      zoom/scale         |

---

## **Input format**

the format for the input file is floats separated by a delimitor. The default delimitor is ```,```
and spaces between entries are ignored, so ```a, b, c```, ```a,b,c``` and ```a, b ,c``` are allowed. 
The values are in ```X, Y, Z``` order, and files ending with an incomplete
coordinate will have it snipped of for example the input: ```1.0, 2.0, 3.0, 4.0, 5.0``` is read as a single coordinate (1.0, 2.0, 3.0)
since the last one is incomplete

---
<img alt="lang" src="https://img.shields.io/github/languages/top/bjorneek/plotter"/>
<img alt="size" src="https://img.shields.io/github/repo-size/bjorneek/plotter"/>
<img alt="lines" src="https://img.shields.io/tokei/lines/github/bjorneek/plotter"/>
