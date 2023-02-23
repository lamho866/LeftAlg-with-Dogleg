# LeftAlg-with-Dogleg
Simple algorithm with dogleg

# How to compile File
g++ -std=c+11 7111056016_cad_finalTerm.cpp -o 7111056016_cad_finalTerm.exe

# Process way
Left edge Algorithm + dogleg
![DogLeg01 JPG](https://user-images.githubusercontent.com/72925954/220959941-05aee2cf-5138-4eb6-b4d2-d78c43deb6a4.jpg)

In this example, we can we that if the net appears more than 2 times in the up layer or down layer, cut them in multi appear points.
How to fix the vertical constraints problem?

![DogLeg02 JPG](https://user-images.githubusercontent.com/72925954/220960004-4f4aa4e0-8874-4ea7-99c2-cbef38f79f7d.jpg)

If more than one nets have the same start column, we prefer to order up layer net id equal edge net id. Therefore net2 is placed before net1.

Conclusion:

1: Place edge way: left-edge algorithm
2: Dogleg:  if the net appears more than 2 times in the up or down layer, cut them in multi appear points.
